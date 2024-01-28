COMPILER = g++

FLAGS = -std=c++11 -lraylib

SOURCES = src/main.cpp
EXECUTABLE = game

all: $(EXECUTABLE)

#to build the executable
$(EXECUTABLE): $(SOURCES)
	$(COMPILER) $(FLAGS) $(SOURCES) -o $(EXECUTABLE)

#to delete the exec
clean:
	rm -f $(EXECUTABLE)