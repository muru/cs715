#! /usr/bin/make -f
#------ MAKE CHANGES TO BASE_DIR : Please put the path to base directory of your pristine gcc-4.7.2 build -----------#
BASE_DIR = /opt/gcc-4.7.2/

INSTALL = $(BASE_DIR)
CC = $(BASE_DIR)/bin/g++-4.7.2

#----- MAKE CHANGES TO OBJS : Add the name of your test file with extension .o (say test as test.o) --------#
#------------------------------- Multiple dependent files maybe also be added ------------------------------#
OBJS = test.o

GCCPLUGINS_DIR:= $(shell $(CC) -print-file-name=plugin)
INCLUDE= -I$(GCCPLUGINS_DIR)/include

FLAGS= -fPIC -O2 -flto -flto-partition=none


%.o : %.cpp
	$(CC) $(FLAGS) $(INCLUDE) -c $< 

plugin.so: plugin.o 
	$(CC) $(INCLUDE) $(FLAGS) -shared $^ -o $@

test:  plugin.so test5.c
#	$(CC) -c test1.c -fdump-ipa-all -O2 -fdump-ipa-pta -fipa-pta -o test1.o
#	$(CC) -c test2.c -fdump-ipa-all -O2 -fdump-ipa-pta -fipa-pta -o test2.o
#	$(CC) -flto -flto-partition=none -fplugin=./plugin.so test1.o test2.o -fdump-ipa-all -O2 -fdump-ipa-pta -fipa-pta -o test
	$(CC) -flto -flto-partition=none -fplugin=./plugin.so test4.c -fdump-ipa-all -O0 -fdump-ipa-pta -fipa-pta -o test
#~/gcc_build/install/bin/gcc -flto -flto-partition=none -fplugin=./plugin.so test.c -fdump-ipa-all -O2 -fdump-ipa-pta -fipa-pta

.PHONY: clean
clean:
	rm -f plugin.so *~ *.o a.out result* test.c.* .pointsto-callstrings.h.swp .pointsto-callstrings.c.swp

.PHONY: clean_test
clean_test:
	rm -f a.out result* test*.c.* ERROR_test.c.* test*.c.*

