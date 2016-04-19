ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx)  :  ORIGIN = 0x0800C000, LENGTH = 976K
  STACK (rwx) :  ORIGIN = 0x20000000, LENGTH = 64K
  RAM (rwx)   :  ORIGIN = 0x20010000, LENGTH = 128K
}
_estack = ORIGIN(STACK) + LENGTH(STACK);

SECTIONS {
  /* Interrupt vector table at the beginning of flash */
  .isr_vector ORIGIN(FLASH) : {
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } >FLASH
  /* Then the text section goes into flash */
  .text : {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH
  _sidata = .;
  /* The data section gets a VMA in RAM but an LMA in flash */
  .data : AT ( _sidata ) {
    . = ALIGN(4);
    _sdata = .;
    *(.data)
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } >RAM
  /* The BSS goes into RAM */
  .bss : {
    . = ALIGN(4);
    _sbss = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } >RAM
}
