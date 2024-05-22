decl @getint(): i32
decl @getch(): i32
decl @getarray(*i32): i32
decl @putint(i32)
decl @putch(i32)
decl @putarray(i32, *i32)
decl @starttime()
decl @stoptime()
fun @main(): i32 {
%entry:
  @arr_0 = alloc [i32, 2]
  %0 = getelemptr @arr_0, 0
  store 1, %0
  %1 = getelemptr @arr_0, 1
  store 0, %1
  @a_1 = alloc i32
  store 1, @a_1
  ret 0
}
