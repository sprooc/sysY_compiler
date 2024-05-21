  .text
  .global fib
fib:
  addi  sp, sp, -48
  sw    ra, 44(sp)
L0:
  sw    a0, 4(sp)
  lw    t0, 4(sp)
  sw    t0, 8(sp)
  lw    t0, 8(sp)
  li    t1, 2
  slt   t0, t0, t1
  sw    t0, 12(sp)
  lw    t0, 12(sp)
  bnez  t0, L2
  j     L1
L2:
  lw    t0, 4(sp)
  sw    t0, 16(sp)
  lw    a0, 16(sp)
  lw    ra, 44(sp)
  addi  sp, sp, 48
  ret
L1:
  lw    t0, 4(sp)
  sw    t0, 20(sp)
  lw    t0, 20(sp)
  li    t1, 1
  sub   t0, t0, t1
  sw    t0, 24(sp)
  lw    a0, 24(sp)
  call  fib
  sw    a0, 28(sp)
  lw    t0, 4(sp)
  sw    t0, 32(sp)
  lw    t0, 32(sp)
  li    t1, 2
  sub   t0, t0, t1
  sw    t0, 36(sp)
  lw    a0, 36(sp)
  call  fib
  sw    a0, 40(sp)
  lw    t0, 28(sp)
  lw    t1, 40(sp)
  add   t0, t0, t1
  sw    t0, 44(sp)
  lw    a0, 44(sp)
  lw    ra, 44(sp)
  addi  sp, sp, 48
  ret

  .text
  .global main
main:
  addi  sp, sp, -16
  sw    ra, 12(sp)
entry:
  li    a0, 20
  call  fib
  sw    a0, 4(sp)
  lw    a0, 4(sp)
  lw    ra, 12(sp)
  addi  sp, sp, 16
  ret

