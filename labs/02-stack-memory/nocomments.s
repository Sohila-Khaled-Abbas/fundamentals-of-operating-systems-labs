func1:   ;func1 function
	sub	sp, sp, #8 ;allocate 8 bytes for func1, truth is cpu may allocate more for performance and alignments reasons, 4 for the main's base pointer, and 4 for z 
    str bp, [sp, #8]  ;store main's base pointer to old base pointer memory
    add bp, sp, #8   ;load sp + 8 as the func1 base pointer
    mov	r0, #1      ;z = 1, store it in r0
	add	r0, r0, #1   ;add one to the register
	str	r0, [bp, #-4] ;store r0 in z's memory 
    ldr bp, [bp] ;load the main's bp in bp register 
	add	sp, sp, #8  ;deallocate
    str pc, lr      ;set to the program counter to the link register, we didn't touch lr so it should still have main's return address
    str sp, bp      ;load the sp pointer set 
    ret  # return;  
main:     ;main function
	sub	sp, sp, #20
	stp	bp, lr, [sp, #20]  ;4,4
	add	bp, sp, #20  ;set the base pointer for main (start of main)
	mov	r0, #1     ;set r0 to 1
	str	r0, [bp, #-8]  ;store 1 in a 
	mov	r0, #3    ;set r0 to 3
	str	r0, [bp, #-12]  ;store r0 in b, because we ware about to leave to a new function..
    ldr lr, [pc, #1] ;load the next instruction in main in the link register ;, its safe because we already stored the old lr in memory
    str pc, #func1
	ldr	r0, [bp, #-8]  ; restore a  from memory
	ldr	r1, [bp, #-12]  ; restore b from memory
    add r3,r0, r1 ;  add r0, r1 store it in r3 (c = a+b )
    str r3, [bp, #-16] ;store r3 to c's address 
	ldp	bp, lr, [bp] ; load the kernel's lr and and base pointer
	add	sp, sp, #20 ;deallocate
	ret  ; done with main