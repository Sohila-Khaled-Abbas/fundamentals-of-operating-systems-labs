sub	sp, sp, #12 ;allocate 12 bytes in stack
stp	bp, lr, [sp, #12] ;store current link register/base pointer
add	bp, sp, #8     ;set the base pointer of main
mov	r1, #0  ;     ;set r1 to 0
str	r1, [sp, #4]  ;init the pointer address to 0
mov	r0, #4        ;set 4 to r0, (32 bit allocation)
bl	_malloc       ;call malloc r0 is a parameter
str	r0, [sp, #4] ;malloc done, r0 now has the allocated address, store it stack
mov	r1, #10      ;set 10 in r1 
str	r1, [r0]     ;store 10 in memory location of r0 (heap)
add	r1, r1, #1   ;add one to r1
str	r1, [r0]     ;store new r1 to heap
ldr	r0, [sp, #4] ;set r0 to be address (can be skipped)
bl	_free        ;call free, take r0 as parameter to free it
ldp	bp, lr, [sp, #12] ;about to quit main load the bp,lr 
add	sp, sp, #12      ;deallocate the stafk
ret


900 _malloc
800 _free
704 ret
700 add	sp, sp, #12      ;deallocate the stack
696 ldp	bp, lr, [sp, #12] ;about to quit main load the bp, lr
692 bl	_free            ;call free, take r0 as parameter to free it
688 ldr	r0, [sp, #4]     ;set r0 to be address (can be skipped)
684 str	r1, [r0]         ;store new r1 to heap
680 add	r1, r1, #1       ;add one to r1
676 str	r1, [r0]         ;store 10 in memory location of r0 (heap)
672 mov	r1, #10          ;set 10 in r1
668 str	r0, [sp, #4]     ;malloc done, r0 now has the allocated address, store it stack
664 bl	_malloc           ;call malloc r0 is a parameter
660 mov	r0, #4            ;set 4 to r0, (32 bit allocation)
656 str	r1, [sp, #4]      ;init the pointer address to 0
652 mov	r1, #0            ;set r1 to 0
648 add	bp, sp, #8         ;set the base pointer of main
644 stp	bp, lr, [sp, #12] ;store current link register/base pointer
640 sub	sp, sp, #12       ;allocate 12 bytes in stack
