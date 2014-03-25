	.text
	.align 2
	.globl add
add:
	sw 	$ra, -4($sp)
	sw 	$fp, -8($sp)
	sw 	$sp, -12($sp)
	move 	$fp,$sp
	addi 	$sp, $fp, -24
	lw 	$a0, 0($fp)
	lw 	$v0, 4($fp)
	add 	$v0, $a0, $v0
	move 	$sp,$fp
	lw 	$fp, -8($sp)
	lw 	$ra, 0($sp)
	jr 	$ra
	.align 2
	.globl main
main:
	sw 	$ra, -4($sp)
	sw 	$fp, -8($sp)
	sw 	$sp, -12($sp)
	move 	$fp,$sp
	addi 	$sp, $fp, -32
	move 	$v0,$zero
	sw 	$v0, -16($fp)
	addi 	$sp, $sp, -8
	move 	$v0,$sp
	li 	$a0, 2
	sw 	$a0, 0($v0)
	li 	$a0, 3
	sw 	$a0, 4($v0)
	jal add
	addi 	$sp, $sp, 8
	sw 	$v0, -16($fp)
	move 	$v0,$zero
	move 	$sp,$fp
	lw 	$fp, -8($sp)
	lw 	$ra, 0($sp)
	jr 	$ra
