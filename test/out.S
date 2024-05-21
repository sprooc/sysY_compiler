  .data
  .text
  .globl f
f:
  addi  sp, sp, 0
  sw    ra, -4(sp)
L0:
  li    a0, 0
  addi  sp, sp, 0
  ret

  .globl main
main:
  addi  sp, sp, -16
  sw    ra, 12(sp)
entry:
  call  f
  sw    a0, 0(sp)
  lw    a0, 0(sp)
  lw    ra, 12(sp)
  addi  sp, sp, 16
  ret

