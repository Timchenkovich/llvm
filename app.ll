; ModuleID = 'app.c'
source_filename = "app.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-macosx14.0.0"

@RADIUS = local_unnamed_addr constant i32 30, align 4
@STEP = local_unnamed_addr constant i32 3, align 4
@CUR_X = common local_unnamed_addr global i32 0, align 4
@CUR_Y = common local_unnamed_addr global i32 0, align 4
@DIR_X = common local_unnamed_addr global i32 0, align 4
@DIR_Y = common local_unnamed_addr global i32 0, align 4
@COLOR = common local_unnamed_addr global i32 0, align 4
@BACKGROUND_COLOR = common local_unnamed_addr global i32 0, align 4
@BC_DELTA = common local_unnamed_addr global i32 0, align 4

; Function Attrs: nounwind ssp uwtable(sync)
define void @init() local_unnamed_addr #0 {
  %1 = tail call i32 @simRand() #4
  %2 = srem i32 %1, 449
  store i32 %2, ptr @CUR_X, align 4, !tbaa !6
  %3 = tail call i32 @simRand() #4
  %4 = srem i32 %3, 193
  store i32 %4, ptr @CUR_Y, align 4, !tbaa !6
  %5 = tail call i32 @simRand() #4
  %6 = and i32 %5, 1
  %7 = icmp eq i32 %6, 0
  %8 = select i1 %7, i32 -1, i32 1
  store i32 %8, ptr @DIR_X, align 4, !tbaa !6
  %9 = tail call i32 @simRand() #4
  %10 = and i32 %9, 1
  %11 = icmp eq i32 %10, 0
  %12 = select i1 %11, i32 -1, i32 1
  store i32 %12, ptr @DIR_Y, align 4, !tbaa !6
  %13 = tail call i32 @simRand() #4
  store i32 %13, ptr @COLOR, align 4, !tbaa !6
  store i32 0, ptr @BACKGROUND_COLOR, align 4, !tbaa !6
  store i32 -16843009, ptr @BC_DELTA, align 4, !tbaa !6
  ret void
}

declare i32 @simRand(...) local_unnamed_addr #1

; Function Attrs: nounwind ssp uwtable(sync)
define void @fill_background() local_unnamed_addr #0 {
  br label %1

1:                                                ; preds = %0, %4
  %2 = phi i32 [ 0, %0 ], [ %5, %4 ]
  br label %7

3:                                                ; preds = %4
  ret void

4:                                                ; preds = %7
  %5 = add nuw nsw i32 %2, 1
  %6 = icmp eq i32 %5, 512
  br i1 %6, label %3, label %1, !llvm.loop !10

7:                                                ; preds = %1, %7
  %8 = phi i32 [ 0, %1 ], [ %10, %7 ]
  %9 = load i32, ptr @BACKGROUND_COLOR, align 4, !tbaa !6
  tail call void @simPutPixel(i32 noundef %2, i32 noundef %8, i32 noundef %9) #4
  %10 = add nuw nsw i32 %8, 1
  %11 = icmp eq i32 %10, 256
  br i1 %11, label %4, label %7, !llvm.loop !12
}

declare void @simPutPixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #1

; Function Attrs: mustprogress nofree norecurse nosync nounwind ssp willreturn uwtable(sync)
define void @adjust_directions() local_unnamed_addr #2 {
  %1 = load i32, ptr @CUR_X, align 4, !tbaa !6
  %2 = add i32 %1, -449
  %3 = icmp ult i32 %2, -446
  br i1 %3, label %4, label %7

4:                                                ; preds = %0
  %5 = load i32, ptr @DIR_X, align 4, !tbaa !6
  %6 = sub nsw i32 0, %5
  store i32 %6, ptr @DIR_X, align 4, !tbaa !6
  br label %7

7:                                                ; preds = %0, %4
  %8 = load i32, ptr @CUR_Y, align 4, !tbaa !6
  %9 = add i32 %8, -193
  %10 = icmp ult i32 %9, -190
  br i1 %10, label %11, label %14

