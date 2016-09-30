# Makefile for building mirage render

# Toolchain, tested ones are gcc, clang and gcc-mingw-w64
ENV ?= gcc

# Linker
LD = $(ENV)

# Include directories
CXXINCS += -I./include/

# Linker flags
ifneq ($(OS), Windows_NT)
	ifeq ($(shell uname -s), Darwin)
		LDFLAGS = -Wl -m64
	endif
else
	LDFLAGS = -Wl,--as-needed -m64
endif

# Linker libs, lua might be -llua, -lluaXX or -lluaX.X, XX being your installed lua version
LDLIBS = -L./lib/

ifneq (,$(findstring mingw, $(ENV)))
  LDLIBS += -lmingw32 -lSDL2main
endif

LDLIBS += -lSDL2 -llua5.3 -lpthread -lstdc++ -lm

ifneq ($(OS), Windows_NT)
	ifneq ($(shell uname -s), Darwin)
		LDLIBS += -static-libgcc -static-libstdc++
	endif
else
	LDLIBS += -static-libgcc -static-libstdc++
endif

# Compiler
CXX = $(ENV)

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function -m64

# Apply optimization parameters if !DEBUG
DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CXXFLAGS += Og -g
else
	CXXFLAGS += -Ofast -flto
	LDFLAGS += -Ofast -flto
endif

# src & bin directories
SRCDIR = src
BINDIR = out_$(ENV)

# Target binary
TARGET = $(BINDIR)/mirage

# CPPs, Includes & Objects
CPPS := $(wildcard $(SRCDIR)/*.cpp)
CPPS += $(wildcard $(SRCDIR)/core/*.cpp)
CPPS += $(wildcard $(SRCDIR)/math/*.cpp)
CPPS += $(wildcard $(SRCDIR)/shapes/*.cpp)
CPPS += $(wildcard $(SRCDIR)/cameras/*.cpp)
CPPS += $(wildcard $(SRCDIR)/accelerators/*.cpp)
CPPS += $(wildcard $(SRCDIR)/renderers/*.cpp)
CPPS += $(wildcard $(SRCDIR)/materials/*.cpp)
CPPS += $(wildcard $(SRCDIR)/lights/*.cpp)
CPPS += $(wildcard $(SRCDIR)/3rdparty/*.cpp)
INCS := $(wildcard $(SRCDIR)/*.h)
INCS += $(wildcard $(SRCDIR)/core/*.h)
INCS += $(wildcard $(SRCDIR)/math/*.h)
INCS += $(wildcard $(SRCDIR)/shapes/*.h)
INCS += $(wildcard $(SRCDIR)/cameras/*.h)
INCS += $(wildcard $(SRCDIR)/accelerators/*.h)
INCS += $(wildcard $(SRCDIR)/renderers/*.h)
INCS += $(wildcard $(SRCDIR)/materials/*.h)
INCS += $(wildcard $(SRCDIR)/lights/*.h)
INCS += $(wildcard $(SRCDIR)/3rdparty/*.h)
OBJS := $(CPPS:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)

# Build target
all: $(TARGET)

# Compile all
$(OBJS): $(BINDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CXX) $(CXXFLAGS) $(CXXINCS) $^ -c $c -o $@
	@echo "Compiling done for file $@"

# Link all to executable
$(TARGET): $(OBJS)
	@$(LD) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
	@echo "Linking done for file $@"

# Clean all in BINDIR
.PHONY: clean
clean:
ifeq ($(OS), Windows_NT)
	- RMDIR /S /Q $(BINDIR)\core
	- RMDIR /S /Q $(BINDIR)\math
	- RMDIR /S /Q $(BINDIR)\shapes
	- RMDIR /S /Q $(BINDIR)\cameras
	- RMDIR /S /Q $(BINDIR)\accelerators
	- RMDIR /S /Q $(BINDIR)\renderers
	- RMDIR /S /Q $(BINDIR)\materials
	- RMDIR /S /Q $(BINDIR)\lights
	- RMDIR /S /Q $(BINDIR)\3rdparty
	- DEL /S /Q $(BINDIR)\main.o
	- DEL /S /Q $(BINDIR)\mirage.exe
	- MKDIR $(BINDIR)\core
	- MKDIR $(BINDIR)\math
	- MKDIR $(BINDIR)\shapes
	- MKDIR $(BINDIR)\cameras
	- MKDIR $(BINDIR)\accelerators
	- MKDIR $(BINDIR)\renderers
	- MKDIR $(BINDIR)\materials
	- MKDIR $(BINDIR)\lights
	- MKDIR $(BINDIR)\3rdparty
else
	- @$(RM) $(OBJS)
	- @$(RM) $(TARGET)
endif
	@echo "Cleanup done."

# Initialize bin directory structure
.PHONY: init
init:
ifeq ($(OS), Windows_NT)
	- MKDIR $(BINDIR)
	- MKDIR $(BINDIR)\core
	- MKDIR $(BINDIR)\math
	- MKDIR $(BINDIR)\shapes
	- MKDIR $(BINDIR)\cameras
	- MKDIR $(BINDIR)\accelerators
	- MKDIR $(BINDIR)\renderers
	- MKDIR $(BINDIR)\materials
	- MKDIR $(BINDIR)\lights
	- MKDIR $(BINDIR)\res
	- MKDIR $(BINDIR)\3rdparty
	- XCOPY /E res $(BINDIR)\res
else
	- mkdir $(BINDIR)
	- mkdir $(BINDIR)/core
	- mkdir $(BINDIR)/math
	- mkdir $(BINDIR)/shapes
	- mkdir $(BINDIR)/cameras
	- mkdir $(BINDIR)/accelerators
	- mkdir $(BINDIR)/renderers
	- mkdir $(BINDIR)/materials
	- mkdir $(BINDIR)/lights
	- mkdir $(BINDIR)/res
	- mkdir $(BINDIR)/3rdparty
	- cp -r res $(BINDIR)
endif
	@echo "Initialization done."
