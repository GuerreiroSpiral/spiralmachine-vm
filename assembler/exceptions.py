"""=> Invalid memory access exception
Raised when an memory offset surpass the boundaries of the specific memory access limits.
"""

class MemoryAccessError(Exception):
    def __init__(self, line, message="Invalid memory access in line: "):
        self.line = line
        self.message = message + str(line)
        super().__init__(self.message) 

"""=> Invalid instruction error
Raised when the instruction is not recognized.
"""

class InvalidInstructionError(Exception):
    def __init__(self, line, message="Invalid instruction: "):
        self.line = line
        self.message = message + str(line)
        super().__init__(self.message)

"""=> Invalid operand errors
Raised when the number of operands is not accurate. Generic and not instruction specific.
"""

class InvalidOperandError(Exception):
    def __init__(self, line, message="Invalid operand in line: "):
        self.line = line
        self.message = message + str(line)
        super().__init__(self.message) 