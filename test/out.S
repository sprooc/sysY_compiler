  .data
  .globl ga
ga:
  .zero 40
  .globl gb
gb:
  .word 5
  .word 6
  .word 7
  .word 8
  .word 0
  .word 0
  .text
  .globl main
main:
  addi  sp, sp, -16
  sw    ra, 12(sp)
entry:
  la    t0, ga_0
