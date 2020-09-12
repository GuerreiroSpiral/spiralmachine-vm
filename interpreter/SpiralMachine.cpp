#include <stdint.h>
#include "SpiralMachine.h"

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
    sixteen_b_registers[PC] = (memory[sixteen_b_registers[SP]] << 8) | (memory[sixteen_b_registers[SP] + 1] >> 8);
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
        sixteen_b_registers[PC] = (memory[sixteen_b_registers[SP]] << 8) | (memory[sixteen_b_registers[SP] + 1] >> 8);
        pop();
        pop();
    }
}

void SpiralMachine::BXG(uint16_t mem_location){
    if (flags[XG] == 0){
        sixteen_b_registers[PC] = mem_location;
        sixteen_b_registers[PC] = (memory[sixteen_b_registers[SP]] << 8) | (memory[sixteen_b_registers[SP] + 1] >> 8);
        pop();
        pop();
    }
}

void SpiralMachine::BYG(uint16_t mem_location){
    if (flags[YG] == 0){
        sixteen_b_registers[PC] = mem_location;
        sixteen_b_registers[PC] = (memory[sixteen_b_registers[SP]] << 8) | (memory[sixteen_b_registers[SP] + 1] >> 8);
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