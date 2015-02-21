pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = libsookee.pc

sookee_includedir = $(prefix)/include/sookee
sookee_include_typesdir = $(prefix)/include/sookee/types

sookee_librarydir = $(prefix)/lib/sookee

DEF_FLAGS = \
	-D DEBUG -D LIBSOOKEE_COMPAT_LOG
	
GEN_FLAGS = -Wl,-E -Wall -Wextra -Winit-self -pedantic -pipe -pthread \
	-I$(top_srcdir)/src/include

AM_CXXFLAGS = $(GEN_FLAGS) $(DEF_FLAGS)

sookee_include_HEADERS = $(wildcard $(srcdir)/include/sookee/*.h)	
sookee_include_types_HEADERS = $(wildcard $(srcdir)/include/sookee/types/*.h)

sookee_library_LTLIBRARIES = \
	libsookee.la libsookee-i18n.la libsookee-db.la 

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
	
libsookee_i18n_la_SOURCES = \
	i18n.cpp 
	
libsookee_db_la_SOURCES = \
	mysql.cpp 
	
libsookee_la_CXXFLAGS = 
libsookee_la_LIBADD =
libsookee_la_LDFLAGS = -version-info 0:0:0 -rdynamic -pthread

libsookee_i18n_la_CXXFLAGS = $(ICU_CFLAGS)
libsookee_i18n_la_LIBADD = $(ICU_LIBS)
libsookee_i18n_la_LDFLAGS = -version-info 0:0:0 -rdynamic -pthread

libsookee_db_la_CXXFLAGS = $(MYSQL_CFLAGS)
libsookee_db_la_LIBADD =
libsookee_db_la_LDFLAGS = $(MYSQL_LDFLAGS) -version-info 0:0:0 -rdynamic -pthread

noinst_PROGRAMS = \
	test \
	test-ios \
	test-radp \
	time-radp \
	test-socketstream \
	test-stencil \
	test-stl \
	test-trim \
	test-cal
	
libsookee.pc: $(top_srcdir)/pkg-config/libsookee.pc.in
	sed -e 's![@]prefix[@]!$(prefix)!g' \
	    -e 's![@]exec_prefix[@]!$(exec_prefix)!g' \
	    -e 's![@]includedir[@]!$(includedir)!g' \
	    -e 's![@]libdir[@]!$(libdir)!g' \
	    -e 's![@]PACKAGE_VERSION[@]!$(PACKAGE_VERSION)!g' \
	    -e 's![@]KRB5_LDFLAGS[@]!$(KRB5_LDFLAGS)!g' \
	    -e 's![@]KRB5_LIBS[@]!$(KRB5_LIBS)!g' \
	    $(top_srcdir)/pkg-config/libsookee.pc.in > $@


test_SOURCES = test.cpp
test_LDADD = -L.libs -lsookee

test_ios_SOURCES = test-ios.cpp
test_ios_LDADD = -L.libs -lsookee

test_radp_SOURCES = test-radp.cpp
test_radp_LDADD = -L.libs -lsookee

time_radp_SOURCES = time-radp.cpp
time_radp_LDADD = -L.libs -lsookee

test_socketstream_SOURCES = test-socketstream.cpp
test_socketstream_LDADD = -L.libs -lsookee

test_stencil_SOURCES = test-stencil.cpp
test_stencil_LDADD = -L.libs -lsookee

test_stl_SOURCES = test-stl.cpp
test_stl_LDADD = -L.libs -lsookee

test_trim_SOURCES = test-trim.cpp
test_trim_LDADD = -L.libs -lsookee

test_cal_SOURCES = test-cal.cpp
test_cal_LDADD = -L.libs -lsookee
