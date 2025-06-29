	.file	"t1.c"
	.text
	.section	.rodata
.LC0:
	.string	"eae "
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp
  
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$44, %eax
	popq	%rbp
	ret
