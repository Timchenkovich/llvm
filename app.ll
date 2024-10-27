; ModuleID = 'app.c'
source_filename = "app.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@0 = private unnamed_addr constant [7 x i8] c"switch\00", align 1
@1 = private unnamed_addr constant [5 x i8] c"load\00", align 1
@2 = private unnamed_addr constant [6 x i8] c"store\00", align 1
@3 = private unnamed_addr constant [4 x i8] c"shl\00", align 1
@4 = private unnamed_addr constant [5 x i8] c"srem\00", align 1
@5 = private unnamed_addr constant [4 x i8] c"and\00", align 1
@6 = private unnamed_addr constant [3 x i8] c"br\00", align 1
@7 = private unnamed_addr constant [5 x i8] c"call\00", align 1
@8 = private unnamed_addr constant [5 x i8] c"icmp\00", align 1
@9 = private unnamed_addr constant [3 x i8] c"or\00", align 1
@10 = private unnamed_addr constant [4 x i8] c"sub\00", align 1
@11 = private unnamed_addr constant [7 x i8] c"select\00", align 1
@12 = private unnamed_addr constant [4 x i8] c"add\00", align 1
@13 = private unnamed_addr constant [4 x i8] c"mul\00", align 1

; Function Attrs: nounwind uwtable
define dso_local void @fill_background(i32 noundef %0) local_unnamed_addr #0 {
  br label %2

2:                                                ; preds = %1, %5
  %3 = phi i32 [ 0, %1 ], [ %6, %5 ]
  br label %8

4:                                                ; preds = %5
  ret void

5:                                                ; preds = %8
  %6 = add nuw nsw i32 %3, 1
  call void @printUse(ptr @8, i64 94659226871552, ptr @12, i64 94659226781856)
  %7 = icmp eq i32 %6, 512
  call void @printUse(ptr @6, i64 94659226776544, ptr @8, i64 94659226871552)
  br i1 %7, label %4, label %2, !llvm.loop !5

8:                                                ; preds = %2, %8
  %9 = phi i32 [ 0, %2 ], [ %10, %8 ]
  tail call void @simPutPixel(i32 noundef %3, i32 noundef %9, i32 noundef %0) #4
  %10 = add nuw nsw i32 %9, 1
  call void @printUse(ptr @8, i64 94659226812592, ptr @12, i64 94659226780752)
  %11 = icmp eq i32 %10, 256
  call void @printUse(ptr @6, i64 94659226779008, ptr @8, i64 94659226812592)
  br i1 %11, label %5, label %8, !llvm.loop !7
}

declare void @simPutPixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #1

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: readwrite) uwtable
define dso_local void @adjust_directions(i32 noundef %0, i32 noundef %1, ptr nocapture noundef %2, ptr nocapture noundef %3, i32 noundef %4, i32 noundef %5) local_unnamed_addr #2 {
  %7 = shl nsw i32 %4, 1
  call void @printUse(ptr @12, i64 94659226794416, ptr @3, i64 94659226778720)
  %8 = add i32 %7, %5
  call void @printUse(ptr @12, i64 94659226794688, ptr @12, i64 94659226794416)
  %9 = add i32 %8, %0
  call void @printUse(ptr @8, i64 94659226794272, ptr @12, i64 94659226794688)
  %10 = icmp sgt i32 %9, 511
  %11 = icmp sgt i32 %5, %0
  call void @printUse(ptr @9, i64 94659226810736, ptr @8, i64 94659226798720)
  call void @printUse(ptr @9, i64 94659226810736, ptr @8, i64 94659226794272)
  %12 = or i1 %11, %10
  call void @printUse(ptr @6, i64 94659226641600, ptr @9, i64 94659226810736)
  br i1 %12, label %13, label %16

13:                                               ; preds = %6
  %14 = load i32, ptr %2, align 4, !tbaa !8
  call void @printUse(ptr @10, i64 94659226796208, ptr @1, i64 94659226796048)
  %15 = sub nsw i32 0, %14
  call void @printUse(ptr @2, i64 94659226796480, ptr @10, i64 94659226796208)
  store i32 %15, ptr %2, align 4, !tbaa !8
  br label %16

