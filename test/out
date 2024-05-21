  .text
  .global sum
sum:
  addi  sp, sp, -96
L0:
  sw    a0, 0(sp)
  sw    a1, 4(sp)
  sw    a2, 8(sp)
  sw    a3, 12(sp)
  sw    a4, 16(sp)
  sw    a5, 20(sp)
  sw    a6, 24(sp)
  sw    a7, 28(sp)
  lw    t0, 0(sp)
  sw    t0, 32(sp)
  lw    t0, 4(sp)
  sw    t0, 36(sp)
  lw    t0, 32(sp)
  lw    t1, 36(sp)
  add   t0, t0, t1
  sw    t0, 40(sp)
  lw    t0, 8(sp)
  sw    t0, 44(sp)
  lw    t0, 40(sp)
  lw    t1, 44(sp)
  add   t0, t0, t1
  sw    t0, 48(sp)
  lw    t0, 12(sp)
  sw    t0, 52(sp)
  lw    t0, 48(sp)
  lw    t1, 52(sp)
  add   t0, t0, t1
  sw    t0, 56(sp)
  lw    t0, 16(sp)
  sw    t0, 60(sp)
  lw    t0, 56(sp)
  lw    t1, 60(sp)
  add   t0, t0, t1
  sw    t0, 64(sp)
  lw    t0, 20(sp)
  sw    t0, 68(sp)
  lw    t0, 64(sp)
  lw    t1, 68(sp)
  add   t0, t0, t1
  sw    t0, 72(sp)
  lw    t0, 24(sp)
  sw    t0, 76(sp)
  lw    t0, 72(sp)
  lw    t1, 76(sp)
  add   t0, t0, t1
  sw    t0, 80(sp)
  lw    t0, 28(sp)
  sw    t0, 84(sp)
  lw    t0, 80(sp)
  lw    t1, 84(sp)
  add   t0, t0, t1
  sw    t0, 88(sp)
  lw    a0, 88(sp)
  addi  sp, sp, 96
  ret

  .text
  .global main
main:
  addi  sp, sp, -16
  sw    ra, 12(sp)
entry:
  li    a0, 1
  li    a1, 2
  li    a2, 3
  li    a3, 4
  li    a4, 5
  li    a5, 6
  li    a6, 7
  li    a7, 8
  call  sum
  sw    a0, 4(sp)
  lw    t0, 4(sp)
  sw    t0, 0(sp)
  lw    t0, 0(sp)
  sw    t0, 8(sp)
  lw    a0, 8(sp)
  lw    ra, 12(sp)
  addi  sp, sp, 16
  ret

