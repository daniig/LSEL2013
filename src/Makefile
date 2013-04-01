main: main.o
	gcc $$CFLAGS $$LDFLAGS -lnative -lwiringPi $^ -o $@

%: %.c
	gcc $$CFLAGS $$LDFLAGS -lnative -lwiringPi $^ -c $@