16:                                               ; preds = %6, %13
  %17 = icmp sgt i32 %5, %1
  call void @printUse(ptr @12, i64 94659226798304, ptr @12, i64 94659226794416)
  %18 = add i32 %8, %1
  call void @printUse(ptr @8, i64 94659226871696, ptr @12, i64 94659226798304)
  %19 = icmp sgt i32 %18, 255
  call void @printUse(ptr @9, i64 94659226798160, ptr @8, i64 94659226797184)
  call void @printUse(ptr @9, i64 94659226798160, ptr @8, i64 94659226871696)
  %20 = or i1 %17, %19
  call void @printUse(ptr @6, i64 94659226797760, ptr @9, i64 94659226798160)
  br i1 %20, label %21, label %24

21:                                               ; preds = %16
  %22 = load i32, ptr %3, align 4, !tbaa !8
  call void @printUse(ptr @10, i64 94659226799296, ptr @1, i64 94659226799136)
  %23 = sub nsw i32 0, %22
  call void @printUse(ptr @2, i64 94659226799568, ptr @10, i64 94659226799296)
  store i32 %23, ptr %3, align 4, !tbaa !8
  br label %24

24:                                               ; preds = %16, %21
  ret void
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: readwrite) uwtable
define dso_local void @adjust_positions(ptr nocapture noundef %0, ptr nocapture noundef %1, ptr nocapture noundef %2, ptr nocapture noundef %3, i32 noundef %4, i32 noundef %5) local_unnamed_addr #2 {
  %7 = load i32, ptr %0, align 4, !tbaa !8
  %8 = load i32, ptr %1, align 4, !tbaa !8
  %9 = shl nsw i32 %4, 1
  call void @printUse(ptr @12, i64 94659226797472, ptr @3, i64 94659226798576)
  %10 = add i32 %9, %5
  call void @printUse(ptr @12, i64 94659227127440, ptr @1, i64 94659226810320)
  call void @printUse(ptr @12, i64 94659227127440, ptr @12, i64 94659226797472)
  %11 = add i32 %7, %10
  call void @printUse(ptr @8, i64 94659227127584, ptr @12, i64 94659227127440)
  %12 = icmp sgt i32 %11, 511
  call void @printUse(ptr @8, i64 94659227264112, ptr @1, i64 94659226810320)
  %13 = icmp slt i32 %7, %5
  call void @printUse(ptr @9, i64 94659227264256, ptr @8, i64 94659227264112)
  call void @printUse(ptr @9, i64 94659227264256, ptr @8, i64 94659227127584)
  %14 = or i1 %13, %12
  call void @printUse(ptr @6, i64 94659226795808, ptr @9, i64 94659227264256)
  br i1 %14, label %15, label %18

15:                                               ; preds = %6
  %16 = load i32, ptr %2, align 4, !tbaa !8
  call void @printUse(ptr @10, i64 94659227211504, ptr @1, i64 94659226852416)
  %17 = sub nsw i32 0, %16
  call void @printUse(ptr @2, i64 94659227211648, ptr @10, i64 94659227211504)
  store i32 %17, ptr %2, align 4, !tbaa !8
  br label %18

18:                                               ; preds = %15, %6
  call void @printUse(ptr @8, i64 94659227264496, ptr @1, i64 94659226810576)
  %19 = icmp slt i32 %8, %5
  call void @printUse(ptr @12, i64 94659227264640, ptr @1, i64 94659226810576)
  call void @printUse(ptr @12, i64 94659227264640, ptr @12, i64 94659226797472)
  %20 = add i32 %8, %10
  call void @printUse(ptr @8, i64 94659227222832, ptr @12, i64 94659227264640)
  %21 = icmp sgt i32 %20, 255
  call void @printUse(ptr @9, i64 94659227222976, ptr @8, i64 94659227264496)
  call void @printUse(ptr @9, i64 94659227222976, ptr @8, i64 94659227222832)
  %22 = or i1 %19, %21
  call void @printUse(ptr @6, i64 94659227102640, ptr @9, i64 94659227222976)
  br i1 %22, label %23, label %26

