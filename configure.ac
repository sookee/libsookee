# /*-----------------------------------------------------------------.
# | Copyright (C) 2011 SooKee oasookee@gmail.com               |
# '------------------------------------------------------------------'
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA.
# 
# http://www.gnu.org/licenses/gpl-2.0.html
# 
# '-----------------------------------------------------------------*/

AC_INIT([libsookee],[0.1],[oasookee@gmail.com],[libsookee])
AC_CONFIG_SRCDIR([src/Makefile.am])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.10 foreign])
AM_SILENT_RULES
AM_PROG_LIBTOOL
#AC_CHECK_LIB(pcre, main,, AC_MSG_ERROR([libpcre not found]))
#AC_CHECK_LIB(pcrecpp, main,, AC_MSG_ERROR([libpcrecpp not found]))

AC_PROG_CXX
AC_PROG_INSTALL

AC_LANG(C++)
AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])
 
AC_CONFIG_MACRO_DIR([m4])

AC_ARG_WITH([speller], AS_HELP_STRING([--with-speller], [Build the speller library]))
AS_IF([test "x$with_speller" = "xyes"], [
	PKG_CHECK_MODULES([ASPELL], [aspell], [HAVE_ASPELL=1], [HAVE_ASPELL=0])
])
AM_CONDITIONAL([MAKE_SPELLER], [test "$HAVE_ASPELL" -eq 1])

AX_LIB_MYSQL([5.0])
AM_CONDITIONAL([MAKE_DB], [test "$found_mysql" = "yes"])

AC_LANG_CPLUSPLUS
AC_CONFIG_FILES([
	pkg-config/libsookee.pc
	pkg-config/libsookee-db.pc
	pkg-config/libsookee-speller.pc
	Makefile
	src/Makefile
	src/test/Makefile])
AC_OUTPUT
