GCC := arm-linux-gnueabihf-gcc
LD := arm-linux-gnueabihf-ld
OBJCOPY := arm-linux-gnueabihf-objcopy
OBJDUMP := arm-linux-gnueabihf-objdump

TARGET ?= bare

# 头文件路径
INCDIRS :=	imx6ull \
			user \
			user/led \
			user/key \

# 源文件路径
SRCDIRS :=	imx6ull \
			user \
			user/led \
			user/key \

# 每个头文件路径加 -I
INCLUDE	:= $(patsubst %, -I %, $(INCDIRS))

# 获取所有源文件
SFILES := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

# 获取所有无路径的源文件
SFILENDIR := $(notdir $(SFILES))
CFILENDIR := $(notdir $(CFILES))

# 将源文件转换成.o文件
SOBJS := $(patsubst %, objects/%, $(SFILENDIR:.S=.o))
COBJS := $(patsubst %, objects/%, $(CFILENDIR:.c=.o))
OBJS := $(SOBJS) $(COBJS)

# 自动寻找文件依赖
VPATH := $(SRCDIRS)

.PHONY: clean
.PHONY: print

$(TARGET).bin : $(OBJS)
	$(LD) -T imx6ull.lds $^ -o $(TARGET).elf
	$(OBJCOPY) -O binary -S $(TARGET).elf $@
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis
	./tools/mkimage -n ./tools/imximage.cfg.cfgtmp -T imximage -e 0x87800000 -d $(TARGET).bin $(TARGET).imx
	dd if=/dev/zero of=./tools/1k.bin bs=1024 count=1
	cat ./tools/1k.bin $(TARGET).imx > $(TARGET).img

$(SOBJS) : objects/%.o : %.S
	$(GCC) -Wall -nostdlib -c -O2 $(INCLUDE) -o $@ $<

$(COBJS) : objects/%.o : %.c
	$(GCC) -Wall -nostdlib -c -O2 $(INCLUDE) -o $@ $<

clean:
	rm -rf $(TARGET).elf $(TARGET).dis $(TARGET).bin $(TARGET).img $(TARGET).imx $(OBJS)

print:
	@echo INCLUDE = $(INCLUDE)
	@echo SFILES = $(SFILES)
	@echo CFILES = $(CFILES)
	@echo SFILENDIR = $(SFILENDIR)
	@echo CFILENDIR = $(CFILENDIR)
	@echo OBJS = $(OBJS)
