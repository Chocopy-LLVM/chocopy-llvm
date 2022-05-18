; ModuleID = 'ChocoPy code'
source_filename = "/Users/yiweiyang/project/bak/chocopy_test/pa3/sample/str_cat.py"

%$union.type = type { i32 }

%$union.len = type { i32 }

%$union.put = type { i32 }

%$union.conslist = type { i32 }

%$object$prototype_type  = type  {
  i32,
  i32,
  %$object$dispatchTable_type*
}
@$object$prototype  = global %$object$prototype_type{
  i32 0,
  i32 3,
  %$object$dispatchTable_type* @$object$dispatchTable
}
%$object$dispatchTable_type = type {
  void(%$object$prototype_type*)*
}
@$object$dispatchTable = global %$object$dispatchTable_type {
  void(%$object$prototype_type*)* @$object.__init__
}

%$int$prototype_type  = type  {
  i32,
  i32,
  %$int$dispatchTable_type*,
  i32 
}
@$int$prototype  = global %$int$prototype_type{
  i32 1,
  i32 4,
  %$int$dispatchTable_type* @$int$dispatchTable,
  i32 0
}
%$int$dispatchTable_type = type {
  void(%$object$prototype_type*)*
}
@$int$dispatchTable = global %$int$dispatchTable_type {
  void(%$object$prototype_type*)* @$object.__init__
}

%$bool$prototype_type  = type  {
  i32,
  i32,
  %$bool$dispatchTable_type*,
  i1 
}
@$bool$prototype  = global %$bool$prototype_type{
  i32 2,
  i32 4,
  %$bool$dispatchTable_type* @$bool$dispatchTable,
  i1 0
}
%$bool$dispatchTable_type = type {
  void(%$object$prototype_type*)*
}
@$bool$dispatchTable = global %$bool$dispatchTable_type {
  void(%$object$prototype_type*)* @$object.__init__
}

%$str$prototype_type  = type  {
  i32,
  i32,
  %$str$dispatchTable_type*,
  i32 ,
  i8* 
}
@$str$prototype  = global %$str$prototype_type{
  i32 3,
  i32 5,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 0,
  i8* inttoptr (i32 0 to i8*)
}
%$str$dispatchTable_type = type {
  void(%$object$prototype_type*)*
}
@$str$dispatchTable = global %$str$dispatchTable_type {
  void(%$object$prototype_type*)* @$object.__init__
}

%$.list$prototype_type  = type  {
  i32,
  i32,
  %$union.type ,
  i32 ,
  %$union.conslist* 
}
@$.list$prototype  = global %$.list$prototype_type{
  i32 -1,
  i32 5,
  %$union.type {i32 0 },
  i32 0,
  %$union.conslist* inttoptr (i32 0 to %$union.conslist*)
}

@const_0 = external global %$bool$prototype_type
@const_1 = external global %$bool$prototype_type
@const_2 = external global %$str$prototype_type
@const_3 = external global %$str$prototype_type
@const_4 = external global %$str$prototype_type
@const_5 = external global %$str$prototype_type
@const_6 = external global %$str$prototype_type
@const_7 = external global %$str$prototype_type
@const_9 = global %$str$prototype_type {
  i32 3,
  i32 6,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 5,
  i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.const_9, i32 0, i32 0) 
}
@str.const_9 = private unnamed_addr global [6 x i8] c"Hello\00", align 1

@const_10 = global %$str$prototype_type {
  i32 3,
  i32 6,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 5,
  i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.const_10, i32 0, i32 0) 
}
@str.const_10 = private unnamed_addr global [6 x i8] c"World\00", align 1

@const_11 = global %$str$prototype_type {
  i32 3,
  i32 6,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 7,
  i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.const_11, i32 0, i32 0) 
}
@str.const_11 = private unnamed_addr global [8 x i8] c"ChocoPy\00", align 1

@const_12 = global %$str$prototype_type {
  i32 3,
  i32 5,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 0,
  i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.const_12, i32 0, i32 0) 
}
@str.const_12 = private unnamed_addr global [1 x i8] c"\00", align 1

@const_13 = global %$str$prototype_type {
  i32 3,
  i32 5,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 1,
  i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.const_13, i32 0, i32 0) 
}
@str.const_13 = private unnamed_addr global [2 x i8] c" \00", align 1

@const_14 = global %$str$prototype_type {
  i32 3,
  i32 5,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 0,
  i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.const_14, i32 0, i32 0) 
}
@str.const_14 = private unnamed_addr global [1 x i8] c"\00", align 1

@const_15 = global %$str$prototype_type {
  i32 3,
  i32 5,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 0,
  i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.const_15, i32 0, i32 0) 
}
@str.const_15 = private unnamed_addr global [1 x i8] c"\00", align 1

