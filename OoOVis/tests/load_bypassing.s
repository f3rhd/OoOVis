li x1,1000
sw x1,0(x1)
sw x1,10(x1)
sw x1,20(x1)
sw x1,30(x1)
sw x1,40(x1)
lw x1,60(x1) # this is a bypassable load instruction since its  address doesnt alias with any of the instructions above
