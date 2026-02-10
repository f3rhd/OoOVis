start:
    addi    x6,  x0,10       #0

    addi    x5,  x0, 0        #1
    addi    x31, x0, 1        #2

    beq     x6,  x0, done_zero   #3
    addi    x7,  x0, 1           #4
    beq     x6,  x7, done_one    #5

    addi    x4,  x0, 1        #6

loop:
    blt     x6,  x4, after_loop   #7
    add     x28, x5, x31          #8
    add     x5,  x31, x0          #9
    add     x31, x28, x0          #10
    addi    x4,  x4, 1            #11
    blt     x4,  x6, loop         #12

after_loop:
    add     x10, x31, x0          #13
    jal     x0, hang              #14

done_zero:
    add     x10, x0, x0           #15
    jal     x0, hang              #16

done_one:
    add     x10, x31, x0          #17
    jal     x0, hang              #18
hang:
    nop
    nop
    nop