# Spiral Machine
This is a small project I started when I was bored. Spiral Machine is a theoretical 8-bit machine with a simple ISA and limited memory. 

My objective was to design the ISA, make an Assembler and develop an Interpreter as a learning tool.
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

# ISA
## Instruction format
Instructions are 4 bytes long.

Format 1: OPCode, Target (Higher Byte), Target (Lower Byte), Value

Format 2: OPCode, Target (Higher Byte), Target (Lower Byte), 0x00

Format 3: OPCode, 0x00, 0x00, 0x00

For instructions that use Immediate Memory Values, target is calculated based on the Program and Video Memory offset.

**Example:**

=> LDI 0x2, 0x3

The immediate value **0x3** would be loaded into memory address **0x0C6 + 0x2**.  


