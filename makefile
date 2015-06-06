# Makefile for a.out
# created with makefile-creator


####################
#Settings:


CXX ?= g++
FLAGS +=  -Wall -Wextra -Wpedantic -std=c++1y 
LIBS += 
INCLUDES += 
TARGET = a.out
OBJECTS = logger.o main.o


####################
#Rules:


$(TARGET) : $(OBJECTS)
	$(CXX) $(FLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

%.o:
	$(CXX) $(FLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm *.o

all: $(TARGET)


####################
#Dependencies:


logger.o: src/logger.cpp include/logger.hpp makefile

main.o: test/main.cpp include/logger.hpp makefile

