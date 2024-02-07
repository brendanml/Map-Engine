COMPILER = clang++
FLAGS = -std=c++11 -Wall
LDFLAGS = -lraylib
SRCDIR = src
BUILDDIR = build
EXECUTABLE = game

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))
DEPENDENCIES = $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(COMPILER) $(FLAGS) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(COMPILER) $(FLAGS) -MMD -MP -c $< -o $@

-include $(DEPENDENCIES)

clean:
	rm -rf $(BUILDDIR) $(EXECUTABLE)
