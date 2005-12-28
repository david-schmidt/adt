COMMON_SRC = crc.cpp dosdisk.cpp serial.cpp
COMMON_OBJ = $(COMMON_SRC:.cpp=.o)

TEST_SRC = $(wildcard test/*.cpp)
TEST_OBJ = $(TEST_SRC:.cpp=.o)

CXXFLAGS = -g -I. -Wall

all: $(COMMON_OBJ)

check: tester
	./tester

tester: $(COMMON_OBJ) $(TEST_OBJ)
	g++ -o tester $(COMMON_OBJ) $(TEST_OBJ) -lcppunit

clean:
	$(RM) *.o test/*.o tester
