	.file	"race_to_data.c"
	.text
	.section	.rodata
.LC0:
	.string	"%d"
	.text
	.globl	thread_body_1
	.type	thread_body_1, @function
thread_body_1:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	-8(%rbp), %rax
	movl	%edx, (%rax)
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	thread_body_1, .-thread_body_1
	.globl	thread_body_2
	.type	thread_body_2, @function
thread_body_2:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	leal	2(%rax), %edx
	movq	-8(%rbp), %rax
	movl	%edx, (%rax)
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	thread_body_2, .-thread_body_2
	.section	.rodata
	.align 8
.LC1:
	.string	"----------------------- %s \t\t\t%d\n"
	.align 8
.LC2:
	.string	" pthread_create: result1 = %d || result2 = %d\n"
	.align 8
.LC3:
	.string	"pthread_join: result1 = %d || result2 = %d\n"
	.align 8
.LC4:
	.string	"\n----------------------- %s \t\t\t%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	$0, -12(%rbp)
	movl	-12(%rbp), %eax
	movl	%eax, %edx
	leaq	__FUNCTION__.3386(%rip), %rsi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-12(%rbp), %rdx
	leaq	-24(%rbp), %rax
	movq	%rdx, %rcx
	leaq	thread_body_1(%rip), %rdx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create@PLT
	movl	%eax, -4(%rbp)
	leaq	-12(%rbp), %rdx
	leaq	-32(%rbp), %rax
	movq	%rdx, %rcx
	leaq	thread_body_2(%rip), %rdx
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_create@PLT
	movl	%eax, -8(%rbp)
	cmpl	$0, -4(%rbp)
	jne	.L6
	cmpl	$0, -8(%rbp)
	je	.L7
.L6:
	movq	stderr(%rip), %rax
	movl	-8(%rbp), %ecx
	movl	-4(%rbp), %edx
	leaq	.LC2(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %edi
	call	exit@PLT
.L7:
	movq	-24(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	movl	%eax, -4(%rbp)
	movq	-32(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	pthread_join@PLT
	movl	%eax, -8(%rbp)
	cmpl	$0, -4(%rbp)
	jne	.L8
	cmpl	$0, -8(%rbp)
	je	.L9
.L8:
	movq	stderr(%rip), %rax
	movl	-8(%rbp), %ecx
	movl	-4(%rbp), %edx
	leaq	.LC3(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$2, %edi
	call	exit@PLT
.L9:
	movl	-12(%rbp), %eax
	movl	%eax, %edx
	leaq	__FUNCTION__.3386(%rip), %rsi
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	main, .-main
	.section	.rodata
	.type	__FUNCTION__.3386, @object
	.size	__FUNCTION__.3386, 5
__FUNCTION__.3386:
	.string	"main"
	.ident	"GCC: (Debian 8.3.0-6) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
