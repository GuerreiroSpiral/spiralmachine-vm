#include <stdint.h>
#include "SpiralMachine.h"

uint16_t SpiralMachine::fetch_instruction(){
    return memory[sixteen_b_registers[PC]];
}

void SpiralMachine::pop(){
    sixteen_b_registers[SP]++;
}

void SpiralMachine::push(uint8_t value){
    memory[STACK_END - (--sixteen_b_registers[SP])] = value;
}

void SpiralMachine::set_flag(Flags flag, bool value){
    flags[flag] = value;
}

void SpiralMachine::LDI(uint16_t mem_location, uint8_t value){
    memory[mem_location] = value;
}

void SpiralMachine::LDX(uint8_t value){
    eight_b_registers[X] = value;
}

void SpiralMachine::LDY(uint8_t value){
    eight_b_registers[Y] = value;
}

void SpiralMachine::LXY(){
    eight_b_registers[X] = eight_b_registers[Y];
}

void SpiralMachine::LYX(){
    eight_b_registers[Y] = eight_b_registers[X];
}

void SpiralMachine::LMX(uint16_t mem_location){
    memory[mem_location] = eight_b_registers[X];
}

void SpiralMachine::LMY(uint16_t mem_location){
    memory[mem_location] = eight_b_registers[Y];
}

void SpiralMachine::JSR(uint16_t mem_location){
    push(sixteen_b_registers[PC] >> 8);
    push(sixteen_b_registers[PC] << 8);
    sixteen_b_registers[PC] = mem_location;
}

void SpiralMachine::JMP(uint16_t mem_location){
    sixteen_b_registers[PC] = mem_location;
}

void SpiralMachine::JTL(uint16_t mem_location){
    sixteen_b_registers[PC] = mem_location;
}

void SpiralMachine::PCS(){
    push(sixteen_b_registers[PC] >> 8);
    push(sixteen_b_registers[PC] << 8);
}

void SpiralMachine::PXA(){
    push(eight_b_registers[X]);
}

void SpiralMachine::PYA(){
    push(eight_b_registers[Y]);
}

void SpiralMachine::POP(){
    pop();
}

void SpiralMachine::PPC(){
    sixteen_b_registers[PC] = (memory[sixteen_b_registers[SP]] << 8) | (memory[sixteen_b_registers[SP] + 1]);
    pop();
    pop();
}

void SpiralMachine::PTM(){
    eight_b_registers[T] = memory[sixteen_b_registers[SP]];
    pop();
}

void SpiralMachine::INX(){
    eight_b_registers[X]++;
}

void SpiralMachine::INY(){
    eight_b_registers[Y]++;
}

void SpiralMachine::DEX(){
    eight_b_registers[X]--;
}

void SpiralMachine::DEY(){
    eight_b_registers[Y]--;
}

void SpiralMachine::LEX(uint16_t mem_location){
    eight_b_registers[X] = memory[mem_location];
}

void SpiralMachine::LEY(uint16_t mem_location){
    eight_b_registers[Y] = memory[mem_location];
}

void SpiralMachine::AXY(){
    eight_b_registers[R] = eight_b_registers[X] + eight_b_registers[Y];
}

void SpiralMachine::SXY(){
    eight_b_registers[R] = eight_b_registers[X] - eight_b_registers[Y];
}

void SpiralMachine::SYX(){
    eight_b_registers[R] = eight_b_registers[Y] - eight_b_registers[X];
}

void SpiralMachine::CMP(){
    SYX();
    if (eight_b_registers[R] == 0){
        set_flag(E, true);
        set_flag(YG, false);
        set_flag(XG, false);
    }
    if (eight_b_registers[R] < 0){
        set_flag(E, false);
        set_flag(YG, true);
        set_flag(XG, false);
    }
    if (eight_b_registers[R] > 0){
        set_flag(E, false);
        set_flag(YG, false);
        set_flag(XG, true);
    }
}

