#ifndef SPIRAL_MACHINE_H
#define SPIRAL_MACHINE_H

#include <stdint.h>

#define MEMORY_SIZE 0x10000
#define STACK_SIZE 0x4000

#define HEADER_START 0x0000
#define HEADER_END 0x0003

#define VRAM_START 0x0004
#define VRAM_END 0x0404

#define STACK_START 0x0405
#define STACK_END 0x0C05

#define PROGRAM_MEMORY_START 0x0C06
#define PROGRAM_MEMORY_END 0x2000

class SpiralMachine{
    enum EightBitRegisters {
        X,
        Y, 
        R, 
        T, 
        EIGHT_B_REGISTERS
    };

    enum SixteenBitRegisters{
        SP, 
        PC,
        SIXTEEN_B_REGISTERS
    };

    enum Flags {
        E,
        XG,
        YG,
        FLAGS
    };

    bool flags[FLAGS];
    uint8_t memory[MEMORY_SIZE];
    uint8_t eight_b_registers[EIGHT_B_REGISTERS];
    uint16_t sixteen_b_registers[SIXTEEN_B_REGISTERS];

    void set_flag(Flags flag, bool value);
    void fde_cycle();
    uint16_t fetch_instruction();

    // Stack operations
    void pop();
    void push(uint8_t value);

    // CPU operations
    void LDI(uint16_t mem_location, uint8_t value);
    void LDX(uint8_t value);
    void LDY(uint8_t value);
    void LXY();
    void LYX();
    void LMX(uint16_t mem_location);
    void LMY(uint16_t mem_location);

    void JSR(uint16_t mem_location);
    void JMP(uint16_t mem_location);
    void JTL(uint16_t mem_location);

    void PCS();
    void PHA(uint8_t value);
    void PXA();
    void PYA();
    void POP();
    void PPC();
    void PTM();

    void INX();
    void INY();
    void DEX();
    void DEY();

    void LEX(uint16_t mem_location);
    void LEY(uint16_t mem_location);

    void AXY();
    void SXY();
    void SYX();
    void CMP();

    void BEQ(uint16_t mem_location);
    void BXG(uint16_t mem_location);
    void BYG(uint16_t mem_location);
    void BLT(uint16_t mem_location);

    void VRI(uint16_t mem_location, uint8_t value);

    void VRX(uint16_t mem_location);
    void VRY(uint16_t mem_location);

    void LTX();
    void LTY();
};

#endif // SPIRAL_MACHINE_H