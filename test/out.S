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
  la    t1, a
  sw    t0, 2(x0)
  la    t0, a
  lw    t0, 0(t0)
  sw    t0, 8(sp)
  lw    a0, 8(sp)
  addi  sp, sp, 16
  ret

  .globl main
main:
  addi  sp, sp, 0
entry:
  addi  sp, sp, 0
  ret

