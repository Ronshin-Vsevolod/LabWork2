PROJECT = game

LIBPROJECT = $(PROJECT).a
LIBTESTPROJECT = $(PROJECT)-test.a

TESTPROJECT = test-$(PROJECT)

CXX = g++

A = ar

AFLAGS = rsv

CXXFLAGS = -Iinclude -std=c++17 -Werror -Wpedantic -Wall -g -fPIC

LDXXFLAGS = $(CXXFLAGS) -L. -l:$(LIBPROJECT)
LDTESTXXFLAGS = $(CXXFLAGS) -L. -l:$(LIBTESTPROJECT)

LDGTESTFLAGS = -lgtest -lgtest_main -lpthread

DEPS=$(wildcard include/*.h)

SRC=$(wildcard src/*.cpp)
OBJ=$(patsubst src/%.cpp,%.o,$(SRC))

MAIN_OBJ=Main.o

TEST-OBJ=GoogleTest.o

TEST-LIB-OBJ=$(filter-out $(MAIN_OBJ), $(OBJ))

.PHONY: default

default: all;

%.o: src/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

GoogleTest.o: GoogleTest.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(LIBPROJECT): $(OBJ)
	$(A) $(AFLAGS) $@ $^

$(LIBTESTPROJECT): $(TEST-LIB-OBJ)
	$(A) $(AFLAGS) $@ $^

$(PROJECT): $(MAIN_OBJ) $(LIBPROJECT)
	$(CXX) -o $@ $(MAIN_OBJ) $(LDXXFLAGS)

$(TESTPROJECT): $(TEST-OBJ) $(LIBTESTPROJECT)
	$(CXX) -o $@ $(TEST-OBJ) $(LDTESTXXFLAGS) $(LDGTESTFLAGS)

test: $(TESTPROJECT)

all: $(PROJECT)

.PHONY: clean

clean:
	rm -f *.o

cleanall: clean
	rm -f $(PROJECT)
	rm -f $(LIBPROJECT)
	rm -f $(LIBTESTPROJECT)
	rm -f $(TESTPROJECT)
