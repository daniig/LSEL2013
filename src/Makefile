main:
	gcc $$CFLAGS $$LDFLAGS -lnative -lwiringPi main.c -lncurses -o main

%: %.c
	gcc $$CFLAGS $$LDFLAGS -lnative -lwiringPi $^ -c $@


