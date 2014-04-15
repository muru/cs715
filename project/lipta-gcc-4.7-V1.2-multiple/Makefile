
#------ MAKE CHANGES TO BASE_DIR : Please put the path to base directory of your pristine gcc-4.7.2 build -----------#
GCC_SRC = /home/bro3886/devel/
GCC_DIR = /opt/cs715/
OBJ_DIR = obj
SRC_DIR = src
LIB_DIR = lib
OUT_DIR = out

DIRS = $(OBJ_DIR) $(SRC_DIR) $(LIB_DIR) $(OUT_DIR)

#INSTALL = $(BASE_DIR)/install

CXX = g++
NEW_PATH = /home/bro3886/devel/gcc-4.8.2/gcc

#----- MAKE CHANGES TO OBJS : Add the name of your test file with extension .o (say test as test.o) --------#
#------------------------------- Multiple dependent files maybe also be added ------------------------------#
OBJS = $(addprefix $(OBJ_DIR)/, test1.o test2.o pointsto-callstrings.o)
OUT = $(OUT_DIR)/result
LIB = $(LIB_DIR)/lfcpa.so

GCCPLUGINS_DIR:= $(shell $(CXX) -print-file-name=plugin)
INCLUDE= -I$(GCCPLUGINS_DIR)/include -I$(NEW_PATH)

FLAGS= -fPIC -O2 -flto -flto-partition=none

all: $(LIB)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp | $(DIRS)
	$(CXX) $(FLAGS) $(INCLUDE) -c $< -o $@

$(LIB): $(OBJ_DIR)/pointsto-callstrings.o 
	$(CXX) $(INCLUDE) $(FLAGS) -shared $^ -o $@

test:  $(OBJS) $(LIB)
	$(CXX) -o $(OUT) -flto -flto-partition=none -fplugin=$(LIB) $(OBJS) -O3 -fdump-ipa-all 
#test:   plugin.so
#	$(CXX) -c -flto -flto-partition=none -fplugin=./plugin.so function-pointer.c -O0 -fdump-ipa-all 

$(DIRS):
	mkdir -p $(DIRS)
	chmod u+wx $(DIRS)

.PHONY: clean
clean:
	rm -f $(LIB) *~ $(OBJS) a.out result* *.c.*

.PHONY: debug
debug: CFLAGS = -ggdb -flto -flto-partition=none
debug: clean
debug: test

.PHONY: install
install: $(LIB)
	cp $(LIB) $(GCCPLUGINS_DIR)
