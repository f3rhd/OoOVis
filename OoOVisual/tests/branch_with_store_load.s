addi sp,zero,0xFFFF0 #0
sw zero,-4(sp)  #1
li t1,100 #2
j loop #3
go_end:
	j end #4
loop:
	lw t0,-4(sp) #5
	addi t0,t0,1 #6
	sw t0,-4(sp)  #7
	bne t0,t1,loop #8
	j go_end #9
end:
	li x31,3169 #10
