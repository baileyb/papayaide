# Kamiah is the document engine of PapayaIDE.
#
# Author: Victor Marmol (vmarmol@gmail.com)

# C++ Compiler to use.
CXX=g++-4.6

# Dox builder.
DOC=doxygen

# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR = gtest-1.6.0

# Flags passed to the preprocessor.
CPPFLAGS += -I$(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -Wall -Wextra -Werror -O2 -g

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = document_test diff_test

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

all :

all_tests : $(TESTS)

clean :
	rm -f $(TESTS) *.o *.a
	rm -rf doc/

diff.o : diff.cc diff.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c diff.cc

diff_test : diff.o diff_test.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -pthread $^ -o $@

document.o : document.cc document.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c document.cc

document_test : diff.o document.o document_test.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -pthread $^ -o $@

doc_perf : diff.o document.o doc_perf.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

%_test.o : %_test.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $^

docs:
	${DOC} doxygen.conf

# Builds Google Test: gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^
