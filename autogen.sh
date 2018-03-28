#!/bin/sh
set -x
aclocal
autoheader
libtoolize --copy --install
automake --gnu --add-missing --copy
autoconf

