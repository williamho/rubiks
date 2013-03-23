# OpenGL makefile for Cygwin on Windows
.NULL: .exe

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(addprefix obj/,$(notdir $(SOURCES:.cpp=.o)))

CXX = g++-3
CXXOPTS = -mno-cygwin -mwin32 -g
CXXDEFS = -DFREEGLUT_STATIC -DGLEW_STATIC
CXXINCS = -Iinclude
CXXFLAGS = $(CXXOPTS) $(CXXDEFS) $(CXXINCS)

LDOPTS =
LDDIRS = -Llib
LDLIBS = -lfreeglut -lglew32 -lopengl32 -lgdi32 -lwinmm
LDFLAGS = $(LDOPTS) $(LDDIRS) $(LDLIBS)

#---------------------------------------------------------------------------

rubiks: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

obj/%.o: src/%.cpp 
	$(CXX) $(CXXFLAGS) -c -o $@ $<

#---------------------------------------------------------------------------

clean:
	find . \( -name '*.exe' -or -name '*.o' \) -delete

