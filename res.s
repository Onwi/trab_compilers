	.file "res.c"
	.text

.data
.LC0:
	.string "%d\n"
_s0:
	.string "ahoooo!\n"
	.text
_s1:
	.string "eae!"
	.text
_s2:
	.string "print this!\n"
	.text
_0:
	.long 0
_10:
	.long 10
_101:
	.long 101
_111:
	.long 111
_20:
	.long 20
_69:
	.long 69
_99:
	.long 99
_ab:
	.long 101
_lol:
	.long 111
_gal:
	.long 99
_funRes:
	.long 0
_funRes:
	.long temp_0
	.globl myInt
	.type  myInt, @function
myInt:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp

	movl %edx, -4(%rbp)
	movl %ecx, -8(%rbp)
	leaq	_s0(%rip), %rax
	movq	%rax, %rdi
	call	printf@PLT
	movl	$20, %eax
	popq	%rbp
	ret
	.globl main
	.type  main, @function
main:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp

	leaq	_s1(%rip), %rax
	movq	%rax, %rdi
	call	printf@PLT
	leaq	_s2(%rip), %rax
	movq	%rax, %rdi
	call	printf@PLT
	movl	_ab(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	_lol(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	_gal(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	subq %16, %rsp
	movl $20, %ecx
	movl $10, %edx
	call myInt
	movl	_funRes(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$69, %eax
	popq	%rbp
	ret
