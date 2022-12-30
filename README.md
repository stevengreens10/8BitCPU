# 8-BitCPU

My 8-bit CPU design simulated on https://www.falstad.com/circuit/. Based on Ben Eater's CPU with modifications such as 8-bit addressing.

You can load cpu.txt of falstad to load the circuit, and example programs are stored in programs/.

Build the project with CMake and run the assembler with
`./Assembler.exe programs/something.prog`. Copy the byte text into SRAM module and start the computer.