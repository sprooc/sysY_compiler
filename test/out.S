  .text
  .global fib
fib:
  addi  sp, sp, -48
  sw    ra, 44(sp)
L0:
  sw    a0, 0(sp)
  lw    t0, 0(sp)
  sw    t0, 4(sp)
  lw    t0, 4(sp)
  li    t1, 2
  slt   t0, t0, t1
  sw    t0, 8(sp)
  lw    t0, 8(sp)
  bnez  t0, L2
  j     L1
L2:
  lw    t0, 0(sp)
  sw    t0, 12(sp)
  lw    a0, 12(sp)
  lw    ra, 44(sp)
  addi  sp, sp, 48
  ret
L1:
  lw    t0, 0(sp)
  sw    t0, 16(sp)
  lw    t0, 16(sp)
  li    t1, 1
  sub   t0, t0, t1
  sw    t0, 20(sp)
  lw    a0, 20(sp)
  call  fib
  sw    a0, 24(sp)
  lw    t0, 0(sp)
  sw    t0, 28(sp)
  lw    t0, 28(sp)
  li    t1, 2
  sub   t0, t0, t1
  sw    t0, 32(sp)
  lw    a0, 32(sp)
  call  fib
  sw    a0, 36(sp)
  lw    t0, 24(sp)
  lw    t1, 36(sp)
  add   t0, t0, t1
  sw    t0, 40(sp)
  lw    a0, 40(sp)
  lw    ra, 44(sp)
  addi  sp, sp, 48
  ret

  .text
  .global main
main:
  addi  sp, sp, -16
  sw    ra, 12(sp)
entry:
  li    a0, 2
  call  fib
  sw    a0, 0(sp)
  lw    a0, 0(sp)
  lw    ra, 12(sp)
  addi  sp, sp, 16
  ret

