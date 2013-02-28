
include Makefile.local

# requirements
REQ_CXX11_TEST := $(shell echo "int main(){}" | $(CXX) -o /dev/null -x c++ -std=c++11 - 2> /dev/null; echo $$?)
REQ_CXX11_FAIL := "ERROR: Requires g++ v4.7 or above to comiile."

DEBUGFLAGS = \
	-D _GLIBCXX_DEBUG \
	-D _GLIBCXX_DEBUG_PEDANTIC \
	-D DEBUG \
	-O0 \
	-g3

RELEASEFLAGS = \
	-O3 \
	-g0

GENERALFLAGS = \
	-D _GLIBCXX_USE_NANOSLEEP \
	-D _GLIBCXX_USE_SCHED_YIELD \
	-pthread \
	-std=c++11 \
	-Wall \
	-Werror

CXXFLAGS = \
	$(GENERALFLAGS) \
	$(DEBUGFLAGS) \
	-Iinclude \
	-fPIC
	
SOFLAGS = \
	-shared \
	-pthread \
	-Wl,-soname,libsookee.so.0 \
	-rdynamic \
	-Wl,-rpath,$(prefix)/lib/sookee

SOURCES := bug.cpp cal.cpp ios.cpp log.cpp network.cpp str.cpp types.cpp
OBJECTS := $(patsubst %.cpp,%.o, $(SOURCES))
HEADERS := $(patsubst %.cpp,%.h, $(SOURCES))

all: reqs libsookee.so.0.0.0

reqs:
	@if(($(REQ_CXX11_TEST))); then echo $(REQ_CXX11_FAIL); exit 1; fi

libsookee.so.0.0.0: $(OBJECTS)
	$(CXX) $(SOFLAGS) -o $@ $(OBJECTS)

%.o: %.cpp %.h
	$(CXX) -c -o $@ $<

install: all
	mkdir -p $(prefix)/lib/sookee
	cp libsookee.so.0.0.0 $(prefix)/lib/sookee
	/sbin/ldconfig -v -n $(prefix)/lib/sookee
	ln -f -s $(prefix)/lib/sookee/libsookee.so.0 $(prefix)/lib/sookee/libsookee.so
	mkdir -p $(prefix)/include/sookee
	cp -r include/sookee $(prefix)/include
	
uninstall:
	rm -fr $(prefix)/lib/sookee
	rm -fr $(prefix)/include/sookee

clean:
	rm -fr *.o *.so.?.?.? install 
	
.PHONY: reqs install uninstall
