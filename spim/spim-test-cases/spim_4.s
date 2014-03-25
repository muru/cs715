	.globl a
	.data 0x10000000
	.align 2
a:
	.word	2
	.text
	.align 2
	.globl main
main:
	sw 	$ra, -4($sp)
	sw 	$fp, -8($sp)
	sw 	$sp, -12($sp)
	move 	$fp,$sp
	addi 	$sp, $fp, -24
	j 	L2
L3:	lw 	$v0, a
	addi 	$v0, $v0, 1
	sw 	$v0, a
L2:	lw 	$a0, a
	li 	$v0, 5
	beq $a0, $v0, L3
	move 	$v0,$zero
	move 	$sp,$fp
	lw 	$fp, -8($sp)
	lw 	$ra, 0($sp)
	jr 	$ra
