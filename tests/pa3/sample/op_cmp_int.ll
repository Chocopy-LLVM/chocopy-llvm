; ModuleID = 'ChocoPy code'
source_filename = "/Users/yiweiyang/project/bak/chocopy_test/pa3/sample/op_cmp_int.py"

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
@x = global i32 42
@y = global i32 7
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
  %op2 = load i32, i32*@x
  %op3 = load i32, i32*@y
  %op4 = icmp eq i32 %op2, %op3
  %op5 = call %$bool$prototype_type* @makebool(i1 %op4)
  %op6 = bitcast %$bool$prototype_type* %op5 to %$union.put*
  call void @print(%$union.put* %op6)
  %op7 = load i32, i32*@x
  %op8 = load i32, i32*@y
  %op9 = icmp ne i32 %op7, %op8
  %op10 = call %$bool$prototype_type* @makebool(i1 %op9)
  %op11 = bitcast %$bool$prototype_type* %op10 to %$union.put*
  call void @print(%$union.put* %op11)
  %op12 = load i32, i32*@x
  %op13 = load i32, i32*@y
  %op14 = icmp slt i32 %op12, %op13
  %op15 = call %$bool$prototype_type* @makebool(i1 %op14)
  %op16 = bitcast %$bool$prototype_type* %op15 to %$union.put*
  call void @print(%$union.put* %op16)
  %op17 = load i32, i32*@x
  %op18 = load i32, i32*@y
  %op19 = icmp sle i32 %op17, %op18
  %op20 = call %$bool$prototype_type* @makebool(i1 %op19)
  %op21 = bitcast %$bool$prototype_type* %op20 to %$union.put*
  call void @print(%$union.put* %op21)
  %op22 = load i32, i32*@x
  %op23 = load i32, i32*@y
  %op24 = icmp sgt i32 %op22, %op23
  %op25 = call %$bool$prototype_type* @makebool(i1 %op24)
  %op26 = bitcast %$bool$prototype_type* %op25 to %$union.put*
  call void @print(%$union.put* %op26)
  %op27 = load i32, i32*@x
  %op28 = load i32, i32*@y
  %op29 = icmp sge i32 %op27, %op28
  %op30 = call %$bool$prototype_type* @makebool(i1 %op29)
  %op31 = bitcast %$bool$prototype_type* %op30 to %$union.put*
  call void @print(%$union.put* %op31)
  %op32 = load i32, i32*@x
  %op33 = load i32, i32*@x
  %op34 = icmp eq i32 %op32, %op33
  %op35 = call %$bool$prototype_type* @makebool(i1 %op34)
  %op36 = bitcast %$bool$prototype_type* %op35 to %$union.put*
  call void @print(%$union.put* %op36)
  %op37 = load i32, i32*@x
  %op38 = load i32, i32*@x
  %op39 = icmp ne i32 %op37, %op38
  %op40 = call %$bool$prototype_type* @makebool(i1 %op39)
  %op41 = bitcast %$bool$prototype_type* %op40 to %$union.put*
  call void @print(%$union.put* %op41)
  %op42 = load i32, i32*@x
  %op43 = load i32, i32*@x
  %op44 = icmp slt i32 %op42, %op43
  %op45 = call %$bool$prototype_type* @makebool(i1 %op44)
  %op46 = bitcast %$bool$prototype_type* %op45 to %$union.put*
  call void @print(%$union.put* %op46)
  %op47 = load i32, i32*@x
  %op48 = load i32, i32*@x
  %op49 = icmp sle i32 %op47, %op48
  %op50 = call %$bool$prototype_type* @makebool(i1 %op49)
  %op51 = bitcast %$bool$prototype_type* %op50 to %$union.put*
  call void @print(%$union.put* %op51)
  %op52 = load i32, i32*@x
  %op53 = load i32, i32*@x
  %op54 = icmp sgt i32 %op52, %op53
  %op55 = call %$bool$prototype_type* @makebool(i1 %op54)
  %op56 = bitcast %$bool$prototype_type* %op55 to %$union.put*
  call void @print(%$union.put* %op56)
  %op57 = load i32, i32*@x
  %op58 = load i32, i32*@x
  %op59 = icmp sge i32 %op57, %op58
  %op60 = call %$bool$prototype_type* @makebool(i1 %op59)
  %op61 = bitcast %$bool$prototype_type* %op60 to %$union.put*
  call void @print(%$union.put* %op61)
  tail call void asm sideeffect "li a7, 93 #exit system call\0Aecall", ""()
  ret void
}
