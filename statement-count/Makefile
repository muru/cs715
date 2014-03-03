
CXX = g++-4.7.2
CC = gcc-4.7.2

GCCPLUGINS_DIR:= $(shell $(CC) -print-file-name=plugin)
INCLUDE= -I$(GCCPLUGINS_DIR)/include

FLAGS= -fPIC -O2
CFLAGS = -fdump-ipa-all -O0 -fdump-tree-all

plugin.so: plugin.cpp
	$(CXX) $(INCLUDE) $(FLAGS) -shared $^ -o $@

test: plugin.so test.c test2.c test.h
	$(CXX) -fplugin=./plugin.so -flto -flto-partition=none -c test.c   $(CFLAGS)
	$(CXX) -fplugin=./plugin.so -flto -flto-partition=none -c test2.c  $(CFLAGS)
	$(CXX) -fplugin=./plugin.so -flto -flto-partition=none -o test test.o  test2.o $(CFLAGS)

clean:
	rm -f plugin.so test *.c.* *~ *.o a.out