11:                                               ; preds = %7
  %12 = load i32, ptr @DIR_Y, align 4, !tbaa !6
  %13 = sub nsw i32 0, %12
  store i32 %13, ptr @DIR_Y, align 4, !tbaa !6
  br label %14

14:                                               ; preds = %7, %11
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind ssp willreturn uwtable(sync)
define void @adjust_positions() local_unnamed_addr #2 {
  %1 = load i32, ptr @CUR_X, align 4, !tbaa !6
  %2 = add i32 %1, -449
  %3 = icmp ult i32 %2, -446
  br i1 %3, label %4, label %7

4:                                                ; preds = %0
  %5 = load i32, ptr @DIR_X, align 4, !tbaa !6
  %6 = sub nsw i32 0, %5
  store i32 %6, ptr @DIR_X, align 4, !tbaa !6
  br label %7

7:                                                ; preds = %4, %0
  %8 = load i32, ptr @CUR_Y, align 4, !tbaa !6
  %9 = add i32 %8, -193
  %10 = icmp ult i32 %9, -190
  %11 = load i32, ptr @DIR_Y, align 4, !tbaa !6
  br i1 %10, label %12, label %14

12:                                               ; preds = %7
  %13 = sub nsw i32 0, %11
  store i32 %13, ptr @DIR_Y, align 4, !tbaa !6
  br label %14

14:                                               ; preds = %7, %12
  %15 = phi i32 [ %13, %12 ], [ %11, %7 ]
  %16 = load i32, ptr @DIR_X, align 4, !tbaa !6
  %17 = mul nsw i32 %16, 3
  %18 = add nsw i32 %17, %1
  store i32 %18, ptr @CUR_X, align 4, !tbaa !6
  %19 = mul nsw i32 %15, 3
  %20 = add nsw i32 %19, %8
  store i32 %20, ptr @CUR_Y, align 4, !tbaa !6
  ret void
}

; Function Attrs: nounwind ssp uwtable(sync)
define void @change_colors() local_unnamed_addr #0 {
  %1 = load i32, ptr @BACKGROUND_COLOR, align 4, !tbaa !6
  %2 = add i32 %1, 1
  %3 = icmp ult i32 %2, 2
  br i1 %3, label %4, label %7

4:                                                ; preds = %0
  %5 = load i32, ptr @BC_DELTA, align 4, !tbaa !6
  %6 = sub nsw i32 0, %5
  store i32 %6, ptr @BC_DELTA, align 4, !tbaa !6
  br label %7

7:                                                ; preds = %0, %4
  %8 = tail call i32 @simRand() #4
  store i32 %8, ptr @COLOR, align 4, !tbaa !6
  %9 = load i32, ptr @BC_DELTA, align 4, !tbaa !6
  %10 = load i32, ptr @BACKGROUND_COLOR, align 4, !tbaa !6
  %11 = add nsw i32 %10, %9
  store i32 %11, ptr @BACKGROUND_COLOR, align 4, !tbaa !6
  ret void
}

; Function Attrs: nounwind ssp uwtable(sync)
define void @reflect_dots(i32 noundef %0, i32 noundef %1, i32 noundef %2, i32 noundef %3) local_unnamed_addr #0 {
  %5 = add nsw i32 %2, %0
  %6 = add nsw i32 %3, %1
  %7 = load i32, ptr @COLOR, align 4, !tbaa !6
  tail call void @simPutPixel(i32 noundef %5, i32 noundef %6, i32 noundef %7) #4
  %8 = sub nsw i32 %0, %2
  %9 = load i32, ptr @COLOR, align 4, !tbaa !6
  tail call void @simPutPixel(i32 noundef %8, i32 noundef %6, i32 noundef %9) #4
  %10 = add nsw i32 %3, %0
  %11 = add nsw i32 %2, %1
  %12 = load i32, ptr @COLOR, align 4, !tbaa !6
  tail call void @simPutPixel(i32 noundef %10, i32 noundef %11, i32 noundef %12) #4
  %13 = sub nsw i32 %0, %3
  %14 = load i32, ptr @COLOR, align 4, !tbaa !6
  tail call void @simPutPixel(i32 noundef %13, i32 noundef %11, i32 noundef %14) #4
  ret void
}

