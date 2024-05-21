  .data
  .globl a
a:
  .word 10
  .text
  .globl inc
inc:
  addi  sp, sp, -16
L0:
  la    t0, a
  lw    t0, 0(t0)
  sw    t0, 0(sp)
  lw    t0, 0(sp)
  li    t1, 1
  add   t0, t0, t1
  sw    t0, 4(sp)
  lw    t0, 4(sp)
