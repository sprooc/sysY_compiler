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
  addi  sp, sp, -32
  sw    ra, 28(sp)
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
  sw    t0, 28(sp)
  lw    a0, 28(sp)
  lw    ra, 28(sp)
  addi  sp, sp, 32
  ret

