# Makefile for building mirage render

# Linker
LD = g++

# Linker flags
LDFLAGS = -Wl,--as-needed

# Linker libs
ifeq ($(OS), Windows_NT)
  LDLIBS = -lmingw32 -lSDL2main
endif
LDLIBS += -lSDL2

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra

# Apply optimization parameters if !DEBUG
DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CXXFLAGS += Og -g
else
	CXXFLAGS += -Ofast
	LDFLAGS += -Ofast
endif

# src & bin directories
SRCDIR = src
BINDIR = out

# Target binary
TARGET = $(BINDIR)/mirage

# CPPs, Includes & Objects
CPPS := $(wildcard $(SRCDIR)/*.cpp)
CPPS += $(wildcard $(SRCDIR)/core/*.cpp)
CPPS += $(wildcard $(SRCDIR)/math/*.cpp)
INCS := $(wildcard $(SRCDIR)/*.h)
INCS += $(wildcard $(SRCDIR)/core/*.h)
INCS += $(wildcard $(SRCDIR)/math/*.h)
OBJS := $(CPPS:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)

# Build target
all: $(TARGET)

# Compile all
$(OBJS): $(BINDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CXX) $(CXXFLAGS) $^ -c $c -o $@
	@echo "Compiling done for file $@"

# Link all to executable
$(TARGET): $(OBJS)
	@$(LD) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
	@echo "Linking done for file $@"

# Clean all in BINDIR
.PHONY: clean
clean:
	@$(RM) $(OBJS)
	@$(RM) $(TARGET)
	@echo "Cleanup done."

# Initialize bin directory structure
.PHONY: init
init:
	@$(MKDIR) $(BINDIR)
	@$(MKDIR) $(BINDIR)/core
	@$(MKDIR) $(BINDIR)/math
	@echo "Initialization done."