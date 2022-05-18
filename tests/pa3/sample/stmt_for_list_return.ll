; ModuleID = 'ChocoPy code'
source_filename = "/Users/yiweiyang/project/bak/chocopy_test/pa3/sample/stmt_for_list_return.py"

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
  %op2 = alloca i32
  store i32 10, i32* %op2
  %op3 = bitcast i32* %op2 to %$union.conslist*
  %op4 = load %$union.conslist, %$union.conslist* %op3
  %op5 = alloca i32
  store i32 20, i32* %op5
  %op6 = bitcast i32* %op5 to %$union.conslist*
  %op7 = load %$union.conslist, %$union.conslist* %op6
  %op8 = alloca i32
  store i32 30, i32* %op8
  %op9 = bitcast i32* %op8 to %$union.conslist*
  %op10 = load %$union.conslist, %$union.conslist* %op9
  %op11 = alloca i32
  store i32 40, i32* %op11
  %op12 = bitcast i32* %op11 to %$union.conslist*
  %op13 = load %$union.conslist, %$union.conslist* %op12
  %op14 = call %$.list$prototype_type* (i32, %$union.conslist, ...) @conslist(i32 4, %$union.conslist %op4, %$union.conslist %op7, %$union.conslist %op10, %$union.conslist %op13)
  call void @$print_list(%$.list$prototype_type* %op14)
  tail call void asm sideeffect "li a7, 93 #exit system call\0Aecall", ""()
  ret void
}
define void @$print_list(%$.list$prototype_type* %arg0) {

label1:
  %op2 = alloca i32
  store i32 0, i32* %op2
  %op3 = load i32, i32* %op2
  %op4 = bitcast %$.list$prototype_type* %arg0 to %$union.len*
  %op5 = call i32 @$len(%$union.len* %op4)
  %op6 = sub i32 %op5, 1
  br label %label7

label7:                                                ; preds = %label1, %label24
  %op8 = phi i32 [ %op3, %label1 ], [ %op13, %label24 ]
  %op9 = icmp ne i32 %op6, %op8
  %op10 = bitcast %$.list$prototype_type* %arg0 to %$.list$prototype_type**
  %op11 = load %$.list$prototype_type*, %$.list$prototype_type** %op10
  %op12 = getelementptr %$.list$prototype_type, %$.list$prototype_type* %op11, i32 0, i32 4
  %op13 = add i32 %op8, 1
  %op14 = load %$union.conslist*, %$union.conslist** %op12
  %op15 = bitcast %$union.conslist* %op14 to i32*
  %op16 = getelementptr i32, i32* %op15, i32 %op8
  %op17 = load i32, i32* %op16
  br label %label18

label18:                                                ; preds = %label7
  %op19 = call %$int$prototype_type* @makeint(i32 %op17)
  %op20 = bitcast %$int$prototype_type* %op19 to %$union.put*
  call void @print(%$union.put* %op20)
  %op21 = icmp sge i32 %op17, 30
  br  i1 %op21, label %label23, label %label24

label22:                                                ; preds = %label24
  ret void

label23:                                                ; preds = %label18
  ret void

label24:                                                ; preds = %label18
  br  i1 %op9, label %label7, label %label22
}
