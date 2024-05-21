  .data
  .text
  .globl main
main:
  addi  sp, sp, -32
entry:
  sw    x0, 0(sp)
  j     L1
L1:
  lw    t0, 0(sp)
  sw    t0, 4(sp)
  lw    t0, 4(sp)
  li    t1, 10
  slt   t0, t0, t1
  sw    t0, 8(sp)
  lw    t0, 8(sp)
  bnez  t0, L2
  j     L0
L2:
  li    t0, 1
  sw    t0, 12(sp)
  lw    t0, 12(sp)
  sw    t0, 16(sp)
  lw    t0, 16(sp)
  li    t1, 2
  add   t0, t0, t1
  sw    t0, 20(sp)
  lw    t0, 20(sp)
  sw    t0, 12(sp)
  lw    t0, 0(sp)
  sw    t0, 24(sp)
  lw    t0, 24(sp)
  li    t1, 1
  add   t0, t0, t1
  sw    t0, 28(sp)
  lw    t0, 28(sp)
  sw    t0, 0(sp)
  j     L1
L0:
  li    a0, 0
  addi  sp, sp, 32
  ret

