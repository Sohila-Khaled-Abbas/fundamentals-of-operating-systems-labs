sub	sp, sp, #12
ldr r0, [#DATA, 0] ;load A
ldr r1, [#DATA, 4] ;load B
add r2, r1, r0
str r2, [sp, #4]
add	sp, sp, #12


700: add	sp, sp, #12
696: str r2, [sp, #4]
692: add r2, r1, r0
688: ldr r1, [#DATA, #4] ;load B
684: ldr r0, [#DATA, #0] ;load A
680: sub	sp, sp, #12