#----------------------------------------------------------------#
#                             OFFICIAL                           #
#                          SPIRAL MACHINE                        #
#                            ASSEMBLER                           #
#----------------------------------------------------------------#

import argparse
import logging
from exceptions import *


class SpiralAssembler():
    def __init__(self):

        # MEMORY ACCESS LIMITS
        # H E A D E R
        self.HEADER_START = 0x000
        self.HEADER_END = 0x003
        # V R A M
        self.VRAM_START = 0x004
        self.VRAM_END = 0x404
        # S T A C K
        self.STACK_START = 0x405
        self.STACK_END = 0xC05
        # P R O G R A M M E M O R Y
        self.P_MEMORY_START = 0xC06
        self.P_MEMORY_END = 0x2000
        # H E A D E R V A L I D
        self.HEADER = 'SPMF'
        self.symbol_table = dict()

    def split(self, word):
        return [char for char in word]

    def _assemble(self, file_path="D:\SpiralMachine\\assembler\d.asm"):
        _instr = list()

        with open(file_path, "r") as asm_file:
            for line in asm_file:
                line = line.replace(
                    '\n', '').replace('\r', '')
                _instr.append(line)
        self._add_header()
        self._parse_assembly(_instr)

    def _write_binary(self, b):
        with open(r"D:\SpiralMachine\\assembler\\rom.spm", "ab") as rom:
            rom.write(bytearray(b))

    def _is_memory_location_valid(self, location, start, limit):
        return location <= limit and location >= start

    def _add_header(self):
        _header = [0x1, 0x2, 0x3, 0x4]
        self._write_binary(_header)
    
    def _parse_labels(self, i, _line):
        _value = hex(i*4+self.P_MEMORY_START+1)
        _mem_location = self.split(_value)
        if len(_mem_location) < 6:
            _mem_location.insert(2, '0')
            _mem_location[0] = int(_mem_location[2] + _mem_location[3], 16)
            _mem_location[1] = int(_mem_location[4] + _mem_location[5], 16)
        self.symbol_table[_line] = _mem_location

    def _parse_assembly(self, file):
        i = 0
        for _line in file:
            if _line.startswith('.'):
                self._parse_labels(i, _line)
                continue
            if _line == '':
                continue

            i += 1

            _original_line = str(_line)
            _line = _line.replace(',', '')
            _final_line = _line.split()

            if len(_final_line) not in [1, 2, 3]:
                raise InvalidOperandError(line=_final_line)

            if _final_line[0].upper() == 'LDI':
                _mem_location = self.split(_final_line[1])
                _mem_location[0] = int(_mem_location[2] + _mem_location[3], 0)
                _mem_location[1] = int(_mem_location[4] + _mem_location[5], 0)
                _value = int(_final_line[2], 0)
                _b = [0x00, _mem_location[0], _mem_location[1], _value]
                if not self._is_memory_location_valid(_mem_location + self.P_MEMORY_START, self.P_MEMORY_START, self.P_MEMORY_END):
                    raise MemoryAccessError(line=_original_line)
                self._write_binary(_b)

            elif _final_line[0].upper() == 'LDX':
                _value = int(_final_line[1], 0)
                _b = [0x01, _value, 0, 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'LXY':
                _b = [0x03, 0, 0, 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'LXY':
                _b = [0x04, 0, 0, 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'LMX':
                _mem_location = self.split(_final_line[1])
                _mem_location[0] = int(_mem_location[2] + _mem_location[3], 0)
                _mem_location[1] = int(_mem_location[4] + _mem_location[5], 0)
                if not self._is_memory_location_valid(_mem_location + self.P_MEMORY_START, self.P_MEMORY_START, self.P_MEMORY_END):
                    raise MemoryAccessError(line=_original_line)
                _b = [0x0A, _mem_location[0], _mem_location[1], 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'LMY':
                _mem_location = self.split(_final_line[1])
                _mem_location[0] = int(_mem_location[2] + _mem_location[3], 0)
                _mem_location[1] = int(_mem_location[4] + _mem_location[5], 0)
                if not self._is_memory_location_valid(_mem_location + self.P_MEMORY_START, self.P_MEMORY_START, self.P_MEMORY_END):
                    raise MemoryAccessError(line=_original_line)
                _b = [0x0B, _mem_location[0], _mem_location[1], 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'LXM':
                _value = int(_final_line[1], 0)
                _b = [0x14, _value, 0, 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'LYM':
                _value = int(_final_line[1], 0)
                _b = [0x15, _value, 0, 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'JSR':
                _mem_location = self.split(_final_line[1])
                _mem_location[0] = int(_mem_location[2] + _mem_location[3], 0)
                _mem_location[1] = int(_mem_location[4] + _mem_location[5], 0)
                if not self._is_memory_location_valid(_mem_location + self.P_MEMORY_START, self.P_MEMORY_START, self.P_MEMORY_END):
                    raise MemoryAccessError(line=_original_line)
                _b = [0x05, _mem_location[0], _mem_location[1], 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'JMP':
                _mem_location = self.split(_final_line[1])
                _mem_location[0] = int(_mem_location[2] + _mem_location[3], 0)
                _mem_location[1] = int(_mem_location[4] + _mem_location[5], 0)
                if not self._is_memory_location_valid(_mem_location + self.P_MEMORY_START, self.P_MEMORY_START, self.P_MEMORY_END):
                    raise MemoryAccessError(line=_original_line)
                _b = [0x06, _mem_location[0], _mem_location[1], 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'JTL':
                _label = _final_line[1]
                try:
                    _mem_location = self.symbol_table[_label]
                except KeyError:
                    logging.error("\"" + _label + "\" label not defined before usage.")
                    return
                _b = [0x16, _mem_location[0], _mem_location[1], 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'PCS':
                _b = [0x03, 0, 0, 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'PHA':
                _value = int(_final_line[1], 0)
                _b = [0x07, _value, 0, 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'PCS':
                _b = [0x08, 0, 0, 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'PPC':
                _b = [0x1C, 0, 0, 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'PTM':
                _b = [0x21, 0, 0, 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'INX':
                _b = [0x21, 0, 0, 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'INY':
                _b = [0x21, 0, 0, 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'DEX':
                _b = [0x21, 0, 0, 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'DEY':
                _b = [0x21, 0, 0, 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'LEX':
                _mem_location = self.split(_final_line[1])
                _mem_location[0] = int(_mem_location[2] + _mem_location[3], 0)
                _mem_location[1] = int(_mem_location[4] + _mem_location[5], 0)
                if not self._is_memory_location_valid(_mem_location + self.P_MEMORY_START, self.P_MEMORY_START, self.P_MEMORY_END):
                    raise MemoryAccessError(line=_original_line)
                _b = [0x0F, _mem_location[0], _mem_location[1], 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'LEY':
                _mem_location = self.split(_final_line[1])
                _mem_location[0] = int(_mem_location[2] + _mem_location[3], 0)
                _mem_location[1] = int(_mem_location[4] + _mem_location[5], 0)
                if not self._is_memory_location_valid(_mem_location + self.P_MEMORY_START, self.P_MEMORY_START, self.P_MEMORY_END):
                    raise MemoryAccessError(line=_original_line)
                _b = [0x10, _mem_location[0], _mem_location[1], 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'AXY':
                _b = [0x11, 0, 0, 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'SXY':
                _b = [0x12, 0, 0, 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'CMP':
                _b = [0x17, 0, 0, 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'BEQ':
                _mem_location = self.split(_final_line[1])
                _mem_location[0] = int(_mem_location[2] + _mem_location[3], 0)
                _mem_location[1] = int(_mem_location[4] + _mem_location[5], 0)
                if not self._is_memory_location_valid(_mem_location + self.P_MEMORY_START, self.P_MEMORY_START, self.P_MEMORY_END):
                    raise MemoryAccessError(line=_original_line)
                _b = [0x18, _mem_location[0], _mem_location[1], 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'BXG':
                _mem_location = self.split(_final_line[1])
                _mem_location[0] = int(_mem_location[2] + _mem_location[3], 0)
                _mem_location[1] = int(_mem_location[4] + _mem_location[5], 0)
                if not self._is_memory_location_valid(_mem_location + self.P_MEMORY_START, self.P_MEMORY_START, self.P_MEMORY_END):
                    raise MemoryAccessError(line=_original_line)
                _b = [0x19, _mem_location[0], _mem_location[1], 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'BYG':
                _mem_location = self.split(_final_line[1])
                _mem_location[0] = int(_mem_location[2] + _mem_location[3], 0)
                _mem_location[1] = int(_mem_location[4] + _mem_location[5], 0)
                if not self._is_memory_location_valid(_mem_location + self.P_MEMORY_START, self.P_MEMORY_START, self.P_MEMORY_END):
                    raise MemoryAccessError(line=_original_line)
                _b = [0x1A, _mem_location[0], _mem_location[1], 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'BLT':
                _mem_location = self.split(_final_line[1])
                _mem_location[0] = int(_mem_location[2] + _mem_location[3], 0)
                _mem_location[1] = int(_mem_location[4] + _mem_location[5], 0)
                if not self._is_memory_location_valid((_mem_location[0] + _mem_location[1]) + self.VRAM_START, self.VRAM_START, self.VRAM_END):
                    raise MemoryAccessError(line=_original_line)
                _b = [0x1B, _mem_location[0], _mem_location[1], 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'VRI':
                _mem_location = self.split(_final_line[1])
                _mem_location[0] = int(_mem_location[2] + _mem_location[3], 0)
                _mem_location[1] = int(_mem_location[4] + _mem_location[5], 0)
                _value = int(_final_line[2], 0)
                _b = [0x1E, _mem_location[0], _mem_location[1], _value]
                if not self._is_memory_location_valid((_mem_location[0] + _mem_location[1]) + self.VRAM_START, self.VRAM_START, self.VRAM_END):
                    raise MemoryAccessError(line=_original_line)
                self._write_binary(_b)

            elif _final_line[0].upper() == 'VRX':
                _mem_location = self.split(_final_line[1])
                _mem_location[0] = int(_mem_location[2] + _mem_location[3], 0)
                _mem_location[1] = int(_mem_location[4] + _mem_location[5], 0)
                if not self._is_memory_location_valid((_mem_location[0] + _mem_location[1]) + self.VRAM_START, self.VRAM_START, self.VRAM_END):
                    raise MemoryAccessError(line=_original_line)
                _b = [0x1F, _mem_location[0], _mem_location[1], 0]
                self._write_binary(_b)

            elif _final_line[0].upper() == 'VRY':
                _mem_location = self.split(_final_line[1])
                _mem_location[0] = int(_mem_location[2] + _mem_location[3], 0)
                _mem_location[1] = int(_mem_location[4] + _mem_location[5], 0)
                if not self._is_memory_location_valid((_mem_location[0] + _mem_location[1]) + self.VRAM_START, self.VRAM_START, self.VRAM_END):
                    raise MemoryAccessError(line=_original_line)
                _b = [0x20, _mem_location[0], _mem_location[1], 0]
                self._write_binary(_b)

            else:
                raise InvalidInstructionError(_original_line)