# Makefile

# C compiler
CC = gcc
# GCC option
CFLAGS = -Wall -O2 -g
# Linker option
LDFLAGS = 
# Library
LIBS =

# Source directory
SRC_DIR = ./src
# Object directory
OBJ_DIR = ./obj
# Binary directory
BIN_DIR = ./bin

TARGET = main
TARGET_SRC = $(patsubst %,%.c,$(TARGET))
TARGET_OBJ = $(patsubst %,$(OBJ_DIR)/%.o,$(TARGET))
#TARGET_DEP = $(TARGET_OBJ:.o=.d)
TARGET_OUT = $(patsubst %,$(BIN_DIR)/%.out,$(TARGET))

TEST= test
TEST_SRC = $(patsubst %,%.c,$(TEST))
TEST_OBJ = $(patsubst %,$(OBJ_DIR)/%.o,$(TEST))
#TEST_DEP = $(TEST_OBJ:.o=.d)
TEST_OUT = $(patsubst %,$(BIN_DIR)/%.out,$(TEST))

# Extract source files
SRCS = $(filter-out $(TARGET_SRC) $(TEST_SRC), $(notdir $(wildcard $(SRC_DIR)/*.c)))

OBJS = $(SRCS:.c=.o)
OBJECTS = $(patsubst %.o,$(OBJ_DIR)/%.o,$(OBJS))
DEPS = $(OBJECTS:.o=.d)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -MD $(LDFLAGS)

$(TARGET) : $(TARGET_OBJ) $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET_OUT) $^

$(TEST) : $(TEST_OBJ) $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TEST_OUT) $^

foo:
	echo $(TARGET_OUT) $(TEST_OUT)

.PHONY : clean main test
clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d $(BIN_DIR)/*.out

-include $(DEPS)
