obj-m    := wgpio.o

KDIR    := /lib/modules/$(shell uname -r)/build
PWD    := $(shell pwd)

# extra warning flags
#ccflags-y := -fdiagnostics-show-option -Wall -Wextra -Wdouble-promotion -Wformat=2 -Winit-self -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wstrict-overflow=5 -Wno-div-by-zero -Wtrampolines -Wfloat-equal -Wundef -Wno-endif-labels -Wshadow -Wunsafe-loop-optimizations -Wsync-nand -Wbad-function-cast -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion -Wjump-misses-init -Wlogical-op -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-noreturn -Wmissing-format-attribute -Wnormalized=nfc -Wpacked -Wpadded -Wredundant-decls -Wnested-externs -Wunreachable-code -Winline -Winvalid-pch -Wdisabled-optimization -Wmissing-prototypes -Wmissing-declarations -Woverlength-strings -Wunsuffixed-float-constants -Wall -Wextra -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith  -Wsystem-headers     -Wa,--fatal-warnings  -Wreturn-type -Wswitch -Wshadow -Wcast-qual -Wwrite-strings -Wextra -Wno-sign-compare -Wold-style-definition -Wconversion -Wsign-compare -Wformat=2   

# disable some warning flags
#ccflags-y += -Wno-declaration-after-statement -Wno-missing-prototypes -Wno-missing-declarations -Wno-cast-qual -Wno-sign-conversion -Wno-conversion -Wno-unused-parameter -Wno-nested-externs -Wno-padded -Wno-pointer-arith -Wno-redundant-decls
ccflags-y += -Wno-unused-parameter 
# enable some extra warning flags
KBUILD_ENABLE_EXTRA_GCC_CHECKS := 3
# increase verbosity
KBUILD_VERBOSE := 1

all: dfread
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm dfread

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

dfread:
	gcc -g --std=c99 dfread.c -o dfread

.PHONY: all clean default dfread
