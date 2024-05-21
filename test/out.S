  .data
  .global x
x:
  .word 0
  .global init
init:
  .word 1
  .text
  .global main
main:
  addi  sp, sp, -16
  sw    ra, 12(sp)
entry:
