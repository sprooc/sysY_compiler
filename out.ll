@gb_0 = global [2 x [3 x i32]] [[3 x i32] [i32 5, i32 6, i32 7], [3 x i32] [i32 8, i32 0, i32 0]]

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
  %j_4 = alloca i32
  %b_1 = alloca [2 x [3 x i32]]
  %$0 = getelementptr inbounds [2 x [3 x i32]], [2 x [3 x i32]]* %b_1, i32 0, i32 0
  %$1 = getelementptr inbounds [3 x i32], [3 x i32]* %$0, i32 0, i32 0
  store i32 1, i32* %$1
  %$2 = getelementptr inbounds [2 x [3 x i32]], [2 x [3 x i32]]* %b_1, i32 0, i32 0
  %$3 = getelementptr inbounds [3 x i32], [3 x i32]* %$2, i32 0, i32 1
  store i32 2, i32* %$3
  %$4 = getelementptr inbounds [2 x [3 x i32]], [2 x [3 x i32]]* %b_1, i32 0, i32 0
  %$5 = getelementptr inbounds [3 x i32], [3 x i32]* %$4, i32 0, i32 2
  store i32 3, i32* %$5
  %$6 = getelementptr inbounds [2 x [3 x i32]], [2 x [3 x i32]]* %b_1, i32 0, i32 1
  %$7 = getelementptr inbounds [3 x i32], [3 x i32]* %$6, i32 0, i32 0
  store i32 4, i32* %$7
  %$8 = getelementptr inbounds [2 x [3 x i32]], [2 x [3 x i32]]* %b_1, i32 0, i32 1
  %$9 = getelementptr inbounds [3 x i32], [3 x i32]* %$8, i32 0, i32 1
  store i32 0, i32* %$9
  %$10 = getelementptr inbounds [2 x [3 x i32]], [2 x [3 x i32]]* %b_1, i32 0, i32 1
  %$11 = getelementptr inbounds [3 x i32], [3 x i32]* %$10, i32 0, i32 2
  store i32 0, i32* %$11
  %sum_2 = alloca i32
  store i32 0, i32* %sum_2
  %i_3 = alloca i32
  store i32 0, i32* %i_3
  br label %$L1

$L1:
  %$12 = load i32, i32* %i_3
  %$13 = icmp slt i32 %$12, 2
  %$14 = zext i1 %$13 to i32
  %$15 = icmp ne i32 %$14, 0
  br i1 %$15, label %$L2, label %$L0

$L2:
  store i32 0, i32* %j_4
  br label %$L4

$L0:
  %$16 = load i32, i32* %sum_2
  ret i32 %$16

$L4:
  %$17 = load i32, i32* %j_4
  %$18 = icmp slt i32 %$17, 3
  %$19 = zext i1 %$18 to i32
  %$20 = icmp ne i32 %$19, 0
  br i1 %$20, label %$L5, label %$L3

$L5:
  %$21 = load i32, i32* %sum_2
  %$22 = load i32, i32* %i_3
  %$23 = getelementptr inbounds [2 x [3 x i32]], [2 x [3 x i32]]* %b_1, i32 0, i32 %$22
  %$24 = load i32, i32* %j_4
  %$25 = getelementptr inbounds [3 x i32], [3 x i32]* %$23, i32 0, i32 %$24
  %$26 = load i32, i32* %$25
  %$27 = add i32 %$21, %$26
  %$28 = load i32, i32* %i_3
  %$29 = getelementptr inbounds [2 x [3 x i32]], [2 x [3 x i32]]* @gb_0, i32 0, i32 %$28
  %$30 = load i32, i32* %j_4
  %$31 = getelementptr inbounds [3 x i32], [3 x i32]* %$29, i32 0, i32 %$30
  %$32 = load i32, i32* %$31
  %$33 = add i32 %$27, %$32
  store i32 %$33, i32* %sum_2
  %$34 = load i32, i32* %j_4
  %$35 = add i32 %$34, 1
  store i32 %$35, i32* %j_4
  br label %$L4

$L3:
  %$36 = load i32, i32* %i_3
  %$37 = add i32 %$36, 1
  store i32 %$37, i32* %i_3
  br label %$L1
}
