; ModuleID = 'test4.fplicm.bc'
source_filename = "test4.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

$__llvm_profile_raw_version = comdat any

$__profc_main = comdat nodeduplicate

@.str = private unnamed_addr constant [7 x i8] c"input4\00", align 1
@.str.1 = private unnamed_addr constant [2 x i8] c"r\00", align 1
@.str.2 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@__llvm_profile_raw_version = constant i64 72057594037927944, comdat
@__profc_main = private global [1 x i64] zeroinitializer, section "__llvm_prf_cnts", comdat, align 8
@__profd_main = private global { i64, i64, i64, ptr, ptr, i32, [2 x i16] } { i64 -2624081020897602054, i64 742261418966908927, i64 sub (i64 ptrtoint (ptr @__profc_main to i64), i64 ptrtoint (ptr @__profd_main to i64)), ptr @main, ptr null, i32 1, [2 x i16] zeroinitializer }, section "__llvm_prf_data", comdat($__profc_main), align 8
@__llvm_prf_nm = private constant [14 x i8] c"\04\0Cx\DA\CBM\CC\CC\03\00\04\1B\01\A6", section "__llvm_prf_names", align 1
@llvm.compiler.used = appending global [1 x ptr] [ptr @__profd_main], section "llvm.metadata"
@llvm.used = appending global [1 x ptr] [ptr @__llvm_prf_nm], section "llvm.metadata"
@0 = private unnamed_addr constant [11 x i8] c"Detected!\0A\00", align 1

; Function Attrs: inlinehint noinline nounwind optnone uwtable
define dso_local i32 @main() #0 !prof !35 {
entry:
  %pgocount = load i64, ptr @__profc_main, align 8
  %0 = add i64 %pgocount, 1
  store i64 %0, ptr @__profc_main, align 8
  %retval = alloca i32, align 4
  %input = alloca i32, align 4
  %pFile = alloca ptr, align 8
  %mul10 = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  %call = call noalias ptr @fopen(ptr noundef @.str, ptr noundef @.str.1)
  store ptr %call, ptr %pFile, align 8
  %1 = load ptr, ptr %pFile, align 8
  %call1 = call i32 (ptr, ptr, ...) @__isoc99_fscanf(ptr noundef %1, ptr noundef @.str.2, ptr noundef %input)
  %2 = load i32, ptr %input, align 4
  %call2 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, i32 noundef %2)
  %3 = load i32, ptr %input, align 4
  %mul = mul i32 %3, 10
  %4 = icmp sgt i32 %3, 0
  %5 = icmp sgt i32 %mul, 0
  %6 = icmp slt i32 %mul, 0
  %7 = icmp eq i1 %4, true
  %8 = icmp ne i1 %4, true
  %9 = and i1 %7, %6
  %10 = and i1 %8, %5
  %11 = or i1 %9, %10
  br i1 %11, label %tempBB, label %12

12:                                               ; preds = %entry
  store i32 %mul, ptr %mul10, align 4
  %13 = load i32, ptr %mul10, align 4
  %call3 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, i32 noundef %13)
  %14 = load ptr, ptr %pFile, align 8
  %call4 = call i32 @fclose(ptr noundef %14)
  ret i32 0

tempBB:                                           ; preds = %entry
  ret i32 43
}

declare noalias ptr @fopen(ptr noundef, ptr noundef) #1

declare i32 @__isoc99_fscanf(ptr noundef, ptr noundef, ...) #1

declare i32 @printf(ptr noundef, ...) #1

declare i32 @fclose(ptr noundef) #1

; Function Attrs: nounwind
declare void @llvm.instrprof.increment(ptr, i64, i32, i32) #2

attributes #0 = { inlinehint noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5}
!llvm.ident = !{!34}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{i32 1, !"ProfileSummary", !6}
!6 = !{!7, !8, !9, !10, !11, !12, !13, !14, !15, !16}
!7 = !{!"ProfileFormat", !"InstrProf"}
!8 = !{!"TotalCount", i64 1}
!9 = !{!"MaxCount", i64 1}
!10 = !{!"MaxInternalCount", i64 0}
!11 = !{!"MaxFunctionCount", i64 1}
!12 = !{!"NumCounts", i64 1}
!13 = !{!"NumFunctions", i64 1}
!14 = !{!"IsPartialProfile", i64 0}
!15 = !{!"PartialProfileRatio", double 0.000000e+00}
!16 = !{!"DetailedSummary", !17}
!17 = !{!18, !19, !20, !21, !22, !23, !24, !25, !26, !27, !28, !29, !30, !31, !32, !33}
!18 = !{i32 10000, i64 0, i32 0}
!19 = !{i32 100000, i64 0, i32 0}
!20 = !{i32 200000, i64 0, i32 0}
!21 = !{i32 300000, i64 0, i32 0}
!22 = !{i32 400000, i64 0, i32 0}
!23 = !{i32 500000, i64 0, i32 0}
!24 = !{i32 600000, i64 0, i32 0}
!25 = !{i32 700000, i64 0, i32 0}
!26 = !{i32 800000, i64 0, i32 0}
!27 = !{i32 900000, i64 0, i32 0}
!28 = !{i32 950000, i64 0, i32 0}
!29 = !{i32 990000, i64 0, i32 0}
!30 = !{i32 999000, i64 0, i32 0}
!31 = !{i32 999900, i64 0, i32 0}
!32 = !{i32 999990, i64 0, i32 0}
!33 = !{i32 999999, i64 0, i32 0}
!34 = !{!"clang version 15.0.6"}
!35 = !{!"function_entry_count", i64 1}
