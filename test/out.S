decl @getint(): i32
decl @getch(): i32
decl @getarray(*i32): i32
decl @putint(i32)
decl @putch(i32)
decl @putarray(i32, *i32)
decl @starttime()
decl @stoptime()
fun @f1d(@arr: *i32) {
%L0:
  @arr_0 = alloc *i32
  store @arr, @arr_0
  %0 = load @arr_0
  %1 = getptr %0, 0
  store 0, %1
  ret 
}
fun @main(): i32 {
%entry:
  ret 33
}
  .data
  .text
  .globl f1d
f1d:
  addi  sp, sp, -16
  sw    ra, 12(sp)
L0:
  addi  t1, sp, 0
  sw    a0, 0(t1)
  addi  t0, sp, 0
  lw    t1, 0(t0)
  sw    t1, 4(sp)
  addi  t0, sp, 4
