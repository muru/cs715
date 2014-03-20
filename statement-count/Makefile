
CXX = g++-4.7.2
CC = gcc-4.7.2

GCCPLUGINS_DIR:= $(shell $(CC) -print-file-name=plugin)
INCLUDE= -I$(GCCPLUGINS_DIR)/include

FLAGS= -fPIC -O2
CFLAGS = -fdump-ipa-all -O0 -fdump-tree-all

plugin.so: plugin.cpp
	$(CXX) $(INCLUDE) $(FLAGS) -shared $^ -o $@

test: plugin.so test1.c test2.c test.h
# Specifiy plugin for each file if output for each file is needed.
# 	$(CXX) -fplugin=./plugin.so -flto -flto-partition=none -c test.c   $(CFLAGS)
# 	$(CXX) -fplugin=./plugin.so -flto -flto-partition=none -c test2.c  $(CFLAGS)
	$(CXX) -flto -c test1.c $(CFLAGS)
	$(CXX) -flto -c test2.c $(CFLAGS)
	$(CXX) -fplugin=./plugin.so -flto -flto-partition=none -o test test1.o  test2.o $(CFLAGS)
# Partition = none explicitly loads all bodies, so they appear part of the same
# translation unit. Partition = 1to1 explicitly retains the original distinction,
# and the default (partition = balanced), does its own thing. Look for ltrans files
# if partition != none.
# 	$(CXX) -fplugin=./plugin.so -flto -flto-partition=1to1 -o test test1.o test2.o $(CFLAGS)
# 	$(CXX) -fplugin=./plugin.so -flto -o test test1.o test2.o $(CFLAGS)

clean:
	rm -f plugin.so test *.*.* *~ *.o a.out
