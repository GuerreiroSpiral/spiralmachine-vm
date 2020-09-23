# Spiral Machine
This is a small project I started when I was bored. Spiral Machine is a theoretical 8-bit machine with a simple ISA and limited memory. 

My objective was to design the ISA, make an Assembler and develop an Interpreter as a learning tool, so there's no support for floating point decimals, overflows/underflows and more advanced stuff.

# Specifications
## Specifications:

RAM: 8kb

Stack: 2kb

VRAM: 1kb (32x32 screen)

## Memory Map:

Range | Definition
------- | -------
$0x0000 - $0x0003 | .spm file header
$0x0004 - $0x0404 | VRAM
$0x0405 - $0x0C05 | Stack
$0x0C06 - $0x2000 | Program Memory

## Registers
Register | Definition
-------  | -------
X (1 byte) | All purpose
Y (1 byte) | All purpose
R (1 byte) | ALU results
T (1 byte) | Temporary Register
SP (2 byte) | Stack Pointer
PC (2 byte) | Program Counter

## Flags
Flag | Definition
------ | -------
E | Equal
XG | X is greater than
YG | Y is greater than

# ISA
## Instruction format
Instructions are 4 bytes long.

Format 1: OPCode, Target (Higher Byte), Target (Lower Byte), Value

Format 2: OPCode, Target (Higher Byte), Target (Lower Byte), 0x00

Format 3: OPCode, 0x00, 0x00, 0x00

For instructions that use Immediate Memory Values, target is calculated based on the Program and Video Memory offset.

**Example:**

=> LDI 0x0002, 0x0003

The immediate value **0x3** would be loaded into memory address **0x0C6 + 0x2**.  **This is implemented in the interpreter, not in the Assembler.**

## Instruction Table
Instruction | Mnemonic | OPCode | Description
----------- | ----------- | ----------- | ----------- |
LOAD TO X | LDI (X), I | 0x00 | Loads an immediate value (I) to the memory address (X).
LOAD TO RX | LDX I | 0x01 | Loads an immediate value (I) to the X register.
LOAD TO RY | LDY I | 0x02 | Loads an immediate value (I) to the Y register.
LOAD Y TO X | LXY | 0x03 | Loads the value of the Y register to the X register.
LOAD X TO Y | LYX | 0x04 | Loads the value of the X register to the Y register.
LOAD X TO (X) | LMX (X) | 0x0A | Loads the value of the X register to memory address (X).
LOAD Y TO (X) | LMY (X) | 0x0B | Loads the value of the Y register to memory address (X).
JUMP TO SUBROUTINE (X) | JSR (X) | 0x05 | Jumps to the instruction at memory address (X). The current Program Counter value is pushed onto the stack.
JUMP TO LOCATION (X) | JMP (X) | 0x06 | Jumps to the instruction at memory address (X).
JUMP TO LABEL (X) | JTL X | 0x16 | Jumps to the previously defined label X.
PUSH PC TO STACK | PCS | 0x1D | Pushes the current value of the Program Counter to the stack.
PUSH X TO STACK | PHA X | 0x07 | Pushes the immediate value X to the stack.
PUSH RX TO STACK | PXA | 0x21 | Pushes the value of the X register to the stack.
PUSH RY TO STACK | PYA | 0x22 | Pushes the value of the Y register to the stack.
POP FROM STACK | POP | 0x08 | pop() the stack.
POP TO PC | PPC | 0x1C | pop() the stack and loads the value into PC.
POP TO TEMP | PTM | 0x23 | pop() the stack and loads the value into the T register.
INCREMENT X | INX | 0x09 | Increments the value of the X register
INCREMENT Y | INY | 0x0C | Increments the value of the Y register
DECREMENT X | DEX | 0x0D | Decrements the value of the X register
DECREMENT Y | DEY | 0x0E | Decrements the value of the Y register
ADD RX + RY | AXY | 0x11 | Sums the value of the X register with the Y register and loads the result into the R register.
SUB RX - RY | SXY | 0x12 | Subtracts the value of the X register with the Y register (X-Y) and loads the result into the R register.
SUB RY - RX | SYX | 0x13 | Subtracts the value of the Y register with the X register (Y-X) and loads the result into the R register.
COMPARE RX, RY* | CMP | 0x17 | Compares the value of the X and Y register through a SXY instruction the R register and sets flags accordingly.
BRANCH IF EQUAL | BEQ (X) | 0x18 | Branches to memory location (X) if the E flag is set. The current Program Counter value is pushed onto the stack.
BRANCH IF X GREATER | BXG X | 0x19 | Branches to memory location (X) if the XG flag is set. The current Program Counter value is pushed onto the stack.
BRANCH IF Y GREATER | BYG X | 0x1A | Branches to memory location (X) if the YG flag is set. The current Program Counter value is pushed onto the stack.
BLIT AT X | BLT X | 0x1B | Blits the pixel value of the (X) memory location on the VRAM into the screen.
LOAD I TO VRAM LOCATION X | VRI (X), I | 0x1E | Loads the immediate value I into VRAM address (X).
LOAD RX TO VRAM LOCATION X | VRX (X) | 0x1F | Loads the value of the X register into VRAM address (X).
LOAD RY TO VRAM LOCATION X | VRY (X) | 0x20 | Loads the value of the Y register into VRAM address (X).
LOAD TEMP TO X | LTX | 0x25 | Loads the value of the T register into the X register.
LOAD TEMP TO Y | LTY | 0x24 | Loads the value of the T register into the Y register.

\* = If RR = 0, sets the E flag and clears the other two. If RR < 0, sets the YG flag and clears the other two. If RR > 0, sets the XG flag and clears the other two.
