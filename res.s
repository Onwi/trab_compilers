	.file "res.c"
	.text

.data
_s0:
	.string "ahoooo!"
	.text
_s1:
	.string "print this!"
	.text
_s2:
	.string "wasup!"
	.text
_101:
	.long 101
_69:
	.long 69
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
	leaq	_s2(%rip), %rax
	movq	%rax, %rdi
	call	printf@PLT
	movl	$69, %eax
	popq	%rbp
	ret
