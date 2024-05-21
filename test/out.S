  .data
  .globl a
a:
  .word {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  .globl b
b:
  .word {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  .globl c
c:
  .word 0
  .globl d
d:
  .word {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  .globl e
e:
  .word {{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}
  .globl f
f:
  .word {{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}
  .globl g
g:
  .word 0
  .globl h
h:
  .word {{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}}
  .text
  .globl main
main:
  addi  sp, sp, 0
entry:
  li    a0, 2
  addi  sp, sp, 0
  ret

