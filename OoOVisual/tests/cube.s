# ============================================================
#  Spinning Cube - RISC-V Assembly
#  Fixed-point 10-bit (scale=1024)
# ============================================================

    li sp, 0x4000

# --- Sin table ---
    li t0, 0x2000
    li t1, 0
    sw t1, 0(t0)
    li t1, 100
    sw t1, 4(t0)
    li t1, 200
    sw t1, 8(t0)
    li t1, 297
    sw t1, 12(t0)
    li t1, 392
    sw t1, 16(t0)
    li t1, 483
    sw t1, 20(t0)
    li t1, 569
    sw t1, 24(t0)
    li t1, 650
    sw t1, 28(t0)
    li t1, 724
    sw t1, 32(t0)
    li t1, 792
    sw t1, 36(t0)
    li t1, 851
    sw t1, 40(t0)
    li t1, 903
    sw t1, 44(t0)
    li t1, 946
    sw t1, 48(t0)
    li t1, 980
    sw t1, 52(t0)
    li t1, 1004
    sw t1, 56(t0)
    li t1, 1019
    sw t1, 60(t0)
    li t1, 1024
    sw t1, 64(t0)
    li t1, 1019
    sw t1, 68(t0)
    li t1, 1004
    sw t1, 72(t0)
    li t1, 980
    sw t1, 76(t0)
    li t1, 946
    sw t1, 80(t0)
    li t1, 903
    sw t1, 84(t0)
    li t1, 851
    sw t1, 88(t0)
    li t1, 792
    sw t1, 92(t0)
    li t1, 724
    sw t1, 96(t0)
    li t1, 650
    sw t1, 100(t0)
    li t1, 569
    sw t1, 104(t0)
    li t1, 483
    sw t1, 108(t0)
    li t1, 392
    sw t1, 112(t0)
    li t1, 297
    sw t1, 116(t0)
    li t1, 200
    sw t1, 120(t0)
    li t1, 100
    sw t1, 124(t0)
    li t1, 0
    sw t1, 128(t0)
    li t1, -100
    sw t1, 132(t0)
    li t1, -200
    sw t1, 136(t0)
    li t1, -297
    sw t1, 140(t0)
    li t1, -392
    sw t1, 144(t0)
    li t1, -483
    sw t1, 148(t0)
    li t1, -569
    sw t1, 152(t0)
    li t1, -650
    sw t1, 156(t0)
    li t1, -724
    sw t1, 160(t0)
    li t1, -792
    sw t1, 164(t0)
    li t1, -851
    sw t1, 168(t0)
    li t1, -903
    sw t1, 172(t0)
    li t1, -946
    sw t1, 176(t0)
    li t1, -980
    sw t1, 180(t0)
    li t1, -1004
    sw t1, 184(t0)
    li t1, -1019
    sw t1, 188(t0)
    li t1, -1024
    sw t1, 192(t0)
    li t1, -1019
    sw t1, 196(t0)
    li t1, -1004
    sw t1, 200(t0)
    li t1, -980
    sw t1, 204(t0)
    li t1, -946
    sw t1, 208(t0)
    li t1, -903
    sw t1, 212(t0)
    li t1, -851
    sw t1, 216(t0)
    li t1, -792
    sw t1, 220(t0)
    li t1, -724
    sw t1, 224(t0)
    li t1, -650
    sw t1, 228(t0)
    li t1, -569
    sw t1, 232(t0)
    li t1, -483
    sw t1, 236(t0)
    li t1, -392
    sw t1, 240(t0)
    li t1, -297
    sw t1, 244(t0)
    li t1, -200
    sw t1, 248(t0)
    li t1, -100
    sw t1, 252(t0)

