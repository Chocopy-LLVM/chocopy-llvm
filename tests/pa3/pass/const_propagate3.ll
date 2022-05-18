; ModuleID = 'Chocopy'
source_filename = "/Users/yiweiyang/project/bak/chocopy_test/pa3/pass/const_propagate3.py"

@opa = global i32 zeroinitializer
@opb = global i32 zeroinitializer
@opc = global i32 zeroinitializer
@opd = global i32 zeroinitializer

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

%$class.anon_make_z = type {%$.list$prototype_type* ,
  %$.list$prototype_type*
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

define i32 @max() {
label_entry:
  store i32 0, i32* @opa
  store i32 40320, i32* @opb
  store i32 362880, i32* @opc
  store i32 1814400, i32* @opd
  br label %label33
label33:                                                ; preds = %label_entry
  %op34 = load i32, i32* @opb
  %op35 = load i32, i32* @opc
  %op36 = icmp slt i32 %op34, %op35
  %op37 = zext i1 %op36 to i32
  %op38 = icmp ne i32 %op37, 0
  br i1 %op38, label %label40, label %label46
label39:                                                ; preds = %label46
  ret i32 0
label40:                                                ; preds = %label33
  %op41 = load i32, i32* @opc
  %op42 = load i32, i32* @opd
  %op43 = icmp slt i32 %op41, %op42
  %op44 = zext i1 %op43 to i32
  %op45 = icmp ne i32 %op44, 0
  br i1 %op45, label %label47, label %label49
label46:                                                ; preds = %label33, %label49
  br label %label39
label47:                                                ; preds = %label40
  %op48 = load i32, i32* @opd
  ret i32 %op48
label49:                                                ; preds = %label40
  br label %label46
}
define void @main() {
label_entry:
  tail call void asm sideeffect "addi fp, sp, 0", ""()
  br label %label1
label1:                                                ; preds = %label_entry, %label6
  %op15 = phi i32 [ 0, %label_entry ], [ %op9, %label6 ]
  %op3 = icmp slt i32 %op15, 200000000
  %op4 = zext i1 %op3 to i32
  %op5 = icmp ne i32 %op4, 0
  br i1 %op5, label %label6, label %label10
label6:                                                ; preds = %label1
  %op7 = call i32 @max()
  %op9 = add i32 %op15, 1
  br label %label1
label10:                                                ; preds = %label1
  %op11 = load i32, i32* @opa
  %op12 = call %$int$prototype_type* @makeint(i32 %op11)
  call void @print(%$int$prototype_type* %op12)
  %op13 = load i32, i32* @opb
  %op14 = call %$int$prototype_type* @makeint(i32 %op13)
  call void @print(%$int$prototype_type* %op14)
  %op15 = load i32, i32* @opc
  %op16 = call %$int$prototype_type* @makeint(i32 %op15)
  call void @print(%$int$prototype_type* %op16)
  %op17 = load i32, i32* @opd
  %op18 = call %$int$prototype_type* @makeint(i32 %op17)
  call void @print(%$int$prototype_type* %op18)
  tail call void asm sideeffect "li a7, 93 #exit system call\0Aecall", ""()
  ret void
}

