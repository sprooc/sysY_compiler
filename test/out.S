  .data
  .globl a
a:
  .word 1
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .globl b
b:
  .zero 40
  .globl c
c:
  .word 0
  .globl d
d:
  .zero 40
  .globl e
e:
  .zero 120
  .globl f
f:
  .zero 120
  .globl g
g:
  .word 0
  .globl h
h:
  .zero 120
  .text
  .globl main
main:
  addi  sp, sp, 0
entry:
  li    a0, 2
  addi  sp, sp, 0
  ret

