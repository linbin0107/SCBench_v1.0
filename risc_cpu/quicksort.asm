movi R14, 4096
movi R1, 2048
movi R0, 0
mov R2, R0
mov R3, R1
bge R0, R1, 9
sw R2, R14, 0
subi R14, R14, 1
sw R3, R14, 0
subi R14, R14, 1
addi R2, R2, 1
subi R3, R3, 1
addi R0, R0, 2
beq R0, R0, -8
lw R0, R14, 0
lw R1, R14, 1
lw R2, R14, 2
lw R3, R14, 3
lw R4, R14, 4
lw R5, R14, 5
lw R6, R14, 6
lw R7, R14, 7
lw R8, R14, 8
lw R9, R14, 9
lw R10, R14, 10
lw R11, R14, 11
lw R12, R14, 12
lw R13, R14, 13
halt
mov R0, R14
addi R0, R0, 1
movi R1, 0
movi R2, 2047
sw R2, R14, 0
subi, R14, R14, 1 
sw R1, R14, 0
subi, R14, R14, 1
sw R0, R14
jal 62
addi R14, R14, 3
lw R0, R14, 0
lw R1, R14, 1
lw R2, R14, 2
lw R3, R14, 3
lw R4, R14, 4
lw R5, R14, 5
lw R6, R14, 6
lw R7, R14, 7
lw R8, R14, 8
lw R9, R14, 9
lw R10, R14, 10
lw R11, R14, 11
lw R12, R14, 12
lw R13, R14, 13
halt
j 5
quit
lw R0, R14, 0
lw R1, R14, 1
lw R2, R14, 2
halt
mov R3, R1 
mov R4, R2
add R6, R1, R2
movi R7, 2
div R6, R6, R7
add R6, R0, R6
lw R6, R6, 0
bgt R3, R4, 23
add R7, R0, R3
lw R7, R7, 0
bge R7, R6, 3
addi R3, R3, 1
beq R0, R0, -4
add R7, R0, R4
lw R7, R7, 0
ble R7, R6 3
subi, R4, R4, 1
beq R0, R0, -4
bgt R3, R4, 11
add R5, R0, R3
lw R5, R5, 0
add R7, R0, R4
lw  R7, R7, 0
add R8, R0, R3
sw R7, R8, 0
add R7, R0, R4
sw R5, R7, 0
addi R3, R3, 1
subi R4, R4, 1
beq R0, R0, -22
bge R1, R4, 33
subi R14, R14, 1
sw R15, R14, 0
subi R14, R14, 1
sw R0, R14, 0
subi R14, R14, 1
sw R1, R14, 0
subi R14, R14, 1
sw R2, R14, 0
subi R14, R14, 1
sw R3, R14, 0
subi R14, R14, 1
sw R4, R14, 0
subi R14, R14, 1
sw R4, R14, 0
subi R14, R14, 1
sw R1, R14, 0
subi R14, R14, 1
sw R0, R14, 0
jal 63
addi, R14, R14, 3
lw R4, R14, 0
addi R14, R14, 1
lw R3, R14, 0
addi R14, R14, 1
lw R2, R14, 0
addi R14, R14, 1
lw R1, R14, 0
addi R14, R14, 1
lw R0, R14, 0
addi R14, R14, 1
lw R15, R14, 0
addi R14, R14, 1
bge R2, R3, 33
subi R14, R14, 1
sw R15, R14, 0
subi R14, R14, 1
sw R0, R14, 0
subi R14, R14, 1
sw R1, R14, 0
subi R14, R14, 1
sw R2, R14, 0
subi R14, R14, 1
sw R3, R14, 0
subi R14, R14, 1
sw R4, R14, 0
subi R14, R14, 1
sw R2, R14, 0
subi R14, R14, 1
sw R3, R14, 0
subi R14, R14, 1
sw R0, R14, 0
jal 63
addi, R14, R14, 3
lw R4, R14, 0
addi R14, R14, 1
lw R3, R14, 0
addi R14, R14, 1
lw R2, R14, 0
addi R14, R14, 1
lw R1, R14, 0
addi R14, R14, 1
lw R0, R14, 0
addi R14, R14, 1
lw R15, R14, 0
addi R14, R14, 1
jr
