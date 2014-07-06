# /*-----------------------------------------------------------------.
# | Copyright (C) 2013 SooKee oasookee@gmail.com                |
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

sookee_includedir = $(prefix)/include/sookee
sookee_include_typesdir = $(prefix)/include/sookee/types

sookee_librarydir = $(prefix)/lib/sookee

DEF_FLAGS = \
	-D _GLIBCXX_USE_NANOSLEEP -D DEBUG
	
GEN_FLAGS = -O0 -g3 -Wl,-E -Wfatal-errors -Wall -Wextra -Winit-self -Werror -ansi -pedantic -pipe -pthread \
	-I$(top_srcdir)/src/include

AM_CXXFLAGS = -std=c++11 $(GEN_FLAGS) $(DEF_FLAGS) $(LIBXML2_FLAG)

sookee_include_HEADERS = $(wildcard $(srcdir)/include/sookee/*.h)	
sookee_include_types_HEADERS = $(wildcard $(srcdir)/include/sookee/types/*.h)

sookee_library_LTLIBRARIES = \
	libsookee.la

# IrcBot framework
libsookee_la_SOURCES = \
	bug.cpp \
	cal.cpp \
	ios.cpp \
	log.cpp \
	network.cpp \
	str.cpp \
	types.cpp
	
libsookee_la_LIBADD = 
libsookee_la_LDFLAGS = -version-info 0:0:0 -rdynamic -pthread

noinst_PROGRAMS = test

test_SOURCES = test.cpp
test_LDADD = -L.libs -lsookee