declare void @$object.__init__(%$object$prototype_type*)
declare void @heap.init()
declare %$str$prototype_type* @initchars(i8)
declare %$int$prototype_type* @noconv()
declare %$.list$prototype_type* @nonlist()
declare void @error.OOB()
declare void @error.None()
declare void @error.Div()
declare %$.list$prototype_type* @concat(%$.list$prototype_type*, %$.list$prototype_type*)
declare %$.list$prototype_type* @conslist(i32, %$union.conslist, ...)
declare i32 @$len(%$union.len*)
declare void @print(%$union.put*)
declare %$bool$prototype_type* @makebool(i1)
declare %$int$prototype_type* @makeint(i32)
declare %$str$prototype_type* @makestr(%$str$prototype_type*)
declare %$str$prototype_type* @$input()
declare %$object$prototype_type* @alloc(%$object$prototype_type*)
declare i1 @streql(%$str$prototype_type*, %$str$prototype_type*)
declare i1 @strneql(%$str$prototype_type*, %$str$prototype_type*)
declare %$str$prototype_type* @strcat(%$str$prototype_type*, %$str$prototype_type*)
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @before_main, i8* null }]
define void @before_main() {

label0:
  tail call void asm sideeffect "lui a0, 8192\0A	add s11, zero, a0", ""()
  call void @heap.init()
  tail call void asm sideeffect "mv s10, gp\0A	add s11, s11, s10\0A	mv fp, zero\0A	lw ra, 12(sp)\0A	addi sp, sp, 16\0A	ret", ""()

unreachable
}
define void @main() {

label0:
  tail call void asm sideeffect "addi fp, sp, 0", ""()
  %op2 = call %$str$prototype_type* @$cat2(%$str$prototype_type* @const_9, %$str$prototype_type* @const_10)
  %op3 = bitcast %$str$prototype_type* %op2 to %$union.put*
  call void @print(%$union.put* %op3)
  %op4 = call %$str$prototype_type* @$cat2(%$str$prototype_type* @const_12, %$str$prototype_type* @const_11)
  %op5 = bitcast %$str$prototype_type* %op4 to %$union.put*
  call void @print(%$union.put* %op5)
  %op6 = call %$str$prototype_type* @$cat3(%$str$prototype_type* @const_9, %$str$prototype_type* @const_13, %$str$prototype_type* @const_11)
  %op7 = bitcast %$str$prototype_type* %op6 to %$union.put*
  call void @print(%$union.put* %op7)
  %op8 = bitcast %$str$prototype_type* @const_9 to %$union.len*
  %op9 = call i32 @$len(%$union.len* %op8)
  %op10 = call %$int$prototype_type* @makeint(i32 %op9)
  %op11 = bitcast %$int$prototype_type* %op10 to %$union.put*
  call void @print(%$union.put* %op11)
  %op12 = call %$str$prototype_type* @$cat2(%$str$prototype_type* @const_9, %$str$prototype_type* @const_9)
  %op13 = bitcast %$str$prototype_type* %op12 to %$union.len*
  %op14 = call i32 @$len(%$union.len* %op13)
  %op15 = call %$int$prototype_type* @makeint(i32 %op14)
  %op16 = bitcast %$int$prototype_type* %op15 to %$union.put*
  call void @print(%$union.put* %op16)
  %op17 = call %$str$prototype_type* @$cat2(%$str$prototype_type* @const_14, %$str$prototype_type* @const_15)
  %op18 = bitcast %$str$prototype_type* %op17 to %$union.len*
  %op19 = call i32 @$len(%$union.len* %op18)
  %op20 = call %$int$prototype_type* @makeint(i32 %op19)
  %op21 = bitcast %$int$prototype_type* %op20 to %$union.put*
  call void @print(%$union.put* %op21)
  tail call void asm sideeffect "li a7, 93 #exit system call\0Aecall", ""()
  ret void
}
define %$str$prototype_type* @$cat2(%$str$prototype_type* %arg0, %$str$prototype_type* %arg1) {

label2:
  %op3 = call %$str$prototype_type* @strcat(%$str$prototype_type* %arg0, %$str$prototype_type* %arg1)
  ret %$str$prototype_type* %op3
}
define %$str$prototype_type* @$cat3(%$str$prototype_type* %arg0, %$str$prototype_type* %arg1, %$str$prototype_type* %arg2) {

label3:
  %op4 = call %$str$prototype_type* @strcat(%$str$prototype_type* %arg0, %$str$prototype_type* %arg1)
  %op5 = call %$str$prototype_type* @strcat(%$str$prototype_type* %op4, %$str$prototype_type* %arg2)
  ret %$str$prototype_type* %op5
}
