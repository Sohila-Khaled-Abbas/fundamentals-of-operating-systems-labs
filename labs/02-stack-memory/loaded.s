400: ret                    ; done with main
399: add	sp, sp, #20         ;deallocate
398: ldp	bp, lr, [bp]        ; load the kernel's lr and base pointer
397: str r3, [bp, #-16]         ;store r3 to c's address 
396: add r3, r0, r1 ;  add r0, r1 store it in r3 (c = a+b )
395: ldr	r1, [bp, #-12]  ; restore b from memory
394: ldr	r0, [bp, #-8]  ; restore a from memory
393: str pc, #func1
392: ldr lr, [pc, #1] ;load the next instruction in main in the link register ;, it's safe because we already stored the old lr in memory
391: str	r0, [bp, #-12]  ;store r0 in b, because we are about to leave to a new function...
390: mov	r0, #3            ;set r0 to 3
389: str	r0, [bp, #-8]          ;store 1 in a 
388: mov	r0, #1               ;set r0 to 1
387: add	bp, sp, #20         ;set the base pointer for main (start of main)
386: stp	bp, lr, [sp, #20]     ;4,4
385: sub	sp, sp, #20
384: ret           # return;  
383: str sp, bp      ;load the sp pointer set 
382: str pc, lr      ;set to the program counter to the link register, we didn't touch lr so it should still have main's return address
381: add	sp, sp, #8  ;deallocate
380: ldr bp, [sp, #8] ;load the main's bp in bp register 
379: str	r0, [bp, #4] ;store r0 in z's memory 
378: add	r0, r0, #1   ;add one to the register
377: mov	r0, #1      ;z = 1, store it in r0
376: add bp, sp, #8   ;load sp + 8 as the func1 base pointer
375: str bp, [sp, #8]  ;store main's base pointer to old base pointer memory
374: sub	sp, sp, #8 ;allocate 8 bytes for func1, truth is cpu may allocate more for performance and alignments reasons, 4 for the main's base pointer, and 4 for z 

