#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([cfglib], [1.0.1], [a.chenxiang.z@gmail.com])
AC_CONFIG_SRCDIR([xml/xml.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])
AM_INIT_AUTOMAKE
AM_SILENT_RULES([yes])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LD
AC_PROG_INSTALL
AM_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])

# Examples
EXAMPLES="xml_testc xml_testcxx section_testc"

AC_ARG_ENABLE([examples],
[  --enable-examples       build example programs],
[case "${enableval}" in
 yes) build_examples=true;;
 no) build_examples=false;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-examples]);;
 esac],[build_examples=false])
AM_CONDITIONAL([EXAMPLES],[test x$build_examples = xtrue])
AC_SUBST([EXAMPLES])

AC_CONFIG_FILES([Makefile section/Makefile xml/Makefile])
AC_OUTPUT

