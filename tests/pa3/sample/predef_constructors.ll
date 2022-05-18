; ModuleID = 'ChocoPy code'
source_filename = "/Users/yiweiyang/project/bak/chocopy_test/pa3/sample/predef_constructors.py"

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
  %op2 = bitcast %$object$prototype_type* @$object$prototype to %$object$prototype_type*
  %op3 = call %$object$prototype_type* @alloc(%$object$prototype_type* %op2)
  %op4 = bitcast %$object$prototype_type* %op3 to %$object$prototype_type*
  %op5 = getelementptr %$object$prototype_type, %$object$prototype_type* %op4, i32 0, i32 2
  %op6 = load %$object$dispatchTable_type*, %$object$dispatchTable_type** %op5
  %op7 = getelementptr %$object$dispatchTable_type, %$object$dispatchTable_type* %op6, i32 0, i32 0
  %op8 = load void (%$object$prototype_type*)*, void (%$object$prototype_type*)** %op7
  call void %op8(%$object$prototype_type* %op3)
  %op9 = call %$int$prototype_type* @noconv()
  %op10 = trunc i32 0 to i1
  %op11 = call %$bool$prototype_type* @makebool(i1 %op10)
  %op12 = bitcast %$bool$prototype_type* %op11 to %$union.put*
  call void @print(%$union.put* %op12)
  %op13 = bitcast %$int$prototype_type* @$int$prototype to %$object$prototype_type*
  %op14 = call %$object$prototype_type* @alloc(%$object$prototype_type* %op13)
  %op15 = bitcast %$object$prototype_type* %op14 to %$object$prototype_type*
  %op16 = getelementptr %$object$prototype_type, %$object$prototype_type* %op15, i32 0, i32 2
  %op17 = load %$object$dispatchTable_type*, %$object$dispatchTable_type** %op16
  %op18 = getelementptr %$object$dispatchTable_type, %$object$dispatchTable_type* %op17, i32 0, i32 0
  %op19 = load void (%$object$prototype_type*)*, void (%$object$prototype_type*)** %op18
  call void %op19(%$object$prototype_type* %op15)
  %op20 = bitcast %$object$prototype_type* %op15 to %$union.put*
  call void @print(%$union.put* %op20)
  %op21 = bitcast %$str$prototype_type* @$str$prototype to %$object$prototype_type*
  %op22 = call %$object$prototype_type* @alloc(%$object$prototype_type* %op21)
  %op23 = bitcast %$object$prototype_type* %op22 to %$object$prototype_type*
  %op24 = getelementptr %$object$prototype_type, %$object$prototype_type* %op23, i32 0, i32 2
  %op25 = load %$object$dispatchTable_type*, %$object$dispatchTable_type** %op24
  %op26 = getelementptr %$object$dispatchTable_type, %$object$dispatchTable_type* %op25, i32 0, i32 0
  %op27 = load void (%$object$prototype_type*)*, void (%$object$prototype_type*)** %op26
  call void %op27(%$object$prototype_type* %op23)
  %op28 = bitcast %$object$prototype_type* %op23 to %$union.put*
  call void @print(%$union.put* %op28)
  %op29 = bitcast %$bool$prototype_type* @$bool$prototype to %$object$prototype_type*
  %op30 = call %$object$prototype_type* @alloc(%$object$prototype_type* %op29)
  %op31 = bitcast %$object$prototype_type* %op30 to %$object$prototype_type*
  %op32 = getelementptr %$object$prototype_type, %$object$prototype_type* %op31, i32 0, i32 2
  %op33 = load %$object$dispatchTable_type*, %$object$dispatchTable_type** %op32
  %op34 = getelementptr %$object$dispatchTable_type, %$object$dispatchTable_type* %op33, i32 0, i32 0
  %op35 = load void (%$object$prototype_type*)*, void (%$object$prototype_type*)** %op34
  call void %op35(%$object$prototype_type* %op31)
  %op36 = bitcast %$object$prototype_type* %op31 to %$union.put*
  call void @print(%$union.put* %op36)
  tail call void asm sideeffect "li a7, 93 #exit system call\0Aecall", ""()
  ret void
}
