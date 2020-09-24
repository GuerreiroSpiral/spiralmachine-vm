#include <stdint.h>
#include "SpiralMachine.h"
#include "SpiralScreen.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

void SpiralMachine::clear_memory(){
    for (int i = 0; i < MEMORY_SIZE; i++){
        memory[i] = 0;
    }
}

void SpiralMachine::init(){
    clear_memory();
    load_rom();
    sixteen_b_registers[PC] = PROGRAM_MEMORY_START;
    sixteen_b_registers[SP] = STACK_END;
    step();
}

void SpiralMachine::load_rom(){
    FILE *fileptr;
    char *buffer;
    long filelen;

    fileptr = fopen("rom.spm", "rb");
    fseek(fileptr, 0, SEEK_END);          
    filelen = ftell(fileptr);             
    rewind(fileptr);                      

    buffer = (char *)malloc(filelen * sizeof(char)); 
    fread(buffer, filelen, 1, fileptr); 
    fclose(fileptr); 

    // Loads the header into memory
    for (int i = 0; i <= 4; i++){
        memory[i] = uint8_t(buffer[i]);
    }

    // Loads the Program data into memory
    for (int i = 0; i < filelen; i++){
        memory[PROGRAM_MEMORY_START + i] = uint8_t(buffer[i+4]);
    }
} 

void SpiralMachine::step(){
    SDL_Event event;
    bool done = false;
    while(!done && SDL_WaitEvent(&event)){
        switch (event.type){
            case SDL_QUIT:
                screen.~SpiralScreen();
                done = true;
                break;
        }
        // Execute exactly 3 cycles and render one frame.
        fde_cycle();
        fde_cycle();
        fde_cycle();
        screen.render_screen(memory);
    }
}

uint16_t SpiralMachine::fetch_instruction(){
    return memory[sixteen_b_registers[PC]];
}

void SpiralMachine::pop(){
    sixteen_b_registers[SP]++;
}

void SpiralMachine::push(uint8_t value){
    memory[--sixteen_b_registers[SP]] = value;
}

void SpiralMachine::set_flag(Flags flag, bool value){
    flags[flag] = value;
}

void SpiralMachine::LDI(uint16_t mem_location, uint8_t value){
    memory[mem_location + PROGRAM_MEMORY_START] = value;
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
    memory[mem_location + PROGRAM_MEMORY_START] = eight_b_registers[X];
}

void SpiralMachine::LMY(uint16_t mem_location){
    memory[mem_location + PROGRAM_MEMORY_START] = eight_b_registers[Y];
}

void SpiralMachine::JSR(uint16_t mem_location){
    sixteen_b_registers[PC] += 4;
    push(sixteen_b_registers[PC] >> 8);
    push((sixteen_b_registers[PC] & 0xFF));
    sixteen_b_registers[PC] = mem_location + PROGRAM_MEMORY_START;
}

void SpiralMachine::JMP(uint16_t mem_location){
    sixteen_b_registers[PC] = mem_location + PROGRAM_MEMORY_START;
}

void SpiralMachine::JTL(uint16_t mem_location){
    sixteen_b_registers[PC] = mem_location;
}

void SpiralMachine::PCS(){
    push(sixteen_b_registers[PC] >> 8);
    push((sixteen_b_registers[PC] & 0xFF));
}

void SpiralMachine::PHA(uint8_t value){
    push(value);
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
    eight_b_registers[X] = memory[mem_location + PROGRAM_MEMORY_START];
}

void SpiralMachine::LEY(uint16_t mem_location){
    eight_b_registers[Y] = memory[mem_location + PROGRAM_MEMORY_START];
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
        sixteen_b_registers[PC] = mem_location + PROGRAM_MEMORY_START;
        sixteen_b_registers[PC] = (memory[sixteen_b_registers[SP]] << 8) | (memory[sixteen_b_registers[SP] + 1]);
        pop();
        pop();
    }
}

void SpiralMachine::BXG(uint16_t mem_location){
    if (flags[XG]){
        sixteen_b_registers[PC] = mem_location + PROGRAM_MEMORY_START;
        sixteen_b_registers[PC] = (memory[sixteen_b_registers[SP]] << 8) | (memory[sixteen_b_registers[SP] + 1]);
        pop();
        pop();
    }
}

void SpiralMachine::BYG(uint16_t mem_location){
    if (flags[YG]){
        sixteen_b_registers[PC] = mem_location + PROGRAM_MEMORY_START;
        sixteen_b_registers[PC] = (memory[sixteen_b_registers[SP]] << 8) | (memory[sixteen_b_registers[SP] + 1]);
        pop();
        pop();
    }
}

void SpiralMachine::BLT(uint16_t mem_location){
    screen.blit_screen(memory[mem_location], mem_location);
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
    uint8_t first_operand = memory[pc_value+1];  // Higher byte
    uint8_t second_operand = memory[pc_value+2]; // Lower byte
    uint8_t third_operand = memory[pc_value+3];  // Only used for LDI and VRI instructions
    
    /* 
    printf("Instruction:\n --\n%d\n", memory[pc_value]);
    printf("%d\n", first_operand);
    printf("%d\n", second_operand);
    printf("%d\n", third_operand);
    printf("----------\n");
    */

    switch(memory[sixteen_b_registers[PC]]){
        case 0x00: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            LDI(mem_location, third_operand);
            break;
        }
        case 0x01: {
            LDX(first_operand);
            break;
        }
        case 0x02: {
            LDY(first_operand);
            break;
        }
        case 0x03: {
            LXY();
            break;
        }
        case 0x04: {
            LYX();
            break;
        }
        case 0x0A: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            LMX(mem_location);
            break;
        }
        case 0x0B: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            LMY(mem_location);
            break;
        }
        case 0x05: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            JSR(mem_location);
            break;
        }
        case 0x06: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            JMP(mem_location);
            break;
        }

        case 0x16: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            JTL(mem_location);
            break;
        }

        case 0x1D: {
            PCS();
            break;
        }

        case 0x07: {
            PHA(first_operand);
            break;
        }

        case 0x21: {
            PXA();
            break;
        }

        case 0x22: {
            PYA();
            break;
        }

        case 0x08: {
            POP();
            break;
        }

        case 0x1C: {
            PPC();
            break;
        }

        case 0x23: {
            PTM();
            break;
        }

        case 0x09: {
            INX();
            break;
        }

        case 0x0C: {
            INY();
            break;
        }

        case 0x0D: {
            DEX();
            break;
        }

        case 0x0E: {
            DEY();
            break;
        }

        case 0x0F: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            LEX(mem_location);
            break;
        }

        case 0x10: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            LEY(mem_location);
            break;
        }

        case 0x11: {
            AXY();
            break;
        }

        case 0x12: {
            SXY();
            break;
        }

        case 0x13: {
            SYX();
            break;
        }

        case 0x17: {
            CMP();
            break;
        }

        case 0x18: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            BEQ(mem_location);
            break;
        }

        case 0x19: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            BXG(mem_location);
            break;
        }

        case 0x1A: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            BYG(mem_location);
            break;
        }

        case 0x1B: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            BLT(mem_location);
            break;
        }

        case 0x1E: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            VRI(mem_location, third_operand);
            break;
        }

        case 0x1F: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            VRX(mem_location);
            break;
        }

        case 0x20: {
            uint16_t mem_location = (first_operand << 8) | second_operand;
            VRY(mem_location);
            break;
        }

        case 0x25: {
            LTX();
            break;
        }

        case 0x24: {
            LTY();
            break;
        }
        default: {
            break;
        }

    }
    sixteen_b_registers[PC] += 4; // Instructions are 4-bytes long
}