# --- Cos table ---
    li t0, 0x2100
    li t1, 1024
    sw t1, 0(t0)
    li t1, 1019
    sw t1, 4(t0)
    li t1, 1004
    sw t1, 8(t0)
    li t1, 980
    sw t1, 12(t0)
    li t1, 946
    sw t1, 16(t0)
    li t1, 903
    sw t1, 20(t0)
    li t1, 851
    sw t1, 24(t0)
    li t1, 792
    sw t1, 28(t0)
    li t1, 724
    sw t1, 32(t0)
    li t1, 650
    sw t1, 36(t0)
    li t1, 569
    sw t1, 40(t0)
    li t1, 483
    sw t1, 44(t0)
    li t1, 392
    sw t1, 48(t0)
    li t1, 297
    sw t1, 52(t0)
    li t1, 200
    sw t1, 56(t0)
    li t1, 100
    sw t1, 60(t0)
    li t1, 0
    sw t1, 64(t0)
    li t1, -100
    sw t1, 68(t0)
    li t1, -200
    sw t1, 72(t0)
    li t1, -297
    sw t1, 76(t0)
    li t1, -392
    sw t1, 80(t0)
    li t1, -483
    sw t1, 84(t0)
    li t1, -569
    sw t1, 88(t0)
    li t1, -650
    sw t1, 92(t0)
    li t1, -724
    sw t1, 96(t0)
    li t1, -792
    sw t1, 100(t0)
    li t1, -851
    sw t1, 104(t0)
    li t1, -903
    sw t1, 108(t0)
    li t1, -946
    sw t1, 112(t0)
    li t1, -980
    sw t1, 116(t0)
    li t1, -1004
    sw t1, 120(t0)
    li t1, -1019
    sw t1, 124(t0)
    li t1, -1024
    sw t1, 128(t0)
    li t1, -1019
    sw t1, 132(t0)
    li t1, -1004
    sw t1, 136(t0)
    li t1, -980
    sw t1, 140(t0)
    li t1, -946
    sw t1, 144(t0)
    li t1, -903
    sw t1, 148(t0)
    li t1, -851
    sw t1, 152(t0)
    li t1, -792
    sw t1, 156(t0)
    li t1, -724
    sw t1, 160(t0)
    li t1, -650
    sw t1, 164(t0)
    li t1, -569
    sw t1, 168(t0)
    li t1, -483
    sw t1, 172(t0)
    li t1, -392
    sw t1, 176(t0)
    li t1, -297
    sw t1, 180(t0)
    li t1, -200
    sw t1, 184(t0)
    li t1, -100
    sw t1, 188(t0)
    li t1, 0
    sw t1, 192(t0)
    li t1, 100
    sw t1, 196(t0)
    li t1, 200
    sw t1, 200(t0)
    li t1, 297
    sw t1, 204(t0)
    li t1, 392
    sw t1, 208(t0)
    li t1, 483
    sw t1, 212(t0)
    li t1, 569
    sw t1, 216(t0)
    li t1, 650
    sw t1, 220(t0)
    li t1, 724
    sw t1, 224(t0)
    li t1, 792
    sw t1, 228(t0)
    li t1, 851
    sw t1, 232(t0)
    li t1, 903
    sw t1, 236(t0)
    li t1, 946
    sw t1, 240(t0)
    li t1, 980
    sw t1, 244(t0)
    li t1, 1004
    sw t1, 248(t0)
    li t1, 1019
    sw t1, 252(t0)

# --- Main loop: s0 = angle index (0..63) ---
    li s0, 0

main_loop:

# Clear screen
    li t0, 1
    sw t0, 0x0(zero)
    sw zero, 0x4(zero)
    sw zero, 0x8(zero)
    li t0, 349
    sw t0, 0xc(zero)
    li t0, 259
    sw t0, 0x10(zero)
    li t0, 0x000000FF
    sw t0, 0x14(zero)
    li t0, 1
    sw t0, 0x18(zero)

# Load sin_y and cos_y from tables
    li t0, 0x2000
    slli t1, s0, 2
    add t0, t0, t1
    lw s1, 0(t0)

    li t0, 0x2100
    slli t1, s0, 2
    add t0, t0, t1
    lw s2, 0(t0)

# Transform and project 8 cube vertices
# Vertex layout: index i => (cx,cy,cz) = CUBE_SIZE * (+-1, +-1, +-1)
    li s3, 0x2300
# Vertex 0: (-1,-1,-1)
    li t0, -200
    mul t2, t0, s2
    li t0, -200
    mul t3, t0, s1
    sub t2, t2, t3
    srai t2, t2, 10
    li t0, -200
    mul t4, t0, s1
    li t0, -200
    mul t3, t0, s2
    add t4, t4, t3
    srai t4, t4, 10
    li t3, -200
    li t5, 600
    add t5, t5, t4
    li t0, 300
    mul t0, t2, t0
    div t0, t0, t5
    li t1, 175
    add t0, t0, t1
    li t1, 300
    mul t1, t3, t1
    div t1, t1, t5
    li t6, 130
    sub t1, t6, t1
    sw t0, 0(s3)
    sw t1, 4(s3)

