; ModuleID = 'test4.ls.bc'
source_filename = "test4.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [7 x i8] c"input4\00", align 1
@.str.1 = private unnamed_addr constant [2 x i8] c"r\00", align 1
@.str.2 = private unnamed_addr constant [3 x i8] c"%d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %input = alloca i32, align 4
  %pFile = alloca ptr, align 8
  %mul10 = alloca i32, align 4
  store i32 0, ptr %retval, align 4
  %call = call noalias ptr @fopen(ptr noundef @.str, ptr noundef @.str.1)
  store ptr %call, ptr %pFile, align 8
  %0 = load ptr, ptr %pFile, align 8
  %call1 = call i32 (ptr, ptr, ...) @__isoc99_fscanf(ptr noundef %0, ptr noundef @.str.2, ptr noundef %input)
  %1 = load i32, ptr %input, align 4
  %call2 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, i32 noundef %1)
  %2 = load i32, ptr %input, align 4
  %mul = mul i32 %2, 10
  store i32 %mul, ptr %mul10, align 4
  %3 = load i32, ptr %mul10, align 4
  %call3 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, i32 noundef %3)
  %4 = load ptr, ptr %pFile, align 8
  %call4 = call i32 @fclose(ptr noundef %4)
  ret i32 0
}

declare noalias ptr @fopen(ptr noundef, ptr noundef) #1

declare i32 @__isoc99_fscanf(ptr noundef, ptr noundef, ...) #1

declare i32 @printf(ptr noundef, ...) #1

declare i32 @fclose(ptr noundef) #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 15.0.6"}
