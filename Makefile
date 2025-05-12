CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lglfw -lGLEW -framework OpenGL

SRCS = main.cpp src/window.cpp src/character.cpp src/stb_image_impl.cpp src/platform.o
OBJS = $(SRCS:.cpp=.o)
TARGET = game

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o src/*.o $(TARGET)

run:
	./$(TARGET)
