# @author   clemedon (Clément Vidon)
# Modified by Brooke Leinberger
####################################### BEG_3 ####

NAME        := jlu_core

#------------------------------------------------#
#   INGREDIENTS                                  #
#------------------------------------------------#
# SRC_DIR   source directory
# OBJ_DIR   object directory
# SRCS      source files
# OBJS      object files
#
# CC        compiler
# CFLAGS    compiler flags
# CPPFLAGS  preprocessor flags

SRC_DIR     := ../src
OBJ_DIR     := build/obj
BIN_DIR		:= build
TEST 		:= $(BIN_DIR)/$(NAME)_test
BINS		:= $(BIN_DIR)/$(NAME)



SRCS        := /utl/format/cobs.c

SRCS        := $(SRCS:%=$(SRC_DIR)/%)
OBJS        := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%_c.o)
OBJS        := $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%_cpp.o)

FLAGS      := -g -Wall -Wextra -Werror -I../src -I/usr/include

#------------------------------------------------#
#   UTENSILS                                     #
#------------------------------------------------#
# RM        force remove
# MAKEFLAGS make flags
# DIR_DUP   duplicate directory tree

RM          := rm -rf
MAKEFLAGS   += --no-print-directory
DIR_DUP     = mkdir -p $(@D)

#------------------------------------------------#
#   RECIPES                                      #
#------------------------------------------------#
# all       default goal
# $(NAME)   linking .o -> binary
# %.o       compilation .c -> .o
# clean     remove .o
# fclean    remove .o + binary
# re        remake default goal

all: test

$(OBJ_DIR)/%_c.o: $(SRC_DIR)/%.c
	$(DIR_DUP)
	gcc $(FLAGS) -c -o $@ $<
	$(info CREATED $@)

$(OBJ_DIR)/%_cpp.o: $(SRC_DIR)/%.cpp
	$(DIR_DUP)
	g++ $(FLAGS) -c -o $@ $<
	$(info CREATED $@)


$(OBJ_DIR)/test/%_c.o: %.c
	$(DIR_DUP)
	gcc $(FLAGS) -c -o $@ $<
	$(info CREATED $@)

$(OBJ_DIR)/test/%_cpp.o: %.cpp
	$(DIR_DUP)
	g++ $(FLAGS) -c -o $@ $<
	$(info CREATED $@)

clean:
	$(RM) $(BIN_DIR) $(OBJ_DIR)
	mkdir -p $(BIN_DIR) $(OBJ_DIR)

re:
	$(MAKE) clean


#------------------------------------------------#
#   Tests                                        #
#------------------------------------------------#

# CPPFLAGS += -I$(CPPUTEST_HOME)/include
# CXXFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorNewMacros.h
# CFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorMallocMacros.h
# LD_LIBRARIES = -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt
# TEST_MAIN = $(SRC_DIR)/all_tests.cpp

UTEST 	:= all_tests.cpp
UTEST 	+= cobs_test.cpp


UTEST_OBJS	:= $(UTEST:%.c=$(OBJ_DIR)/test/%_c.o)
UTEST_OBJS	:= $(UTEST_OBJS:%.cpp=$(OBJ_DIR)/test/%_cpp.o)
$(info OBJS is $(OBJS))

test: re $(UTEST_OBJS) $(OBJS)
	g++ $(UTEST_OBJS) $(OBJS) -o $(TEST) $(FLAGS) -lCppUTest -lCppUTestExt
	$(TEST)

#------------------------------------------------#
#   SPEC                                         #
#------------------------------------------------#

.PHONY: clean fclean re
.SILENT:



####################################### END_3 ####