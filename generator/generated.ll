; ModuleID = 'executor.cpp'
source_filename = "executor.cpp"

declare void @simPutPixel(i32, i32, i32)

declare void @simFlush()

declare i32 @simRand()

define internal void @fill_background(i32 %0) {
  br label %2

2:                                                ; preds = %5, %1
  %3 = phi i32 [ 0, %1 ], [ %6, %5 ]
  br label %8

4:                                                ; preds = %5
  ret void

5:                                                ; preds = %8
  %6 = add nuw nsw i32 %3, 1
  %7 = icmp eq i32 %6, 512
  br i1 %7, label %4, label %2

8:                                                ; preds = %8, %2
  %9 = phi i32 [ 0, %2 ], [ %10, %8 ]
  tail call void @simPutPixel(i32 %3, i32 %9, i32 %0)
  %10 = add nuw nsw i32 %9, 1
  %11 = icmp eq i32 %10, 256
  br i1 %11, label %5, label %8
}

define internal void @adjust_directions(i32 %0, i32 %1, ptr %2, ptr %3, i32 %4, i32 %5) {
  %7 = shl nsw i32 %4, 1
  %8 = add i32 %7, %5
  %9 = add i32 %8, %0
  %10 = icmp sgt i32 %9, 511
  %11 = icmp sgt i32 %5, %0
  %12 = or i1 %11, %10
  br i1 %12, label %13, label %16

13:                                               ; preds = %6
  %14 = load i32, ptr %2, align 4
  %15 = sub nsw i32 0, %14
  store i32 %15, ptr %2, align 4
  br label %16

16:                                               ; preds = %13, %6
  %17 = icmp sgt i32 %5, %1
  %18 = add i32 %8, %1
  %19 = icmp sgt i32 %18, 255
  %20 = or i1 %17, %19
  br i1 %20, label %21, label %24

21:                                               ; preds = %16
  %22 = load i32, ptr %3, align 4
  %23 = sub nsw i32 0, %22
  store i32 %23, ptr %3, align 4
  br label %24

24:                                               ; preds = %21, %16
  ret void
}

define internal void @adjust_positions(ptr %0, ptr %1, ptr %2, ptr %3, i32 %4, i32 %5) {
  %7 = load i32, ptr %0, align 4
  %8 = load i32, ptr %1, align 4
  %9 = shl nsw i32 %4, 1
  %10 = add i32 %9, %5
  %11 = add i32 %7, %10
  %12 = icmp sgt i32 %11, 511
  %13 = icmp slt i32 %7, %5
  %14 = or i1 %13, %12
  br i1 %14, label %15, label %18

15:                                               ; preds = %6
  %16 = load i32, ptr %2, align 4
  %17 = sub nsw i32 0, %16
  store i32 %17, ptr %2, align 4
  br label %18

18:                                               ; preds = %15, %6
  %19 = icmp slt i32 %8, %5
  %20 = add i32 %8, %10
  %21 = icmp sgt i32 %20, 255
  %22 = or i1 %19, %21
  br i1 %22, label %23, label %26

23:                                               ; preds = %18
  %24 = load i32, ptr %3, align 4
  %25 = sub nsw i32 0, %24
  store i32 %25, ptr %3, align 4
  br label %26

26:                                               ; preds = %23, %18
  %27 = load i32, ptr %2, align 4
  %28 = mul nsw i32 %27, %5
  %29 = load i32, ptr %0, align 4
  %30 = add nsw i32 %29, %28
  store i32 %30, ptr %0, align 4
  %31 = load i32, ptr %3, align 4
  %32 = mul nsw i32 %31, %5
  %33 = load i32, ptr %1, align 4
  %34 = add nsw i32 %33, %32
  store i32 %34, ptr %1, align 4
  ret void
}

define internal void @change_colors(ptr %0, ptr %1, ptr %2) {
  %4 = load i32, ptr %0, align 4
  switch i32 %4, label %8 [
    i32 -1, label %5
    i32 0, label %5
  ]

5:                                                ; preds = %3, %3
  %6 = load i32, ptr %1, align 4
  %7 = sub nsw i32 0, %6
  store i32 %7, ptr %1, align 4
  br label %8

8:                                                ; preds = %5, %3
  %9 = tail call i32 @simRand()
  store i32 %9, ptr %2, align 4
  %10 = load i32, ptr %1, align 4
  %11 = load i32, ptr %0, align 4
  %12 = add nsw i32 %11, %10
  store i32 %12, ptr %0, align 4
  ret void
}

