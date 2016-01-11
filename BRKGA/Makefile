
CPP= g++
CFLAGS= -O3 -fopenmp -std=c++11
OBJECTS= Decoder.o

# Targets:
all: main

main: main.cpp $(OBJECTS)
	$(CPP) $(CFLAGS) $(OBJECTS) main.cpp -o main
        
Decoder.o: Decoder.cpp
	$(CPP) $(CFLAGS) -c Decoder.cpp

# Remove:
clean:
	rm -f main $(OBJECTS)
