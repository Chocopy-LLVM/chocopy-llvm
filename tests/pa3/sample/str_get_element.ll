; ModuleID = 'ChocoPy code'
source_filename = "/Users/yiweiyang/project/bak/chocopy_test/pa3/sample/str_get_element.py"

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
  i32 5,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 3,
  i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.const_9, i32 0, i32 0) 
}
@str.const_9 = private unnamed_addr global [4 x i8] c"abc\00", align 1

@const_10 = global %$str$prototype_type {
  i32 3,
  i32 5,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 0,
  i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.const_10, i32 0, i32 0) 
}
@str.const_10 = private unnamed_addr global [1 x i8] c"\00", align 1

@const_11 = global %$str$prototype_type {
  i32 3,
  i32 5,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 0,
  i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.const_11, i32 0, i32 0) 
}
@str.const_11 = private unnamed_addr global [1 x i8] c"\00", align 1

@const_12 = global %$str$prototype_type {
  i32 3,
  i32 5,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 0,
  i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.const_12, i32 0, i32 0) 
}
@str.const_12 = private unnamed_addr global [1 x i8] c"\00", align 1

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
  %op2 = load %$str$prototype_type, %$str$prototype_type* @const_10
  %op3 = call %$str$prototype_type* @$str_get(%$str$prototype_type* @const_9, i32 0)
  %op4 = load %$str$prototype_type, %$str$prototype_type* @const_11
  %op5 = call %$str$prototype_type* @$str_get(%$str$prototype_type* @const_9, i32 1)
  %op6 = load %$str$prototype_type, %$str$prototype_type* @const_12
  %op7 = call %$str$prototype_type* @$str_get(%$str$prototype_type* @const_9, i32 2)
  %op8 = bitcast %$str$prototype_type* %op3 to %$union.put*
  call void @print(%$union.put* %op8)
  %op9 = bitcast %$str$prototype_type* %op5 to %$union.put*
  call void @print(%$union.put* %op9)
  %op10 = bitcast %$str$prototype_type* %op7 to %$union.put*
  call void @print(%$union.put* %op10)
  tail call void asm sideeffect "li a7, 93 #exit system call\0Aecall", ""()
  ret void
}
define %$str$prototype_type* @$str_get(%$str$prototype_type* %arg0, i32 %arg1) {

label2:
  %op3 = bitcast %$str$prototype_type* %arg0 to %$str$prototype_type*
  %op4 = getelementptr %$str$prototype_type, %$str$prototype_type* %op3, i32 0, i32 3
  %op5 = load i32, i32* %op4
  %op6 = icmp sge i32 %arg1, %op5
  %op7 = icmp slt i32 %arg1, 0
  %op8 = or i1 %op6, %op7
  br  i1 %op8, label %label9, label %label10

label9:                                                ; preds = %label2
  call void @error.OOB()
  br label %label10

label10:                                                ; preds = %label2, %label9
  %op11 = getelementptr %$str$prototype_type, %$str$prototype_type* %op3, i32 0, i32 4
  %op12 = load i8*, i8** %op11
  %op13 = getelementptr i8, i8* %op12, i32 %arg1
  %op14 = load i8, i8* %op13
  %op15 = call %$str$prototype_type* @initchars(i8 %op14)
  ret %$str$prototype_type* %op15
}
