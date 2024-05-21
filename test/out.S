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
  @arr_1 = alloc [i32, 1]
  %0 = getelemptr @arr_1, 0
  store 1, %0
  %1 = getelemptr @arr_1, 0
  %2 = load %1
  ret %2
}
  .data
  .globl arr
arr:
  .zero 262144
  .text
  .globl main
main:
  addi  sp, sp, -16
entry:
  addi  t0, sp, 0
  li    t2, 4
  mul   t2, t2, x0
  add   t0, t0, t2
  sw    t0, 4(sp)
  li    t0, 1
  lw    t1, 4(sp)
  sw    t0, 0(t1)
  addi  t0, sp, 0
  li    t2, 4
  mul   t2, t2, x0
  add   t0, t0, t2
  sw    t0, 8(sp)
  lw    t0, 8(sp)
  sw    t0, 12(sp)
  lw    a0, 12(sp)
  addi  sp, sp, 16
  ret