23:                                               ; preds = %18
  %24 = load i32, ptr %3, align 4, !tbaa !8
  call void @printUse(ptr @10, i64 94659227223312, ptr @1, i64 94659226853504)
  %25 = sub nsw i32 0, %24
  call void @printUse(ptr @2, i64 94659226867632, ptr @10, i64 94659227223312)
  store i32 %25, ptr %3, align 4, !tbaa !8
  br label %26

26:                                               ; preds = %18, %23
  %27 = load i32, ptr %2, align 4, !tbaa !8
  call void @printUse(ptr @13, i64 94659226766576, ptr @1, i64 94659226766288)
  %28 = mul nsw i32 %27, %5
  %29 = load i32, ptr %0, align 4, !tbaa !8
  call void @printUse(ptr @12, i64 94659226766976, ptr @1, i64 94659226766816)
  call void @printUse(ptr @12, i64 94659226766976, ptr @13, i64 94659226766576)
  %30 = add nsw i32 %29, %28
  call void @printUse(ptr @2, i64 94659226767120, ptr @12, i64 94659226766976)
  store i32 %30, ptr %0, align 4, !tbaa !8
  %31 = load i32, ptr %3, align 4, !tbaa !8
  call void @printUse(ptr @13, i64 94659226767664, ptr @1, i64 94659226767376)
  %32 = mul nsw i32 %31, %5
  %33 = load i32, ptr %1, align 4, !tbaa !8
  call void @printUse(ptr @12, i64 94659226768064, ptr @1, i64 94659226767904)
  call void @printUse(ptr @12, i64 94659226768064, ptr @13, i64 94659226767664)
  %34 = add nsw i32 %33, %32
  call void @printUse(ptr @2, i64 94659226815872, ptr @12, i64 94659226768064)
  store i32 %34, ptr %1, align 4, !tbaa !8
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local void @change_colors(ptr nocapture noundef %0, ptr nocapture noundef %1, ptr nocapture noundef writeonly %2) local_unnamed_addr #0 {
  %4 = load i32, ptr %0, align 4, !tbaa !8
  call void @printUse(ptr @0, i64 94659226778008, ptr @1, i64 94659226808352)
  switch i32 %4, label %8 [
    i32 -1, label %5
    i32 0, label %5
  ]

5:                                                ; preds = %3, %3
  %6 = load i32, ptr %1, align 4, !tbaa !8
  call void @printUse(ptr @10, i64 94659226820416, ptr @1, i64 94659226820256)
  %7 = sub nsw i32 0, %6
  call void @printUse(ptr @2, i64 94659226820688, ptr @10, i64 94659226820416)
  store i32 %7, ptr %1, align 4, !tbaa !8
  br label %8

8:                                                ; preds = %3, %5
  %9 = tail call i32 (...) @simRand() #4
  call void @printUse(ptr @2, i64 94659226821344, ptr @7, i64 94659226821056)
  store i32 %9, ptr %2, align 4, !tbaa !8
  %10 = load i32, ptr %1, align 4, !tbaa !8
  %11 = load i32, ptr %0, align 4, !tbaa !8
  call void @printUse(ptr @12, i64 94659226822016, ptr @1, i64 94659226821856)
  call void @printUse(ptr @12, i64 94659226822016, ptr @1, i64 94659226821600)
  %12 = add nsw i32 %11, %10
  call void @printUse(ptr @2, i64 94659226822160, ptr @12, i64 94659226822016)
  store i32 %12, ptr %0, align 4, !tbaa !8
  ret void
}

declare i32 @simRand(...) local_unnamed_addr #1

