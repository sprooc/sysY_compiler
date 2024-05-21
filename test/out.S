  .text
  .global sum
sum:
  addi  sp, sp, -112
L0:
  sw    a0, 0(sp)
  sw    a1, 4(sp)
  sw    a2, 8(sp)
  sw    a3, 12(sp)
  sw    a4, 16(sp)
  sw    a5, 20(sp)
  sw    a6, 24(sp)
  sw    a7, 28(sp)
  lw    t0, 112(sp)
  sw    t0, 32(sp)
  lw    t0, 0(sp)
  sw    t0, 36(sp)
  lw    t0, 4(sp)
  sw    t0, 40(sp)
  lw    t0, 36(sp)
  lw    t1, 40(sp)
  add   t0, t0, t1
  sw    t0, 44(sp)
  lw    t0, 8(sp)
  sw    t0, 48(sp)
  lw    t0, 44(sp)
  lw    t1, 48(sp)
  add   t0, t0, t1
  sw    t0, 52(sp)
  lw    t0, 12(sp)
  sw    t0, 56(sp)
  lw    t0, 52(sp)
  lw    t1, 56(sp)
  add   t0, t0, t1
  sw    t0, 60(sp)
  lw    t0, 16(sp)
  sw    t0, 64(sp)
  lw    t0, 60(sp)
  lw    t1, 64(sp)
  add   t0, t0, t1
  sw    t0, 68(sp)
  lw    t0, 20(sp)
  sw    t0, 72(sp)
  lw    t0, 68(sp)
  lw    t1, 72(sp)
  add   t0, t0, t1
  sw    t0, 76(sp)
  lw    t0, 24(sp)
  sw    t0, 80(sp)
  lw    t0, 76(sp)
  lw    t1, 80(sp)
  add   t0, t0, t1
  sw    t0, 84(sp)
  lw    t0, 28(sp)
  sw    t0, 88(sp)
  lw    t0, 84(sp)
  lw    t1, 88(sp)
  add   t0, t0, t1
  sw    t0, 92(sp)
  lw    t0, 32(sp)
  sw    t0, 96(sp)
  lw    t0, 92(sp)
  lw    t1, 96(sp)
  add   t0, t0, t1
  sw    t0, 100(sp)
  lw    a0, 100(sp)
  addi  sp, sp, 112
  ret

  .text
  .global main
main:
  addi  sp, sp, -32
  sw    ra, 28(sp)
entry:
  li    a0, 1
  li    a1, 2
  li    a2, 3
  li    a3, 4
  li    a4, 5
  li    a5, 6
  li    a6, 7
  li    a7, 8
  li    t0, 9
  sw    t0, 0(sp)
  call  sum
  sw    a0, 4(sp)
  lw    t1, 4(sp)
  sw    t1, 0(sp)
  lw    t0, 0(sp)
  sw    t0, 8(sp)
  lw    a0, 8(sp)
  lw    ra, 28(sp)
  addi  sp, sp, 32
  ret

