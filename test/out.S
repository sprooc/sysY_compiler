decl @getint(): i32
decl @getch(): i32
decl @getarray(*i32): i32
decl @putint(i32)
decl @putch(i32)
decl @putarray(i32, *i32)
decl @starttime()
decl @stoptime()
global @a_0 = alloc i32, 10
fun @inc(): i32 {
%L0:
  %0 = load @a_0
  %1 = add %0, 1
  store %1, @a_0
  %2 = load @a_0
  ret %2
}
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
