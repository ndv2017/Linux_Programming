EXTRA_CFLAGS = -Wall
obj-m = first_driver.o

KDIR := /lib/modules/`uname -r`/build

all:
	make -C $(KDIR) M=`pwd` modules
	gcc -o test_app test_app.c

clean:
	make -C $(KDIR) M=`pwd` clean
	rm -rf test_app
