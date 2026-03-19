OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)

MEMORY
{
  TCM (RW)      : ORIGIN = 0x00000080, LENGTH = 0x0001FF80
  IRAM_LP (RW)  : ORIGIN = 0x00F00000, LENGTH = 0x00020000
  RAM (RW)      : ORIGIN = 0x00A04000, LENGTH = 0x000FC000
  CRAM (RW)     : ORIGIN = 0x00800000, LENGTH = 0x00040000
  DMALOCALBUF (RW)     : ORIGIN = 0xf0978000, LENGTH = 0x1000
  CRAMECC (RW)  : ORIGIN = 0x00900000, LENGTH = 0x00020000
  RAMECC (RW)   : ORIGIN = 0x00D00000, LENGTH = 0x00060000
}

CSTACK_SIZE = 0x2000;
SVC_STACK_SIZE = 0x1000;
IRQ_STACK_SIZE = 0x80;
FIQ_STACK_SIZE = 0x1000;
HYP_STACK_SIZE = 0x1000;
UND_STACK_SIZE = 0x80;
ABT_STACK_SIZE = 0x80;

SECTIONS
{
    /* Code and read-only data */
    .text :
    {
        KEEP(*(.exception_vectors))
        __hyp_vector = .;
        KEEP(*(.hyp_intvec))
        . = ALIGN(32);
        __sys_vector = .;
        KEEP(*(.sys_intvec))
        . = ALIGN(128);
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
        __etext = .;
    } > RAM

    /* Data initialized with non-zero values */
    .data :
    {
        . = ALIGN(4);
        __data_start = .;
        *(.data*)
        
        . = ALIGN(4);
        __commands_start = .;
        KEEP(*(.commands))
        __commands_end = .;
        
        . = ALIGN(4);
        __data_end = .;
    } > RAM

    /* BSS (Block Started by Symbol) - Uninitialized data */
    .bss :
    {
        . = ALIGN(4);
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end = .;
    } > RAM

    /* Early copy table */
    .earlycopy :
    {
        . = ALIGN(4);
        __earlycopy_start = .;
        __earlycopy_end = .;
    } > RAM

    /* Stacks in TCM */
    .stack (NOLOAD) :
    {
        . = ALIGN(8);
        __stack_start__ = .;
        . += HYP_STACK_SIZE;
        __hyp_stack_end = .;
        . += CSTACK_SIZE;
        __cstack_end = .;
        . += SVC_STACK_SIZE;
        __svc_stack_end = .;
        . += IRQ_STACK_SIZE;
        __irq_stack_end = .;
        . += FIQ_STACK_SIZE;
        __fiq_stack_end = .;
        . += UND_STACK_SIZE;
        __und_stack_end = .;
        . += ABT_STACK_SIZE;
        __abt_stack_end = .;
        __stack_end__ = .;
    } > TCM

    /* Heap in RAM */
    .heap (NOLOAD) :
    {
        . = ALIGN(8);
        __heap_start__ = .;
        . += 0x8000;
        __heap_end__ = .;
    } > RAM

    /* Thread Local Storage (TLS) sections */
    .tdata :
    {
        __tdata_start = .;
        *(.tdata .tdata.* .gnu.linkonce.td.*)
        __tdata_end = .;
    } > RAM

    .tbss :
    {
        __tbss_start = .;
        *(.tbss .tbss.* .gnu.linkonce.tb.*)
        *(.tcommon)
        __tbss_end = .;
    } > RAM

    __tls_base = __tdata_start;
    __tls_end = __tbss_end;
    __end__ = .;
}
