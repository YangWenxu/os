#//生成打模块名称是list.ko
obj-m := list.o                                 
#//内核源码目录：由当前运行内核使用打模块目录中打build链接指定
KDIR := /lib/modules/$(shell uname -r)/build    
#//当前目录
PWD := $(shell pwd) 
#//-c指定内核源码目录，M指定模块源码目录                          
default: 
	make -C $(KDIR) M=$(PWD) modules
clean: 
	make -C $(KDIR) M=$(PWD) clean
