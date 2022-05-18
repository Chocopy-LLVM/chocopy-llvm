
%class.anon = type { i32* }

@.str = private unnamed_addr constant [6 x i8] c"Main\0A\00", align 1
@.str.1 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.2 = private unnamed_addr constant [6 x i8] c"GEEKS\00", align 1
@.str.3 = private unnamed_addr constant [6 x i8] c"View\0A\00", align 1

define dso_local noundef i32 @main() #0{
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca %class.anon, align 4
  store i32 0, i32* %1, align 4
  store i32 0, i32* %2, align 4
  %4 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([6 x i8], [6 x i8]* @.str, i32 0, i32 0))
  %5 = getelementptr inbounds %class.anon, %class.anon* %3, i32 0, i32 0
  store i32* %2, i32** %5, align 4
  %6 = call noundef i32 @"_ZZ4mainENK3$_0clEv"(%class.anon* noundef nonnull align 4 dereferenceable(4) %3)
  %7 = load i32, i32* %2, align 4
  %8 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i32 0, i32 0), i32 noundef %7)
  %9 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([6 x i8], [6 x i8]* @.str.2, i32 0, i32 0))
  ret i32 0
}

declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare dso_local i32 @printf(i8* noundef, ...) #2

define internal noundef i32 @"_ZZ4mainENK3$_0clEv"(%class.anon* noundef nonnull align 4 dereferenceable(4) %0)  {
  %2 = alloca %class.anon*, align 4
  store %class.anon* %0, %class.anon** %2, align 4
  %3 = load %class.anon*, %class.anon** %2, align 4
  %4 = getelementptr inbounds %class.anon, %class.anon* %3, i32 0, i32 0
  %5 = load i32*, i32** %4, align 4
  %6 = load i32, i32* %5, align 4
  %7 = add nsw i32 %6, 1
  store i32 %7, i32* %5, align 4
  %8 = call i32 (i8*, ...) @printf(i8* noundef getelementptr inbounds ([6 x i8], [6 x i8]* @.str.3, i32 0, i32 0))
  %9 = getelementptr inbounds %class.anon, %class.anon* %3, i32 0, i32 0
  %10 = load i32*, i32** %9, align 4
  %11 = load i32, i32* %10, align 4
  ret i32 %11
}

attributes #0 = { mustprogress noinline norecurse optnone "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+a,+c,+d,+f,+m,+relax,-save-restore" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+a,+c,+d,+f,+m,+relax,-save-restore" }
attributes #3 = { mustprogress noinline optnone "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+a,+c,+d,+f,+m,+relax,-save-restore" }

