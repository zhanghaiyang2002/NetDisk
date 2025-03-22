SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
CC:=gcc
CFLAGS:=-g
ELF:=process_pool
$(ELF):$(OBJS)
	gcc -o $@ $^

rebuild:clean $(ELF)
clean:
	rm -rf $(OBJS) $(ELF)
