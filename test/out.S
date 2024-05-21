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
  addi  sp, sp, -48
  sw    ra, 44(sp)
entry:
  sw    x0, 0(sp)
  j     L2
L2:
  lw    t0, 0(sp)
  sw    t0, 4(sp)
  lw    t0, 4(sp)
  li    t1, 10
  slt   t0, t0, t1
  sw    t0, 8(sp)
  lw    t0, 8(sp)
  bnez  t0, L3
  j     L1
L3:
  call  inc
  sw    a0, 12(sp)
  li    t0, 1
  sw    t0, 16(sp)
  lw    t0, 16(sp)
  sw    t0, 20(sp)
  lw    t0, 20(sp)
  li    t1, 2
  add   t0, t0, t1
  sw    t0, 24(sp)
  lw    t0, 24(sp)
  sw    t0, 16(sp)
  lw    t0, 0(sp)
  sw    t0, 28(sp)
  lw    t0, 28(sp)
  li    t1, 1
  add   t0, t0, t1
  sw    t0, 32(sp)
  lw    t0, 32(sp)
  sw    t0, 0(sp)
  j     L2
L1:
  lw    ra, 44(sp)
  addi  sp, sp, 48
  ret

