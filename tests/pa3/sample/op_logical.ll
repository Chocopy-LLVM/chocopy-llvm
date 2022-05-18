; ModuleID = 'ChocoPy code'
source_filename = "/Users/yiweiyang/project/bak/chocopy_test/pa3/sample/op_logical.py"

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
  i32 7,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 8,
  i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.const_9, i32 0, i32 0) 
}
@str.const_9 = private unnamed_addr global [9 x i8] c"f called\00", align 1

@const_10 = global %$str$prototype_type {
  i32 3,
  i32 7,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 8,
  i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.const_10, i32 0, i32 0) 
}
@str.const_10 = private unnamed_addr global [9 x i8] c"g called\00", align 1

@const_11 = global %$str$prototype_type {
  i32 3,
  i32 6,
  %$str$dispatchTable_type* @$str$dispatchTable,
  i32 5,
  i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.const_11, i32 0, i32 0) 
}
@str.const_11 = private unnamed_addr global [6 x i8] c"Never\00", align 1

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
  %op2 = call i1 @$f()
  %op3 = call i1 @$g()
  %op4 = zext i1 %op2 to i32
  %op5 = zext i1 %op3 to i32
  %op6 = or i32 %op4, %op5
  %op7 = icmp ne i32 %op6, 0
  br  i1 %op7, label %label8, label %label15

label8:                                                ; preds = %label0
  %op9 = call i1 @$g()
  %op10 = call i1 @$f()
  %op11 = zext i1 %op9 to i32
  %op12 = zext i1 %op10 to i32
  %op13 = and i32 %op11, %op12
  %op14 = icmp ne i32 %op13, 0
  br  i1 %op14, label %label16, label %label18

label15:                                                ; preds = %label0, %label31
  tail call void asm sideeffect "li a7, 93 #exit system call\0Aecall", ""()
  ret void

label16:                                                ; preds = %label8
  %op17 = bitcast %$str$prototype_type* @const_11 to %$union.put*
  call void @print(%$union.put* %op17)
  br label %label31

label18:                                                ; preds = %label8
  %op19 = call i1 @$f()
  %op20 = call i1 @$g()
  %op21 = call i1 @$f()
  %op22 = zext i1 %op20 to i32
  %op23 = zext i1 %op21 to i32
  %op24 = or i32 %op22, %op23
  %op25 = zext i1 %op19 to i32
  %op26 = and i32 %op25, %op24
  %op27 = sub i32 1,  %op26
  %op28 = trunc i32 %op27 to i1
  %op29 = call %$bool$prototype_type* @makebool(i1 %op28)
  %op30 = bitcast %$bool$prototype_type* %op29 to %$union.put*
  call void @print(%$union.put* %op30)
  br label %label31

label31:                                                ; preds = %label16, %label18
  br label %label15
}
define i1 @$f() {

label0:
  %op1 = bitcast %$str$prototype_type* @const_9 to %$union.put*
  call void @print(%$union.put* %op1)
  ret i1 1
}
define i1 @$g() {

label0:
  %op1 = bitcast %$str$prototype_type* @const_10 to %$union.put*
  call void @print(%$union.put* %op1)
  ret i1 0
}
