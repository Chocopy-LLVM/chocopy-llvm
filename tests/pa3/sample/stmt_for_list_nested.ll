; ModuleID = 'ChocoPy code'
source_filename = "/Users/yiweiyang/project/bak/chocopy_test/pa3/sample/stmt_for_list_nested.py"

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
@x = global i32 0
@y = global i32 0
@z = global [3 x i32] [i32 1, i32 2, i32 3  ]
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
  br label %label3

label3:                                                ; preds = %label0, %label24
  %op4 = phi i32 [ %op2, %label0 ], [ %op8, %label24 ]
  %op5 = icmp ne i32 2, %op4
  %op6 = getelementptr inbounds [3 x i32], [3 x i32]* @z, i32 0, i32 0
  %op7 = getelementptr i32, i32* %op6, i32 %op4
  %op8 = add i32 %op4, 1
  %op9 = load i32, i32* %op7
  br label %label10

label10:                                                ; preds = %label3
  %op11 = load i32, i32*@y
  br label %label13

label12:                                                ; preds = %label24
  tail call void asm sideeffect "li a7, 93 #exit system call\0Aecall", ""()
  ret void

label13:                                                ; preds = %label10, %label20
  %op14 = phi i32 [ %op11, %label10 ], [ %op18, %label20 ]
  %op15 = icmp ne i32 2, %op14
  %op16 = getelementptr inbounds [3 x i32], [3 x i32]* @z, i32 0, i32 0
  %op17 = getelementptr i32, i32* %op16, i32 %op14
  %op18 = add i32 %op14, 1
  %op19 = load i32, i32* %op17
  br label %label20

label20:                                                ; preds = %label13
  %op21 = mul i32 %op9, %op19
  %op22 = call %$int$prototype_type* @makeint(i32 %op21)
  %op23 = bitcast %$int$prototype_type* %op22 to %$union.put*
  call void @print(%$union.put* %op23)
  br  i1 %op15, label %label13, label %label24

label24:                                                ; preds = %label20
  br  i1 %op5, label %label3, label %label12
}
