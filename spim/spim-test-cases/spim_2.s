	.text
	.align 2
	.globl main
main:
	sw 	$ra, -4($sp)
	sw 	$fp, -8($sp)
	sw 	$sp, -12($sp)
	move 	$fp,$sp
	addi 	$sp, $fp, -40
	lw 	$v0, -16($fp)
	div 	$v0, $v0, -20($fp)
	sw 	$v0, -24($fp)
	lw 	$a0, -16($fp)
	lw 	$v0, -20($fp)
	and 	$v0, $a0, $v0
	sw 	$v0, -24($fp)
	move 	$v0,$zero
	move 	$sp,$fp
	lw 	$fp, -8($sp)
	lw 	$ra, 0($sp)
	jr 	$ra
