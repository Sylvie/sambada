##########################################################################
# Copyright (©) 2011-2018 EPFL (Ecole Polytechnique fédérale de Lausanne)
# Laboratory of Geographic information systems (LaSIG)
#
# This file is part of Sambada.
#
# Sambada is free software ; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation ; either version 3 of the License, or (at your option) any later version.
# Sambada is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY ; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
# You should have received a copy of the GNU General Public License along with Sambada ; if not, see <http://www.gnu.org/licenses/>.
#
# Authors : Sylvie Stucki (sylvie.stucki@a3.epfl.ch), Stéphane Joost (stephane.joost@epfl.ch)
# Laboratory of Geographic information systems
# EPFL ENAC IIE LASIG
# Station 18
# CH-1015 Lausanne
# Web site : http://lasig.epfl.ch/sambada
#
# Sambada includes two libraries: Scythe Statistical Library (under GPL 3) and Shapefile C Library (under LGPL 2.1, courtesy of Frank Warmerdam).
#
# Scythe Statistical Library
# Copyright (C) 2000-2002 Andrew D. Martin and Kevin M. Quinn;
# 2002-2012 Andrew D. Martin, Kevin M. Quinn, and Daniel Pemstein.  All Rights Reserved.
#
# Shapefile C Library
# Copyright (c) 1999, Frank Warmerdam
##########################################################################

dnl run `autoreconf -i` to generate a configure script.
dnl Then run ./configure to generate a Makefile.
dnl Finally run make to generate the project.

AC_INIT([Sambada], m4_esyscmd([./git-version-gen .tarball-version]), [sylvie.stucki@a3.epfl.ch], [sambada], [lasig.epfl.ch/sambada])
AM_INIT_AUTOMAKE([foreign subdir-objects dist-zip tar-pax])

AC_CONFIG_MACRO_DIR([m4])

sambada_releasedate=m4_esyscmd([./git-releasedate-gen .tarball-releasedate])
AC_SUBST(sambada_releasedate)

AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])
AC_PROG_CC
AC_PROG_RANLIB

AC_CONFIG_FILES([Makefile])


# AC_CANONICAL_HOST is needed to access the 'host_os' variable
AC_CANONICAL_HOST
AC_MSG_NOTICE([host_os = ${host_os}])

# Defining system identifier
build_linux=no
build_windows=no
build_mac=no
system_name=""

# Detect the target system
case "${host_os}" in
linux*)
build_linux=yes
system_name="Linux"
;;
cygwin*|mingw*)
build_windows=yes
system_name="Windows"
;;
darwin*)
build_mac=yes
system_name="OSX"
;;
*)
system_name="UnknownOS"
AC_MSG_WARN(["OS $host_os is not known"])
;;
esac

AC_ARG_VAR(sambadahostsystemname, [Use this argument to provide a more specific OS name than the guessed one.])

# Use the system name provided by the user if any
if test "${sambadahostsystemname}" ; then
	system_name="${sambadahostsystemname}"
fi
AC_MSG_NOTICE([system_name = ${system_name}])
AC_SUBST(system_name)

# Pass the conditionals to automake
AM_CONDITIONAL([LINUX], [test "$build_linux" = "yes"])
AM_CONDITIONAL([WINDOWS], [test "$build_windows" = "yes"])
AM_CONDITIONAL([OSX], [test "$build_mac" = "yes"])

# Pass the system's extension for executable to automake
sambada_executable_extension=${EXEEXT}
AC_MSG_NOTICE([sambada_executable_extension = ${sambada_executable_extension}])
AC_SUBST(sambada_executable_extension)

# Pass the system's folder delimiter to automake
AS_IF([test "x${build_windows}" = xyes], [sambada_folder_separator="\\\\"], [sambada_folder_separator="/"])
AC_MSG_NOTICE([sambada_folder_separator = ${sambada_folder_separator}])
AC_DEFINE_UNQUOTED([sambada_folder_separator], ["${sambada_folder_separator}"])

# Check for presence of latexmk
AC_CHECK_PROG(LATEXMK, latexmk, latexmk)
if test -z "$LATEXMK"; then
	AC_MSG_WARN([Unable to create PDF version of the user manual.])
fi

# Check if the user disabled the manual
AC_ARG_ENABLE([manual],
            [AS_HELP_STRING([--disable-manual],
              [disable compilation of user manual])],
            [],
            [with_manual=yes])

# The manual is compiled if latexmk is available and if the user did not disable it
AM_CONDITIONAL([HAVE_LATEXMK], test -n "$LATEXMK" -a x${with_manual} = xyes )

AC_OUTPUT
