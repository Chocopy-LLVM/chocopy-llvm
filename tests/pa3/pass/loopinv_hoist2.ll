; ModuleID = 'Chocopy'
source_filename = "/Users/yiweiyang/project/bak/chocopy_test/pa3/pass/loopinv_hoist2.py"

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

define void @main() {
label_entry:
  tail call void asm sideeffect "addi fp, sp, 0", ""()
  %op20 = mul i32 2, 2
  %op22 = mul i32 %op20, 2
  %op24 = mul i32 %op22, 2
  %op26 = mul i32 %op24, 2
  %op28 = mul i32 %op26, 2
  %op30 = mul i32 %op28, 2
  %op32 = mul i32 %op30, 2
  %op34 = mul i32 %op32, 2
  %op36 = mul i32 %op34, 2
  %op38 = sdiv i32 %op36, 2
  %op40 = sdiv i32 %op38, 2
  %op42 = sdiv i32 %op40, 2
  %op44 = sdiv i32 %op42, 2
  %op46 = sdiv i32 %op44, 2
  %op48 = sdiv i32 %op46, 2
  %op50 = sdiv i32 %op48, 2
  %op52 = sdiv i32 %op50, 2
  %op54 = sdiv i32 %op52, 2
  %op56 = sdiv i32 %op54, 2
  br label %label4
label4:                                                ; preds = %label_entry, %label59
  %op62 = phi i32 [ %op65, %label59 ], [ undef, %label_entry ]
  %op63 = phi i32 [ 0, %label_entry ], [ %op61, %label59 ]
  %op64 = phi i32 [ %op66, %label59 ], [ undef, %label_entry ]
  %op6 = icmp slt i32 %op63, 10000000
  %op7 = zext i1 %op6 to i32
  %op8 = icmp ne i32 %op7, 0
  br i1 %op8, label %label9, label %label10
label9:                                                ; preds = %label4
  br label %label12
label10:                                                ; preds = %label4
  %op68 = call %$int$prototype_type* @makeint(i32 %op62)
  call void @print(%$int$prototype_type* %op68)
  tail call void asm sideeffect "li a7, 93 #exit system call\0Aecall", ""()
  ret void
label12:                                                ; preds = %label9, %label17
  %op65 = phi i32 [ %op62, %label9 ], [ %op56, %label17 ]
  %op66 = phi i32 [ 0, %label9 ], [ %op58, %label17 ]
  %op14 = icmp slt i32 %op66, 2
  %op15 = zext i1 %op14 to i32
  %op16 = icmp ne i32 %op15, 0
  br i1 %op16, label %label17, label %label59
label17:                                                ; preds = %label12
  %op58 = add i32 %op66, 1
  br label %label12
label59:                                                ; preds = %label12
  %op61 = add i32 %op63, 1
  br label %label4
}


