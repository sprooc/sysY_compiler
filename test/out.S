  .data
  .text
  .globl main
main:
  addi  sp, sp, -48
  sw    ra, 44(sp)
entry:
  addi  t0, sp, 0
  li    t2, 4
  mul   t2, t2, x0
  add   t0, t0, t2
  sw    t0, 16(sp)
  li    t0, 1
  lw    t1, 16(sp)
  sw    t0, 0(t1)
  addi  t0, sp, 0
  li    t1, 1
  li    t2, 4
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 20(sp)
  lw    t1, 20(sp)
  sw    x0, 0(t1)
  addi  t0, sp, 0
  li    t1, 2
  li    t2, 4
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 24(sp)
  lw    t1, 24(sp)
  sw    x0, 0(t1)
  addi  t0, sp, 0
  li    t1, 3
  li    t2, 4
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 28(sp)
  lw    t1, 28(sp)
  sw    x0, 0(t1)
  addi  t0, sp, 0
  li    t2, 4
  mul   t2, t2, x0
  add   t0, t0, t2
  sw    t0, 32(sp)
  lw    t0, 32(sp)
  lw    t1, 0(t0)
  sw    t1, 36(sp)
  lw    a0, 36(sp)
  addi  sp, sp, 48
  ret