void SpiralMachine::BEQ(uint16_t mem_location){
    if (flags[E] == 0){
        sixteen_b_registers[PC] = mem_location;
        sixteen_b_registers[PC] = (memory[sixteen_b_registers[SP]] << 8) | (memory[sixteen_b_registers[SP] + 1]);
        pop();
        pop();
    }
}

void SpiralMachine::BXG(uint16_t mem_location){
    if (flags[XG]){
        sixteen_b_registers[PC] = mem_location;
        sixteen_b_registers[PC] = (memory[sixteen_b_registers[SP]] << 8) | (memory[sixteen_b_registers[SP] + 1]);
        pop();
        pop();
    }
}

void SpiralMachine::BYG(uint16_t mem_location){
    if (flags[YG]){
        sixteen_b_registers[PC] = mem_location;
        sixteen_b_registers[PC] = (memory[sixteen_b_registers[SP]] << 8) | (memory[sixteen_b_registers[SP] + 1]);
        pop();
        pop();
    }
}

void SpiralMachine::BLT(uint16_t mem_location){
    // To be implemented
}

void SpiralMachine::VRI(uint16_t mem_location, uint8_t value){
    uint16_t location = VRAM_START + mem_location;
    memory[location] = value;
}

void SpiralMachine::VRX(uint16_t mem_location){
    uint16_t location = VRAM_START + mem_location;
    memory[location] = eight_b_registers[X];
}

void SpiralMachine::VRY(uint16_t mem_location){
    uint16_t location = VRAM_START + mem_location;
    memory[location] = eight_b_registers[Y];
}

void SpiralMachine::LTX(){
    eight_b_registers[X] = eight_b_registers[T];
}

void SpiralMachine::LTY(){
    eight_b_registers[Y] = eight_b_registers[T];
}

void SpiralMachine::fde_cycle(){

    uint16_t pc_value = sixteen_b_registers[PC];
    uint8_t first_operand = memory[sixteen_b_registers[pc_value+1]];  // Higher byte
    uint8_t second_operand = memory[sixteen_b_registers[pc_value+2]]; // Lower byte
    uint8_t third_operand = memory[sixteen_b_registers[pc_value+3]];  // Only used for LDI and VRI instructions

    switch(memory[sixteen_b_registers[PC]]){
        case 0x00: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            LDI(mem_location, third_operand);
        }
        case 0x01: {
            LDX(first_operand);
        }
        case 0x02: {
            LDY(first_operand);
        }
        case 0x03: {
            LXY();
        }
        case 0x04: {
            LYX();
        }
        case 0x0A: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            LMX(mem_location);
        }
        case 0x0B: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            LMY(mem_location);
        }
        case 0x05: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            JSR(mem_location);
        }
        case 0x06: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            JMP(mem_location);
        }

        case 0x16: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            JTL(mem_location);
        }

        case 0x1D: {
            PCS();
        }

        case 0x07: {
            PHA(first_operand);
        }

        case 0x21: {
            PXA();
        }

        case 0x22: {
            PYA();
        }

        case 0x08: {
            POP();
        }

        case 0x1C: {
            PPC();
        }

        case 0x23: {
            PTM();
        }

        case 0x09: {
            INX();
        }

        case 0x0C: {
            INY();
        }

        case 0x0D: {
            DEX();
        }

        case 0x0E: {
            DEY();
        }

        case 0x0F: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            LEX(mem_location);
        }

        case 0x10: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            LEY(mem_location);
        }

        case 0x11: {
            AXY();
        }

        case 0x12: {
            SXY();
        }

        case 0x13: {
            SYX();
        }

        case 0x17: {
            CMP();
        }

        case 0x18: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            BEQ(mem_location);
        }

        case 0x19: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            BXG(mem_location);
        }

        case 0x1A: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            BYG(mem_location);
        }

        case 0x1B: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            BLT(mem_location);
        }

        case 0x1E: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            VRI(mem_location, third_operand);
        }

        case 0x1F: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            VRX(mem_location);
        }

        case 0x20: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            VRY(mem_location);
        }

        case 0x25: {
            LTX();
        }

        case 0x24: {
            LTY();
        }

    }
    sixteen_b_registers[PC] += 4; // Instructions are 4-bytes long
}