; Function Attrs: nounwind uwtable
define dso_local void @reflect_dots(i32 noundef %0, i32 noundef %1, i32 noundef %2, i32 noundef %3, i32 noundef %4) local_unnamed_addr #0 {
  %6 = add nsw i32 %2, %0
  %7 = add nsw i32 %3, %1
  call void @printUse(ptr @7, i64 94659226113088, ptr @12, i64 94659226818816)
  call void @printUse(ptr @7, i64 94659226113088, ptr @12, i64 94659226819216)
  tail call void @simPutPixel(i32 noundef %6, i32 noundef %7, i32 noundef %4) #4
  %8 = sub nsw i32 %0, %2
  call void @printUse(ptr @7, i64 94659226112864, ptr @10, i64 94659226830240)
  call void @printUse(ptr @7, i64 94659226112864, ptr @12, i64 94659226819216)
  tail call void @simPutPixel(i32 noundef %8, i32 noundef %7, i32 noundef %4) #4
  %9 = add nsw i32 %3, %0
  %10 = add nsw i32 %2, %1
  call void @printUse(ptr @7, i64 94659226112608, ptr @12, i64 94659226811264)
  call void @printUse(ptr @7, i64 94659226112608, ptr @12, i64 94659226811664)
  tail call void @simPutPixel(i32 noundef %9, i32 noundef %10, i32 noundef %4) #4
  %11 = sub nsw i32 %0, %3
  call void @printUse(ptr @7, i64 94659225635872, ptr @10, i64 94659226812192)
  call void @printUse(ptr @7, i64 94659225635872, ptr @12, i64 94659226811664)
  tail call void @simPutPixel(i32 noundef %11, i32 noundef %10, i32 noundef %4) #4
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local void @draw_half_circle(i32 noundef %0, i32 noundef %1, i32 noundef %2, i32 noundef %3, i32 noundef %4) local_unnamed_addr #0 {
  %6 = mul nsw i32 %3, %2
  call void @printUse(ptr @12, i64 94659227245296, ptr @13, i64 94659226847424)
  %7 = add nsw i32 %6, %1
  call void @printUse(ptr @7, i64 94659227200320, ptr @12, i64 94659227245296)
  tail call void @simPutPixel(i32 noundef %0, i32 noundef %7, i32 noundef %4) #4
  call void @printUse(ptr @7, i64 94659227141440, ptr @12, i64 94659227245296)
  tail call void @simPutPixel(i32 noundef %0, i32 noundef %7, i32 noundef %4) #4
  call void @printUse(ptr @12, i64 94659227141600, ptr @13, i64 94659226847424)
  %8 = add nsw i32 %6, %0
  call void @printUse(ptr @7, i64 94659227141952, ptr @12, i64 94659227141600)
  tail call void @simPutPixel(i32 noundef %8, i32 noundef %1, i32 noundef %4) #4
  call void @printUse(ptr @10, i64 94659227233616, ptr @13, i64 94659226847424)
  %9 = sub nsw i32 %0, %6
  call void @printUse(ptr @7, i64 94659227233824, ptr @10, i64 94659227233616)
  tail call void @simPutPixel(i32 noundef %9, i32 noundef %1, i32 noundef %4) #4
  %10 = icmp slt i32 %2, 0
  call void @printUse(ptr @6, i64 94659227145072, ptr @8, i64 94659226851696)
  br i1 %10, label %42, label %11

11:                                               ; preds = %5
  %12 = shl nuw nsw i32 %2, 1
  call void @printUse(ptr @10, i64 94659226846208, ptr @3, i64 94659226808512)
  %13 = sub nsw i32 3, %12
  br label %14

14:                                               ; preds = %11, %29
  %15 = phi i32 [ %31, %29 ], [ %13, %11 ]
  %16 = phi i32 [ %30, %29 ], [ %2, %11 ]
  %17 = phi i32 [ %32, %29 ], [ 0, %11 ]
  %18 = icmp sgt i32 %15, 0
  call void @printUse(ptr @6, i64 94659226849696, ptr @8, i64 94659226849520)
  br i1 %18, label %19, label %25

19:                                               ; preds = %14
  %20 = add nsw i32 %16, -1
  call void @printUse(ptr @10, i64 94659226850656, ptr @12, i64 94659226849968)
  %21 = sub nsw i32 %17, %20
  call void @printUse(ptr @3, i64 94659227280304, ptr @10, i64 94659226850656)
  %22 = shl nsw i32 %21, 2
  %23 = add nuw i32 %15, 10
  call void @printUse(ptr @12, i64 94659226851136, ptr @12, i64 94659226850944)
  call void @printUse(ptr @12, i64 94659226851136, ptr @3, i64 94659227280304)
  %24 = add i32 %23, %22
  br label %29

