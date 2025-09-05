# configure.ac: Autoconf template for bash-dot-files.
#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([inhibit-suspend], [0.0.1], [])
AM_INIT_AUTOMAKE([foreign])

exec_prefix=/usr

dnl AS_IF(
dnl   [test ."$bindir" = .'${exec_prefix}/bin'],
dnl     [bindir=$HOME])

# Checks for programs.
AC_PROG_CC

AC_CHECK_FUNCS([strcasecmp strchr strerror strncasecmp strrchr strtol])
AC_CHECK_HEADER_STDBOOL

LOCAL_PREFIX=/usr/local/sbin${PATH_SEPARATOR}/usr/local/bin${PATH_SEPARATOR}

AC_MSG_CHECKING([whether to search optional path])
AC_ARG_WITH([opt-path],
  [AS_HELP_STRING([--with-opt-path@<:@=PATH@:>@],
    [search optional PATH @{:@default: /opt/local/bin@:}@])],
  [], [with_opt_path=check])
if test ."$with_opt_path" = .'yes' || test ."$with_opt_path" = .'check'; then
    if test -d /opt/local/bin; then
        OPT_PREFIX=/opt/local/sbin${PATH_SEPARATOR}/opt/local/bin
    fi
elif test ! ."$with_opt_path" = .'no'; then
    if test -d "$with_opt_path"; then
        OPT_PREFIX=${with_opt_path}
    else
        AC_MSG_WARN([${with_opt_path}: No such file or directory])
    fi
fi
AC_MSG_RESULT([${OPT_PREFIX:-no}])
OPT_PATHS=$(echo "$OPT_PREFIX" | tr "${PATH_SEPARATOR}" ' ')
AC_SUBST([OPT_PATHS], ["$OPT_PATHS"])
OPT_PREFIX=${OPT_PREFIX:+"${OPT_PREFIX}${PATH_SEPARATOR}"}

# Checks for programs.
AC_PATH_PROGS([CAT_CMD],        [gcat cat],           [], [${OPT_PREFIX}${PATH}])

AC_PATH_PROG([GETOPT_CMD],      [getopt],             [], [${OPT_PREFIX}${PATH}])
AC_PATH_PROG([INSTALL_CMD],     [install],            [], [${OPT_PREFIX}${PATH}])
AC_PATH_PROG([KILL_CMD],        [kill],               [], [${OPT_PREFIX}${PATH}])
AC_PATH_PROG([PGREP_CMD],       [pgrep],              [], [${OPT_PREFIX}${PATH}])

AC_PATH_PROGS([READLINK_CMD],   [greadlink readlink], [], [${OPT_PREFIX}${PATH}])
AC_PATH_PROGS([REALPATH_CMD],   [grealpath realpath], [], [${OPT_PREFIX}${PATH}])
AC_PATH_PROGS([RM_CMD],         [grm rm],             [], [${OPT_PREFIX}${PATH}])
AC_PATH_PROGS([SLEEP_CMD],      [gsleep sleep],       [], [${OPT_PREFIX}${PATH}])

AC_PATH_PROG([SUDO_CMD],        [sudo],               [], [${OPT_PREFIX}${PATH}])
AC_PATH_PROG([SYSTEMCTL_CMD],   [systemctl])
AC_PATH_PROG([SHUTDOWN_CMD],    [shutdown])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([restart],                            [chmod +x restart])

AC_OUTPUT
