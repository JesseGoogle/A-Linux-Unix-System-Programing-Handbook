	.file	"abuseJmp.c"
	.text
	.local	jumpBuffer
	.comm	jumpBuffer,200,32
	.local	f1CallTimes
	.comm	f1CallTimes,4,4
	.section	.rodata
	.align 8
.LC0:
	.string	"Call function f1(), set jump position."
.LC1:
	.string	"Call f1() %d times.\n"
	.text
	.globl	f1
	.type	f1, @function
f1:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	leaq	jumpBuffer(%rip), %rax
	movq	%rax, %rdi
	call	_setjmp@PLT
	endbr64
	movl	f1CallTimes(%rip), %eax
	addl	$1, %eax
	movl	%eax, f1CallTimes(%rip)
	movl	f1CallTimes(%rip), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	f1, .-f1
	.section	.rodata
	.align 8
.LC2:
	.string	"Call function f2(), jump to f1()."
	.text
	.globl	f2
	.type	f2, @function
f2:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$1, %esi
	leaq	jumpBuffer(%rip), %rax
	movq	%rax, %rdi
	call	longjmp@PLT
	.cfi_endproc
.LFE7:
	.size	f2, .-f2
	.globl	main
	.type	main, @function
main:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	call	f1
	call	f2
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.2.0-4ubuntu3) 13.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
