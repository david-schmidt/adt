COMMON_SRC = crc.cpp dosdisk.cpp serial.cpp dosxfer.cpp unix.cpp
COMMON_OBJ = $(COMMON_SRC:.cpp=.o)

TEST_SRC = $(wildcard test/*.cpp)
TEST_OBJ = $(TEST_SRC:.cpp=.o)

ADT_SRC = adt.cpp
ADT_OBJ = $(ADT_SRC:.cpp=.o)

CXX := ccache $(CXX)
CXXFLAGS = -g -I. -Wall

all: adt

adt: $(COMMON_OBJ) $(ADT_OBJ)
	$(CXX) -o adt $(COMMON_OBJ) $(ADT_OBJ)

check: tester
	./tester

tester: $(COMMON_OBJ) $(TEST_OBJ)
	$(CXX) -o tester $(COMMON_OBJ) $(TEST_OBJ) -lcppunit

clean:
	$(RM) *.o test/*.o adt tester