25:                                               ; preds = %14
  %26 = shl nsw i32 %17, 2
  call void @printUse(ptr @12, i64 94659226851840, ptr @3, i64 94659227260544)
  %27 = add nuw i32 %26, 6
  call void @printUse(ptr @12, i64 94659226852032, ptr @12, i64 94659226851840)
  %28 = add i32 %27, %15
  br label %29

29:                                               ; preds = %25, %19
  %30 = phi i32 [ %20, %19 ], [ %16, %25 ]
  %31 = phi i32 [ %24, %19 ], [ %28, %25 ]
  %32 = add nuw nsw i32 %17, 1
  call void @printUse(ptr @13, i64 94659226853392, ptr @12, i64 94659226852576)
  %33 = mul nsw i32 %32, %3
  %34 = mul nsw i32 %30, %3
  call void @printUse(ptr @12, i64 94659227141744, ptr @13, i64 94659226853392)
  %35 = add nsw i32 %33, %0
  call void @printUse(ptr @12, i64 94659227141232, ptr @13, i64 94659226853792)
  %36 = add nsw i32 %34, %1
  call void @printUse(ptr @7, i64 94659227234496, ptr @12, i64 94659227141744)
  call void @printUse(ptr @7, i64 94659227234496, ptr @12, i64 94659227141232)
  tail call void @simPutPixel(i32 noundef %35, i32 noundef %36, i32 noundef %4) #4
  call void @printUse(ptr @10, i64 94659226776256, ptr @13, i64 94659226853392)
  %37 = sub nsw i32 %0, %33
  call void @printUse(ptr @7, i64 94659226847760, ptr @10, i64 94659226776256)
  call void @printUse(ptr @7, i64 94659226847760, ptr @12, i64 94659227141232)
  tail call void @simPutPixel(i32 noundef %37, i32 noundef %36, i32 noundef %4) #4
  call void @printUse(ptr @12, i64 94659227243120, ptr @13, i64 94659226853792)
  %38 = add nsw i32 %34, %0
  call void @printUse(ptr @12, i64 94659227243264, ptr @13, i64 94659226853392)
  %39 = add nsw i32 %33, %1
  call void @printUse(ptr @7, i64 94659227243472, ptr @12, i64 94659227243120)
  call void @printUse(ptr @7, i64 94659227243472, ptr @12, i64 94659227243264)
  tail call void @simPutPixel(i32 noundef %38, i32 noundef %39, i32 noundef %4) #4
  call void @printUse(ptr @10, i64 94659227243632, ptr @13, i64 94659226853792)
  %40 = sub nsw i32 %0, %34
  call void @printUse(ptr @7, i64 94659227243840, ptr @10, i64 94659227243632)
  call void @printUse(ptr @7, i64 94659227243840, ptr @12, i64 94659227243264)
  tail call void @simPutPixel(i32 noundef %40, i32 noundef %39, i32 noundef %4) #4
  %41 = icmp sgt i32 %30, %17
  call void @printUse(ptr @6, i64 94659226848960, ptr @8, i64 94659226846064)
  br i1 %41, label %14, label %42, !llvm.loop !12

42:                                               ; preds = %29, %5
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local void @draw_circle(i32 noundef %0, i32 noundef %1, i32 noundef %2, i32 noundef %3) local_unnamed_addr #0 {
  tail call void @draw_half_circle(i32 noundef %0, i32 noundef %1, i32 noundef %2, i32 noundef 1, i32 noundef %3)
  tail call void @draw_half_circle(i32 noundef %0, i32 noundef %1, i32 noundef %2, i32 noundef -1, i32 noundef %3)
  ret void
}

