include $(top_srcdir)/aminclude.am

EXTRA_DIST = LICENSE
SUBDIRS = scythestat

## Include the m4 subdirectory when looking for macros
ACLOCAL_AMFLAGS = -I m4

## Create the package.m4 file required by autotest
$(srcdir)/package.m4: $(top_srcdir)/configure.ac
	{                                      \
		echo '# Signature of the current package.'; \
		echo 'm4_define([AT_PACKAGE_NAME],      [@PACKAGE_NAME@])'; \
		echo 'm4_define([AT_PACKAGE_TARNAME],   [@PACKAGE_TARNAME@])'; \
		echo 'm4_define([AT_PACKAGE_VERSION],   [@PACKAGE_VERSION@])'; \
		echo 'm4_define([AT_PACKAGE_STRING],    [@PACKAGE_STRING@])'; \
		echo 'm4_define([AT_PACKAGE_BUGREPORT], [@PACKAGE_BUGREPORT@])'; \
	} >$(srcdir)/package.m4
