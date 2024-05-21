  .data
  .global x
x:
  .word 0
  .global init
init:
  .word 1
  .text
  .global main
main:
  addi  sp, sp, -16
  sw    ra, 12(sp)
entry:
  la    t0, x
  sw    t0, 0(sp)
  lw    a0, 0(sp)
  call  putint
  li    a0, 32
  call  putch
  li    a0, 10
  call  putint
  li    a0, 32
  call  putch
  li    a0, 11
  call  putint
  li    a0, 32
  call  putch
  la    t0, init
  sw    t0, 4(sp)
  lw    a0, 4(sp)
  call  putint
  li    a0, 10
  call  putch
  lw    ra, 12(sp)
  addi  sp, sp, 16
  ret

