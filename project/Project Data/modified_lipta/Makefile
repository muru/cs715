
#------ MAKE CHANGES TO BASE_DIR : Please put the path to base directory of your pristine gcc-4.7.2 build -----------#
BASE_DIR = /home/nikhil/gcc_build

INSTALL = $(BASE_DIR)/install
CC = $(INSTALL)/bin/g++
NEW_PATH = $(BASE_DIR)/gcc-4.7.2/gcc

#----- MAKE CHANGES TO OBJS : Add the name of your test file with extension .o (say test as test.o) --------#
#------------------------------- Multiple dependent files maybe also be added ------------------------------#
OBJS = test.o

GCCPLUGINS_DIR:= $(shell $(CC) -print-file-name=plugin)
INCLUDE= -I$(GCCPLUGINS_DIR)/include -I$(NEW_PATH)

FLAGS= -fPIC -O2 -flto -flto-partition=none


%.o : %.c
	$(CC) $(FLAGS) $(INCLUDE) -c $< 

plugin.so: pointsto-callstrings.o 
	$(CC) $(INCLUDE) $(FLAGS) -shared $^ -o $@

test:  plugin.so
	~/gcc_build/install/bin/gcc -flto -flto-partition=none -fplugin=./plugin.so test.c -fdump-ipa-all -O2 -fdump-ipa-pta -fipa-pta
#	$(CC) -flto -flto-partition=none -fplugin=./plugin.so test.c -fdump-ipa-all -O2 -fdump-ipa-pta -fipa-pta

clean:
	\rm -f plugin.so *~ *.o a.out result* test.c.* .pointsto-callstrings.h.swp .pointsto-callstrings.c.swp

clean_test:
	\rm -f a.out result* test.c.* ERROR_test.c.* test*.c.*

