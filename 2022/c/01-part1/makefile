# Makefile to build simple C program

CC ?= clang

CFLAGS ?= \
    -std=c99 \
	-O0 \


LIBS =

OBJ_DIR ?= ./obj
TARGET ?= prog

C_SRC := \
    main.c \

	
C_HEADERS := 

C_INC_DIRS := $(sort $(dir $(C_HEADERS))) 
C_INC := $(addprefix -I,$(C_INC_DIRS))

OBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(C_SRC))

# set default target
default: build

# rule to create directory for object files
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# rule to build object files
$(OBJ_DIR)/%.o: %.c $(C_HEADERS) $(OBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

# rule to link object files into executable
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
.PHONY: input.txt

clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

INPUT ?= input.txt

run: $(TARGET) $(INPUT)
	./$(TARGET) < $(INPUT)

all: clean build

build: $(TARGET)
