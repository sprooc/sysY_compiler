decl @getint(): i32
decl @getch(): i32
decl @getarray(*i32): i32
decl @putint(i32)
decl @putch(i32)
decl @putarray(i32, *i32)
decl @starttime()
decl @stoptime()
global @a_0 = alloc [i32, 10], zeroinit
global @b_1 = alloc [i32, 10], zeroinit
global @c_2 = alloc i32, 0
global @d_3 = alloc [i32, 10], zeroinit
global @e_4 = alloc [[i32, 6], 5], zeroinit
global @f_5 = alloc [[i32, 6], 5], zeroinit
global @g_6 = alloc i32, 0
global @h_7 = alloc [[i32, 6], 5], zeroinit
fun @main(): i32 {
%entry:
  ret 2
}
  .data
  .globl a
a:
  .word zeroinit
  .globl b
b:
  .word zeroinit
  .globl c
c:
  .word 0
  .globl d
d:
  .word zeroinit
  .globl e
e:
  .word zeroinit
  .globl f
f:
  .word zeroinit
  .globl g
g:
  .word 0
  .globl h
h:
  .word zeroinit
  .text
  .globl main
main:
  addi  sp, sp, 0
entry:
  li    a0, 2
  addi  sp, sp, 0
  ret

