; ModuleID = 'ChocoPy code'
source_filename = "/Users/yiweiyang/project/bak/chocopy_test/pa3/sample/list_get_element_complex.py"

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

%$class.anon_next_int = type {i32 ,
  i32 
}

@const_0 = external global %$bool$prototype_type
@const_1 = external global %$bool$prototype_type
@const_2 = external global %$str$prototype_type
@const_3 = external global %$str$prototype_type
@const_4 = external global %$str$prototype_type
@const_5 = external global %$str$prototype_type
@const_6 = external global %$str$prototype_type
@const_7 = external global %$str$prototype_type
@next = global i32 0
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
  %op2 = call %$.list$prototype_type* @$make_list()
  %op3 = alloca %$class.anon_next_int, align 4
  %op4 = getelementptr %$class.anon_next_int, %$class.anon_next_int* %op3, i32 0, i32 0
  store i32*@next, i32* %op4
  %op5 = getelementptr %$class.anon_next_int, %$class.anon_next_int* %op3, i32 0, i32 1
  store i8 %op4, i8* %op5
  %op6 = call i32 @$next_int(%$class.anon_next_int* %op3)
  %op7 = sub i32 %op6, 3
  %op8 = bitcast %$.list$prototype_type* %op2 to %$.list$prototype_type**
  %op9 = load %$.list$prototype_type*, %$.list$prototype_type** %op8
  %op10 = getelementptr %$.list$prototype_type, %$.list$prototype_type* %op9, i32 0, i32 4
  %op11 = load %$union.conslist*, %$union.conslist** %op10
  %op12 = bitcast %$union.conslist* %op11 to i32*
  %op13 = getelementptr i32, i32* %op12, i32 %op7
  %op14 = load i32, i32* %op13
  %op15 = call %$int$prototype_type* @makeint(i32 %op14)
  %op16 = bitcast %$int$prototype_type* %op15 to %$union.put*
  call void @print(%$union.put* %op16)
  tail call void asm sideeffect "li a7, 93 #exit system call\0Aecall", ""()
  ret void
}
define i32 @$next_int(%$class.anon_next_int* %arg0) {

label1:
  %op2 = getelementptr %$class.anon_next_int, %$class.anon_next_int* %arg0, i32 0, i32 0
  %op3 = getelementptr %$class.anon_next_int, %$class.anon_next_int* %arg0, i32 0, i32 0
  %op4 = load i32, i32* %op2
  %op5 = add i32 %op4, 1
  %op6 = load i32, i32* %op2
  ret i32 %op6
}
define %$.list$prototype_type* @$make_list() {

label0:
  %op1 = alloca %$class.anon_next_int, align 4
  %op2 = getelementptr %$class.anon_next_int, %$class.anon_next_int* %op1, i32 0, i32 0
  store i32*@next, i32* %op2
  %op3 = getelementptr %$class.anon_next_int, %$class.anon_next_int* %op1, i32 0, i32 1
  store i8 %op2, i8* %op3
  %op4 = call i32 @$next_int(%$class.anon_next_int* %op1)
  %op5 = alloca i32
  store i32 %op4, i32* %op5
  %op6 = bitcast i32* %op5 to %$union.conslist*
  %op7 = load %$union.conslist, %$union.conslist* %op6
  %op8 = alloca %$class.anon_next_int, align 4
  %op9 = getelementptr %$class.anon_next_int, %$class.anon_next_int* %op8, i32 0, i32 0
  store i8 %op3, i8* %op9
  %op10 = getelementptr %$class.anon_next_int, %$class.anon_next_int* %op8, i32 0, i32 1
  store i8 %op9, i8* %op10
  %op11 = call i32 @$next_int(%$class.anon_next_int* %op8)
  %op12 = alloca i32
  store i32 %op11, i32* %op12
  %op13 = bitcast i32* %op12 to %$union.conslist*
  %op14 = load %$union.conslist, %$union.conslist* %op13
  %op15 = alloca %$class.anon_next_int, align 4
  %op16 = getelementptr %$class.anon_next_int, %$class.anon_next_int* %op15, i32 0, i32 0
  store i8 %op10, i8* %op16
  %op17 = getelementptr %$class.anon_next_int, %$class.anon_next_int* %op15, i32 0, i32 1
  store i8 %op16, i8* %op17
  %op18 = call i32 @$next_int(%$class.anon_next_int* %op15)
  %op19 = alloca i32
  store i32 %op18, i32* %op19
  %op20 = bitcast i32* %op19 to %$union.conslist*
  %op21 = load %$union.conslist, %$union.conslist* %op20
  %op22 = call %$.list$prototype_type* (i32, %$union.conslist, ...) @conslist(i32 3, %$union.conslist %op7, %$union.conslist %op14, %$union.conslist %op21)
  ret %$.list$prototype_type* %op22
}
