#!/bin/bash

source ${0%*.sh}.conf 2> /dev/null

top_dir=$(pwd)

PREFIX=${PREFIX:-$HOME}
LIBDIR=$PREFIX/lib

MAKE_DB={$MAKE_DB:-no}
MAKE_I18N={$MAKE_I18N:-no}
MAKE_SPELLER={$MAKE_SPELLER:-no}

WITH_DB="--with-mysql=$MAKE_DB"
WITH_I18N="--with-icu=$MAKE_I18N"
WITH_SPELLER="--with-speller=$MAKE_SPELLER"

export PKG_CONFIG_PATH="$LIBDIR/pkgconfig"

export CXXFLAGS="-g0 -O3 -D NDEBUG"

rm -fr $top_dir/build-release
mkdir -p $top_dir/build-release

cd $top_dir/build-release
$top_dir/configure $WITH_DB $WITH_I18N $WITH_SPELLER --prefix=$PREFIX --enable-silent-rules