# Vertex 1: (1,-1,-1)
    li t0, 200
    mul t2, t0, s2
    li t0, -200
    mul t3, t0, s1
    sub t2, t2, t3
    srai t2, t2, 10
    li t0, 200
    mul t4, t0, s1
    li t0, -200
    mul t3, t0, s2
    add t4, t4, t3
    srai t4, t4, 10
    li t3, -200
    li t5, 600
    add t5, t5, t4
    li t0, 300
    mul t0, t2, t0
    div t0, t0, t5
    li t1, 175
    add t0, t0, t1
    li t1, 300
    mul t1, t3, t1
    div t1, t1, t5
    li t6, 130
    sub t1, t6, t1
    sw t0, 8(s3)
    sw t1, 12(s3)

# Vertex 2: (1,1,-1)
    li t0, 200
    mul t2, t0, s2
    li t0, -200
    mul t3, t0, s1
    sub t2, t2, t3
    srai t2, t2, 10
    li t0, 200
    mul t4, t0, s1
    li t0, -200
    mul t3, t0, s2
    add t4, t4, t3
    srai t4, t4, 10
    li t3, 200
    li t5, 600
    add t5, t5, t4
    li t0, 300
    mul t0, t2, t0
    div t0, t0, t5
    li t1, 175
    add t0, t0, t1
    li t1, 300
    mul t1, t3, t1
    div t1, t1, t5
    li t6, 130
    sub t1, t6, t1
    sw t0, 16(s3)
    sw t1, 20(s3)

# Vertex 3: (-1,1,-1)
    li t0, -200
    mul t2, t0, s2
    li t0, -200
    mul t3, t0, s1
    sub t2, t2, t3
    srai t2, t2, 10
    li t0, -200
    mul t4, t0, s1
    li t0, -200
    mul t3, t0, s2
    add t4, t4, t3
    srai t4, t4, 10
    li t3, 200
    li t5, 600
    add t5, t5, t4
    li t0, 300
    mul t0, t2, t0
    div t0, t0, t5
    li t1, 175
    add t0, t0, t1
    li t1, 300
    mul t1, t3, t1
    div t1, t1, t5
    li t6, 130
    sub t1, t6, t1
    sw t0, 24(s3)
    sw t1, 28(s3)

# Vertex 4: (-1,-1,1)
    li t0, -200
    mul t2, t0, s2
    li t0, 200
    mul t3, t0, s1
    sub t2, t2, t3
    srai t2, t2, 10
    li t0, -200
    mul t4, t0, s1
    li t0, 200
    mul t3, t0, s2
    add t4, t4, t3
    srai t4, t4, 10
    li t3, -200
    li t5, 600
    add t5, t5, t4
    li t0, 300
    mul t0, t2, t0
    div t0, t0, t5
    li t1, 175
    add t0, t0, t1
    li t1, 300
    mul t1, t3, t1
    div t1, t1, t5
    li t6, 130
    sub t1, t6, t1
    sw t0, 32(s3)
    sw t1, 36(s3)

# Vertex 5: (1,-1,1)
    li t0, 200
    mul t2, t0, s2
    li t0, 200
    mul t3, t0, s1
    sub t2, t2, t3
    srai t2, t2, 10
    li t0, 200
    mul t4, t0, s1
    li t0, 200
    mul t3, t0, s2
    add t4, t4, t3
    srai t4, t4, 10
    li t3, -200
    li t5, 600
    add t5, t5, t4
    li t0, 300
    mul t0, t2, t0
    div t0, t0, t5
    li t1, 175
    add t0, t0, t1
    li t1, 300
    mul t1, t3, t1
    div t1, t1, t5
    li t6, 130
    sub t1, t6, t1
    sw t0, 40(s3)
    sw t1, 44(s3)

# Vertex 6: (1,1,1)
    li t0, 200
    mul t2, t0, s2
    li t0, 200
    mul t3, t0, s1
    sub t2, t2, t3
    srai t2, t2, 10
    li t0, 200
    mul t4, t0, s1
    li t0, 200
    mul t3, t0, s2
    add t4, t4, t3
    srai t4, t4, 10
    li t3, 200
    li t5, 600
    add t5, t5, t4
    li t0, 300
    mul t0, t2, t0
    div t0, t0, t5
    li t1, 175
    add t0, t0, t1
    li t1, 300
    mul t1, t3, t1
    div t1, t1, t5
    li t6, 130
    sub t1, t6, t1
    sw t0, 48(s3)
    sw t1, 52(s3)

