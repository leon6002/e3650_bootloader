TARGET = simple_bootloader
PART_ID = E3650

# Project and SDK locations
PROJECT_ROOT ?= ..
SDK_ROOT ?= ../..

# Architecture and CPU
ARCH = armv8-r
CPU = cortex-r52

# Toolchain settings
CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
SIZE = $(CROSS_COMPILE)size

# Output directory
OUT_DIR = build

ifeq ($(OS),Windows_NT)
  HOST_OS := windows
  SHELL := cmd.exe
  MKDIR_CMD = if not exist "$(OUT_DIR)" mkdir "$(OUT_DIR)"
  CLEAN_CMD = if exist $(OUT_DIR) rd /s /q $(OUT_DIR)
  COPY_CMD = copy /Y "$(subst /,\,$(OUT_DIR)\image_gen_output\$(TARGET).pac)" "$(subst /,\,$@)" >nul
  IMAGE_GEN_TOOL = $(SDK_ROOT)/tools/common/image_gen/image_gen.bat
  IMAGE_GEN_CMD = cmd /c "$(subst /,\,$(IMAGE_GEN_TOOL)) $(DLP_OFFSET) $(ENTRY_ADDR) $(abspath $<) $(FLASH_ADDR)"
else
  HOST_OS := linux
  MKDIR_CMD = mkdir -p "$(OUT_DIR)"
  CLEAN_CMD = rm -rf "$(OUT_DIR)"
  COPY_CMD = cp "$(OUT_DIR)/image_gen_output/$(TARGET).pac" "$@"
  IMAGE_GEN_TOOL_DIR = $(SDK_ROOT)/tools/common/image_gen/builtin_tools/sdtools/linux
  IMAGE_GEN_SETUP_CMD = chmod +x "$(IMAGE_GEN_TOOL_DIR)/atb_signer" "$(IMAGE_GEN_TOOL_DIR)/bin2hex" "$(IMAGE_GEN_TOOL_DIR)/pactool"
  IMAGE_GEN_TOOL = $(SDK_ROOT)/tools/common/image_gen/image_gen.sh
  IMAGE_GEN_CMD = bash "$(IMAGE_GEN_TOOL)" $(DLP_OFFSET) $(ENTRY_ADDR) "$(abspath $<)"
endif

# Include directories
INC_DIRS = \
  $(SDK_ROOT)/arch/include \
  $(PROJECT_ROOT)/board \
  $(SDK_ROOT)/devices/$(PART_ID)/include/cr52p \
  $(SDK_ROOT)/drivers/source/gic \
  $(SDK_ROOT)/middleware/CLI/include \
  $(SDK_ROOT)/middleware/CLI \
  $(SDK_ROOT)/middleware \
  $(SDK_ROOT)/drivers/include \
  $(SDK_ROOT)/devices/include \
  $(SDK_ROOT)/drivers/source/lld/include \
  $(SDK_ROOT)/drivers/source/clk \
  $(SDK_ROOT)/drivers/include/crypto \
  $(SDK_ROOT)/drivers/source/crypto/hal \
  $(SDK_ROOT)/drivers/source/crypto/hal/pke_lld \
  $(SDK_ROOT)/drivers/source/dma \
  $(SDK_ROOT)/drivers/source/fuse \
  $(SDK_ROOT)/drivers/source/lld/src/mbox \
  $(SDK_ROOT)/devices/$(PART_ID)/include \
  $(SDK_ROOT)/drivers/source/reset \
  $(SDK_ROOT)/drivers/source/lld/src/mram \
  $(SDK_ROOT)/drivers/source/spi_nor \
  $(SDK_ROOT)/middleware/bpt/include \
  $(SDK_ROOT)/middleware/checksum \
  $(SDK_ROOT)/middleware/crypto/host/inc \
  $(SDK_ROOT)/middleware/crypto/proxy/inc \
  $(SDK_ROOT)/middleware/sd_boot_img \
  $(SDK_ROOT)/middleware/secure_boot \
  $(SDK_ROOT)/middleware/slot_ab \
  $(PROJECT_ROOT)/configs \
  $(PROJECT_ROOT)/common \
  $(SDK_ROOT)/middleware/remap

