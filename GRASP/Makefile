# Macros:
CPP= g++
CFLAGS= -std=c++11
OBJECTS= Grasp.o

# Targets:
all: grasp

grasp: $(OBJECTS)
	$(CPP) $(CFLAGS) $(OBJECTS) main.cpp -o main
        
Grasp.o: Grasp.cpp
	$(CPP) $(CFLAGS) -c Grasp.cpp

# Remove:
clean:
	rm -f main $(OBJECTS)
