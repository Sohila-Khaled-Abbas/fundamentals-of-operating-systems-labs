	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 13, 0	sdk_version 14, 0
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	mov	w0, #0
	str	wzr, [sp, #12]
	adrp	x8, _A@PAGE
	ldr	w8, [x8, _A@PAGEOFF]
	adrp	x9, _B@PAGE
	ldr	w9, [x9, _B@PAGEOFF]
	add	w8, w8, w9
	str	w8, [sp, #8]
	add	sp, sp, #16
	ret
	.cfi_endproc
                                        ; -- End function
	.section	__DATA,__data
	.globl	_A                              ; @A
	.p2align	2, 0x0
_A:
	.long	10                              ; 0xa

	.globl	_B                              ; @B
	.p2align	2, 0x0
_B:
	.long	20                              ; 0x14

.subsections_via_symbols
