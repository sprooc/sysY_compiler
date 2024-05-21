  .data
  .global x
x:
  .global init
init:
  .text
  .global main
main:
  addi  sp, sp, -16
  sw    ra, 12(sp)
entry:
