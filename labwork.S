  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
.global hexasc
	hexasc:
	and $a0, $a0,0xf	# we only want the 4 LSBs
	bge $a0, 0xa, letters	# handle letters elsewhere if argument is > 10
	nop
	addi $v0, $a0, 0x30	# add 0x30 for values from 0-9
	jr $ra
	nop
	
        letters:	
	addi $v0, $a0, 0x37	# add 0x37 for values from A-F
	jr $ra
	nop
	
.global delay
	delay:
	PUSH $ra
	PUSH $s0 # s0 = max i
	PUSH $s1 # s1 = ms

	move $s1, $a0			
	li $s1, 13		# change argument value from $a0, default 1000, optimal 50
	li $s0, 4711		# change i = 4711 variable in for loop
	
while: 	beqz $s1, done
	nop
	addi $s1, $s1, -1 
	li $t0, 0 # t0 = i

for: 	beq $t0, $s0, while 	# if i = 4711, return to while
	nop
	addi $t0, $t0, 1
	j for
	nop

done: 
	POP $s1
	POP $s0
	POP $ra
	jr $ra
	nop 

    .global delay2
	delay2:
	PUSH $ra
	PUSH $s0 # s0 = max i
	PUSH $s1 # s1 = ms

	move $s1, $a0			
	li $s1, 700		# change argument value from $a0, default 1000, optimal 50
	li $s0, 4711		# change i = 4711 variable in for loop
	
while2: 	beqz $s1, done2
	nop
	addi $s1, $s1, -1 
	li $t0, 0 # t0 = i

for2: 	beq $t0, $s0, while2	# if i = 4711, return to while
	nop
	addi $t0, $t0, 1
	j for2
	nop

done2: 
	POP $s1
	POP $s0
	POP $ra
	jr $ra
	nop 
	
.global time2string
	time2string:
	PUSH $ra
	PUSH $s0
	PUSH $s1
	
	move $s0, $a0
	move $s1, $a1
	
andi $a0, $s1, 0x000f # begin with first digit, convert to hexascii
andi $t7, $s1, 1
beqz $t7, even # if odd = binary ends with 1
nop # branch depending on ending
beq $t7, 1, odd
nop
time2return: 	
	jal hexasc 
	nop
	move $t0, $v0 #save to memory with offset 4, so its "leftmost" of the digits
	sb $t0, 4($s0) # we want to read it in the correct order of 5 8 : 5 9 /0 		
	andi $a0, $s1, 0x00f0
	srl $a0, $a0, 4 
	jal hexasc
	nop
	move $t0, $v0
	sb $t0, 3($s0)
	
	li $t0, 0x3A 		# load colon
	sb $t0, 2($s0)
	
	andi $a0, $s1, 0x0f00
	srl $a0, $a0, 8
	jal hexasc
	nop
	move $t0, $v0	
	sb $t0, 1($s0)
	
	andi $a0, $s1, 0xf000
	srl $a0, $a0, 12
	jal hexasc
	nop
	move $t0, $v0
	sb $t0, 0($s0)
	
	li $t0, 0x00
	sb $t0, 6($s0)	# null byte

	POP $s1
	POP $s0
	POP $ra

	jr $ra
	nop

odd: # if odd, save D - return
li $t0, 0x44
sb $t0, 5($s0)
j time2return
nop
even: # if odd, save E - return 
li $t0, 0x45
sb $t0, 5($s0)
j time2return
nop 

.global enable_interrupt
enable_interrupt:
ei
jr $ra
nop