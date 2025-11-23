obj-m := flag_proc.o

KDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

ifeq ($(wildcard $(KDIR)),)
$(error Kernel build directory $(KDIR) not found. Install matching kernel headers (e.g. sudo apt install linux-headers-$(shell uname -r)))
endif

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
