_main:
	li a0,5
	li a1,9
	call _add
	j _end
_add:
	add a2,a0,a1
	mv a0,a2
	ret
_end:
	addi t0,a0,1
	addi t1,a0,2
	addi t2,a0,3
