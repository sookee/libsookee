#!/bin/bash

source ${0%*.sh}.conf 2> /dev/null

top_dir=$(pwd)

PREFIX=${PREFIX:-$HOME}
LIBDIR=$PREFIX/lib

WITH="$WITH --with-mysql=${MAKE_DB:-no}"
#WITH="$WITH --with-icu=${MAKE_I18N:-no}"
WITH="$WITH --with-speller=${MAKE_SPELLER:-no}"

export PKG_CONFIG_PATH="$LIBDIR/pkgconfig"

export CXXFLAGS="-g0 -O3 -D NDEBUG"

rm -fr $top_dir/build-release
mkdir -p $top_dir/build-release

cd $top_dir/build-release
#$top_dir/configure $WITH --prefix=$PREFIX
$top_dir/configure $WITH --prefix=$PREFIX --enable-silent-rules