; Function Attrs: nounwind ssp uwtable(sync)
define void @draw_half_circle(i32 noundef %0, i32 noundef %1, i32 noundef %2, i32 noundef %3) local_unnamed_addr #0 {
  %5 = mul nsw i32 %3, %2
  %6 = add nsw i32 %5, %1
  %7 = load i32, ptr @COLOR, align 4, !tbaa !6
  tail call void @simPutPixel(i32 noundef %0, i32 noundef %6, i32 noundef %7) #4
  %8 = load i32, ptr @COLOR, align 4, !tbaa !6
  tail call void @simPutPixel(i32 noundef %0, i32 noundef %6, i32 noundef %8) #4
  %9 = add nsw i32 %5, %0
  %10 = load i32, ptr @COLOR, align 4, !tbaa !6
  tail call void @simPutPixel(i32 noundef %9, i32 noundef %1, i32 noundef %10) #4
  %11 = sub nsw i32 %0, %5
  %12 = load i32, ptr @COLOR, align 4, !tbaa !6
  tail call void @simPutPixel(i32 noundef %11, i32 noundef %1, i32 noundef %12) #4
  %13 = icmp slt i32 %2, 0
  br i1 %13, label %49, label %14

14:                                               ; preds = %4
  %15 = shl nuw nsw i32 %2, 1
  %16 = sub nsw i32 3, %15
  br label %17

17:                                               ; preds = %14, %32
  %18 = phi i32 [ %34, %32 ], [ %16, %14 ]
  %19 = phi i32 [ %33, %32 ], [ %2, %14 ]
  %20 = phi i32 [ %35, %32 ], [ 0, %14 ]
  %21 = icmp sgt i32 %18, 0
  br i1 %21, label %22, label %28

22:                                               ; preds = %17
  %23 = add nsw i32 %19, -1
  %24 = sub nsw i32 %20, %23
  %25 = shl nsw i32 %24, 2
  %26 = add nuw i32 %18, 10
  %27 = add i32 %26, %25
  br label %32

28:                                               ; preds = %17
  %29 = shl nsw i32 %20, 2
  %30 = add nuw i32 %29, 6
  %31 = add i32 %30, %18
  br label %32

32:                                               ; preds = %28, %22
  %33 = phi i32 [ %23, %22 ], [ %19, %28 ]
  %34 = phi i32 [ %27, %22 ], [ %31, %28 ]
  %35 = add nuw nsw i32 %20, 1
  %36 = mul nsw i32 %35, %3
  %37 = mul nsw i32 %33, %3
  %38 = add nsw i32 %36, %0
  %39 = add nsw i32 %37, %1
  %40 = load i32, ptr @COLOR, align 4, !tbaa !6
  tail call void @simPutPixel(i32 noundef %38, i32 noundef %39, i32 noundef %40) #4
  %41 = sub nsw i32 %0, %36
  %42 = load i32, ptr @COLOR, align 4, !tbaa !6
  tail call void @simPutPixel(i32 noundef %41, i32 noundef %39, i32 noundef %42) #4
  %43 = add nsw i32 %37, %0
  %44 = add nsw i32 %36, %1
  %45 = load i32, ptr @COLOR, align 4, !tbaa !6
  tail call void @simPutPixel(i32 noundef %43, i32 noundef %44, i32 noundef %45) #4
  %46 = sub nsw i32 %0, %37
  %47 = load i32, ptr @COLOR, align 4, !tbaa !6
  tail call void @simPutPixel(i32 noundef %46, i32 noundef %44, i32 noundef %47) #4
  %48 = icmp sgt i32 %33, %20
  br i1 %48, label %17, label %49, !llvm.loop !13

49:                                               ; preds = %32, %4
  ret void
}

; Function Attrs: nounwind ssp uwtable(sync)
define void @draw_circle(i32 noundef %0, i32 noundef %1, i32 noundef %2) local_unnamed_addr #0 {
  tail call void @draw_half_circle(i32 noundef %0, i32 noundef %1, i32 noundef %2, i32 noundef 1)
  tail call void @draw_half_circle(i32 noundef %0, i32 noundef %1, i32 noundef %2, i32 noundef -1)
  ret void
}

