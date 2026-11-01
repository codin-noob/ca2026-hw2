# Bare-metal RV32I programs for rv32emu system emulation (ELF_LOADER build).
#
#   make                       build every program at $(OPT)
#   make run-hello             build and run one program
#   make OPT=-O2 run-hello     objects go to build/O2, so no clean needed
#   make clean

RV32EMU ?= $(HOME)/rv32emu
include $(RV32EMU)/mk/toolchain.mk
EMU ?= $(RV32EMU)/build/rv32emu

OPT ?= -O0
BUILD = build/$(subst -,,$(OPT))

ARCH = -march=rv32i_zicsr -mabi=ilp32
AFLAGS = -g $(ARCH)
CFLAGS = -g $(OPT) $(ARCH) -Wall -Wextra -ffreestanding -Icommon
LDFLAGS = -T common/linker.ld --no-warn-rwx-segments

CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
LD = $(CROSS_COMPILE)ld
OBJDUMP = $(CROSS_COMPILE)objdump

PROGS = hello

COMMON_SRCS = common/start.S common/perfcounter.S \
              common/print.c common/string.c common/mul.c
objs = $(patsubst %,$(BUILD)/%.o,$(basename $(1)))
COMMON_OBJS = $(call objs,$(COMMON_SRCS))

.PHONY: all clean
all: $(foreach p,$(PROGS),$(BUILD)/$(p).elf)

# One ELF per program directory: common objects + every .c/.S in it.
define PROG_template
$(BUILD)/$(1).elf: $(COMMON_OBJS) $(call objs,$(wildcard $(1)/*.c $(1)/*.S)) common/linker.ld
	$$(LD) $$(LDFLAGS) -o $$@ $$(filter %.o,$$^)

.PHONY: run-$(1) dump-$(1)
run-$(1): $(BUILD)/$(1).elf
	@test -x $$(EMU) || (echo "Error: $$(EMU) not found" && exit 1)
	$$(EMU) $$<

dump-$(1): $(BUILD)/$(1).elf
	$$(OBJDUMP) -Ds $$< | less
endef
$(foreach p,$(PROGS),$(eval $(call PROG_template,$(p))))

# string.c must not have its own loops turned into calls to itself.
$(BUILD)/common/string.o: common/string.c common/runtime.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -fno-tree-loop-distribute-patterns -c $< -o $@

$(BUILD)/%.o: %.c common/runtime.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.S
	@mkdir -p $(dir $@)
	$(AS) $(AFLAGS) $< -o $@

clean:
	rm -rf build
