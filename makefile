# Makefile for build/test
# created with makefile-creator


####################
#Settings:


CXX ?= g++
FLAGS +=  -Wall -Wextra -Wpedantic -std=c++11 -O0 -D_GLIBCXX_DEBUG -g 
LIBS += -lboost_unit_test_framework 
INCLUDES += 
TARGET = build/test
OBJECTS = build/logger.o build/main.o


####################
#Rules:


$(TARGET) : $(OBJECTS)
	$(CXX) $(FLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

build/%.o:
	@if test ! -d 'build'; then mkdir 'build'; echo "created 'build'" ; fi
	$(CXX) $(FLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm build/*.o

all: $(TARGET)


####################
#Dependencies:


build/logger.o: src/logger.cpp src/logger.hpp makefile

build/main.o: test/main.cpp src/logger.hpp makefile

