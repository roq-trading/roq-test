AC_REVISION([m4_esyscmd_s([build-aux/git-version])])

AC_INIT([roq-test],
        m4_esyscmd_s([build-aux/git-version]),
        [thraneh@gmail.com],
        [roq-test])

AC_PREREQ([2.59])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AC_CONFIG_FILES([
 Makefile
 src/Makefile
 src/roq/Makefile
 src/roq/test/Makefile
 test/Makefile
])
AC_CONFIG_FILES([roq-test.pc])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AM_PROG_AR
LT_INIT([disable-shared])
AC_PROG_CXX
AC_LANG([C++])

AX_PTHREAD

ROQ_CHECK_STDCXX

ROQ_CHECK_FMT
ROQ_CHECK_GFLAGS

ROQ_CHECK_GTEST([third_party/googletest])

PKG_CHECK_MODULES([ROQ_LOGGING], [roq-logging])
PKG_CHECK_MODULES([ROQ_API], [roq-api])
PKG_CHECK_MODULES([ROQ_CLIENT], [roq-client])

AC_OUTPUT