# Source files
C_SRCS = \
  $(SDK_ROOT)/arch/armv8-r/arm_faults.c \
  $(SDK_ROOT)/arch/armv8-r/arm_fpu.c \
  $(SDK_ROOT)/arch/armv8-r/arm_irq.c \
  $(SDK_ROOT)/arch/armv8-r/arm_mpu.c \
  $(SDK_ROOT)/arch/armv8-r/arm_tcm.c \
  $(SDK_ROOT)/arch/armv8-r/arm_timer.c \
  $(PROJECT_ROOT)/configs/clock_cfg.c \
  $(PROJECT_ROOT)/configs/dma_cfg.c \
  $(PROJECT_ROOT)/configs/mbox_cfg.c \
  $(PROJECT_ROOT)/configs/pinmux_cfg.c \
  $(PROJECT_ROOT)/configs/reset_cfg.c \
  $(PROJECT_ROOT)/main.c \
  $(PROJECT_ROOT)/board/board.c \
  $(SDK_ROOT)/devices/$(PART_ID)/clock_default_cfg.c \
  $(SDK_ROOT)/devices/$(PART_ID)/clock_ip.c \
  $(SDK_ROOT)/devices/$(PART_ID)/device.c \
  $(SDK_ROOT)/devices/$(PART_ID)/device_init.c \
  $(SDK_ROOT)/devices/$(PART_ID)/device_pin.c \
  $(SDK_ROOT)/devices/$(PART_ID)/reset_ip.c \
  $(SDK_ROOT)/drivers/source/clk/sdrv_ckgen.c \
  $(SDK_ROOT)/drivers/source/clk/sdrv_ckgen_dump.c \
  $(SDK_ROOT)/drivers/source/clk/sdrv_ckgen_hw_access.c \
  $(SDK_ROOT)/drivers/source/clk/sdrv_fs24m_hw_access.c \
  $(SDK_ROOT)/drivers/source/clk/sdrv_fs32k_hw_access.c \
  $(SDK_ROOT)/drivers/source/clk/sdrv_pll_hw_access.c \
  $(SDK_ROOT)/drivers/source/crypto/hal/hfe_lld.c \
  $(SDK_ROOT)/drivers/source/crypto/hal/pke_lld/pke_curve.c \
  $(SDK_ROOT)/drivers/source/crypto/hal/pke_lld/pke_lld.c \
  $(SDK_ROOT)/drivers/source/crypto/hal/pke_lld/pke_prime.c \
  $(SDK_ROOT)/drivers/source/crypto/hal/ske_lld.c \
  $(SDK_ROOT)/drivers/source/crypto/hal/trng_lld.c \
  $(SDK_ROOT)/drivers/source/crypto/src/crypto_common.c \
  $(SDK_ROOT)/drivers/source/crypto/src/crypto_init.c \
  $(SDK_ROOT)/drivers/source/crypto/src/crypto_sys.c \
  $(SDK_ROOT)/drivers/source/crypto/src/hash.c \
  $(SDK_ROOT)/drivers/source/crypto/src/hmac.c \
  $(SDK_ROOT)/drivers/source/crypto/src/pke/ecc.c \
  $(SDK_ROOT)/drivers/source/crypto/src/pke/rsa.c \
  $(SDK_ROOT)/drivers/source/crypto/src/pke/sm2.c \
  $(SDK_ROOT)/drivers/source/crypto/src/pke/sm9.c \
  $(SDK_ROOT)/drivers/source/crypto/src/self_check.c \
  $(SDK_ROOT)/drivers/source/crypto/src/ske.c \
  $(SDK_ROOT)/drivers/source/crypto/src/trng.c \
  $(SDK_ROOT)/drivers/source/dma/sdrv_dma.c \
  $(SDK_ROOT)/drivers/source/fuse/sdrv_fuse.c \
  $(SDK_ROOT)/drivers/source/gic/arm_gic.c \
  $(SDK_ROOT)/drivers/source/gic/irq.c \
  $(SDK_ROOT)/drivers/source/gpio/sdrv_gpio.c \
  $(SDK_ROOT)/drivers/source/iecc/sdrv_iecc.c \
  $(SDK_ROOT)/drivers/source/lld/src/Semag/Semag.c \
  $(SDK_ROOT)/drivers/source/lld/src/fmmu/sdrv_fmmu_lld.c \
  $(SDK_ROOT)/drivers/source/lld/src/mbox/Mbox.c \
  $(SDK_ROOT)/drivers/source/lld/src/mbox/Mbox_Ip.c \
  $(SDK_ROOT)/drivers/source/lld/src/mram/Mram_Ip.c \
  $(SDK_ROOT)/drivers/source/mram/sdrv_mram.c \
  $(SDK_ROOT)/drivers/source/pinctrl/sdrv_pinctrl.c \
  $(SDK_ROOT)/drivers/source/reset/sdrv_rstgen.c \
  $(SDK_ROOT)/drivers/source/scr/sdrv_scr.c \
  $(SDK_ROOT)/drivers/source/spi_nor/sdrv_hyperbus.c \
  $(SDK_ROOT)/drivers/source/spi_nor/sdrv_spi_nor.c \
  $(SDK_ROOT)/drivers/source/spi_nor/sdrv_xspi.c \
  $(SDK_ROOT)/drivers/source/syscnt/sdrv_syscnt.c \
  $(SDK_ROOT)/drivers/source/uart/sdrv_uart.c \
  $(SDK_ROOT)/middleware/CLI/CLI_console.c \
  $(SDK_ROOT)/middleware/CLI/FreeRTOS_CLI.c \
  $(SDK_ROOT)/middleware/bootloader_message/bootloader_message.c \
  $(SDK_ROOT)/middleware/bpt/bpt.c \
  $(SDK_ROOT)/middleware/checksum/crc32.c \
  $(SDK_ROOT)/middleware/checksum/sha256.c \
  $(SDK_ROOT)/middleware/checksum/sha512.c \
  $(SDK_ROOT)/middleware/crypto/host/src/crypto_host_common.c \
  $(SDK_ROOT)/middleware/crypto/host/src/crypto_host_ecc.c \
  $(SDK_ROOT)/middleware/crypto/host/src/crypto_host_hash.c \
  $(SDK_ROOT)/middleware/crypto/host/src/crypto_host_mac.c \
  $(SDK_ROOT)/middleware/crypto/host/src/crypto_host_padding.c \
  $(SDK_ROOT)/middleware/crypto/host/src/crypto_host_rsa.c \
  $(SDK_ROOT)/middleware/crypto/host/src/crypto_host_trng.c \
  $(SDK_ROOT)/middleware/crypto/proxy/src/crypto_proxy_mbox.c \
  $(SDK_ROOT)/middleware/debug/debug.c \
  $(SDK_ROOT)/middleware/printf/printf.c \
  $(SDK_ROOT)/middleware/remap/remap.c \
  $(SDK_ROOT)/middleware/sd_boot_img/sd_boot_img.c \
  $(SDK_ROOT)/middleware/secure_boot/secure_boot.c \
  $(SDK_ROOT)/middleware/slot_ab/slot_ab.c \
  $(SDK_ROOT)/middleware/udelay/udelay.c \
  gcc_support.c

