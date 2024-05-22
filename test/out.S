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
  @a_2 = alloc [i32, 10]
  @b_3 = alloc [[i32, 3], 2]
  %0 = getelemptr @b_3, 0
  %1 = getelemptr %0, 0
  store 1, %1
  %2 = getelemptr @b_3, 0
  %3 = getelemptr %2, 1
  store 2, %3
  %4 = getelemptr @b_3, 0
  %5 = getelemptr %4, 2
  store 3, %5
  %6 = getelemptr @b_3, 1
  %7 = getelemptr %6, 0
  store 4, %7
  %8 = getelemptr @b_3, 1
  %9 = getelemptr %8, 1
  store 0, %9
  %10 = getelemptr @b_3, 1
  %11 = getelemptr %10, 2
  store 0, %11
  @sum_4 = alloc i32
  store 0, @sum_4
  @i_5 = alloc i32
  store 0, @i_5
  jump %L1
%L1:
  %12 = load @i_5
  %13 = lt %12, 2
  br %13, %L2, %L0
%L2:
  @j_6 = alloc i32
  store 0, @j_6
  jump %L4
%L4:
  %14 = load @j_6
  %15 = lt %14, 3
  br %15, %L5, %L3
%L5:
  %16 = load @sum_4
  %17 = load @i_5
  %18 = getelemptr @b_3, %17
  %19 = load @j_6
  %20 = getelemptr %18, %19
  %21 = load %20
  %22 = add %16, %21
  %23 = load @i_5
  %24 = getelemptr @gb_1, %23
  %25 = load @j_6
  %26 = getelemptr %24, %25
  %27 = load %26
  %28 = add %22, %27
  store %28, @sum_4
  %29 = load @j_6
  %30 = add %29, 1
  store %30, @j_6
  jump %L4
%L3:
  %31 = load @i_5
  %32 = add %31, 1
  store %32, @i_5
  jump %L1
%L0:
  store 0, @i_5
  jump %L7
%L7:
  %33 = load @i_5
  %34 = lt %33, 10
  br %34, %L8, %L6
%L8:
  %35 = load @sum_4
  %36 = load @i_5
  %37 = add %35, %36
  %38 = load @i_5
  %39 = getelemptr @a_2, %38
  store %37, %39
  %40 = load @sum_4
  %41 = load @i_5
  %42 = add %40, %41
  %43 = load @i_5
  %44 = getelemptr @ga_0, %43
  store %42, %44
  %45 = load @i_5
  %46 = add %45, 1
  store %46, @i_5
  jump %L7
%L6:
  %47 = load @sum_4
  ret %47
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
  addi  sp, sp, -272
  sw    ra, 268(sp)
entry:
  addi  t0, sp, 40
  li    t2, 12
  mul   t2, t2, x0
  add   t0, t0, t2
  sw    t0, 64(sp)
  addi  t0, sp, 64
  li    t2, 4
  mul   t2, t2, x0
  add   t0, t0, t2
  sw    t0, 68(sp)
  li    t0, 1
  lw    t1, 68(sp)
  sw    t0, 0(t1)
  addi  t0, sp, 40
  li    t2, 12
  mul   t2, t2, x0
  add   t0, t0, t2
  sw    t0, 72(sp)
  addi  t0, sp, 72
  li    t1, 1
  li    t2, 4
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 76(sp)
  li    t0, 2
  lw    t1, 76(sp)
  sw    t0, 0(t1)
  addi  t0, sp, 40
  li    t2, 12
  mul   t2, t2, x0
  add   t0, t0, t2
  sw    t0, 80(sp)
  addi  t0, sp, 80
  li    t1, 2
  li    t2, 4
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 84(sp)
  li    t0, 3
  lw    t1, 84(sp)
  sw    t0, 0(t1)
  addi  t0, sp, 40
  li    t1, 1
  li    t2, 12
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 88(sp)
  addi  t0, sp, 88
  li    t2, 4
  mul   t2, t2, x0
  add   t0, t0, t2
  sw    t0, 92(sp)
  li    t0, 4
  lw    t1, 92(sp)
  sw    t0, 0(t1)
  addi  t0, sp, 40
  li    t1, 1
  li    t2, 12
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 96(sp)
  addi  t0, sp, 96
  li    t1, 1
  li    t2, 4
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 100(sp)
  lw    t1, 100(sp)
  sw    x0, 0(t1)
  addi  t0, sp, 40
  li    t1, 1
  li    t2, 12
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 104(sp)
  addi  t0, sp, 104
  li    t1, 2
  li    t2, 4
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 108(sp)
  lw    t1, 108(sp)
  sw    x0, 0(t1)
  sw    x0, 112(sp)
  sw    x0, 116(sp)
  j     L1
L1:
  lw    t0, 116(sp)
  sw    t0, 120(sp)
  lw    t0, 120(sp)
  li    t1, 2
  slt   t0, t0, t1
  sw    t0, 124(sp)
  lw    t0, 124(sp)
  bnez  t0, L2
  j     L0
L2:
  sw    x0, 128(sp)
  j     L4
L4:
  lw    t0, 128(sp)
  sw    t0, 132(sp)
  lw    t0, 132(sp)
  li    t1, 3
  slt   t0, t0, t1
  sw    t0, 136(sp)
  lw    t0, 136(sp)
  bnez  t0, L5
  j     L3
L5:
  lw    t0, 112(sp)
  sw    t0, 140(sp)
  lw    t0, 116(sp)
  sw    t0, 144(sp)
  addi  t0, sp, 40
  lw    t1, 144(sp)
  li    t2, 12
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 148(sp)
  lw    t0, 128(sp)
  sw    t0, 152(sp)
  addi  t0, sp, 148
  lw    t1, 152(sp)
  li    t2, 4
  mul   t2, t2, t1
  add   t0, t0, t2
  sw    t0, 156(sp)
  lw    t0, 156(sp)
  lw    t1, 0(t0)
  sw    t1, 160(sp)
  lw    t0, 140(sp)
  lw    t1, 160(sp)
  add   t0, t0, t1
  sw    t0, 164(sp)
  lw    t0, 116(sp)
  sw    t0, 168(sp)
