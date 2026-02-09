li x2,5
loop:
	addi x1,x1,1
	bne  x1,x2,loop
loop2:
	addi x1,x1,-1
	bne x1,zero,loop2