declare i32 @getint()

declare i32 @getch()

declare i32 @getarray(i32*)

declare void @putint(i32)

declare void @putch(i32)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define i32 @main() {
$entry:
  %arr_0 = alloca [2 x i32]
  %$0 = getelementptr inbounds [2 x i32], [2 x i32]* %arr_0, i32 0, i32 0
  store i32 1, i32* %$0
  %$1 = getelementptr inbounds [2 x i32], [2 x i32]* %arr_0, i32 0, i32 1
  store i32 0, i32* %$1
  %a_1 = alloca i32
  store i32 1, i32* %a_1
  ret i32 0
}
