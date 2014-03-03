
CC = g++-4.7.2

OBJS = plugin.o

GCCPLUGINS_DIR:= $(shell $(CC) -print-file-name=plugin)
INCLUDE= -I$(GCCPLUGINS_DIR)/include -I$(CC)

FLAGS= -fPIC -O2

plugin.so: plugin.c 
	$(CC) $(INCLUDE) $(FLAGS) -shared $^ -o $@

test: plugin.so
	$(CC) -fplugin=./plugin.so -flto -flto-partition=none -c test.c -fdump-ipa-all -O0

clean:
	rm -f plugin.so test *.c.* *~ *.o a.out
