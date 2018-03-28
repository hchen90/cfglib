lib_LTLIBRARIES=libsec.la
libsec_la_SOURCES=section.c
include_HEADERS=section.h

if EXAMPLES
bin_PROGRAMS=section_testc
section_testc_SOURCES=test.c
section_testc_LDADD=libsec.la
endif

EXTRA_DIST=test.cfg
