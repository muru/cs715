
#------ MAKE CHANGES TO BASE_DIR : Please put the path to base directory of your pristine gcc-4.7.2 build -----------#
BASE_DIR = /opt/gcc-4.7.2/

INSTALL = /opt/gcc-4.7.2
CC = $(INSTALL)/bin/g++-4.7.2

#----- MAKE CHANGES TO OBJS : Add the name of your test file with extension .o (say test as test.o) --------#
#------------------------------- Multiple dependent files maybe also be added ------------------------------#
OBJS = test.o
LIB = mlfcpa.so
GCCPLUGINS_DIR:= $(shell $(CC) -print-file-name=plugin)
INCLUDE= -I$(GCCPLUGINS_DIR)/include

FLAGS= -fPIC -O0 -flto -flto-partition=none

.PHONY: clean test install 
%.o : %.cpp
	$(CC) $(FLAGS) $(INCLUDE) -c $< 

$(LIB): pointsto-callstrings.o 
	$(CC) $(FLAGS) $(INCLUDE) -shared $^ -o $@

test:  $(LIB)
	$(CC) -flto -flto-partition=none -fplugin=./$(LIB) flow_sensitive_3.c -fdump-ipa-all -O2 -fipa-pta
#~/gcc_build/install/bin/gcc -flto -flto-partition=none -fplugin=./$(LIB) test.c -fdump-ipa-all -O2 -fdump-ipa-pta -fipa-pta

clean:
	rm -f $(LIB) *~ *.o a.out result* *.c.*.* .pointsto-callstrings.h.swp .pointsto-callstrings.c.swp

clean_test:
	rm -f a.out result* test.c.* ERROR_test.c.* test*.c.*

install: $(LIB)
	cp $(LIB) $(GCCPLUGINS_DIR)