# Vertex 7: (-1,1,1)
    li t0, -200
    mul t2, t0, s2
    li t0, 200
    mul t3, t0, s1
    sub t2, t2, t3
    srai t2, t2, 10
    li t0, -200
    mul t4, t0, s1
    li t0, 200
    mul t3, t0, s2
    add t4, t4, t3
    srai t4, t4, 10
    li t3, 200
    li t5, 600
    add t5, t5, t4
    li t0, 300
    mul t0, t2, t0
    div t0, t0, t5
    li t1, 175
    add t0, t0, t1
    li t1, 300
    mul t1, t3, t1
    div t1, t1, t5
    li t6, 130
    sub t1, t6, t1
    sw t0, 56(s3)
    sw t1, 60(s3)

# Draw 6 faces (12 triangles)
# Face 0 color=0xff4444ff
    lw t0, 0(s3)
    lw t1, 4(s3)
    lw t2, 8(s3)
    lw t3, 12(s3)
    lw t4, 16(s3)
    lw t5, 20(s3)
    li t6, 5
    sw t6, 0x0(zero)
    sw t0, 0x4(zero)
    sw t1, 0x8(zero)
    sw t2, 0xc(zero)
    sw t3, 0x10(zero)
    sw t4, 0x1c(zero)
    sw t5, 0x20(zero)
    li t6, 0xff4444ff
    sw t6, 0x14(zero)
    li t6, 1
    sw t6, 0x18(zero)

    lw t0, 0(s3)
    lw t1, 4(s3)
    lw t2, 16(s3)
    lw t3, 20(s3)
    lw t4, 24(s3)
    lw t5, 28(s3)
    li t6, 5
    sw t6, 0x0(zero)
    sw t0, 0x4(zero)
    sw t1, 0x8(zero)
    sw t2, 0xc(zero)
    sw t3, 0x10(zero)
    sw t4, 0x1c(zero)
    sw t5, 0x20(zero)
    li t6, 0xff4444ff
    sw t6, 0x14(zero)
    li t6, 1
    sw t6, 0x18(zero)

# Face 1 color=0x44ff44ff
    lw t0, 40(s3)
    lw t1, 44(s3)
    lw t2, 32(s3)
    lw t3, 36(s3)
    lw t4, 56(s3)
    lw t5, 60(s3)
    li t6, 5
    sw t6, 0x0(zero)
    sw t0, 0x4(zero)
    sw t1, 0x8(zero)
    sw t2, 0xc(zero)
    sw t3, 0x10(zero)
    sw t4, 0x1c(zero)
    sw t5, 0x20(zero)
    li t6, 0x44ff44ff
    sw t6, 0x14(zero)
    li t6, 1
    sw t6, 0x18(zero)

    lw t0, 40(s3)
    lw t1, 44(s3)
    lw t2, 56(s3)
    lw t3, 60(s3)
    lw t4, 48(s3)
    lw t5, 52(s3)
    li t6, 5
    sw t6, 0x0(zero)
    sw t0, 0x4(zero)
    sw t1, 0x8(zero)
    sw t2, 0xc(zero)
    sw t3, 0x10(zero)
    sw t4, 0x1c(zero)
    sw t5, 0x20(zero)
    li t6, 0x44ff44ff
    sw t6, 0x14(zero)
    li t6, 1
    sw t6, 0x18(zero)

# Face 2 color=0x4444ffff
    lw t0, 32(s3)
    lw t1, 36(s3)
    lw t2, 0(s3)
    lw t3, 4(s3)
    lw t4, 24(s3)
    lw t5, 28(s3)
    li t6, 5
    sw t6, 0x0(zero)
    sw t0, 0x4(zero)
    sw t1, 0x8(zero)
    sw t2, 0xc(zero)
    sw t3, 0x10(zero)
    sw t4, 0x1c(zero)
    sw t5, 0x20(zero)
    li t6, 0x4444ffff
    sw t6, 0x14(zero)
    li t6, 1
    sw t6, 0x18(zero)

    lw t0, 32(s3)
    lw t1, 36(s3)
    lw t2, 24(s3)
    lw t3, 28(s3)
    lw t4, 56(s3)
    lw t5, 60(s3)
    li t6, 5
    sw t6, 0x0(zero)
    sw t0, 0x4(zero)
    sw t1, 0x8(zero)
    sw t2, 0xc(zero)
    sw t3, 0x10(zero)
    sw t4, 0x1c(zero)
    sw t5, 0x20(zero)
    li t6, 0x4444ffff
    sw t6, 0x14(zero)
    li t6, 1
    sw t6, 0x18(zero)