; Function Attrs: noreturn nounwind ssp uwtable(sync)
define void @app() local_unnamed_addr #3 {
  %1 = tail call i32 @simRand() #4
  %2 = srem i32 %1, 449
  store i32 %2, ptr @CUR_X, align 4, !tbaa !6
  %3 = tail call i32 @simRand() #4
  %4 = srem i32 %3, 193
  store i32 %4, ptr @CUR_Y, align 4, !tbaa !6
  %5 = tail call i32 @simRand() #4
  %6 = and i32 %5, 1
  %7 = icmp eq i32 %6, 0
  %8 = select i1 %7, i32 -1, i32 1
  store i32 %8, ptr @DIR_X, align 4, !tbaa !6
  %9 = tail call i32 @simRand() #4
  %10 = and i32 %9, 1
  %11 = icmp eq i32 %10, 0
  %12 = select i1 %11, i32 -1, i32 1
  store i32 %12, ptr @DIR_Y, align 4, !tbaa !6
  %13 = tail call i32 @simRand() #4
  store i32 %13, ptr @COLOR, align 4, !tbaa !6
  store i32 0, ptr @BACKGROUND_COLOR, align 4, !tbaa !6
  store i32 -16843009, ptr @BC_DELTA, align 4, !tbaa !6
  br label %14

14:                                               ; preds = %19, %0
  %15 = phi i32 [ 0, %0 ], [ %20, %19 ]
  br label %21

16:                                               ; preds = %21
  %17 = add nuw nsw i32 %15, 1
  %18 = icmp eq i32 %17, 512
  br i1 %18, label %26, label %19

19:                                               ; preds = %16, %69
  %20 = phi i32 [ %17, %16 ], [ 0, %69 ]
  br label %14, !llvm.loop !10

21:                                               ; preds = %21, %14
  %22 = phi i32 [ 0, %14 ], [ %24, %21 ]
  %23 = load i32, ptr @BACKGROUND_COLOR, align 4, !tbaa !6
  tail call void @simPutPixel(i32 noundef %15, i32 noundef %22, i32 noundef %23) #4
  %24 = add nuw nsw i32 %22, 1
  %25 = icmp eq i32 %24, 256
  br i1 %25, label %16, label %21, !llvm.loop !12

26:                                               ; preds = %16
  %27 = load i32, ptr @CUR_X, align 4, !tbaa !6
  %28 = add nsw i32 %27, 30
  %29 = load i32, ptr @CUR_Y, align 4, !tbaa !6
  %30 = add nsw i32 %29, 30
  tail call void @draw_half_circle(i32 noundef %28, i32 noundef %30, i32 noundef 30, i32 noundef 1)
  tail call void @draw_half_circle(i32 noundef %28, i32 noundef %30, i32 noundef 30, i32 noundef -1)
  %31 = load i32, ptr @CUR_X, align 4, !tbaa !6
  %32 = add nsw i32 %31, 20
  %33 = load i32, ptr @CUR_Y, align 4, !tbaa !6
  %34 = add nsw i32 %33, 20
  tail call void @draw_half_circle(i32 noundef %32, i32 noundef %34, i32 noundef 6, i32 noundef 1)
  tail call void @draw_half_circle(i32 noundef %32, i32 noundef %34, i32 noundef 6, i32 noundef -1)
  %35 = load i32, ptr @CUR_X, align 4, !tbaa !6
  %36 = add nsw i32 %35, 40
  %37 = load i32, ptr @CUR_Y, align 4, !tbaa !6
  %38 = add nsw i32 %37, 20
  tail call void @draw_half_circle(i32 noundef %36, i32 noundef %38, i32 noundef 6, i32 noundef 1)
  tail call void @draw_half_circle(i32 noundef %36, i32 noundef %38, i32 noundef 6, i32 noundef -1)
  %39 = load i32, ptr @CUR_X, align 4, !tbaa !6
  %40 = add nsw i32 %39, 30
  %41 = load i32, ptr @CUR_Y, align 4, !tbaa !6
  %42 = add nsw i32 %41, 33
  tail call void @draw_half_circle(i32 noundef %40, i32 noundef %42, i32 noundef 15, i32 noundef 1)
  %43 = load i32, ptr @CUR_X, align 4, !tbaa !6
  %44 = add i32 %43, -449
  %45 = icmp ult i32 %44, -446
  br i1 %45, label %46, label %49

