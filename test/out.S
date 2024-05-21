  .data
  .globl arr
arr:
  .zero 262144
  .text
  .globl main
main:
  addi  sp, sp, -32
entry:
  addi  t0, sp, 0
  li    t2, 4
  mul   t2, t2, x0
  add   t0, t0, t2
  sw    t0, 16(sp)
  li    t0, 1
  sw    t0, 16(sp)
  addi  t0, sp, 0
  li    t1, 1
  li    t2, 4
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 20(sp)
  sw    x0, 20(sp)
  addi  t0, sp, 0
  li    t1, 2
  li    t2, 4
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 24(sp)
  sw    x0, 24(sp)
  addi  t0, sp, 0
  li    t1, 3
  li    t2, 4
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 28(sp)
  sw    x0, 28(sp)
  li    a0, 1
  addi  sp, sp, 32
  ret