; Function Attrs: noreturn nounwind uwtable
define dso_local void @app() local_unnamed_addr #3 {
  %1 = tail call i32 (...) @simRand() #4
  call void @printUse(ptr @4, i64 94659226868768, ptr @7, i64 94659226868560)
  %2 = srem i32 %1, 449
  %3 = tail call i32 (...) @simRand() #4
  call void @printUse(ptr @4, i64 94659226869616, ptr @7, i64 94659226869360)
  %4 = srem i32 %3, 193
  %5 = tail call i32 (...) @simRand() #4
  call void @printUse(ptr @5, i64 94659226850800, ptr @7, i64 94659226870208)
  %6 = and i32 %5, 1
  call void @printUse(ptr @8, i64 94659226795344, ptr @5, i64 94659226850800)
  %7 = icmp eq i32 %6, 0
  call void @printUse(ptr @11, i64 94659226870800, ptr @8, i64 94659226795344)
  %8 = select i1 %7, i32 -1, i32 1
  %9 = tail call i32 (...) @simRand() #4
  call void @printUse(ptr @5, i64 94659226870368, ptr @7, i64 94659226871392)
  %10 = and i32 %9, 1
  call void @printUse(ptr @8, i64 94659226870512, ptr @5, i64 94659226870368)
  %11 = icmp eq i32 %10, 0
  call void @printUse(ptr @11, i64 94659226871984, ptr @8, i64 94659226870512)
  %12 = select i1 %11, i32 -1, i32 1
  %13 = tail call i32 (...) @simRand() #4
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
  call void @printUse(ptr @8, i64 94659227194368, ptr @12, i64 94659227194224)
  %26 = icmp eq i32 %25, 512
  call void @printUse(ptr @6, i64 94659227124272, ptr @8, i64 94659227194368)
  br i1 %26, label %31, label %22, !llvm.loop !5

27:                                               ; preds = %27, %22
  %28 = phi i32 [ 0, %22 ], [ %29, %27 ]
  tail call void @simPutPixel(i32 noundef %23, i32 noundef %28, i32 noundef %20) #4
  %29 = add nuw nsw i32 %28, 1
  call void @printUse(ptr @8, i64 94659227260144, ptr @12, i64 94659226848448)
  %30 = icmp eq i32 %29, 256
  call void @printUse(ptr @6, i64 94659227102816, ptr @8, i64 94659227260144)
  br i1 %30, label %24, label %27, !llvm.loop !7

