GCC := arm-linux-gnueabihf-gcc
LD := arm-linux-gnueabihf-ld
OBJCOPY := arm-linux-gnueabihf-objcopy
OBJDUMP := arm-linux-gnueabihf-objdump

TARGET ?= bare

# ͷ�ļ�·��
INCDIRS :=	imx6ull \
			user \
			user/led \
			user/key \

# Դ�ļ�·��
SRCDIRS :=	imx6ull \
			user \
			user/led \
			user/key \

# ÿ��ͷ�ļ�·���� -I
INCLUDE	:= $(patsubst %, -I %, $(INCDIRS))

# ��ȡ����Դ�ļ�
SFILES := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

# ��ȡ������·����Դ�ļ�
SFILENDIR := $(notdir $(SFILES))
CFILENDIR := $(notdir $(CFILES))

# ��Դ�ļ�ת����.o�ļ�
SOBJS := $(patsubst %, objects/%, $(SFILENDIR:.S=.o))
COBJS := $(patsubst %, objects/%, $(CFILENDIR:.c=.o))
OBJS := $(SOBJS) $(COBJS)

# �Զ�Ѱ���ļ�����
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
