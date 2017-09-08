dnl run `autoreconf -i` to generate a configure script.
dnl Then run ./configure to generate a Makefile.
dnl Finally run make to generate the project.

AC_INIT([Sambada], m4_esyscmd([./git-version-gen .tarball-version]), [sylvie.stucki@a3.epfl.ch], [sambada], [lasig.epfl.ch/sambada])
AM_INIT_AUTOMAKE([foreign subdir-objects dist-zip])

AC_PROG_CXX
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
tiny_sam_system_name=""

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

# Check for presence of latexmk
AC_CHECK_PROG(LATEXMK, latexmk, latexmk)
if test -z "$LATEXMK"; then
	AC_MSG_WARN([Unable to create PDF version of the user manual.])
fi

AM_CONDITIONAL([HAVE_LATEXMK], test -n "$LATEXMK")

AC_OUTPUT
