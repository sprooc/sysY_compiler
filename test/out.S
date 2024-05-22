  .data
  .text
  .globl main
main:
  addi  sp, sp, -32
  sw    ra, 28(sp)
entry:
  addi  t0, sp, 0
  li    t2, 4
  mul   t2, t2, x0
  add   t0, t0, t2
  sw    t0, 8(sp)
  li    t0, 1
  lw    t1, 8(sp)
  sw    t0, 0(t1)
  addi  t0, sp, 0
  li    t1, 1
  li    t2, 4
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 12(sp)
  lw    t1, 12(sp)
  sw    x0, 0(t1)
  addi  t0, sp, 0
  li    t1, 1
  li    t2, 4
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 16(sp)
  lw    t0, 16(sp)
  lw    t1, 0(t0)
  sw    t1, 20(sp)
  lw    a0, 20(sp)
  addi  sp, sp, 32
  ret

