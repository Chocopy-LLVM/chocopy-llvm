; ModuleID = 'ChocoPy code'
source_filename = "/Users/yiweiyang/project/bak/chocopy_test/pa3/sample/list_concat.py"

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
@i = global i32 0
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
  %op2 = call %$.list$prototype_type* @nonlist()
  %op3 = alloca i32
  store i32 1, i32* %op3
  %op4 = bitcast i32* %op3 to %$union.conslist*
  %op5 = load %$union.conslist, %$union.conslist* %op4
  %op6 = alloca i32
  store i32 2, i32* %op6
  %op7 = bitcast i32* %op6 to %$union.conslist*
  %op8 = load %$union.conslist, %$union.conslist* %op7
  %op9 = alloca i32
  store i32 3, i32* %op9
  %op10 = bitcast i32* %op9 to %$union.conslist*
  %op11 = load %$union.conslist, %$union.conslist* %op10
  %op12 = call %$.list$prototype_type* (i32, %$union.conslist, ...) @conslist(i32 3, %$union.conslist %op5, %$union.conslist %op8, %$union.conslist %op11)
  %op13 = alloca i32
  store i32 4, i32* %op13
  %op14 = bitcast i32* %op13 to %$union.conslist*
  %op15 = load %$union.conslist, %$union.conslist* %op14
  %op16 = alloca i32
  store i32 5, i32* %op16
  %op17 = bitcast i32* %op16 to %$union.conslist*
  %op18 = load %$union.conslist, %$union.conslist* %op17
  %op19 = alloca i32
  store i32 6, i32* %op19
  %op20 = bitcast i32* %op19 to %$union.conslist*
  %op21 = load %$union.conslist, %$union.conslist* %op20
  %op22 = call %$.list$prototype_type* (i32, %$union.conslist, ...) @conslist(i32 3, %$union.conslist %op15, %$union.conslist %op18, %$union.conslist %op21)
  %op23 = call %$.list$prototype_type* @$concat_list(%$.list$prototype_type* %op12, %$.list$prototype_type* %op22)
  %op24 = load %$.list$prototype_type, %$.list$prototype_type* %op23
  store %$.list$prototype_type %op24, %$.list$prototype_type* %op2
  br label %label25

label25:                                                ; preds = %label0, %label30
  %op26 = load i32, i32*@i
  %op27 = bitcast %$.list$prototype_type* %op2 to %$union.len*
  %op28 = call i32 @$len(%$union.len* %op27)
  %op29 = icmp slt i32 %op26, %op28
  br  i1 %op29, label %label30, label %label41

label30:                                                ; preds = %label25
  %op31 = bitcast %$.list$prototype_type* %op2 to %$.list$prototype_type**
  %op32 = load %$.list$prototype_type*, %$.list$prototype_type** %op31
  %op33 = getelementptr %$.list$prototype_type, %$.list$prototype_type* %op32, i32 0, i32 4
  %op34 = load %$union.conslist*, %$union.conslist** %op33
  %op35 = bitcast %$union.conslist* %op34 to i32*
  %op36 = getelementptr i32, i32* %op35, i32 %op26
  %op37 = load i32, i32* %op36
  %op38 = call %$int$prototype_type* @makeint(i32 %op37)
  %op39 = bitcast %$int$prototype_type* %op38 to %$union.put*
  call void @print(%$union.put* %op39)
  %op40 = add i32 %op26, 1
  store i32 %op40, i32*@i
  br label %label25

label41:                                                ; preds = %label25
  tail call void asm sideeffect "li a7, 93 #exit system call\0Aecall", ""()
  ret void
}
define %$.list$prototype_type* @$concat_list(%$.list$prototype_type* %arg0, %$.list$prototype_type* %arg1) {

label2:
  %op3 = call %$.list$prototype_type* @concat(%$.list$prototype_type* %arg0, %$.list$prototype_type* %arg1)
  ret %$.list$prototype_type* %op3
}
