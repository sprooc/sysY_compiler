decl @getint(): i32
decl @getch(): i32
decl @getarray(*i32): i32
decl @putint(i32)
decl @putch(i32)
decl @putarray(i32, *i32)
decl @starttime()
decl @stoptime()
global @x_0 = alloc i32, 0
global @init_1 = alloc i32, 1
fun @main(): i32 {
%entry:
  %1 = load @x_0
  call @putint(%1)
  call @putch(32)
  call @putint(10)
  call @putch(32)
  call @putint(11)
  call @putch(32)
  %8 = load @init_1
  call @putint(%8)
  call @putch(10)
  ret 0
}
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

