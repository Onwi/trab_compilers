// fixed init
	.file	"t1.c"
	.text

// global vars
.data
a:
	.long 1	
b:
	.long	199
d:
	.long	1079194419

// end global vars

	.section	.rodata
.LC0:
	.string	"i cant belive im doing assembly again, kill me\n"
.LC1:
	.string	"%d\n"
  .text

// TAC BEGINFUN
.globl	main
.type	main, @function

main:
	endbr64
	pushq	%rbp
	movq	%rsp, %rbp

  // my instructions
  // TAC_PRINT
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	call	printf@PLT
  // TAC_ADD && TAC_MOVE
	movl	$7, -4(%rbp)
	addl	$3, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %esi
  // TAC_PRINT
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT

	movl	a(%rip), %eax
	addl	$3, %eax
	movl	%eax, a(%rip)
	movl	a(%rip), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
  // my instructions

// TAC_ENDFUN
	leave
	ret

