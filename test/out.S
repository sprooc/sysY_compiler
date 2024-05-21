  .data
  .global x
x:
  .word x_0
  .global init
init:
  .word init_1
  .text
  .global main
main:
  addi  sp, sp, -16
  sw    ra, 12(sp)
entry:
