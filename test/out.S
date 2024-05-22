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
  li    t2, 4
  mul   t2, t2, x0
  add   t0, t0, t2
  sw    t0, 0(sp)
  lw    t1, 0(sp)
  sw    x0, 0(t1)
  li    a0, 0
  addi  sp, sp, 16
  ret

