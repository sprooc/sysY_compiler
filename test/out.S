  .data
  .globl arr
arr:
  .zero 262144
  .text
  .globl main
main:
  addi  sp, sp, -16
entry:
  addi  t0, sp, 0
  li    t2, 4
  mul   t2, t2, x0
  add   t0, t0, t2
  sw    t0, 4(sp)
  li    t0, 1
  lw    t1, 4(sp)
  sw    t0, 0(t1)
  addi  t0, sp, 0
  li    t2, 4
  mul   t2, t2, x0
  add   t0, t0, t2
  sw    t0, 8(sp)
  lw    t0, 8(sp)
  lw    t1, 0(t0)
  sw    t0, 0(t1)
  lw    a0, 12(sp)
  addi  sp, sp, 16
  ret

