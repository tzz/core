AM_CFLAGS = $(ENTERPRISE_CFLAGS) -I$(top_srcdir)/libpromises -I$(srcdir)/../../libutils

noinst_PROGRAMS =

if !BUILTIN_EXTENSIONS
noinst_PROGRAMS += mock-package-manager

mock_package_manager_SOURCES = mock-package-manager.c
mock_package_manager_LDADD = ../../libpromises/libpromises.la
endif # !BUILTIN_EXTENSIONS

if HAVE_LIBXML2
noinst_PROGRAMS += xml-c14nize

xml_c14nize_CPPFLAGS = $(LIBXML2_CPPFLAGS)
xml_c14nize_CFLAGS = -I$(top_srcdir)/libpromises -I$(srcdir)/../../libutils \
    $(LIBXML2_CFLAGS)
xml_c14nize_LDADD = ../../libutils/libutils.la \
    $(LIBXML2_LIBS)
endif

EXTRA_DIST = default.cf.sub testall

TESTS = testall
TESTS_ENVIRONMENT = env \
	AGENT=`pwd`/../../cf-agent/cf-agent \
	CF_PROMISES=`pwd`/../../cf-promises/cf-promises \
	CF_SERVERD=`pwd`/../../cf-serverd/cf-serverd \
	CF_KEY=`pwd`/../../cf-key/cf-key \
	MOCK_PACKAGE_MANAGER=`pwd`/mock-package-manager
if !HAVE_LIBXML2
TESTS_ENVIRONMENT += LIBXML2_TESTS=0
endif

dist-hook:
	cd $(srcdir); tar -c -f - [0-9]* | tar -x -C $(abspath $(distdir)) -f -
	chmod -R go-w $(distdir)

CLEANFILES = *.gcno *.gcda
