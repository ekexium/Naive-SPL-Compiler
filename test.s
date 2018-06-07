	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 12
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:                               ## %entry
	movq	$2, -8(%rsp)
	movabsq	$4638371957281883423, %rax ## imm = 0x405ECEB851EB851F
	movq	%rax, -16(%rsp)
	movq	$10, -32(%rsp)
	movq	$0, -24(%rsp)
	cmpq	$10, -24(%rsp)
	je	LBB0_3
	.p2align	4, 0x90
LBB0_2:                                 ## %loopStmt
                                        ## =>This Inner Loop Header: Depth=1
	movq	-24(%rsp), %rax
	addq	%rax, -32(%rsp)
	incq	%rax
	movq	%rax, -24(%rsp)
	cmpq	$10, -24(%rsp)
	jne	LBB0_2
	jmp	LBB0_3
	.p2align	4, 0x90
LBB0_4:                                 ## %loopStmt2
                                        ##   in Loop: Header=BB0_3 Depth=1
	decq	-24(%rsp)
LBB0_3:                                 ## %startloop1
                                        ## =>This Inner Loop Header: Depth=1
	cmpq	$6, -24(%rsp)
	jge	LBB0_4
	.p2align	4, 0x90
LBB0_5:                                 ## %loopStmt4
                                        ## =>This Inner Loop Header: Depth=1
	movq	-32(%rsp), %rax
	addq	%rax, %rax
	movq	%rax, -32(%rsp)
	cmpq	$99, %rax
	jg	LBB0_5
## %bb.6:                               ## %eixtStmt5
	cmpq	$0, -32(%rsp)
	js	LBB0_8
## %bb.7:                               ## %thenStmt
	incq	-32(%rsp)
LBB0_8:                                 ## %mergeStmt
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90         ## -- Begin function add
_add:                                   ## @add
	.cfi_startproc
## %bb.0:                               ## %entry
	movq	-16(%rsp), %rax
	addq	-8(%rsp), %rax
	movq	%rax, -24(%rsp)
	retq
	.cfi_endproc
                                        ## -- End function

.subsections_via_symbols
