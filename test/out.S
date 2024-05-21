decl @getint(): i32
decl @getch(): i32
decl @getarray(*i32): i32
decl @putint(i32)
decl @putch(i32)
decl @putarray(i32, *i32)
decl @starttime()
decl @stoptime()
global @arr_0 = alloc [i32, 65536], zeroinit
fun @main(): i32 {
%entry:
  @arr_1 = alloc [i32, 4]
  %0 = getelemptr @arr_1, 0
  store 1, %0
  %1 = getelemptr @arr_1, 1
  store 0, %1
  %2 = getelemptr @arr_1, 2
  store 0, %2
  %3 = getelemptr @arr_1, 3
  store 0, %3
  %4 = getelemptr @arr_1, 0
  %5 = load %4
  ret %5
}
  .data
  .globl arr
arr:
  .zero 262144
  .text
  .globl main
main:
  addi  sp, sp, -48
entry:
  addi  t0, sp, 0
  li    t2, 4
  mul   t2, t2, x0
  add   t0, t0, t2
  sw    t0, 16(sp)
  li    t0, 1
  sw    t0, 16(sp)
  addi  t0, sp, 0
  li    t1, 1
  li    t2, 4
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 20(sp)
  sw    x0, 20(sp)
  addi  t0, sp, 0
  li    t1, 2
  li    t2, 4
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 24(sp)
  sw    x0, 24(sp)
  addi  t0, sp, 0
  li    t1, 3
  li    t2, 4
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 28(sp)
  sw    x0, 28(sp)
  addi  t0, sp, 0
  li    t2, 4
  mul   t2, t2, x0
  add   t0, t0, t2
  sw    t0, 32(sp)
  lw    t0, 32(sp)
  sw    t0, 36(sp)
  lw    a0, 36(sp)
  addi  sp, sp, 48
  ret

