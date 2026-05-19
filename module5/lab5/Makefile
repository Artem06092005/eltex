obj-m += kernelspace.o

all : userspace kernelspace
	
userspace:
	gcc -o userspace userspace.c

kernelspace:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean: cleanUser
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

cleanUser:
	rm -rf userspace