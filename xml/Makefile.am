lib_LTLIBRARIES=libxml.la
libxml_la_SOURCES=xml.c
include_HEADERS=xml.h xml.hxx

if EXAMPLES
bin_PROGRAMS=xml_testc xml_testcxx
xml_testc_SOURCES=test.c
xml_testc_LDADD=libxml.la
xml_testcxx_SOURCES=testc.cxx
xml_testcxx_LDADD=libxml.la
endif

EXTRA_DIST=test.xml manual.htm genhdr.pl
