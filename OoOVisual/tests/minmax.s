.reset_vector:
addi    sp, x0, 1000    # 0
jal     ra, .main            # 1

.min:
addi    sp,sp,-32            # 2
sw      ra,28(sp)            # 3
sw      s0,24(sp)            # 4
addi    s0,sp,32             # 5
sw      a0,-12(s0)           # 6
sw      a1,-16(s0)           # 7
addi    t6,x0,0              # 8
slli    t0,t6,2              # 9
lw      t2,-12(s0)           # 10
add     t0,t2,t0             # 11
lw      t0,0(t0)             # 12
addi    t3,t0,0              # 13
sw      t3,-20(s0)           # 14
addi    t1,x0,1              # 15
sw      t1,-24(s0)           # 16
jal     x0,.L0               # 17

.L0:
lw      t1,-24(s0)           # 18
slli    t0,t1,2              # 19
lw      t2,-12(s0)           # 20
add     t0,t2,t0             # 21
lw      t0,0(t0)             # 22
lw      t3,-20(s0)           # 23
bge     t0,t3,.L2            # 24

.L4:
lw      t1,-24(s0)           # 25
slli    t0,t1,2              # 26
lw      t2,-12(s0)           # 27
add     t0,t2,t0             # 28
lw      t0,0(t0)             # 29
addi    t3,t0,0              # 30
sw      t3,-20(s0)           # 31

.L2:
addi    x0,x0,0              # 32

.L1:
lw      t1,-24(s0)           # 33
addi    t1,t1,1              # 34
sw      t1,-24(s0)           # 35
lw      t1,-24(s0)           # 36
lw      t0,-16(s0)           # 37
blt     t1,t0,.L0            # 38

.L5:
addi    x0,x0,0              # 39

.L6:
lw      t3,-20(s0)           # 40
addi    a0,t3,0              # 41
lw      ra,28(sp)            # 42
lw      s0,24(sp)            # 43
addi    sp,sp,32             # 44
jalr    x0,ra,0             # 45

.max:
addi    sp,sp,-32            # 46
sw      ra,28(sp)            # 47
sw      s0,24(sp)            # 48
addi    s0,sp,32             # 49
sw      a0,-12(s0)           # 50
sw      a1,-16(s0)           # 51
addi    t3,x0,0              # 52
sw      t3,-20(s0)           # 53
addi    t2,x0,1              # 54
sw      t2,-24(s0)           # 55
jal     x0,.L7               # 56

.L7:
lw      t2,-24(s0)           # 57
slli    t0,t2,2              # 58
lw      t1,-12(s0)           # 59
add     t0,t1,t0             # 60
lw      t0,0(t0)             # 61
lw      t3,-20(s0)           # 62
ble     t0,t3,.L9            # 63

.L11:
lw      t2,-24(s0)           # 64
slli    t0,t2,2              # 65
lw      t1,-12(s0)           # 66
add     t0,t1,t0             # 67
lw      t0,0(t0)             # 68
addi    t3,t0,0              # 69
sw      t3,-20(s0)           # 70

.L9:
addi    x0,x0,0              # 71

.L8:
lw      t2,-24(s0)           # 72
addi    t2,t2,1              # 73
sw      t2,-24(s0)           # 74
lw      t2,-24(s0)           # 75
lw      t0,-16(s0)           # 76
blt     t2,t0,.L7            # 77

.L12:
addi    x0,x0,0              # 78

.L13:
lw      t3,-20(s0)           # 79
addi    a0,t3,0              # 80
lw      ra,28(sp)            # 81
lw      s0,24(sp)            # 82
addi    sp,sp,32             # 83
jalr    x0,ra,0             # 84
.main:
addi    sp,sp,-48            # 85
sw      ra,44(sp)            # 86
sw      s0,40(sp)            # 87
addi    s0,sp,48             # 88
addi    t0,x0,1              # 89
sw      t0,-32(s0)           # 90
addi    t0,x0,2              # 91
sw      t0,-28(s0)           # 92
addi    t0,x0,3              # 93
sw      t0,-24(s0)           # 94
addi    t0,x0,4              # 95
sw      t0,-20(s0)           # 96
addi    t0,x0,5              # 97
sw      t0,-16(s0)           # 98
addi    t6,s0,-32            # 99
addi    a0,t6,0              # 100
addi    a1,x0,5              # 101
jal     ra,.min              # 102
addi    t0,a0,0              # 103
addi    t1,t0,0              # 104
sw      t1,-36(s0)           # 105
addi    t6,s0,-32            # 106
addi    a0,t6,0              # 107
addi    a1,x0,5              # 108
jal     ra,.max              # 109
addi    t0,a0,0              # 110
sw      t0,-40(s0)           # 111
lw      t1,-36(s0)           # 112
addi    t6,x0,1              # 113
bne     t1,t6,.L14           # 114
lw      t0,-40(s0)           # 115
addi    t6,x0,5              # 116
bne     t0,t6,.L14           # 117
jal     x0,.L18              # 118
addi    x0,x0,0              # 119

.L18:
addi    x31,x0,1000          # 120
jal     x0,.L14              # 121
addi    x0,x0,0              # 122
addi    x0,x0,0              # 123

.L14:
addi    x0,x0,0              # 124