# Face 3 color=0xffff44ff
    lw t0, 8(s3)
    lw t1, 12(s3)
    lw t2, 40(s3)
    lw t3, 44(s3)
    lw t4, 48(s3)
    lw t5, 52(s3)
    li t6, 5
    sw t6, 0x0(zero)
    sw t0, 0x4(zero)
    sw t1, 0x8(zero)
    sw t2, 0xc(zero)
    sw t3, 0x10(zero)
    sw t4, 0x1c(zero)
    sw t5, 0x20(zero)
    li t6, 0xffff44ff
    sw t6, 0x14(zero)
    li t6, 1
    sw t6, 0x18(zero)

    lw t0, 8(s3)
    lw t1, 12(s3)
    lw t2, 48(s3)
    lw t3, 52(s3)
    lw t4, 16(s3)
    lw t5, 20(s3)
    li t6, 5
    sw t6, 0x0(zero)
    sw t0, 0x4(zero)
    sw t1, 0x8(zero)
    sw t2, 0xc(zero)
    sw t3, 0x10(zero)
    sw t4, 0x1c(zero)
    sw t5, 0x20(zero)
    li t6, 0xffff44ff
    sw t6, 0x14(zero)
    li t6, 1
    sw t6, 0x18(zero)

# Face 4 color=0xff44ffff
    lw t0, 24(s3)
    lw t1, 28(s3)
    lw t2, 16(s3)
    lw t3, 20(s3)
    lw t4, 48(s3)
    lw t5, 52(s3)
    li t6, 5
    sw t6, 0x0(zero)
    sw t0, 0x4(zero)
    sw t1, 0x8(zero)
    sw t2, 0xc(zero)
    sw t3, 0x10(zero)
    sw t4, 0x1c(zero)
    sw t5, 0x20(zero)
    li t6, 0xff44ffff
    sw t6, 0x14(zero)
    li t6, 1
    sw t6, 0x18(zero)

    lw t0, 24(s3)
    lw t1, 28(s3)
    lw t2, 48(s3)
    lw t3, 52(s3)
    lw t4, 56(s3)
    lw t5, 60(s3)
    li t6, 5
    sw t6, 0x0(zero)
    sw t0, 0x4(zero)
    sw t1, 0x8(zero)
    sw t2, 0xc(zero)
    sw t3, 0x10(zero)
    sw t4, 0x1c(zero)
    sw t5, 0x20(zero)
    li t6, 0xff44ffff
    sw t6, 0x14(zero)
    li t6, 1
    sw t6, 0x18(zero)

# Face 5 color=0x44ffffff
    lw t0, 32(s3)
    lw t1, 36(s3)
    lw t2, 40(s3)
    lw t3, 44(s3)
    lw t4, 8(s3)
    lw t5, 12(s3)
    li t6, 5
    sw t6, 0x0(zero)
    sw t0, 0x4(zero)
    sw t1, 0x8(zero)
    sw t2, 0xc(zero)
    sw t3, 0x10(zero)
    sw t4, 0x1c(zero)
    sw t5, 0x20(zero)
    li t6, 0x44ffffff
    sw t6, 0x14(zero)
    li t6, 1
    sw t6, 0x18(zero)

    lw t0, 32(s3)
    lw t1, 36(s3)
    lw t2, 8(s3)
    lw t3, 12(s3)
    lw t4, 0(s3)
    lw t5, 4(s3)
    li t6, 5
    sw t6, 0x0(zero)
    sw t0, 0x4(zero)
    sw t1, 0x8(zero)
    sw t2, 0xc(zero)
    sw t3, 0x10(zero)
    sw t4, 0x1c(zero)
    sw t5, 0x20(zero)
    li t6, 0x44ffffff
    sw t6, 0x14(zero)
    li t6, 1
    sw t6, 0x18(zero)

# Advance angle
    addi s0, s0, 1
    li t0, 64
    rem s0, s0, t0

# Frame delay
    li t0, 2
frame_delay:
    addi t0, t0, -1
    bnez t0, frame_delay

    j main_loop
