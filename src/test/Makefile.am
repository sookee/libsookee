DEF_FLAGS = \
	-D LIBSOOKEE_COMPAT_LOG
	
GEN_FLAGS = -Wl,-E -Wall -Wextra -Winit-self -pedantic -pipe -pthread \
	-I$(top_srcdir)/src/include

AM_CXXFLAGS =  $(CXXFLAGS) $(GEN_FLAGS) $(DEF_FLAGS) $(CXXFLAGS)

noinst_PROGRAMS = \
	test \
	test-ios \
	test-radp \
	time-radp \
	test-socketstream \
	test-stencil \
	test-stl \
	test-sub \
	test-trim \
	test-cal
	
LIB_SOOKEE = -L../.libs -lsookee
	
test_SOURCES = test.cpp
test_LDADD = $(LIB_SOOKEE)

test_ios_SOURCES = test-ios.cpp
test_ios_LDADD = $(LIB_SOOKEE)

test_radp_SOURCES = test-radp.cpp
test_radp_LDADD = $(LIB_SOOKEE)

time_radp_SOURCES = time-radp.cpp
time_radp_LDADD = $(LIB_SOOKEE)

test_socketstream_SOURCES = test-socketstream.cpp
test_socketstream_LDADD = $(LIB_SOOKEE)

test_stencil_SOURCES = test-stencil.cpp
test_stencil_LDADD = $(LIB_SOOKEE)

test_stl_SOURCES = test-stl.cpp
test_stl_LDADD = $(LIB_SOOKEE)

test_sub_SOURCES = test-sub.cpp
test_sub_LDADD = $(LIB_SOOKEE)

test_trim_SOURCES = test-trim.cpp
test_trim_LDADD = $(LIB_SOOKEE)

test_cal_SOURCES = test-cal.cpp
test_cal_LDADD = $(LIB_SOOKEE)
