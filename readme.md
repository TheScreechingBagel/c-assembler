# Instruction set
```asm
mov reg1, CONSTANT ; Move CONSTANT into register 1
    ; 00 CCCCCC where C is the bits of the constant

mov A, B ; Move register B into register A
    ; 10 BBB AAA where A/B are the bits of register A/B

or A, reg1, reg2 ; Perform reg1 | reg2 and store result in register A
    ; 01 AAA 000

add A, reg1, reg2 ; Perform reg1 + reg2 and store result in register A
    ; 01 AAA 001

sub A, reg1, reg2 ; Perform reg1 - reg2 and store result in register A
    ; 01 AAA 010

nand A, reg1, reg2 ; Perform ~(reg1 & reg2) and store result in register A
    ; 01 AAA 011

and A, reg1, reg2 ; Perform reg1 & reg2 and store result in register A
    ; 01 AAA 100

not A, reg1 ; Perform ~reg1 and store result in register A
    ; 01 AAA 101

xor A, reg1, reg2 ; Perform reg1 ^ reg2 and store result in register A
    ; 01 AAA 110

eq A, reg1, reg2 ; Perform reg1 == reg2 and store result in register A
    ; 01 AAA 111

mov reg1, [reg2] ; Read from RAM at address register 2 and store result in register 1
    ; 11 000000

mov [reg2], reg1 ; Move register 1 into RAM at address register 2
    ; 11 000001

```