S_SRCS = \
  $(SDK_ROOT)/arch/armv8-r/hightec/arm_atomic.S \
  $(SDK_ROOT)/arch/armv8-r/hightec/arm_cache.S \
  $(SDK_ROOT)/arch/armv8-r/hightec/arm_exceptions.S \
  $(SDK_ROOT)/arch/armv8-r/hightec/arm_fullcontextrestore.S \
  $(SDK_ROOT)/arch/armv8-r/hightec/arm_saveusercontext.S \
  $(SDK_ROOT)/arch/armv8-r/hightec/arm_switchcontext.S \
  $(SDK_ROOT)/arch/armv8-r/hightec/arm_tcm_asm.S \
  $(SDK_ROOT)/devices/$(PART_ID)/startup/hightec/cr52_startup.S \
  $(SDK_ROOT)/drivers/source/lld/src/mram/Mram_IpAsm.S

# Compiler flags
COMMON_FLAGS = -mcpu=$(CPU) -mfloat-abi=hard -mfpu=fp-armv8 -g -finline -include $(PROJECT_ROOT)/configs/config.h
CFLAGS = $(COMMON_FLAGS) -mthumb -Og -std=c99 -Wall -Wno-unused-parameter -Wno-unused-function \
         -ffunction-sections -fdata-sections -mno-unaligned-access -D__ARM_ARCH=8 \
         $(addprefix -I, $(INC_DIRS))

ASMFLAGS = $(COMMON_FLAGS) -D__ARM_ARCH=8 -DASSEMBLY \
           $(addprefix -I, $(INC_DIRS))

LDFLAGS = --specs=nosys.specs --specs=nano.specs -Tlinker.ld \
          $(COMMON_FLAGS) -mthumb \
          -Wl,--gc-sections -Wl,-Map=$(OUT_DIR)/$(TARGET).map

# Objects
OBJS = $(addprefix $(OUT_DIR)/, $(notdir $(C_SRCS:.c=.o)))
OBJS += $(addprefix $(OUT_DIR)/, $(notdir $(S_SRCS:.S=.o)))

vpath %.c $(sort $(dir $(C_SRCS)))
vpath %.S $(sort $(dir $(S_SRCS)))

# PAC generation settings
ENTRY_ADDR = 0xA04000
DLP_OFFSET = 0x8
FLASH_ADDR = 0x8000000

# Targets
.PHONY: all pac clean

all: $(OUT_DIR) $(OUT_DIR)/$(TARGET).bin

pac: $(OUT_DIR)/$(TARGET).pac

$(OUT_DIR):
	@$(MKDIR_CMD)

$(OUT_DIR)/%.o: %.c
	@echo "Compiling $(notdir $<)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR)/%.o: %.S
	@echo "Assembling $(notdir $<)"
	@$(CC) $(ASMFLAGS) -c $< -o $@

$(OUT_DIR)/$(TARGET).elf: $(OBJS)
	@echo "Linking $@"
	@$(LD) $(LDFLAGS) -o $@ $^
	@$(SIZE) $@

$(OUT_DIR)/$(TARGET).bin: $(OUT_DIR)/$(TARGET).elf
	@echo "Generating binary $@"
	@$(OBJCOPY) -O binary $< $@

$(OUT_DIR)/$(TARGET).pac: $(OUT_DIR)/$(TARGET).bin
	@echo "Generating PAC image using $(notdir $(IMAGE_GEN_TOOL))..."
ifdef IMAGE_GEN_SETUP_CMD
	@$(IMAGE_GEN_SETUP_CMD)
endif
	@$(IMAGE_GEN_CMD)
	@$(COPY_CMD)

clean:
	@$(CLEAN_CMD)
