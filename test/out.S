  .data
  .text
  .globl main
main:
  addi  sp, sp, -208
entry:
  addi  t0, sp, 0
  li    t1, 2
  li    t2, 4
  mul   t2, t2, t1
  add   t0, t0, 3
  sw    t0, 200(sp)
  lw    t0, 200(sp)
  sw    t0, 204(sp)
  lw    a0, 204(sp)
  addi  sp, sp, 208
  ret

