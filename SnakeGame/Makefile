CXX = g++
CXXFLAGS = -std=c++11 -fopenmp
SDL2_FLAGS = `sdl2-config --cflags --libs`
SOURCES = main.cpp game.cpp snake.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = snake_game

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(SDL2_FLAGS) $(CXXFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