46:                                               ; preds = %26
  %47 = load i32, ptr @DIR_X, align 4, !tbaa !6
  %48 = sub nsw i32 0, %47
  store i32 %48, ptr @DIR_X, align 4, !tbaa !6
  br label %49

49:                                               ; preds = %46, %26
  %50 = load i32, ptr @CUR_Y, align 4, !tbaa !6
  %51 = add i32 %50, -193
  %52 = icmp ult i32 %51, -190
  %53 = load i32, ptr @DIR_Y, align 4, !tbaa !6
  br i1 %52, label %54, label %56

54:                                               ; preds = %49
  %55 = sub nsw i32 0, %53
  store i32 %55, ptr @DIR_Y, align 4, !tbaa !6
  br label %56

56:                                               ; preds = %49, %54
  %57 = phi i32 [ %55, %54 ], [ %53, %49 ]
  %58 = load i32, ptr @DIR_X, align 4, !tbaa !6
  %59 = mul nsw i32 %58, 3
  %60 = add nsw i32 %59, %43
  store i32 %60, ptr @CUR_X, align 4, !tbaa !6
  %61 = mul nsw i32 %57, 3
  %62 = add nsw i32 %61, %50
  store i32 %62, ptr @CUR_Y, align 4, !tbaa !6
  %63 = load i32, ptr @BACKGROUND_COLOR, align 4, !tbaa !6
  %64 = add i32 %63, 1
  %65 = icmp ult i32 %64, 2
  br i1 %65, label %66, label %69

66:                                               ; preds = %56
  %67 = load i32, ptr @BC_DELTA, align 4, !tbaa !6
  %68 = sub nsw i32 0, %67
  store i32 %68, ptr @BC_DELTA, align 4, !tbaa !6
  br label %69

69:                                               ; preds = %56, %66
  %70 = tail call i32 @simRand() #4
  store i32 %70, ptr @COLOR, align 4, !tbaa !6
  %71 = load i32, ptr @BC_DELTA, align 4, !tbaa !6
  %72 = load i32, ptr @BACKGROUND_COLOR, align 4, !tbaa !6
  %73 = add nsw i32 %72, %71
  store i32 %73, ptr @BACKGROUND_COLOR, align 4, !tbaa !6
  tail call void @simFlush() #4
  br label %19
}

declare void @simFlush(...) local_unnamed_addr #1

attributes #0 = { nounwind ssp uwtable(sync) "frame-pointer"="non-leaf" "min-legal-vector-width"="0" "no-trapping-math"="true" "probe-stack"="__chkstk_darwin" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz" }
attributes #1 = { "frame-pointer"="non-leaf" "no-trapping-math"="true" "probe-stack"="__chkstk_darwin" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz" }
attributes #2 = { mustprogress nofree norecurse nosync nounwind ssp willreturn uwtable(sync) "frame-pointer"="non-leaf" "min-legal-vector-width"="0" "no-trapping-math"="true" "probe-stack"="__chkstk_darwin" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz" }
attributes #3 = { noreturn nounwind ssp uwtable(sync) "frame-pointer"="non-leaf" "min-legal-vector-width"="0" "no-trapping-math"="true" "probe-stack"="__chkstk_darwin" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz" }
attributes #4 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 14, i32 4]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 1}
!5 = !{!"Apple clang version 15.0.0 (clang-1500.3.9.4)"}
!6 = !{!7, !7, i64 0}
!7 = !{!"int", !8, i64 0}
!8 = !{!"omnipotent char", !9, i64 0}
!9 = !{!"Simple C/C++ TBAA"}
!10 = distinct !{!10, !11}
!11 = !{!"llvm.loop.mustprogress"}
!12 = distinct !{!12, !11}
!13 = distinct !{!13, !11}
