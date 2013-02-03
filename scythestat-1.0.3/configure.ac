dnl init and sanity test
AC_INIT(Scythe Statistical Library, 1.0.3, dpemstei@olemiss.edu, scythestat)
AC_PREREQ([2.58])
AC_COPYRIGHT(GNU General Public License)
AC_CONFIG_SRCDIR(scythestat/matrix.h)

dnl Put all the autoconf mess in a a subdir
AC_CONFIG_AUX_DIR(config)
dnl AC_CONFIG_MACRO_DIR(m4)

dnl What platform are we on
AC_CANONICAL_TARGET([])

dnl Doxygen support: document this out when doing a make dist
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN(Scythe, doc/doxygen.conf)

dnl initialize automake; ask for 1.7 or better
AM_INIT_AUTOMAKE([$PACKAGE_TARNAME], [$PACKAGE_VERSION])

dnl scythestat_config.h macros, XXX should probably make a move to the
dnl big leagues, but need to think about direct compilation first
dnl AM_CONFIG_HEADER(config/config.h)
dnl AX_PREFIX_CONFIG_H(scythestat/_config.h)

dnl Set the library version number.  This is different from
dnl the Scythe package version number, and follows a strict
dnl format.  Violating this format may break programs that use
dnl Scythe.
dnl
dnl CURRENT.REVISION.AGE
dnl
dnl CURRENT
dnl   Increment this number when the interface changes.
dnl REVISION
dnl   Increment this number when the implementation changes
dnl   without changing the interface.  Roll over to 0 on
dnl   CURRENT++
dnl AGE
dnl   The difference between the oldest and newest interfaces
dnl   that the library implements.  If CURRENT=7 but the
dnl   library implements the (deprecated) 6, 5, and 4 level
dnl   interfaces, AGE=7-4=3
VERSION_INFO=1:3:0
AC_SUBST(VERSION_INFO)

dnl Check for the C++ compiler and preprocessor
AC_PROG_CXX
AC_PROG_CXXCPP

dnl Set the test language as C++
AC_LANG([C++])

AC_CONFIG_FILES([Makefile \
	scythestat/Makefile \
	scythestat/rng/Makefile])

dnl  Output the files
AC_OUTPUT