define internal void @reflect_dots(i32 %0, i32 %1, i32 %2, i32 %3, i32 %4) {
  %6 = add nsw i32 %2, %0
  %7 = add nsw i32 %3, %1
  tail call void @simPutPixel(i32 %6, i32 %7, i32 %4)
  %8 = sub nsw i32 %0, %2
  tail call void @simPutPixel(i32 %8, i32 %7, i32 %4)
  %9 = add nsw i32 %3, %0
  %10 = add nsw i32 %2, %1
  tail call void @simPutPixel(i32 %9, i32 %10, i32 %4)
  %11 = sub nsw i32 %0, %3
  tail call void @simPutPixel(i32 %11, i32 %10, i32 %4)
  ret void
}

define internal void @draw_half_circle(i32 %0, i32 %1, i32 %2, i32 %3, i32 %4) {
  %6 = mul nsw i32 %3, %2
  %7 = add nsw i32 %6, %1
  tail call void @simPutPixel(i32 %0, i32 %7, i32 %4)
  tail call void @simPutPixel(i32 %0, i32 %7, i32 %4)
  %8 = add nsw i32 %6, %0
  tail call void @simPutPixel(i32 %8, i32 %1, i32 %4)
  %9 = sub nsw i32 %0, %6
  tail call void @simPutPixel(i32 %9, i32 %1, i32 %4)
  %10 = icmp slt i32 %2, 0
  br i1 %10, label %42, label %11

11:                                               ; preds = %5
  %12 = shl nuw nsw i32 %2, 1
  %13 = sub nsw i32 3, %12
  br label %14

14:                                               ; preds = %29, %11
  %15 = phi i32 [ %31, %29 ], [ %13, %11 ]
  %16 = phi i32 [ %30, %29 ], [ %2, %11 ]
  %17 = phi i32 [ %32, %29 ], [ 0, %11 ]
  %18 = icmp sgt i32 %15, 0
  br i1 %18, label %19, label %25

19:                                               ; preds = %14
  %20 = add nsw i32 %16, -1
  %21 = sub nsw i32 %17, %20
  %22 = shl nsw i32 %21, 2
  %23 = add nuw i32 %15, 10
  %24 = add i32 %23, %22
  br label %29

25:                                               ; preds = %14
  %26 = shl nsw i32 %17, 2
  %27 = add nuw i32 %26, 6
  %28 = add i32 %27, %15
  br label %29

29:                                               ; preds = %25, %19
  %30 = phi i32 [ %20, %19 ], [ %16, %25 ]
  %31 = phi i32 [ %24, %19 ], [ %28, %25 ]
  %32 = add nuw nsw i32 %17, 1
  %33 = mul nsw i32 %32, %3
  %34 = mul nsw i32 %30, %3
  %35 = add nsw i32 %33, %0
  %36 = add nsw i32 %34, %1
  tail call void @simPutPixel(i32 %35, i32 %36, i32 %4)
  %37 = sub nsw i32 %0, %33
  tail call void @simPutPixel(i32 %37, i32 %36, i32 %4)
  %38 = add nsw i32 %34, %0
  %39 = add nsw i32 %33, %1
  tail call void @simPutPixel(i32 %38, i32 %39, i32 %4)
  %40 = sub nsw i32 %0, %34
  tail call void @simPutPixel(i32 %40, i32 %39, i32 %4)
  %41 = icmp sgt i32 %30, %17
  br i1 %41, label %14, label %42

42:                                               ; preds = %29, %5
  ret void
}

define internal void @draw_circle(i32 %0, i32 %1, i32 %2, i32 %3) {
  tail call void @draw_half_circle(i32 %0, i32 %1, i32 %2, i32 1, i32 %3)
  tail call void @draw_half_circle(i32 %0, i32 %1, i32 %2, i32 -1, i32 %3)
  ret void
}

