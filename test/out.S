decl @getint(): i32
decl @getch(): i32
decl @getarray(*i32): i32
decl @putint(i32)
decl @putch(i32)
decl @putarray(i32, *i32)
decl @starttime()
decl @stoptime()
global @ga_0 = alloc [i32, 10], zeroinit
global @gb_1 = alloc [[i32, 3], 2], {{5, 6, 7}, {8, 0, 0}}
fun @main(): i32 {
%entry:
  %0 = getelemptr @ga_0, 0
  store 0, %0
  ret 0
}
  .data
  .globl ga
ga:
  .zero 40
  .globl gb
gb:
  .word 5
  .word 6
  .word 7
  .word 8
  .word 0
  .word 0
  .text
  .globl main
main:
  addi  sp, sp, -16
  sw    ra, 12(sp)
entry:
