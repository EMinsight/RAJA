.. ##
.. ## Copyright (c) 2016-21, Lawrence Livermore National Security, LLC
.. ## and RAJA project contributors. See the RAJA/COPYRIGHT file
.. ## for details.
.. ##
.. ## SPDX-License-Identifier: (BSD-3-Clause)
.. ##

.. _contributing-label:

*********************
Contributing to RAJA
*********************

RAJA is a collaborative open source software project. We embrace contributions 
from anyone who wants to add features or improve its existing capabilities.
This section is intended for folks who want to contribute new features or 
bugfixes to RAJA. It assumes you are familiar with Git, which we use for
source code revision control, and GitHub, which is the site where our project
is hosted. It describes the basics, such as: 

  * what a good pull request (PR) looks like
  * tests that your PR must pass before it can be merged into RAJA

============
Forking RAJA
============

If you aren't in the LLNL organization on GitHub or not a member of the core
RAJA team of developers at LLNL, then you won't have permission to push a
new branch to the RAJA repository. This is due to the policy adopted by the LLNL
organization on GitHub in which the RAJA project resides. Fortunately, you may 
still contribute to RAJA by `forking the RAJA repo 
<https://github.com/LLNL/RAJA/fork>`_. This will create a copy of the RAJA 
repository that you own. You can push code changes to that copy to GitHub and 
create pull requests in the RAJA project.

.. note:: Contributors who are not in the LLNL GitHub organization and members
          of the core team of RAJA developers cannot create branches in the 
          RAJA repo. However, anyone can create a fork of the RAJA project
          and create a pull request in the RAJA project.

=========================
Developing RAJA Code
=========================

New features, bugfixes, and other changes are developed on a **feature branch**,
**bugfix branch**, etc. Each such branch should be based on the RAJA 
``develop`` branch. When you want to make a contribution a new feature, 
first ensure you have an up-to-date copy of the ``develop`` branch locally:

.. code-block:: bash

    $ git checkout develop
    $ git pull origin develop

Then, create a new branch to on which to perform your development. For example:

.. code-block:: bash

    $ git checkout -b feature/<name-of-feature>

Proceed to modify your branch by pushing pushing changes with reasonably-sized 
**atomic commits**, and add tests that will exercise your new code. If you are 
creating new functionality, please add documentation to the appropriate
section of the `RAJA User Guide <https://readthedocs.org/projects/raja/>`_. The
source files for the RAJA documentation are maintained in the ``RAJA/docs``
directory.

After your new code is complete and you've tested it and developed appropriate
documentation, you can push your branch to GitHub and create a PR in the RAJA
project. It will be reviewed by members of the core RAJA team, who will provide 
comments, suggestions, etc. Once approved, your contribution will be merged into
the GitHub RAJA repository.

.. note:: When creating a branch that you intend to be merged into the RAJA repo,          please give it a succinct name that clearly describes the contribution.
          For example, **feature/<name-of-feature>** for a new feature, 
          **bugfix/<fixed-issue>** for a bugfix, etc.

--------------------
Developing a Bug Fix
--------------------

Contributing a bugfix follows the same process as described above. Be sure to
indicate in the name of your branch that it is for a bugfix; for example,
**bugfix/<fixed-issue>**.

Also, we recommend that you add a test that reproduces the issue you have found
and which demonstrates that issue is addressed. To verify that you have done
this properly, build the code for your branch and then run ``make test`` to 
ensure that your new test passes.

When you are done, push your branch to GitHub, then create a PR in the RAJA
project.

-----------------------
Creating a Pull Request
-----------------------

You can create a pull request (PR) 
`here <https://github.com/LLNL/RAJA/compare>`_. GitHub has a good 
`PR guide <https://help.github.com/articles/about-pull-requests/>`_ on
PR basics if you want more information. Ensure that the base branch for your 
PR is the ``develop`` branch of RAJA.

When you create a RAJA PR, you must enter a description of the contents of the 
PR. We have a *PR template* for this purpose for you to fill in. Be sure to add
a descriptive title explaining the bug you fixed or the feature you have added
and any other relevant details that will assist the RAJA team in reviewing your
contribution.

After a PR has been created in RAJA, it will be run through our automated testing
process and be reviewed by RAJA team members. Providing the branch passes all 
tests and is approved.

-----
Tests
-----

RAJA uses multiple continuous integration (CI) tools to test every pull
request. See :ref:`ci-label` for more information. 

.. note:: Passing all tests is a requirement for merging a PR into RAJA. 

All RAJA tests are in the ``RAJA/test`` directory and are split into 
*unit tests* and *functional tests*. Unit tests are intended to test basic
interfaces and features of individual classes, methods, etc. Functional tests
are used to test combinations of RAJA features. Please follow the implementation
pattern of existing tests. We have organized our tests to make it easy to see
what is being tested and easy to add new tests, for a new programming model
back-end, for example. 

.. _prfromfork-label::

-----------------------------------------------------------
Testing Pull Requests from Branches in Forked Repositories
-----------------------------------------------------------

Due to LLNL security policies and RAJA project policies, only a PR created
by someone on the RAJA core development team will be run automatically
through all RAJA CI tools. A PR made from a forked repository will not. 
Specifically, Gitlab CI on internal LLNL platforms and Travis CI will only
be run on PRs that are made on branches within the GitHub RAJA repository.

.. note:: **RAJA core team members:**

          To facilitate testing contributions in PRs from forked repositories, 
          we maintain a script to pull a PR branch from a forked repo into the 
          RAJA repo. First, identify the number of the PR. Then, run the 
          script from the top-level RAJA directory::

            $ ./scripts/make_local_branch_from_fork_pr -b <PR #>

          If successful, this will create a branch in your local copy of the
          RAJA repo labeled ``pr-from-fork/<PR #> and you will be on that
          local branch. To verify this, you can run::

            $ git branch

          You will see the new branch in the listing of branches and the branch
          you are on will be starred.  

          You can push the new branch to the RAJA repo on GitHub by running::

            $ git push origin <branch-name>

          and make a PR for the new branch. It is good practice to reference the
          original PR in the description of this new PR to track the original
          PR discussion and reviews.

          All RAJA CI checks will be triggered to run on the PR. When everything
          passes and the PR is approved, it may be merged. When it is merged,
          the original PR from the forked repo will be closed and marked as
          merged also unless it is referenced elsewhere, such as in an issue.
          If this is the case, then the original PR must be closed manually. 
 
