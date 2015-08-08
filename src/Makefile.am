sookee_includedir = $(prefix)/include/sookee
#sookee_include_typesdir = $(prefix)/include/sookee/types

sookee_librarydir = $(prefix)/lib/sookee

#AUTOMAKE_OPTIONS = subdir-objects

DEF_FLAGS = \
	-D LIBSOOKEE_COMPAT_LOG
	
GEN_FLAGS = -Wl,-E -Wall -Wextra -Winit-self -pedantic -pipe -pthread \
	-I$(top_srcdir)/src/include

AM_CXXFLAGS = $(CXXFLAGS) $(GEN_FLAGS) $(DEF_FLAGS)

sookee_include_HEADERS = \
	$(srcdir)/include/sookee/ansi.h \
	$(srcdir)/include/sookee/args.h \
	$(srcdir)/include/sookee/bug.h \
	$(srcdir)/include/sookee/cal.h \
	$(srcdir)/include/sookee/cfg.h \
	$(srcdir)/include/sookee/factory.h \
	$(srcdir)/include/sookee/fdstream.h \
	$(srcdir)/include/sookee/filedb.h \
	$(srcdir)/include/sookee/i18n.h \
	$(srcdir)/include/sookee/ios.h \
	$(srcdir)/include/sookee/linux.h \
	$(srcdir)/include/sookee/log.h \
	$(srcdir)/include/sookee/macros.h \
	$(srcdir)/include/sookee/mail.h \
	$(srcdir)/include/sookee/math.h \
	$(srcdir)/include/sookee/mem.h \
	$(srcdir)/include/sookee/mysql.h \
	$(srcdir)/include/sookee/network.h \
	$(srcdir)/include/sookee/radp.h \
	$(srcdir)/include/sookee/reg.h \
	$(srcdir)/include/sookee/require-c++11.h \
	$(srcdir)/include/sookee/rnd.h \
	$(srcdir)/include/sookee/scoper.h \
	$(srcdir)/include/sookee/socketstream.h \
	$(srcdir)/include/sookee/speller.h \
	$(srcdir)/include/sookee/ssl_socketstream.h \
	$(srcdir)/include/sookee/stdio_filebuf.h \
	$(srcdir)/include/sookee/stencil.h \
	$(srcdir)/include/sookee/stl.h \
	$(srcdir)/include/sookee/str.h \
	$(srcdir)/include/sookee/sub.h \
	$(srcdir)/include/sookee/test.h \
	$(srcdir)/include/sookee/types.h \
	$(srcdir)/include/sookee/utf8.h \
	$(srcdir)/include/sookee/utils.h \
	$(srcdir)/include/sookee/xio.h
		