define void @app() {
  %1 = tail call i32 @simRand()
  %2 = srem i32 %1, 449
  %3 = tail call i32 @simRand()
  %4 = srem i32 %3, 193
  %5 = tail call i32 @simRand()
  %6 = and i32 %5, 1
  %7 = icmp eq i32 %6, 0
  %8 = select i1 %7, i32 -1, i32 1
  %9 = tail call i32 @simRand()
  %10 = and i32 %9, 1
  %11 = icmp eq i32 %10, 0
  %12 = select i1 %11, i32 -1, i32 1
  %13 = tail call i32 @simRand()
  br label %14

14:                                               ; preds = %56, %0
  %15 = phi i32 [ %2, %0 ], [ %51, %56 ]
  %16 = phi i32 [ %4, %0 ], [ %53, %56 ]
  %17 = phi i32 [ %8, %0 ], [ %43, %56 ]
  %18 = phi i32 [ %12, %0 ], [ %49, %56 ]
  %19 = phi i32 [ %13, %0 ], [ %58, %56 ]
  %20 = phi i32 [ 0, %0 ], [ %59, %56 ]
  %21 = phi i32 [ -16843009, %0 ], [ %57, %56 ]
  br label %22

22:                                               ; preds = %24, %14
  %23 = phi i32 [ 0, %14 ], [ %25, %24 ]
  br label %27

24:                                               ; preds = %27
  %25 = add nuw nsw i32 %23, 1
  %26 = icmp eq i32 %25, 512
  br i1 %26, label %31, label %22

27:                                               ; preds = %27, %22
  %28 = phi i32 [ 0, %22 ], [ %29, %27 ]
  tail call void @simPutPixel(i32 %23, i32 %28, i32 %20)
  %29 = add nuw nsw i32 %28, 1
  %30 = icmp eq i32 %29, 256
  br i1 %30, label %24, label %27

31:                                               ; preds = %24
  %32 = add nsw i32 %15, 30
  %33 = add nsw i32 %16, 30
  tail call void @draw_half_circle(i32 %32, i32 %33, i32 30, i32 1, i32 %19)
  tail call void @draw_half_circle(i32 %32, i32 %33, i32 30, i32 -1, i32 %19)
  %34 = add nsw i32 %15, 20
  %35 = add nsw i32 %16, 20
  tail call void @draw_half_circle(i32 %34, i32 %35, i32 6, i32 1, i32 %19)
  tail call void @draw_half_circle(i32 %34, i32 %35, i32 6, i32 -1, i32 %19)
  %36 = add nsw i32 %15, 40
  tail call void @draw_half_circle(i32 %36, i32 %35, i32 6, i32 1, i32 %19)
  tail call void @draw_half_circle(i32 %36, i32 %35, i32 6, i32 -1, i32 %19)
  %37 = add nsw i32 %16, 33
  tail call void @draw_half_circle(i32 %32, i32 %37, i32 15, i32 1, i32 %19)
  %38 = add i32 %15, 63
  %39 = icmp sgt i32 %38, 511
  %40 = icmp slt i32 %15, 3
  %41 = or i1 %40, %39
  %42 = sub nsw i32 0, %17
  %43 = select i1 %41, i32 %42, i32 %17
  %44 = icmp slt i32 %16, 3
  %45 = add i32 %16, 63
  %46 = icmp sgt i32 %45, 255
  %47 = or i1 %44, %46
  %48 = sub nsw i32 0, %18
  %49 = select i1 %47, i32 %48, i32 %18
  %50 = mul nsw i32 %43, 3
  %51 = add nsw i32 %50, %15
  %52 = mul nsw i32 %49, 3
  %53 = add nsw i32 %52, %16
  switch i32 %20, label %56 [
    i32 -1, label %54
    i32 0, label %54
  ]

54:                                               ; preds = %31, %31
  %55 = sub nsw i32 0, %21
  br label %56

56:                                               ; preds = %54, %31
  %57 = phi i32 [ %21, %31 ], [ %55, %54 ]
  %58 = tail call i32 @simRand()
  %59 = add nsw i32 %57, %20
  tail call void @simFlush()
  br label %14
}
