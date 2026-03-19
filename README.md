# E3650 Bootloader

This repository contains the standalone `simple_bootloader` project for
the E3650 platform.

It provides a minimal CR52 boot flow for E3650 that:

- Initializes the basic platform state, including clocks, reset signals,
  pinmux, optional MPU setup, and MRAM access
- Brings up the debug console on `UART15` and also enables additional
  debug output on `UART18` and `UART19`
- Reads the application boot package table (BPT) from MRAM flash
- Validates and loads the target `CR52P0` image from the BPT
- Obtains the application entry point and remaps the CR52 cores to start
  the loaded image
- Produces `.bin` and `.pac` boot artifacts for deployment with the
  SemiDrive image generation flow

The official SSDK provides multiple build environments, including
IAR and GHS. This repository currently keeps and maintains the GCC
build flow only, so the project-specific build files are placed under
`GCC/`.

Project layout:

- `main.c`
- `configs/`
- `board/`
- `common/`
- `GCC/`

Supported debug UARTs in the current configuration:

- `UART15`
- `UART18`
- `UART19`

## Prerequisites

Required SSDK version: `E3 SSDK PTG5.2 release`

Linux:

- GNU Arm Embedded Toolchain with `arm-none-eabi-gcc` in `PATH`
- `make`
- `bash`
- Standard Unix utilities used by the build and PAC flow, including
  `chmod`, `cp`, `mkdir`, and `rm`
- Execute permission for the Linux image generation tools under
  `$(SDK_ROOT)/tools/common/image_gen/builtin_tools/sdtools/linux/`

Windows:

- GNU Arm Embedded Toolchain with `arm-none-eabi-gcc` in `PATH`
- `mingw32-make.exe`
- `cmd.exe` environment for running `image_gen.bat`
- A checkout of `E3 SSDK PTG5.2 release` available through `SDK_ROOT`
- `SDToolBox_R2.25.1401` or later for the PTG5.2 release tool flow

Documentation and release packages:

- Documentation and images are distributed through the customer support
  system: `http://support.semidrive.com/account/login`
- Contact the SemiDrive marketing or product support team to request
  access permission

Source code and tools:

- `E3 SSDK PTG5.2 release` requires `SDToolBox_R2.25.1401` or later
- The required tools can be obtained from the SemiDrive customer support
  system through the appropriate support contact

## Build

From `e3650_bootloader/GCC`:

Windows:

```sh
mingw32-make.exe SDK_ROOT=D:/path/to/ssdk all pac
```

Linux:

```sh
make SDK_ROOT=/path/to/ssdk all pac
```

## Outputs

- `build/simple_bootloader.elf`
- `build/simple_bootloader.bin`
- `build/simple_bootloader.pac`

## Notes

- Windows PAC generation uses `tools/common/image_gen/image_gen.bat`.
- Linux PAC generation uses `tools/common/image_gen/image_gen.sh`.
- `SDK_ROOT` points to the external SSDK checkout that provides `arch/`,
  `devices/`, `drivers/`, `middleware/`, and `tools/`.
- The current Makefile and PAC generation flow were validated against
  `E3 SSDK PTG5.2 release`.
- The expected tool package version for this setup is
  `SDToolBox_R2.25.1401` or later.
- The Linux path was validated with a Makefile dry-run in this workspace;
  Windows build and PAC generation were executed end-to-end.
