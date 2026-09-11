CC  = gcc
CXX = g++

CFLAGS   = -O3 -Wall -Wextra
CXXFLAGS = -O3 -Wall -Wextra -std=c++17

SRC_DIR      = src
INC_DIR      = include
OBJ_DIR      = obj
BIN_DIR      = bin
EXTERNAL_DIR = external

DSFMT_DIR = $(EXTERNAL_DIR)/dSFMT
FMM3D_DIR ?= $(HOME)/FMM3D_v1.0.1

# ============================================================
# FMM3D installation
#
# Override from command line if necessary:
#
# make FMM3D_LIB=/path/to/fmm3d/lib
#
# ============================================================

FMM3D_LIB ?= $(HOME)/FMM3D_v1.0.1_install

# ============================================================
# Include paths
# ============================================================

INCLUDES = \
	-I$(INC_DIR) \
	-I$(EXTERNAL_DIR) \
	-I$(FMM3D_DIR)/c

# ============================================================
# Linker libraries
# ============================================================

LDLIBS = -L$(FMM3D_LIB) -lfmm3d -lgomp

# ============================================================
# Target
# ============================================================

TARGET = $(BIN_DIR)/main_2dgaussian_nersc.exe

# ============================================================
# Object files
# ============================================================

OBJS = \
	$(OBJ_DIR)/bend_kick_c_2dgaussian_nersc.o \
	$(OBJ_DIR)/read_parameters_2dgaussian_nersc.o \
	$(OBJ_DIR)/array2d.o \
	$(OBJ_DIR)/ca_rand.o \
	$(OBJ_DIR)/dSFMT.o

# ============================================================
# Default target
# ============================================================

all: $(TARGET)

# ============================================================
# Link executable
#
# Use g++ because the final program contains C++ code.
# ============================================================

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(OBJS) -o $@ $(LDLIBS)

# ============================================================
# C++ source files
# ============================================================

$(OBJ_DIR)/bend_kick_c_2dgaussian_nersc.o: \
	$(SRC_DIR)/bend_kick_c_2dgaussian_nersc.cpp \
	$(INC_DIR)/bend_kick_c_with_doppler_nersc.h \
	$(INC_DIR)/read_parameters_with_doppler_nersc.h \
	$(INC_DIR)/array2d.h \
	$(INC_DIR)/ca_rand.h \
	$(DSFMT_DIR)/dSFMT.h \
	$(FMM3D_DIR)/c/lfmm3d_c.h \
	$(FMM3D_DIR)/c/utils.h | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/read_parameters_2dgaussian_nersc.o: \
	$(SRC_DIR)/read_parameters_2dgaussian_nersc.cpp \
	$(INC_DIR)/read_parameters_with_doppler_nersc.h \
	$(INC_DIR)/array2d.h \
	$(INC_DIR)/ca_rand.h \
	$(DSFMT_DIR)/dSFMT.h | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/array2d.o: \
	$(SRC_DIR)/array2d.cpp \
	$(INC_DIR)/array2d.h | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ============================================================
# C source files
# ============================================================

$(OBJ_DIR)/ca_rand.o: \
	$(SRC_DIR)/ca_rand.c \
	$(INC_DIR)/ca_rand.h \
	$(DSFMT_DIR)/dSFMT.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/dSFMT.o: \
	$(DSFMT_DIR)/dSFMT.c \
	$(DSFMT_DIR)/dSFMT.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(DSFMT_DIR) -c $< -o $@

# ============================================================
# Create build directories automatically
# ============================================================

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# ============================================================
# Clean
# ============================================================

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# ============================================================
# Phony targets
# ============================================================

.PHONY: all clean
