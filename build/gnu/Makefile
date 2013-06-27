# main source dir
SOURCE_DIR = ../../src

# external library root
LIB_DIR =../../lib

# test folder 
TEST_DIR = ../../test

# All header files
HEADERS = $(SOURCE_DIR)/*.h $(SOURCE_DIR)/*.hpp $(LIB_DIR)/geohash/*.h

# All source files
SOURCES = $(SOURCE_DIR)/*.cpp $(LIB_DIR)/geohash/*.c

#c++ compiler 
CXX = clang++

#flags passed to the preprocessor 
# macros 
# header search dirs
CPPFLAGS += -DGTEST_USE_OWN_TR1_TUPLE=1  -I$(SOURCE_DIR) -I$(LIB_DIR)/geohash

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -stdlib=libc++  -stdlib=libc++ -std=c++11 -fPIC

# linkder flags
LDFLAGS += -lGeographic /usr/local/lib/gtest.a 

# library search path
#LD_LIBRARY_PATH += /usr/local/lib

# object files
OBJECTS = geolib.o gctree.o geohash.o


all : libgeocached.a

test : main_test
	./main_test

clean :
	rm -rfv libgeocached.a *.o main_test

### LIB
 
libgeocached.a : $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

#libgeocached.so : $(OBJECTS)
#	$(LD) -shared -soname libgeocached.so.1 -o libgeocached.so.1.0 -lc $^

### OBJECTS

# test main obj
main_test.o : $(TEST_DIR)/main_test.cpp
	$(CXX) $(CPPFLAGS) -I$(TEST_DIR) $(CXXFLAGS) -c $(TEST_DIR)/main_test.cpp

# individual obj 
geolib.o : $(SOURCE_DIR)/geolib.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SOURCE_DIR)/geolib.cpp
	
gctree.o : $(SOURCE_DIR)/gctree.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SOURCE_DIR)/gctree.cpp

geohash.o : $(LIB_DIR)/geohash/geohash.c
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(LIB_DIR)/geohash/geohash.c


### BINARY

# test binary 
main_test : main_test.o libgeocached.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $^ -o $@