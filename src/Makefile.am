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
sookee_librarydir = $(prefix)/lib/sookee

DEF_FLAGS = \
	-D _GLIBCXX_USE_NANOSLEEP
	
GEN_FLAGS = -Wl,-E -Wfatal-errors -Wall -Wextra -Winit-self -ansi -pedantic -pipe -pthread \
	-I$(top_srcdir)/src/include

AM_CXXFLAGS = -std=gnu++11 $(GEN_FLAGS) $(DEF_FLAGS) $(LIBXML2_FLAG)

sookee_include_HEADERS = \
	$(srcdir)/include/sookee/bug.h \
	$(srcdir)/include/sookee/cal.h \
	$(srcdir)/include/sookee/ios.h \
	$(srcdir)/include/sookee/log.h \
	$(srcdir)/include/sookee/socketstream.h \
	$(srcdir)/include/sookee/ssl_socketstream.h \
	$(srcdir)/include/sookee/stl.h \
	$(srcdir)/include/sookee/str.h \
	$(srcdir)/include/sookee/types.h
	
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
	
libsookee_la_LIBADD = -lreadline
libsookee_la_LDFLAGS = -version-info 0:0:0

# Development utils
noinst_PROGRAMS = \
	test

test_SOURCES = test.cpp
test_LDADD =
