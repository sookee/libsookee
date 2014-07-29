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
	-D DEBUG
	
GEN_FLAGS = -Wl,-E -Wfatal-errors -Wall -Wextra -Winit-self -Werror -ansi -pedantic -pipe -pthread \
	-I$(top_srcdir)/src/include

AM_CXXFLAGS = $(GEN_FLAGS) $(DEF_FLAGS)

sookee_include_HEADERS = $(wildcard $(srcdir)/include/sookee/*.h)	
sookee_include_types_HEADERS = $(wildcard $(srcdir)/include/sookee/types/*.h)

sookee_library_LTLIBRARIES = \
	libsookee.la

libsookee_la_SOURCES = \
	bug.cpp \
	cal.cpp \
	cfg.cpp \
	ios.cpp \
	log.cpp \
	network.cpp \
	radp.cpp \
	str.cpp \
	types.cpp
	
libsookee_la_LIBADD = 
libsookee_la_LDFLAGS = -version-info 0:0:0 -rdynamic -pthread

noinst_PROGRAMS = test test-radp time-radp

test_SOURCES = test.cpp
test_LDADD = -L.libs -lsookee

test_radp_SOURCES = test-radp.cpp
test_radp_LDADD = -L.libs -lsookee

time_radp_SOURCES = time-radp.cpp
time_radp_LDADD = -L.libs -lsookee