sookee_include_HEADERS += \
	$(srcdir)/include/sookee/types/basic.h \
	$(srcdir)/include/sookee/types/dbl_deq.h \
	$(srcdir)/include/sookee/types/dbl_lst.h \
	$(srcdir)/include/sookee/types/dbl_map.h \
	$(srcdir)/include/sookee/types/dbl_set.h \
	$(srcdir)/include/sookee/types/dbl_vec.h \
	$(srcdir)/include/sookee/types/deq.h \
	$(srcdir)/include/sookee/types/flt_deq.h \
	$(srcdir)/include/sookee/types/flt_lst.h \
	$(srcdir)/include/sookee/types/flt_map.h \
	$(srcdir)/include/sookee/types/flt_set.h \
	$(srcdir)/include/sookee/types/flt_vec.h \
	$(srcdir)/include/sookee/types/int_deq.h \
	$(srcdir)/include/sookee/types/int_lst.h \
	$(srcdir)/include/sookee/types/int_map.h \
	$(srcdir)/include/sookee/types/int_set.h \
	$(srcdir)/include/sookee/types/int_vec.h \
	$(srcdir)/include/sookee/types/lst.h \
	$(srcdir)/include/sookee/types/map.h \
	$(srcdir)/include/sookee/types/pair.h \
	$(srcdir)/include/sookee/types/regex.h \
	$(srcdir)/include/sookee/types/set.h \
	$(srcdir)/include/sookee/types/siz_deq.h \
	$(srcdir)/include/sookee/types/siz_lst.h \
	$(srcdir)/include/sookee/types/siz_map.h \
	$(srcdir)/include/sookee/types/siz_set.h \
	$(srcdir)/include/sookee/types/siz_vec.h \
	$(srcdir)/include/sookee/types/str_deq.h \
	$(srcdir)/include/sookee/types/str_lst.h \
	$(srcdir)/include/sookee/types/str_map.h \
	$(srcdir)/include/sookee/types/str_set.h \
	$(srcdir)/include/sookee/types/str_vec.h \
	$(srcdir)/include/sookee/types/stream.h \
	$(srcdir)/include/sookee/types/typed_integral.h \
	$(srcdir)/include/sookee/types/typedefs_cnt.h \
	$(srcdir)/include/sookee/types/typedefs_deq.h \
	$(srcdir)/include/sookee/types/typedefs_lst.h \
	$(srcdir)/include/sookee/types/typedefs_map.h \
	$(srcdir)/include/sookee/types/typedefs_pair.h \
	$(srcdir)/include/sookee/types/typedefs_set.h \
	$(srcdir)/include/sookee/types/typedefs_vec.h \
	$(srcdir)/include/sookee/types/typedefs.h \
	$(srcdir)/include/sookee/types/vec.h

sookee_include_HEADERS += \
	$(srcdir)/include/sookee/utf8/checked.h \
	$(srcdir)/include/sookee/utf8/core.h \
	$(srcdir)/include/sookee/utf8/unchecked.h

sookee_library_LTLIBRARIES = libsookee.la

#if MAKE_I18N
#sookee_library_LTLIBRARIES += libsookee-i18n.la
#libsookee_i18n_la_SOURCES = i18n.cpp 
#libsookee_i18n_la_CXXFLAGS = $(GEN_FLAGS) $(DEF_FLAGS) $(CXXFLAGS) $(ICU_CFLAGS)
#libsookee_i18n_la_LIBADD = $(ICU_LIBS)
#libsookee_i18n_la_LDFLAGS = -version-info 0:0:0 -rdynamic -pthread
#endif

if MAKE_SPELLER
sookee_library_LTLIBRARIES += libsookee-speller.la
libsookee_speller_la_SOURCES = speller.cpp 
libsookee_speller_la_CXXFLAGS = $(GEN_FLAGS) $(DEF_FLAGS) $(CXXFLAGS) $(ASPELL_CFLAGS)
libsookee_speller_la_LIBADD = $(ASPELL_LIBS)
libsookee_speller_la_LDFLAGS = -version-info 0:0:0 -rdynamic -pthread
endif

if MAKE_DB
sookee_library_LTLIBRARIES += libsookee-db.la
libsookee_db_la_SOURCES = mysql.cpp
libsookee_db_la_CXXFLAGS = $(GEN_FLAGS) $(DEF_FLAGS) $(CXXFLAGS) $(MYSQL_CFLAGS)
libsookee_db_la_LIBADD =
libsookee_db_la_LDFLAGS = $(MYSQL_LDFLAGS) -version-info 0:0:0 -rdynamic -pthread
endif

libsookee_la_SOURCES = \
	bug.cpp \
	cal.cpp \
	cfg.cpp \
	i18n.cpp \
	ios.cpp \
	log.cpp \
	network.cpp \
	radp.cpp \
	reg.cpp \
	str.cpp \
	sub.cpp \
	types.cpp

libsookee_la_CXXFLAGS = $(GEN_FLAGS) $(DEF_FLAGS) $(CXXFLAGS)
libsookee_la_LIBADD = -lrt
libsookee_la_LDFLAGS = -version-info 0:0:0 -rdynamic -pthread
