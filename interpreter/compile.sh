#!/bin/bash
g++ -o interpreter main.cpp SpiralMachine.cpp SpiralScreen.cpp -lmingw32 -lSDL2main -lSDL2 -g
echo "Done!"
./interpreter.exe