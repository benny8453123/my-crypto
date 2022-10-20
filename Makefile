CONFIG_MODULE_SIG=n

obj-m							+= encrypt-test-module.o
encrypt-test-module-objs		:= my-crypto.o tty.o sha256.o proc.o

#CC=gcc
KVERSION = $(shell uname -r)
KERNEL_DIR := /lib/modules/$(KVERSION)/build

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
