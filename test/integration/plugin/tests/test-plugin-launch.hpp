//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2016-22, Lawrence Livermore National Security, LLC
// and RAJA project contributors. See the RAJA/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

///
/// Header file containing basic integration tests for plugins with launch.
///

#ifndef __TEST_PLUGIN_LAUNCH_HPP__
#define __TEST_PLUGIN_LAUNCH_HPP__

#include "test-plugin.hpp"


// Check that the plugin is called with the right Platform.
// Check that the plugin is called the correct number of times,
// once before and after each launch capture for the capture counter,
// once before and after each launch invocation for the launch counter.

// test with basic launch
template <typename LaunchPolicy,
          typename WORKING_RES,
          RAJA::Platform PLATFORM>
void PluginLaunchTestImpl()
{
  SetupPluginVars spv(WORKING_RES::get_default());

  CounterData* data = plugin_test_resource->allocate<CounterData>(10);

  for (int i = 0; i < 10; i++) {

    PluginTestCallable p_callable{data};

    RAJA::launch<LaunchPolicy>
      (RAJA::LaunchParams(RAJA::Teams(1), RAJA::Threads(1)),
      [=] RAJA_HOST_DEVICE(RAJA::LaunchContext ctx)
      {
        p_callable(i);
      });

    CounterData loop_data;
    plugin_test_resource->memcpy(&loop_data, &data[i], sizeof(CounterData));
    ASSERT_EQ(loop_data.capture_platform_active, PLATFORM);
    ASSERT_EQ(loop_data.capture_counter_pre,     i+1);
    ASSERT_EQ(loop_data.capture_counter_post,    i);
    ASSERT_EQ(loop_data.launch_platform_active, PLATFORM);
    ASSERT_EQ(loop_data.launch_counter_pre,     i+1);
    ASSERT_EQ(loop_data.launch_counter_post,    i);
  }

  CounterData plugin_data;
  plugin_test_resource->memcpy(&plugin_data, plugin_test_data, sizeof(CounterData));
  ASSERT_EQ(plugin_data.capture_platform_active, RAJA::Platform::undefined);
  ASSERT_EQ(plugin_data.capture_counter_pre,     10);
  ASSERT_EQ(plugin_data.capture_counter_post,    10);
  ASSERT_EQ(plugin_data.launch_platform_active, RAJA::Platform::undefined);
  ASSERT_EQ(plugin_data.launch_counter_pre,     10);
  ASSERT_EQ(plugin_data.launch_counter_post,    10);

  plugin_test_resource->deallocate(data);
}


TYPED_TEST_SUITE_P(PluginLaunchTest);
template <typename T>
class PluginLaunchTest : public ::testing::Test
{
};

TYPED_TEST_P(PluginLaunchTest, PluginLaunch)
{
  using LaunchPolicy = typename camp::at<TypeParam, camp::num<0>>::type;
  using ResType = typename camp::at<TypeParam, camp::num<1>>::type;
  using PlatformHolder = typename camp::at<TypeParam, camp::num<2>>::type;

  PluginLaunchTestImpl<LaunchPolicy, ResType, PlatformHolder::platform>( );
}

REGISTER_TYPED_TEST_SUITE_P(PluginLaunchTest,
                            PluginLaunch);

#endif  //__TEST_PLUGIN_LAUNCH_HPP__
