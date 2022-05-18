; ModuleID = 'ChocoPy code'
source_filename = "/Users/yiweiyang/project/bak/chocopy_test/pa3/sample/object_method_none.py"

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

%$A$prototype_type  = type  {
  i32,
  i32,
  %$A$dispatchTable_type*,
  i32 
}
@$A$prototype  = global %$A$prototype_type{
  i32 7,
  i32 4,
  %$A$dispatchTable_type* @$A$dispatchTable,
  i32 42
}
%$A$dispatchTable_type = type {
  void(%$object$prototype_type*)*,
  i32(%$A$prototype_type*, %$object$dispatchTable_type*)*
}
@$A$dispatchTable = global %$A$dispatchTable_type {
  void(%$object$prototype_type*)* @$object.__init__,
  i32(%$A$prototype_type*, %$object$dispatchTable_type*)* @$A.foo
}

%$B$prototype_type  = type  {
  i32,
  i32,
  %$B$dispatchTable_type*,
  i32 ,
  i1 
}
@$B$prototype  = global %$B$prototype_type{
  i32 8,
  i32 5,
  %$B$dispatchTable_type* @$B$dispatchTable,
  i32 42,
  i1 1
}
%$B$dispatchTable_type = type {
  void(%$B$prototype_type*)*,
  i32(%$A$prototype_type*, %$object$dispatchTable_type*)*,
  i32(%$B$prototype_type*)*
}
@$B$dispatchTable = global %$B$dispatchTable_type {
  void(%$B$prototype_type*)* @$B.__init__,
  i32(%$A$prototype_type*, %$object$dispatchTable_type*)* @$A.foo,
  i32(%$B$prototype_type*)* @$B.bar
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
  i32 1,
  i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.const_9, i32 0, i32 0) 
}
@str.const_9 = private unnamed_addr global [2 x i8] c"B\00", align 1

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
  %op2 = bitcast %$B$prototype_type* @$B$prototype to %$object$prototype_type*
  %op3 = call %$object$prototype_type* @alloc(%$object$prototype_type* %op2)
  %op4 = bitcast %$object$prototype_type* %op3 to %$B$prototype_type*
  %op5 = getelementptr %$B$prototype_type, %$B$prototype_type* %op4, i32 0, i32 2
  %op6 = load %$B$dispatchTable_type*, %$B$dispatchTable_type** %op5
  %op7 = getelementptr %$B$dispatchTable_type, %$B$dispatchTable_type* %op6, i32 0, i32 0
  %op8 = load void (%$B$prototype_type*)*, void (%$B$prototype_type*)** %op7
  call void %op8(%$B$prototype_type* %op4)
  %op9 = load %$B$prototype_type, %$B$prototype_type* %op4
  %op10 = bitcast %$B$prototype_type* @$B$prototype to %$object$prototype_type*
  %op11 = call %$object$prototype_type* @alloc(%$object$prototype_type* %op10)
  %op12 = bitcast %$object$prototype_type* %op11 to %$B$prototype_type*
  %op13 = getelementptr %$B$prototype_type, %$B$prototype_type* %op12, i32 0, i32 2
  %op14 = load %$B$dispatchTable_type*, %$B$dispatchTable_type** %op13
  %op15 = getelementptr %$B$dispatchTable_type, %$B$dispatchTable_type* %op14, i32 0, i32 2
  %op16 = load i32 (%$B$prototype_type*)*, i32 (%$B$prototype_type*)** %op15
  %op17 = call i32 %op16(%$B$prototype_type* %op12)
  tail call void asm sideeffect "li a7, 93 #exit system call\0Aecall", ""()
  ret void
}
define i32 @$A.foo(%$A$prototype_type* %arg0, %$object$dispatchTable_type* %arg1) {

label2:
  %op3 = load %$object$dispatchTable_type, %$object$dispatchTable_type* %arg1
  %op4 = getelementptr %$A$prototype_type, %$A$prototype_type* %arg0, i32 0, i32 3
  %op5 = load i32, i32* %op4
  ret i32 %op5
}
define void @$B.__init__(%$B$prototype_type* %arg0) {

label1:
  %op2 = bitcast %$str$prototype_type* @const_9 to %$union.put*
  call void @print(%$union.put* %op2)
  ret void
}
define i32 @$B.bar(%$B$prototype_type* %arg0) {

label1:
  %op2 = bitcast %$A$prototype_type* @$A$prototype to %$A$prototype_type*
  call void @error.None()
  ret i32 0
}
