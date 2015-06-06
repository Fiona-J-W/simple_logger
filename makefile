# Makefile for build/test
# created with makefile-creator


####################
#Settings:


CXX ?= g++
FLAGS +=  -Wall -Wextra -Wpedantic -std=c++1y 
LIBS += 
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


build/logger.o: src/logger.cpp include/logger.hpp makefile

build/main.o: test/main.cpp include/logger.hpp makefile

