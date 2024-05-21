  .text
  .global add
add:
  addi  sp, sp, -32
L0:
  sw    a0, 0(sp)
  sw    a1, 4(sp)
  lw    t0, 0(sp)
  sw    t0, 8(sp)
  lw    t0, 4(sp)
  sw    t0, 12(sp)
  lw    t0, 8(sp)
  lw    t1, 12(sp)
  add   t0, t0, t1
  sw    t0, 16(sp)
  lw    a0, 16(sp)
  addi  sp, sp, 32
  ret

  .text
  .global sub
sub:
  addi  sp, sp, -32
L1:
  sw    a0, 0(sp)
  sw    a1, 4(sp)
  lw    t0, 0(sp)
  sw    t0, 8(sp)
  lw    t0, 4(sp)
  sw    t0, 12(sp)
  lw    t0, 8(sp)
  lw    t1, 12(sp)
  sub   t0, t0, t1
  sw    t0, 16(sp)
  lw    a0, 16(sp)
  addi  sp, sp, 32
  ret

  .text
  .global mul
mul:
  addi  sp, sp, -32
L2:
  sw    a0, 0(sp)
  sw    a1, 4(sp)
  lw    t0, 0(sp)
  sw    t0, 8(sp)
  lw    t0, 4(sp)
  sw    t0, 12(sp)
  lw    t0, 8(sp)
  lw    t1, 12(sp)
  mul   t0, t0, t1
  sw    t0, 16(sp)
  lw    a0, 16(sp)
  addi  sp, sp, 32
  ret

  .text
  .global div
div:
  addi  sp, sp, -32
L3:
  sw    a0, 0(sp)
  sw    a1, 4(sp)
  lw    t0, 0(sp)
  sw    t0, 8(sp)
  lw    t0, 4(sp)
  sw    t0, 12(sp)
  lw    t0, 8(sp)
  lw    t1, 12(sp)
  div   t0, t0, t1
  sw    t0, 16(sp)
  lw    a0, 16(sp)
  addi  sp, sp, 32
  ret

  .text
  .global main
main:
  addi  sp, sp, -80
  sw    ra, 76(sp)
entry:
  li    a0, 1
  li    a1, 2
  call  sub
  sw    a0, 12(sp)
  li    a0, 4
  li    a1, 5
  call  div
  sw    a0, 16(sp)
  li    a0, 3
  lw    a1, 16(sp)
  call  mul
  sw    a0, 20(sp)
  lw    a0, 12(sp)
  lw    a1, 20(sp)
  call  add
  sw    a0, 24(sp)
  lw    t0, 24(sp)
  sw    t0, 8(sp)
  lw    t0, 8(sp)
  sw    t0, 32(sp)
  lw    t0, 32(sp)
  sw    t0, 36(sp)
  lw    t0, 32(sp)
  bnez  t0, L10
  j     L11
L11:
  li    a0, 1
  li    a1, 2
  call  add
  sw    a0, 40(sp)
  lw    t0, 40(sp)
  li    t1, 10
  sgt   t0, t0, t1
  sw    t0, 44(sp)
  lw    t0, 44(sp)
  sw    t0, 36(sp)
  j     L10
L10:
  lw    t0, 36(sp)
  sw    t0, 48(sp)
  lw    t1, 48(sp)
  xor   t1, x0, t1
  snez  t1, t1
  sw    t1, 52(sp)
  lw    a0, 52(sp)
  li    a1, 5
  call  div
  sw    a0, 56(sp)
  li    a0, 3
  lw    a1, 56(sp)
  call  mul
  sw    a0, 60(sp)
  lw    t1, 60(sp)
  xor   t1, x0, t1
  snez  t1, t1
  sw    t1, 64(sp)
  li    a0, 1
  lw    a1, 64(sp)
  call  add
  sw    a0, 68(sp)
  lw    t0, 68(sp)
  sw    t0, 28(sp)
  lw    t0, 8(sp)
  sw    t0, 72(sp)
  lw    t0, 28(sp)
  sw    t0, 76(sp)
  lw    t0, 72(sp)
  lw    t1, 76(sp)
  add   t0, t0, t1
  sw    t0, 80(sp)
  lw    a0, 80(sp)
  lw    ra, 76(sp)
  addi  sp, sp, 80
  ret

