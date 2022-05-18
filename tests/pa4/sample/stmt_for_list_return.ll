; ModuleID = 'ChocoPy code'
source_filename = "/Users/yiweiyang/project/bak/chocopy_test/pa4/sample/stmt_for_list_return.py"

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
  %$object$dispatchTable_type(%$object$dispatchTable_type)*
}
@$object$dispatchTable = global %$object$dispatchTable_type {
  %$object$dispatchTable_type(%$object$dispatchTable_type)* @$object.__init__
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
  %$object$dispatchTable_type(%$object$dispatchTable_type)*
}
@$int$dispatchTable = global %$int$dispatchTable_type {
  %$object$dispatchTable_type(%$object$dispatchTable_type)* @$object.__init__
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
  %$object$dispatchTable_type(%$object$dispatchTable_type)*
}
@$bool$dispatchTable = global %$bool$dispatchTable_type {
  %$object$dispatchTable_type(%$object$dispatchTable_type)* @$object.__init__
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
  %$object$dispatchTable_type(%$object$dispatchTable_type)*
}
@$str$dispatchTable = global %$str$dispatchTable_type {
  %$object$dispatchTable_type(%$object$dispatchTable_type)* @$object.__init__
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
declare %$object$dispatchTable_type @$object.__init__(%$object$dispatchTable_type)
declare void @heap.init()
declare %$str$dispatchTable_type* @initchars(i8)
declare %$int$dispatchTable_type* @noconv()
declare void @error.OOB()
declare void @error.None()
declare void @error.Div()
declare %$.list$prototype_type* @concat(%$.list$prototype_type*, %$.list$prototype_type*)
declare %$.list$prototype_type* @conslist(i32, %$union.conslist, ...)
declare i32 @$len(%$union.len)
declare void @print(%$union.put*)
declare %$bool$prototype_type* @makebool(i1)
declare %$int$prototype_type* @makeint(i32)
declare %$str$prototype_type* @makestr(%$str$prototype_type*)
declare %$str$prototype_type* @$input()
declare i1 @streql(%$str$prototype_type*)
declare i1 @strneql(%$str$prototype_type*)
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
  tail call void asm sideeffect "li a7, 93 #exit system call\0Aecall", ""()
  ret void
}
define void @$print_list(%$.list$prototype_type* %arg0) {

label1:
  %op2 = alloca i32
  store i32 0, i32* %op2
  %op3 = load i32, i32* %op2
  %op4 = bitcast %$.list$prototype_type* %arg0 to %$union.len*
  %op5 = load %$union.len, %$union.len* %op4
  %op6 = call i32 @$len(%$union.len %op5)
  %op7 = sub i32 %op6, 1
  br label %label8

label8:                                                ; preds = %label1, %label26
  %op9 = phi i32 [ %op3, %label1 ], [ %op14, %label19 ]
  %op10 = icmp ne i32 %op7, %op9
  %op11 = bitcast %$.list$prototype_type* %arg0 to %$.list$prototype_type**
  %op12 = load %$.list$prototype_type*, %$.list$prototype_type** %op11
  %op13 = getelementptr %$.list$prototype_type, %$.list$prototype_type* %op12, i32 0, i32 4
  %op14 = add i32 %op9, 1
  %op15 = load i8*, i8** %op13
  %op16 = bitcast %$union.conslist* %op15 to i32*
  %op17 = getelementptr i32, i32* %op16, i32 %op9
  %op18 = load i32, i32* %op17
  store i32 %op18, i32 0
  br label %label19

label19:                                                ; preds = %label8
  %op20 = call %$int$prototype_type* @makeint(i32 %op18)
  %op21 = bitcast %$int$prototype_type* %op20 to %$union.put*
  call void @print(%$union.put* %op21)
  %op22 = icmp sge i32 %op18, i32 30
  br  i1 %op22, label %label24, label %label26

label23:                                                ; preds = %label26
  ret void

label24:                                                ; preds = %label19
  %op25 = call %$int$dispatchTable_type* @noconv()
  ret %$int$dispatchTable_type* %op25

label26:                                                ; preds = %label19
  %op27 = alloca i32
  store i32 10, i32* %op27
  %op28 = bitcast i32* %op27 to %$union.conslist*
  %op29 = load %$union.conslist, %$union.conslist* %op28
  %op30 = alloca i32
  store i32 20, i32* %op30
  %op31 = bitcast i32* %op30 to %$union.conslist*
  %op32 = load %$union.conslist, %$union.conslist* %op31
  %op33 = alloca i32
  store i32 30, i32* %op33
  %op34 = bitcast i32* %op33 to %$union.conslist*
  %op35 = load %$union.conslist, %$union.conslist* %op34
  %op36 = alloca i32
  store i32 40, i32* %op36
  %op37 = bitcast i32* %op36 to %$union.conslist*
  %op38 = load %$union.conslist, %$union.conslist* %op37
  %op39 = call %$.list$prototype_type* (i32, %$union.conslist, ...) @conslist(i32 4, %$union.conslist %op29, %$union.conslist %op32, %$union.conslist %op35, %$union.conslist %op38)
  call void @$print_list(%$.list$prototype_type* %op39)
  br  i1 %op10, label %label8, label %label23
}
declare void @$print_list(%$.list$prototype_type*)
