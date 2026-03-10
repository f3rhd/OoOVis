_start:
    li x5, 0                # Base address of MMIO (FB_START)
    li x6, 364000            # Total pixel count (350 * 260)
    li x7, 0xFFFFFFFF               # White color (0xFFFFFFFF in two's complement)
    li x28, 0               # Loop counter (i = 0)

fill_loop:
    beq x28, x6, end_loop
    
    sw x7, 0(x5)      # Pixel 1
    sw x7, 4(x5)      # Pixel 2
    sw x7, 8(x5)      # Pixel 3
    sw x7, 12(x5)     # Pixel 4
    
    addi x5, x5, 16    # Jump 16 bytes (4 pixels)
    addi x28, x28, 4   # Increment counter by 4
    j fill_loop

end_loop:                   # Infinite loop to stop execution
    jal x0, end_loop