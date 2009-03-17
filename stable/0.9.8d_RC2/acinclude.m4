dnl acinclude.m4 generated automatically by ac-archive's acinclude 0.5.38

dnl Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY, to the extent permitted by law; without
dnl even the implied warranty of MERCHANTABILITY or FITNESS FOR A
dnl PARTICULAR PURPOSE.

dnl ______ /usr/share/aclocal/rleigh/ac_path_lib.m4 ______
dnl Available from the GNU Autoconf Macro Archive at:
dnl http://www.gnu.org/software/ac-archive/htmldoc/ac_path_lib.html
dnl




# AC_PATH_LIB(LIBRARY, MINIMUM-VERSION, HEADER, CONFIG-SCRIPT,
#              MODULES, ACTION-IF-FOUND, ACTION-IF-NOT-FOUND)
# -----------------------------------------------------------
# Check for the presence of libLIBRARY, with a minumum version
# MINIMUM-VERSION.
#
# If needed, use the libconfig script CONFIG-SCRIPT.  If the script
# needs extra modules specifying, pass them as MODULES.
#
# Run ACTION-IF-FOUND if the library is present and all tests pass, or
# ACTION-IF-NOT-FOUND if it is not present or any tests fail.
AC_DEFUN([AC_PATH_LIB],[# check for presence of lib$1
dnl We're going to need uppercase, lowercase and user-friendly
dnl versions of the string `LIBRARY', and long and cache variants.
m4_pushdef([UP], m4_translit([$1], [a-z], [A-Z]))dnl
m4_pushdef([DOWN], m4_translit([$1], [A-Z], [a-z]))dnl
m4_pushdef([LDOWN], ac_path_lib_[]DOWN)dnl
m4_pushdef([CACHE], ac_cv_path_lib_[]DOWN)dnl
m4_pushdef([ERRORLOG], error.[]DOWN[]test)dnl
_AC_PATH_LIB_INIT([$1], [$3], [$4])
_AC_PATH_LIB_CHECK([$1], [$2], [$5])
_AC_PATH_LIB_CHECK_TESTS([$2])
_AC_PATH_LIB_ERROR_DUMP
_AC_PATH_LIB_FINI([$6], [$7])
dnl Pop the macros we defined earlier.
m4_popdef([UP])dnl
m4_popdef([DOWN])dnl
m4_popdef([LDOWN])dnl
m4_popdef([CACHE])dnl
m4_popdef([ERRORLOG])dnl
])# AC_PATH_LIB






# _AC_PATH_LIB_INIT(LIBRARY, HEADER, CONFIG-SCRIPT)
# -----------------------------------------
# Initialisation of defaults and options.
# Remove error log from previous runs.
AC_DEFUN([_AC_PATH_LIB_INIT],
[_AC_PATH_LIB_DEFAULTS([$1], [$2], [$3])
_AC_PATH_LIB_OPTIONS
rm -f ERRORLOG
# Save variables in case check fails.
ac_save_[]UP[]_CFLAGS="$UP[]_CFLAGS"
ac_save_[]UP[]_LIBS="$UP[]_LIBS"
ac_save_[]UP[]_VERSION="$UP[]_VERSION"
])


# _AC_PATH_LIB_DEFAULTS(LIBRARY, HEADER, CONFIG-SCRIPT)
# -----------------------------------------------------
# Set up default behaviour.
AC_DEFUN([_AC_PATH_LIB_DEFAULTS],
[dnl Set up pkgconfig as default config script.
m4_ifdef([AC_PATH_LIB_USEPKGCONFIG],, [AC_PATH_LIB_PKGCONFIG])
dnl Set default header and config script names.
LDOWN[]_header="m4_default([$2], [$1/$1.h])"
LDOWN[]_config="m4_default([$3], [$1-config])"
ifdef([_AC_PATH_LIB_VERSION_PREFIX],,
      [m4_define([_AC_PATH_LIB_VERSION_PREFIX],
                 DOWN[_])
      ])
ifdef([_AC_PATH_LIB_VERSION_MAJOR],,
      [m4_define([_AC_PATH_LIB_VERSION_MAJOR],
                 [major])
      ])
ifdef([_AC_PATH_LIB_VERSION_MINOR],,
      [m4_define([_AC_PATH_LIB_VERSION_MINOR],
                 [minor])
      ])
ifdef([_AC_PATH_LIB_VERSION_MICRO],,
      [m4_define([_AC_PATH_LIB_VERSION_MICRO],
                 [micro])
      ])
ifdef([_AC_PATH_LIB_VERSION_SUFFIX],,
      [m4_define([_AC_PATH_LIB_VERSION_SUFFIX],
                 [_version])
      ])
ifdef([_AC_PATH_LIB_DEFVERSION_PREFIX],,
      [m4_define([_AC_PATH_LIB_DEFVERSION_PREFIX],
                 UP[_])
      ])
ifdef([_AC_PATH_LIB_DEFVERSION_MAJOR],,
      [m4_define([_AC_PATH_LIB_DEFVERSION_MAJOR],
                 [MAJOR])
      ])
ifdef([_AC_PATH_LIB_DEFVERSION_MINOR],,
      [m4_define([_AC_PATH_LIB_DEFVERSION_MINOR],
                 [MINOR])
      ])
ifdef([_AC_PATH_LIB_DEFVERSION_MICRO],,
      [m4_define([_AC_PATH_LIB_DEFVERSION_MICRO],
                 [MICRO])
      ])
ifdef([_AC_PATH_LIB_DEFVERSION_SUFFIX],,
      [m4_define([_AC_PATH_LIB_DEFVERSION_SUFFIX],
                 [_VERSION])
      ])
])# _AC_PATH_LIB_DEFAULTS


# _AC_PATH_LIB_OPTIONS
# --------------------
# configure options.
AC_DEFUN([_AC_PATH_LIB_OPTIONS],
[m4_if(AC_PATH_LIB_USEPKGCONFIG, [true], ,
[AC_ARG_WITH(DOWN-prefix,
            AC_HELP_STRING([--with-DOWN-prefix=PFX],
                           [prefix where UP is installed (optional)]),
            [LDOWN[]_config_prefix="$withval"],
            [LDOWN[]_config_prefix=""])dnl
AC_ARG_WITH(DOWN-exec-prefix,
            AC_HELP_STRING([--with-DOWN-exec-prefix=PFX],
                           [exec-prefix where UP is installed (optional)]),
            [LDOWN[]_config_exec_prefix="$withval"],
            [LDOWN[]_config_exec_prefix=""])dnl
])
AC_ARG_ENABLE(DOWN[]test,
              AC_HELP_STRING([--disable-DOWN[]test],
                             [do not try to compile and run a test UP program]),
              [LDOWN[]_test_enabled="no"],
              [LDOWN[]_test_enabled="yes"])dnl
])# _AC_PATH_LIB_OPTIONS


# _AC_PATH_LIB_CHECK(LIBRARY, MINIMUM-VERSION, MODULES)
# -----------------------------------------------------
# Obtain library CFLAGS, LIBS and VERSION information.  Cache results,
# but set avoidcache to no if config program is not available.  Break
# up available and minumum version into major, minor and micro version
# numbers.
AC_DEFUN([_AC_PATH_LIB_CHECK],
[
# Set up LIBRARY-config script parameters
m4_if([$3], , ,
      [LDOWN[]_config_args="$LDOWN[]_config_args $3"])
LDOWN[]_min_version=`echo "$2" | sed -e 's/ -nocheck//'`
m4_if([$2], , ,[if test "$LDOWN[]_min_version" = "$2" ; then
                  LDOWN[]_version_test_enabled="yes"
                fi])
m4_if(AC_PATH_LIB_USEPKGCONFIG, [true],
[LDOWN[]_config_args="$1 $LDOWN[]_config_args"
],
[  if test x$LDOWN[]_config_exec_prefix != x ; then
    LDOWN[]_config_args="$LDOWN[]_config_args --exec-prefix=$LDOWN[]_config_exec_prefix"
  fi
  if test x$LDOWN[]_config_prefix != x ; then
    LDOWN[]_config_args="$LDOWN[]_config_args --prefix=$LDOWN[]_config_prefix"
  fi
])
dnl set --version for libconfig or --modversion for pkgconfig
m4_if(AC_PATH_LIB_USEPKGCONFIG, [true],
      [AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
       if test x$PKG_CONFIG != xno ; then
         if pkg-config --atleast-pkgconfig-version 0.7 ; then
           :
         else
           AC_PATH_LIB_ERROR([A new enough version of pkg-config was not found:])
           AC_PATH_LIB_ERROR([version 0.7 or better required.])
           AC_PATH_LIB_ERROR([See http://pkgconfig.sourceforge.net])
           PKG_CONFIG=no
         fi
       fi
       UP[]_CONFIG="$PKG_CONFIG"
       LDOWN[]_config="pkg-config"
       m4_pushdef([LIBCONFIG_CFLAGS], [--cflags])
       m4_pushdef([LIBCONFIG_LIBS], [--libs])
       m4_pushdef([LIBCONFIG_VERSION], [--modversion])
      ],
      [AC_PATH_PROG(UP[]_CONFIG, $LDOWN[]_config, no)
       m4_pushdef([LIBCONFIG_CFLAGS], [--cflags])
       m4_pushdef([LIBCONFIG_LIBS], [--libs])
       m4_pushdef([LIBCONFIG_VERSION], [--version])
       if test x$UP[]_CONFIG = xno ; then
         AC_PATH_LIB_ERROR([The $LDOWN[]_config script installed by UP could not be found.])
         AC_PATH_LIB_ERROR([If UP was installed in PREFIX, make sure PREFIX/bin is in])
         AC_PATH_LIB_ERROR([your path, or set the UP[]_CONFIG environment variable to the])
         AC_PATH_LIB_ERROR([full path to $LDOWN[]_config.])
       fi
      ])

if test x$UP[]_CONFIG = xno ; then
  LDOWN[]_present_avoidcache="no"
else
  LDOWN[]_present_avoidcache="yes"

  AC_CACHE_CHECK([for UP CFLAGS],
                 [CACHE[]_cflags],
                 [CACHE[]_cflags=`$UP[]_CONFIG $LDOWN[]_config_args LIBCONFIG_CFLAGS`])
  AC_CACHE_CHECK([for UP LIBS],
                 [CACHE[]_libs],
                 [CACHE[]_libs=`$UP[]_CONFIG $LDOWN[]_config_args LIBCONFIG_LIBS`])
  AC_CACHE_CHECK([for UP VERSION],
                 [CACHE[]_version],
                 [CACHE[]_version=`$UP[]_CONFIG $LDOWN[]_config_args LIBCONFIG_VERSION`])
  UP[]_CFLAGS="$CACHE[]_cflags"
  UP[]_LIBS="$CACHE[]_libs"
  UP[]_VERSION="$CACHE[]_version"
  LDOWN[]_config_major_version=`echo "$CACHE[]_version" | \
      sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
  LDOWN[]_config_minor_version=`echo "$CACHE[]_version" | \
      sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
  LDOWN[]_config_micro_version=`echo "$CACHE[]_version" | \
      sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
  LDOWN[]_min_major_version=`echo "$LDOWN[]_min_version" | \
      sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
  LDOWN[]_min_minor_version=`echo "$LDOWN[]_min_version" | \
      sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
  LDOWN[]_min_micro_version=`echo "$LDOWN[]_min_version" | \
      sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
fi
m4_popdef([LIBCONFIG_CFLAGS])dnl
m4_popdef([LIBCONFIG_LIBS])dnl
m4_popdef([LIBCONFIG_VERSION])dnl
])# _AC_PATH_LIB_CHECK


# _AC_PATH_LIB_FINI(ACTION-IF-FOUND, ACTION-IF-NOT-FOUND)
# -------------------------------------------------------
# Finish: report errors and define/substitute variables.  Run any
# user-supplied code for success or failure.  Restore defaults.
AC_DEFUN([_AC_PATH_LIB_FINI],
[dnl define variables and run extra code
UP[]_CFLAGS="$CACHE[]_cflags"
UP[]_LIBS="$CACHE[]_libs"
UP[]_VERSION="$CACHE[]_version"
AC_SUBST(UP[]_CFLAGS)dnl
AC_SUBST(UP[]_LIBS)dnl
AC_SUBST(UP[]_VERSION)dnl
# Run code which depends upon the test result.
if test x$CACHE[]_present = xyes ; then
  m4_if([$1], , :, [$1])
else
# Restore saved variables if test fails
  UP[]_CFLAGS="$ac_save_[]UP[]_CFLAGS"
  UP[]_LIBS="$ac_save_[]UP[]_LIBS"
  UP[]_VERSION="$ac_save_[]UP[]_VERSION"
  m4_if([$2], , :, [$2])
fi
dnl Restore defaults
AC_PATH_LIB_CHECK_REGISTER_DEFAULTS
AC_PATH_LIB_PKGCONFIG
])# _AC_PATH_LIB_FINI






# _AC_PATH_LIB_CHECK_TESTS(MINIMUM-VERSION)
# -----------------------------------------
# Now check if the installed UP is sufficiently new. (Also sanity
# checks the results of DOWN-config to some extent
AC_DEFUN([_AC_PATH_LIB_CHECK_TESTS],
[AC_CACHE_CHECK([for UP - m4_if([$1], ,
                               [any version],
                               [version >= $LDOWN[]_min_version])],
               [CACHE[]_present],
[CACHE[]_present="$LDOWN[]_present_avoidcache"
if test x$CACHE[]_present = xyes -a x$LDOWN[]_test_enabled = xyes -a \
    x$LDOWN[]_version_test_enabled = xyes ; then
  m4_default([_AC_PATH_LIB_CHECK_TEST_COMPILE],
             [_AC_PATH_LIB_CHECK_TEST_COMPILE],
             [_AC_PATH_LIB_CHECK_TEST_COMPILE_DEFAULT])
else
  m4_default([_AC_PATH_LIB_CHECK_VERSION],
             [_AC_PATH_LIB_CHECK_VERSION],
             [_AC_PATH_LIB_CHECK_VERSION_DEFAULT])
# If the user allowed it, try linking with the library
  if test x$LDOWN[]_test_enabled = xyes ; then
    _AC_PATH_LIB_CHECK_LINK(, [
      CACHE[]_present="no"
      if test x$LDOWN[]_version_test_error = xyes ; then
        AC_PATH_LIB_ERROR
      fi
      AC_PATH_LIB_ERROR([The test program failed to compile or link.  See the file])
      AC_PATH_LIB_ERROR([config.log for the exact error that occured.  This usually])
      AC_PATH_LIB_ERROR([means UP was not installed, was incorrectly installed])
      AC_PATH_LIB_ERROR([or that you have moved UP since it was installed.  In])
      AC_PATH_LIB_ERROR([the latter case, you may want to edit the $LDOWN[]_config])
      AC_PATH_LIB_ERROR([script: $UP[]_CONFIG])
    ])
  fi
fi])
dnl end tests
])# _AC_PATH_LIB_CHECK_TESTS


# _AC_PATH_LIB_CHECK_TEST_COMPILE_DEFAULT
# ---------------------------------------
# Check if the installed UP is sufficiently new. (Also sanity checks
# the results of DOWN-config to some extent.  The test program must
# compile, link and run sucessfully
AC_DEFUN([_AC_PATH_LIB_CHECK_TEST_COMPILE],
[m4_pushdef([RUNLOG], run.[]DOWN[]test)dnl
m4_pushdef([MAJOR], _AC_PATH_LIB_VERSION_PREFIX[]_AC_PATH_LIB_VERSION_MAJOR[]_AC_PATH_LIB_VERSION_SUFFIX)dnl
m4_pushdef([MINOR], _AC_PATH_LIB_VERSION_PREFIX[]_AC_PATH_LIB_VERSION_MINOR[]_AC_PATH_LIB_VERSION_SUFFIX)dnl
m4_pushdef([MICRO], _AC_PATH_LIB_VERSION_PREFIX[]_AC_PATH_LIB_VERSION_MICRO[]_AC_PATH_LIB_VERSION_SUFFIX)dnl
m4_pushdef([DEFMAJOR], _AC_PATH_LIB_DEFVERSION_PREFIX[]_AC_PATH_LIB_DEFVERSION_MAJOR[]_AC_PATH_LIB_DEFVERSION_SUFFIX)dnl
m4_pushdef([DEFMINOR], _AC_PATH_LIB_DEFVERSION_PREFIX[]_AC_PATH_LIB_DEFVERSION_MINOR[]_AC_PATH_LIB_DEFVERSION_SUFFIX)dnl
m4_pushdef([DEFMICRO], _AC_PATH_LIB_DEFVERSION_PREFIX[]_AC_PATH_LIB_DEFVERSION_MICRO[]_AC_PATH_LIB_DEFVERSION_SUFFIX)dnl
  ac_save_CFLAGS="$CFLAGS"
  ac_save_LIBS="$LIBS"
  CFLAGS="$CFLAGS $UP[]_CFLAGS"
  LIBS="$UP[]_LIBS $LIBS"
  rm -f RUNLOG
  AC_TRY_RUN([
#include <$]LDOWN[_header>
#include <stdio.h>
#include <stdlib.h>

/*
 * XXX FIXME Francesco:
 *   This is a pigsty patch for undefined strdup (defined in string.h).
 *   Maybe we should look for strdup() or wrapping it using functions
 *   like malloc && strcpy().
 */
#include <string.h>

int
main ()
{
  int major, minor, micro;
  char *tmp_version;
  FILE *errorlog;

  if ((errorlog = fopen("]ERRORLOG[", "w")) == NULL) {
     exit(1);
   }

  system ("touch ]RUNLOG[");

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = strdup("$]LDOWN[_min_version");
  if (!tmp_version) {
     exit(1);
   }
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {
     fprintf(errorlog, "*** %s: bad version string\n", "$]LDOWN[_min_version");
     exit(1);
   }

  if ((]MAJOR[ != $]LDOWN[_config_major_version) ||
      (]MINOR[ != $]LDOWN[_config_minor_version) ||
      (]MICRO[ != $]LDOWN[_config_micro_version))
    {
      fprintf(errorlog, "*** '$]UP[_CONFIG ]LIBCONFIG_VERSION[' returned %d.%d.%d, but \n", \
        $]LDOWN[_config_major_version, $]LDOWN[_config_minor_version, \
        $]LDOWN[_config_micro_version);
      fprintf(errorlog, "*** ]UP[ (%d.%d.%d) was found!\n", \
        ]MAJOR[, ]MINOR[, ]MICRO[);
      fprintf(errorlog, "***\n");
      fprintf(errorlog, "*** If $]LDOWN[_config was correct, then it is best to remove\n");
      fprintf(errorlog, "*** the old version of ]UP[.  You may also be able to\n");
      fprintf(errorlog, "*** fix the error by modifying your LD_LIBRARY_PATH enviroment\n");
      fprintf(errorlog, "*** variable, or by editing /etc/ld.so.conf.  Make sure you have\n");
      fprintf(errorlog, "*** run ldconfig if that is required on your system.\n");
      fprintf(errorlog, "*** If $]LDOWN[_config was wrong, set the environment\n");
      fprintf(errorlog, "*** variable ]UP[_CONFIG to point to the correct copy of\n");
      fprintf(errorlog, "*** $]LDOWN[_config, and remove the file config.cache\n");
      fprintf(errorlog, "*** before re-running configure.\n");
    }
#if defined (]DEFMAJOR[) && defined (]DEFMINOR[) && defined (]DEFMICRO[)
  else if ((]MAJOR[ != ]DEFMAJOR[) ||
           (]MINOR[ != ]DEFMINOR[) ||
           (]MICRO[ != ]DEFMICRO[))
    {
      fprintf(errorlog, "*** ]UP[ header files (version %d.%d.%d) do not match\n",
             ]DEFMAJOR[, ]DEFMINOR[, ]DEFMICRO[);
      fprintf(errorlog, "*** library (version %d.%d.%d)\n",
             ]MAJOR[, ]MINOR[, ]MICRO[);
    }
#endif /* defined (]DEFMAJOR[) ... */
  else
    {
      if ((]MAJOR[ > major) ||
        ((]MAJOR[ == major) && (]MINOR[  > minor)) ||
        ((]MAJOR[ == major) && (]MINOR[ == minor) && (]MICRO[ >= micro)))
      {
        return 0;
       }
     else
      {
        fprintf(errorlog, "*** An old version of ]UP[ (%d.%d.%d) was found.\n",
               ]MAJOR[, ]MINOR[, ]MICRO[);
        fprintf(errorlog, "*** You need a version of ]UP[ newer than %d.%d.%d.\n",
               major, minor, micro);
        /*fprintf(errorlog, "*** The latest version of ]UP[ is always available from ftp://ftp.my.site\n");*/
        fprintf(errorlog, "***\n");
        fprintf(errorlog, "*** If you have already installed a sufficiently new version, this\n");
        fprintf(errorlog, "*** error probably means that the wrong copy of the $]LDOWN[_config\n");
        fprintf(errorlog, "*** shell script is being found.  The easiest way to fix this is to\n");
        fprintf(errorlog, "*** remove the old version of ]UP[, but you can also set the\n");
        fprintf(errorlog, "*** ]UP[_CONFIG environment variable to point to the correct\n");
        fprintf(errorlog, "*** copy of $]LDOWN[_config.  (In this case, you will have to\n");
        fprintf(errorlog, "*** modify your LD_LIBRARY_PATH environment variable, or edit\n");
        fprintf(errorlog, "*** /etc/ld.so.conf so that the correct libraries are found at\n");
        fprintf(errorlog, "*** run-time.)\n");
      }
    }
  return 1;
}
],, [CACHE[]_present="no"],
    [AC_PATH_LIB_ERROR([cross compiling; assumed OK.])])
  CFLAGS="$ac_save_CFLAGS"
  LIBS="$ac_save_LIBS"

if test -f RUNLOG ; then
  :
elif test x$LDOWN[]_version_test_enabled = xyes ; then
  AC_PATH_LIB_ERROR([Could not run UP test program, checking why...])
  AC_PATH_LIB_ERROR
  _AC_PATH_LIB_CHECK_LINK(dnl
    [AC_PATH_LIB_ERROR([The test program compiled, but did not run.  This usually])
     AC_PATH_LIB_ERROR([means that the run-time linker is not finding UP or])
     AC_PATH_LIB_ERROR([finding the wrong version of UP.  If it is not finding])
     AC_PATH_LIB_ERROR([UP, you will need to set your LD_LIBRARY_PATH environment])
     AC_PATH_LIB_ERROR([variable, or edit /etc/ld.so.conf to point to the installed])
     AC_PATH_LIB_ERROR([location.  Also, make sure you have run ldconfig if that is])
     AC_PATH_LIB_ERROR([required on your system.])
     AC_PATH_LIB_ERROR
     AC_PATH_LIB_ERROR([If you have an old version installed, it is best to remove])
     AC_PATH_LIB_ERROR([it, although you may also be able to get things to work by])
     AC_PATH_LIB_ERROR([modifying LD_LIBRARY_PATH])
    ],
    [AC_PATH_LIB_ERROR([The test program failed to compile or link.  See the file])
     AC_PATH_LIB_ERROR([config.log for the exact error that occured.  This usually])
     AC_PATH_LIB_ERROR([means UP was incorrectly installed or that you have])
     AC_PATH_LIB_ERROR([moved UP since it was installed.  In the latter case,])
     AC_PATH_LIB_ERROR([you may want to edit the $LDOWN[]_config script:])
     AC_PATH_LIB_ERROR([$UP[]_CONFIG])
    ])
fi
rm -f RUNLOG
m4_popdef([RUNLOG])dnl
m4_popdef([MAJOR])dnl
m4_popdef([MINOR])dnl
m4_popdef([MICRO])dnl
m4_popdef([DEFMAJOR])dnl
m4_popdef([DEFMINOR])dnl
m4_popdef([DEFMICRO])dnl
])# _AC_PATH_LIB_CHECK_TEST_COMPILE_DEFAULT


# _AC_PATH_LIB_CHECK_VERSION_DEFAULT
# ----------------------------------
# Check that the library version (config) is greater than or equal to
# the requested (minimum) version.
AC_DEFUN([_AC_PATH_LIB_CHECK_VERSION],
[m4_if([$2], , ,
       [if test x$LDOWN[]_present_avoidcache = xyes ; then
         if test \
             "$LDOWN[]_config_major_version" -lt "$LDOWN[]_min_major_version" -o \
             "$LDOWN[]_config_major_version" -eq "$LDOWN[]_min_major_version" -a \
             "$LDOWN[]_config_minor_version" -lt "$LDOWN[]_min_minor_version" -o \
             "$LDOWN[]_config_major_version" -eq "$LDOWN[]_min_major_version" -a \
             "$LDOWN[]_config_minor_version" -eq "$LDOWN[]_min_minor_version" -a \
             "$LDOWN[]_config_micro_version" -lt "$LDOWN[]_min_micro_version" ; then
           CACHE[]_present="no"
           LDOWN[]_version_test_error="yes"
           AC_PATH_LIB_ERROR([$UP[]_CONFIG --version' returned $LDOWN[]_config_major_version.$LDOWN[]_config_minor_version.$LDOWN[]_config_micro_version, but])
           AC_PATH_LIB_ERROR([UP (>= $LDOWN[]_min_version) was needed.])
           AC_PATH_LIB_ERROR
           AC_PATH_LIB_ERROR([If $]LDOWN[_config was wrong, set the environment])
           AC_PATH_LIB_ERROR([variable ]UP[_CONFIG to point to the correct copy of])
           AC_PATH_LIB_ERROR([$]LDOWN[_config, and remove the file config.cache])
           AC_PATH_LIB_ERROR([before re-running configure.])
         else
           CACHE[]_present="yes"
         fi
       fi])
])# _AC_PATH_LIB_CHECK_VERSION_DEFAULT


# _AC_PATH_LIB_CHECK_LINK_DEFAULT(SUCCESS, FAIL)
# ----------------------------------------------
# Check if the library will link successfully.  If specified, run
# SUCCESS or FAIL on success or failure
AC_DEFUN([_AC_PATH_LIB_CHECK_LINK],
[ac_save_CFLAGS="$CFLAGS"
ac_save_LIBS="$LIBS"
CFLAGS="$CFLAGS $UP[]_CFLAGS"
LIBS="$LIBS $UP[]_LIBS"
AC_TRY_LINK([ #include <stdio.h> ], ,
            [m4_if([$1], , :, [$1])],
            [m4_if([$2], , :, [$2])])
CFLAGS="$ac_save_CFLAGS"
LIBS="$ac_save_LIBS"
])# _AC_PATH_LIB_CHECK_LINK_DEFAULT






# AC_PATH_LIB_ERROR(MESSAGE)
# --------------------------
# Print an error message, MESSAGE, to the error log.
AC_DEFUN([AC_PATH_LIB_ERROR],
[echo '*** m4_if([$1], , , [$1])' >>ERRORLOG])


# _AC_PATH_LIB_ERROR_DUMP
# -----------------------
# Print the error log (after main AC_CACHE_CHECK completes).
AC_DEFUN([_AC_PATH_LIB_ERROR_DUMP],
[if test -f ERRORLOG ; then
  cat ERRORLOG
  rm -f ERRORLOG
fi])






# AC_PATH_LIB_PKGCONFIG
# ---------------------
# Enable pkgconfig support in libconfig script (default).
AC_DEFUN([AC_PATH_LIB_PKGCONFIG],
[m4_define([AC_PATH_LIB_USEPKGCONFIG], [true])
])dnl


# AC_PATH_LIB_LIBCONFIG
# ---------------------
# Disable pkgconfig support in libconfig script.
AC_DEFUN([AC_PATH_LIB_LIBCONFIG],
[m4_define([AC_PATH_LIB_USEPKGCONFIG], [false])
])dnl


# AC_PATH_LIB_REGISTER (MACRO, REPLACEMENT)
# -----------------------------------------
# Register a macro to replace the default checks.  Use the REPLACEMENT
# macro for the check macro MACRO.
#
# Possible MACROs are:
#   _AC_PATH_LIB_CHECK_COMPILE and
#   _AC_PATH_LIB_CHECK_VERSION
# You should make sure that replacement macros use the same arguments
# (if any), and do error logging in the same manner and behave in the
# same way as the original.

# Non-default header names may be specified, as well as version
# variable names in the library itself (needed for
# _AC_PATH_LIB_CHECK_COMPILE):
#   _AC_PATH_LIB_HEADER
#   _AC_PATH_LIB_VERSION_PREFIX (default library_)
#   _AC_PATH_LIB_VERSION_MAJOR (default major)
#   _AC_PATH_LIB_VERSION_MINOR (default minor)
#   _AC_PATH_LIB_VERSION_MICRO (default micro)
#   _AC_PATH_LIB_VERSION_SUFFIX (default _version)
#   _AC_PATH_LIB_DEFVERSION_PREFIX (default LIBRARY_)
#   _AC_PATH_LIB_DEFVERSION_MAJOR (default MAJOR)
#   _AC_PATH_LIB_DEFVERSION_MINOR (default MINOR)
#   _AC_PATH_LIB_DEFVERSION_MICRO (default MICRO)
#   _AC_PATH_LIB_DEFVERSION_SUFFIX (default _VERSION)
# For example, library_major_version.
AC_DEFUN([AC_PATH_LIB_REGISTER],
[m4_define([$1], [$2])])


# AC_PATH_LIB_CHECK_REGISTER_DEFAULTS
# -----------------------------------
# Restore the default check macros.
AC_DEFUN([AC_PATH_LIB_CHECK_REGISTER_DEFAULTS],
[_AC_PATH_LIB_CHECK_REGISTER_DEFAULTS([_AC_PATH_LIB_CHECK_COMPILE],
                                       [_AC_PATH_LIB_CHECK_VERSION],
                                       [_AC_PATH_LIB_HEADER],
                                       [_AC_PATH_LIB_VERSION_PREFIX],
                                       [_AC_PATH_LIB_VERSION_MAJOR],
                                       [_AC_PATH_LIB_VERSION_MINOR],
                                       [_AC_PATH_LIB_VERSION_MICRO],
                                       [_AC_PATH_LIB_VERSION_SUFFIX],
                                       [_AC_PATH_LIB_DEFVERSION_PREFIX],
                                       [_AC_PATH_LIB_DEFVERSION_MAJOR],
                                       [_AC_PATH_LIB_DEFVERSION_MINOR],
                                       [_AC_PATH_LIB_DEFVERSION_MICRO],
                                       [_AC_PATH_LIB_DEFVERSION_SUFFIX])
])# AC_PATH_LIB_CHECK_REGISTER_DEFAULTS


# _AC_PATH_LIB_CHECK_REGISTER_DEFAULTS(MACROs ...)
# ------------------------------------------------
# Undefine MACROs.
AC_DEFUN([AC_PATH_LIB_CHECK_REGISTER_DEFAULTS],
[m4_if([$1], , ,
       [m4_ifdef([$1],
                 [m4_undefine([$1])])
        AC_PATH_LIB_CHECK_REGISTER_DEFAULTS(m4_shift($@))
       ])
])

dnl ______ /usr/share/aclocal/Installed_Packages/acx_pthread.m4 ______
dnl @synopsis ACX_PTHREAD([ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]])
dnl
dnl This macro figures out how to build C programs using POSIX
dnl threads.  It sets the PTHREAD_LIBS output variable to the threads
dnl library and linker flags, and the PTHREAD_CFLAGS output variable
dnl to any special C compiler flags that are needed.  (The user can also
dnl force certain compiler flags/libs to be tested by setting these
dnl environment variables.)
dnl
dnl Also sets PTHREAD_CC to any special C compiler that is needed for
dnl multi-threaded programs (defaults to the value of CC otherwise).
dnl (This is necessary on AIX to use the special cc_r compiler alias.)
dnl
dnl If you are only building threads programs, you may wish to
dnl use these variables in your default LIBS, CFLAGS, and CC:
dnl
dnl        LIBS="$PTHREAD_LIBS $LIBS"
dnl        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
dnl        CC="$PTHREAD_CC"
dnl
dnl In addition, if the PTHREAD_CREATE_JOINABLE thread-attribute
dnl constant has a nonstandard name, defines PTHREAD_CREATE_JOINABLE
dnl to that name (e.g. PTHREAD_CREATE_UNDETACHED on AIX).
dnl
dnl ACTION-IF-FOUND is a list of shell commands to run if a threads
dnl library is found, and ACTION-IF-NOT-FOUND is a list of commands
dnl to run it if it is not found.  If ACTION-IF-FOUND is not specified,
dnl the default action will define HAVE_PTHREAD.
dnl
dnl Please let the authors know if this macro fails on any platform,
dnl or if you have any other suggestions or comments.  This macro was
dnl based on work by SGJ on autoconf scripts for FFTW (www.fftw.org)
dnl (with help from M. Frigo), as well as ac_pthread and hb_pthread
dnl macros posted by AFC to the autoconf macro repository.  We are also
dnl grateful for the helpful feedback of numerous users.
dnl
dnl @version %Id: acx_pthread.m4,v 1.3 2002/12/12 23:15:12 guidod Exp %
dnl @author Steven G. Johnson <stevenj@alum.mit.edu> and Alejandro Forero Cuervo <bachue@bachue.com>

AC_DEFUN([ACX_PTHREAD], [
AC_REQUIRE([AC_CANONICAL_HOST])
AC_LANG_SAVE
AC_LANG_C
acx_pthread_ok=no

# We used to check for pthread.h first, but this fails if pthread.h
# requires special compiler flags (e.g. on True64 or Sequent).
# It gets checked for in the link test anyway.

# First of all, check if the user has set any of the PTHREAD_LIBS,
# etcetera environment variables, and if threads linking works using
# them:
if test x"$PTHREAD_LIBS$PTHREAD_CFLAGS" != x; then
        save_CFLAGS="$CFLAGS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
        save_LIBS="$LIBS"
        LIBS="$PTHREAD_LIBS $LIBS"
        AC_MSG_CHECKING([for pthread_join in LIBS=$PTHREAD_LIBS with CFLAGS=$PTHREAD_CFLAGS])
        AC_TRY_LINK_FUNC(pthread_join, acx_pthread_ok=yes)
        AC_MSG_RESULT($acx_pthread_ok)
        if test x"$acx_pthread_ok" = xno; then
                PTHREAD_LIBS=""
                PTHREAD_CFLAGS=""
        fi
        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"
fi

# We must check for the threads library under a number of different
# names; the ordering is very important because some systems
# (e.g. DEC) have both -lpthread and -lpthreads, where one of the
# libraries is broken (non-POSIX).

# Create a list of thread flags to try.  Items starting with a "-" are
# C compiler flags, and other items are library names, except for "none"
# which indicates that we try without any flags at all.

acx_pthread_flags="pthreads none -Kthread -kthread lthread -pthread -pthreads -mthreads pthread --thread-safe -mt"

# The ordering *is* (sometimes) important.  Some notes on the
# individual items follow:

# pthreads: AIX (must check this before -lpthread)
# none: in case threads are in libc; should be tried before -Kthread and
#       other compiler flags to prevent continual compiler warnings
# -Kthread: Sequent (threads in libc, but -Kthread needed for pthread.h)
# -kthread: FreeBSD kernel threads (preferred to -pthread since SMP-able)
# lthread: LinuxThreads port on FreeBSD (also preferred to -pthread)
# -pthread: Linux/gcc (kernel threads), BSD/gcc (userland threads)
# -pthreads: Solaris/gcc
# -mthreads: Mingw32/gcc, Lynx/gcc
# -mt: Sun Workshop C (may only link SunOS threads [-lthread], but it
#      doesn't hurt to check since this sometimes defines pthreads too;
#      also defines -D_REENTRANT)
# pthread: Linux, etcetera
# --thread-safe: KAI C++

case "${host_cpu}-${host_os}" in
        *solaris*)

        # On Solaris (at least, for some versions), libc contains stubbed
        # (non-functional) versions of the pthreads routines, so link-based
        # tests will erroneously succeed.  (We need to link with -pthread or
        # -lpthread.)  (The stubs are missing pthread_cleanup_push, or rather
        # a function called by this macro, so we could check for that, but
        # who knows whether they'll stub that too in a future libc.)  So,
        # we'll just look for -pthreads and -lpthread first:

        acx_pthread_flags="-pthread -pthreads pthread -mt $acx_pthread_flags"
        ;;
esac

if test x"$acx_pthread_ok" = xno; then
for flag in $acx_pthread_flags; do

        case $flag in
                none)
                AC_MSG_CHECKING([whether pthreads work without any flags])
                ;;

                -*)
                AC_MSG_CHECKING([whether pthreads work with $flag])
                PTHREAD_CFLAGS="$flag"
                ;;

                *)
                AC_MSG_CHECKING([for the pthreads library -l$flag])
                PTHREAD_LIBS="-l$flag"
                ;;
        esac

        save_LIBS="$LIBS"
        save_CFLAGS="$CFLAGS"
        LIBS="$PTHREAD_LIBS $LIBS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"

        # Check for various functions.  We must include pthread.h,
        # since some functions may be macros.  (On the Sequent, we
        # need a special flag -Kthread to make this header compile.)
        # We check for pthread_join because it is in -lpthread on IRIX
        # while pthread_create is in libc.  We check for pthread_attr_init
        # due to DEC craziness with -lpthreads.  We check for
        # pthread_cleanup_push because it is one of the few pthread
        # functions on Solaris that doesn't have a non-functional libc stub.
        # We try pthread_create on general principles.
        AC_TRY_LINK([#include <pthread.h>],
                    [pthread_t th; pthread_join(th, 0);
                     pthread_attr_init(0); pthread_cleanup_push(0, 0);
                     pthread_create(0,0,0,0); pthread_cleanup_pop(0); ],
                    [acx_pthread_ok=yes])

        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"

        AC_MSG_RESULT($acx_pthread_ok)
        if test "x$acx_pthread_ok" = xyes; then
                break;
        fi

        PTHREAD_LIBS=""
        PTHREAD_CFLAGS=""
done
fi

# Various other checks:
if test "x$acx_pthread_ok" = xyes; then
        save_LIBS="$LIBS"
        LIBS="$PTHREAD_LIBS $LIBS"
        save_CFLAGS="$CFLAGS"
        CFLAGS="$CFLAGS $PTHREAD_CFLAGS"

        # Detect AIX lossage: threads are created detached by default
        # and the JOINABLE attribute has a nonstandard name (UNDETACHED).
        AC_MSG_CHECKING([for joinable pthread attribute])
        AC_TRY_LINK([#include <pthread.h>],
                    [int attr=PTHREAD_CREATE_JOINABLE;],
                    ok=PTHREAD_CREATE_JOINABLE, ok=unknown)
        if test x"$ok" = xunknown; then
                AC_TRY_LINK([#include <pthread.h>],
                            [int attr=PTHREAD_CREATE_UNDETACHED;],
                            ok=PTHREAD_CREATE_UNDETACHED, ok=unknown)
        fi
        if test x"$ok" != xPTHREAD_CREATE_JOINABLE; then
                AC_DEFINE(PTHREAD_CREATE_JOINABLE, $ok,
                          [Define to the necessary symbol if this constant
                           uses a non-standard name on your system.])
        fi
        AC_MSG_RESULT(${ok})
        if test x"$ok" = xunknown; then
                AC_MSG_WARN([we do not know how to create joinable pthreads])
        fi

        AC_MSG_CHECKING([if more special flags are required for pthreads])
        flag=no
        case "${host_cpu}-${host_os}" in
                *-aix* | *-freebsd*)     flag="-D_THREAD_SAFE";;
                *solaris* | *-osf* | *-hpux*) flag="-D_REENTRANT";;
        esac
        AC_MSG_RESULT(${flag})
        if test "x$flag" != xno; then
                PTHREAD_CFLAGS="$flag $PTHREAD_CFLAGS"
        fi

        LIBS="$save_LIBS"
        CFLAGS="$save_CFLAGS"

        # More AIX lossage: must compile with cc_r
        AC_CHECK_PROG(PTHREAD_CC, cc_r, cc_r, ${CC})
else
        PTHREAD_CC="$CC"
fi

AC_SUBST(PTHREAD_LIBS)
AC_SUBST(PTHREAD_CFLAGS)
AC_SUBST(PTHREAD_CC)

# Finally, execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
if test x"$acx_pthread_ok" = xyes; then
        ifelse([$1],,AC_DEFINE(HAVE_PTHREAD,1,[Define if you have POSIX threads libraries and header files.]),[$1])
        :
else
        acx_pthread_ok=no
        $2
fi
AC_LANG_RESTORE
])dnl ACX_PTHREAD
