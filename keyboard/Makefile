obj-m += counter.o

# all:
# 	make -C /disk/tmp/linux-6.1.9 M=$(PWD) M=$(PWD) modules

# clean:
# 	make -C /disk/tmp/linux-6.1.9 M=$(PWD) M=$(PWD) clean
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
