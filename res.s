	.file "res.c"
	.text

.data
.LC0:
	.string "%d\n"
_s0:
	.string "ahoooo!\n"
	.text
_s1:
	.string "print this!\n"
	.text
_101:
	.long 101
_69:
	.long 69
_label_0:
	.long label_0
	.globl main
	.type  main, @function
main:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp

	leaq	_s0(%rip), %rax
	movq	%rax, %rdi
	call	printf@PLT
	leaq	_s1(%rip), %rax
	movq	%rax, %rdi
	call	printf@PLT
	movl	_ab(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$69, %eax
	popq	%rbp
	ret