31:                                               ; preds = %24
  %32 = add nsw i32 %15, 30
  %33 = add nsw i32 %16, 30
  call void @printUse(ptr @7, i64 94659226812032, ptr @12, i64 94659226877120)
  call void @printUse(ptr @7, i64 94659226812032, ptr @12, i64 94659226877392)
  tail call void @draw_half_circle(i32 noundef %32, i32 noundef %33, i32 noundef 30, i32 noundef 1, i32 noundef %19)
  call void @printUse(ptr @7, i64 94659227129312, ptr @12, i64 94659226877120)
  call void @printUse(ptr @7, i64 94659227129312, ptr @12, i64 94659226877392)
  tail call void @draw_half_circle(i32 noundef %32, i32 noundef %33, i32 noundef 30, i32 noundef -1, i32 noundef %19)
  %34 = add nsw i32 %15, 20
  %35 = add nsw i32 %16, 20
  call void @printUse(ptr @7, i64 94659226811104, ptr @12, i64 94659226878096)
  call void @printUse(ptr @7, i64 94659226811104, ptr @12, i64 94659226878368)
  tail call void @draw_half_circle(i32 noundef %34, i32 noundef %35, i32 noundef 6, i32 noundef 1, i32 noundef %19)
  call void @printUse(ptr @7, i64 94659226851504, ptr @12, i64 94659226878096)
  call void @printUse(ptr @7, i64 94659226851504, ptr @12, i64 94659226878368)
  tail call void @draw_half_circle(i32 noundef %34, i32 noundef %35, i32 noundef 6, i32 noundef -1, i32 noundef %19)
  %36 = add nsw i32 %15, 40
  call void @printUse(ptr @7, i64 94659226828480, ptr @12, i64 94659226879120)
  call void @printUse(ptr @7, i64 94659226828480, ptr @12, i64 94659226878368)
  tail call void @draw_half_circle(i32 noundef %36, i32 noundef %35, i32 noundef 6, i32 noundef 1, i32 noundef %19)
  call void @printUse(ptr @7, i64 94659226777632, ptr @12, i64 94659226879120)
  call void @printUse(ptr @7, i64 94659226777632, ptr @12, i64 94659226878368)
  tail call void @draw_half_circle(i32 noundef %36, i32 noundef %35, i32 noundef 6, i32 noundef -1, i32 noundef %19)
  %37 = add nsw i32 %16, 33
  call void @printUse(ptr @7, i64 94659226880912, ptr @12, i64 94659226877120)
  call void @printUse(ptr @7, i64 94659226880912, ptr @12, i64 94659226880464)
  tail call void @draw_half_circle(i32 noundef %32, i32 noundef %37, i32 noundef 15, i32 noundef 1, i32 noundef %19)
  %38 = add i32 %15, 63
  call void @printUse(ptr @8, i64 94659227305776, ptr @12, i64 94659227305632)
  %39 = icmp sgt i32 %38, 511
  %40 = icmp slt i32 %15, 3
  call void @printUse(ptr @9, i64 94659227298976, ptr @8, i64 94659227298832)
  call void @printUse(ptr @9, i64 94659227298976, ptr @8, i64 94659227305776)
  %41 = or i1 %40, %39
  %42 = sub nsw i32 0, %17
  call void @printUse(ptr @11, i64 94659227311984, ptr @9, i64 94659227298976)
  call void @printUse(ptr @11, i64 94659227311984, ptr @10, i64 94659227316016)
  %43 = select i1 %41, i32 %42, i32 %17
  %44 = icmp slt i32 %16, 3
  %45 = add i32 %16, 63
  call void @printUse(ptr @8, i64 94659227299504, ptr @12, i64 94659227299360)
  %46 = icmp sgt i32 %45, 255
  call void @printUse(ptr @9, i64 94659227299648, ptr @8, i64 94659227299216)
  call void @printUse(ptr @9, i64 94659227299648, ptr @8, i64 94659227299504)
  %47 = or i1 %44, %46
  %48 = sub nsw i32 0, %18
  call void @printUse(ptr @11, i64 94659227145248, ptr @9, i64 94659227299648)
  call void @printUse(ptr @11, i64 94659227145248, ptr @10, i64 94659227315536)
  %49 = select i1 %47, i32 %48, i32 %18
  call void @printUse(ptr @13, i64 94659227299888, ptr @11, i64 94659227311984)
  %50 = mul nsw i32 %43, 3
  call void @printUse(ptr @12, i64 94659227300032, ptr @13, i64 94659227299888)
  %51 = add nsw i32 %50, %15
  call void @printUse(ptr @13, i64 94659227300304, ptr @11, i64 94659227145248)
  %52 = mul nsw i32 %49, 3
  call void @printUse(ptr @12, i64 94659227300576, ptr @13, i64 94659227300304)
  %53 = add nsw i32 %52, %16
  switch i32 %20, label %56 [
    i32 -1, label %54
    i32 0, label %54
  ]

54:                                               ; preds = %31, %31
  %55 = sub nsw i32 0, %21
  br label %56

56:                                               ; preds = %31, %54
  %57 = phi i32 [ %21, %31 ], [ %55, %54 ]
  %58 = tail call i32 (...) @simRand() #4
  %59 = add nsw i32 %57, %20
  tail call void (...) @simFlush() #4
  br label %14
}

declare void @simFlush(...) local_unnamed_addr #1

declare void @printUse(ptr, i64, ptr, i64)

attributes #0 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: readwrite) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { noreturn nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"Ubuntu clang version 19.1.2 (++20241016063442+7ba7d8e2f7b6-1~exp1~20241016063607.53)"}
!5 = distinct !{!5, !6}
!6 = !{!"llvm.loop.mustprogress"}
!7 = distinct !{!7, !6}
!8 = !{!9, !9, i64 0}
!9 = !{!"int", !10, i64 0}
!10 = !{!"omnipotent char", !11, i64 0}
!11 = !{!"Simple C/C++ TBAA"}
!12 = distinct !{!12, !6}
