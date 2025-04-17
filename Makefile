# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -pthread -Wall -Wextra
OBJDIR = obj
BINDIR = Networking

# Source files (all in Networking/)
SRCS = \
	Networking/ClientGame.cpp \
	Networking/ClientNetwork.cpp \
	Networking/NetworkServices.cpp \
	Networking/ServerGame.cpp \
	Networking/ServerNetwork.cpp \
	Networking/server.cpp \
	Networking/client.cpp

# Object files (placed in obj/)
OBJS = $(SRCS:Networking/%.cpp=$(OBJDIR)/%.o)

# Targets: server and client executables
all: $(BINDIR)/server $(BINDIR)/client

# Server executable
$(BINDIR)/server: $(OBJDIR)/server.o $(OBJDIR)/ServerNetwork.o $(OBJDIR)/NetworkServices.o $(OBJDIR)/ServerGame.o
	$(CXX) $^ -o $@ $(CXXFLAGS)

# Client executable
$(BINDIR)/client: $(OBJDIR)/client.o $(OBJDIR)/ClientGame.o $(OBJDIR)/ClientNetwork.o $(OBJDIR)/NetworkServices.o
	$(CXX) $^ -o $@ $(CXXFLAGS)

# Rule for object files (with path adjustment)
$(OBJDIR)/%.o: Networking/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Clean rule
clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/server $(BINDIR)/client
