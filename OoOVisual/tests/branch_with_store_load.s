addi sp,zero,0xFFFF0
sw zero,-4(sp)
li t1,100
j loop
go_end:
	j end
loop:
	lw t0,-4(sp)
	addi t0,t0,1
	sw t0,-4(sp)
	bne t0,t1,loop
	j go_end
end:
	li x31,3160
