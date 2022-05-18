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
  %op71 = mul i32 2, 2
  %op73 = mul i32 %op71, 2
  %op75 = mul i32 %op73, 2
  %op77 = mul i32 %op75, 2
  %op79 = mul i32 %op77, 2
  %op81 = mul i32 %op79, 2
  %op83 = mul i32 %op81, 2
  %op85 = mul i32 %op83, 2
  %op87 = mul i32 %op85, 2
  %op89 = sdiv i32 %op87, 2
  %op91 = sdiv i32 %op89, 2
  %op93 = sdiv i32 %op91, 2
  %op95 = sdiv i32 %op93, 2
  %op97 = sdiv i32 %op95, 2
  %op99 = sdiv i32 %op97, 2
  %op101 = sdiv i32 %op99, 2
  %op103 = sdiv i32 %op101, 2
  %op105 = sdiv i32 %op103, 2
  %op107 = sdiv i32 %op105, 2
  %op59 = icmp sgt i32 2, 1
  %op60 = zext i1 %op59 to i32
  %op61 = icmp ne i32 %op60, 0
  br label %label8
label8:                                                ; preds = %label_entry, %label22
  %op110 = phi i32 [ %op117, %label22 ], [ undef, %label_entry ]
  %op111 = phi i32 [ %op118, %label22 ], [ undef, %label_entry ]
  %op112 = phi i32 [ %op119, %label22 ], [ undef, %label_entry ]
  %op113 = phi i32 [ %op120, %label22 ], [ undef, %label_entry ]
  %op114 = phi i32 [ %op121, %label22 ], [ undef, %label_entry ]
  %op115 = phi i32 [ 0, %label_entry ], [ %op24, %label22 ]
  %op116 = phi i32 [ %op122, %label22 ], [ undef, %label_entry ]
  %op10 = icmp slt i32 %op115, 1000000
  %op11 = zext i1 %op10 to i32
  %op12 = icmp ne i32 %op11, 0
  br i1 %op12, label %label13, label %label14
label13:                                                ; preds = %label8
  br label %label16
label14:                                                ; preds = %label8
  %op150 = call %$int$prototype_type* @makeint(i32 %op110)
  call void @print(%$int$prototype_type* %op150)
  tail call void asm sideeffect "li a7, 93 #exit system call\0Aecall", ""()
  ret void
label16:                                                ; preds = %label13, %label31
  %op117 = phi i32 [ %op110, %label13 ], [ %op123, %label31 ]
  %op118 = phi i32 [ %op111, %label13 ], [ %op124, %label31 ]
  %op119 = phi i32 [ %op112, %label13 ], [ %op125, %label31 ]
  %op120 = phi i32 [ %op113, %label13 ], [ %op126, %label31 ]
  %op121 = phi i32 [ %op114, %label13 ], [ %op127, %label31 ]
  %op122 = phi i32 [ 0, %label13 ], [ %op33, %label31 ]
  %op18 = icmp slt i32 %op122, 2
  %op19 = zext i1 %op18 to i32
  %op20 = icmp ne i32 %op19, 0
  br i1 %op20, label %label21, label %label22
label21:                                                ; preds = %label16
  br label %label25
label22:                                                ; preds = %label16
  %op24 = add i32 %op115, 1
  br label %label8
label25:                                                ; preds = %label21, %label40
  %op123 = phi i32 [ %op117, %label21 ], [ %op129, %label40 ]
  %op124 = phi i32 [ %op118, %label21 ], [ %op130, %label40 ]
  %op125 = phi i32 [ %op119, %label21 ], [ %op131, %label40 ]
  %op126 = phi i32 [ %op120, %label21 ], [ %op132, %label40 ]
  %op127 = phi i32 [ 0, %label21 ], [ %op42, %label40 ]
  %op128 = phi i32 [ %op122, %label21 ], [ %op133, %label40 ]
  %op27 = icmp slt i32 %op127, 2
  %op28 = zext i1 %op27 to i32
  %op29 = icmp ne i32 %op28, 0
  br i1 %op29, label %label30, label %label31
label30:                                                ; preds = %label25
  br label %label34
label31:                                                ; preds = %label25
  %op33 = add i32 %op128, 1
  br label %label16
label34:                                                ; preds = %label30, %label49
  %op129 = phi i32 [ %op123, %label30 ], [ %op134, %label49 ]
  %op130 = phi i32 [ %op124, %label30 ], [ %op135, %label49 ]
  %op131 = phi i32 [ %op125, %label30 ], [ %op136, %label49 ]
  %op132 = phi i32 [ 0, %label30 ], [ %op51, %label49 ]
  %op133 = phi i32 [ %op128, %label30 ], [ %op137, %label49 ]
  %op36 = icmp slt i32 %op132, 2
  %op37 = zext i1 %op36 to i32
  %op38 = icmp ne i32 %op37, 0
  br i1 %op38, label %label39, label %label40
label39:                                                ; preds = %label34
  br label %label43
label40:                                                ; preds = %label34
  %op42 = add i32 %op127, 1
  br label %label25
label43:                                                ; preds = %label39, %label62
  %op134 = phi i32 [ %op129, %label39 ], [ %op138, %label62 ]
  %op135 = phi i32 [ %op130, %label39 ], [ %op139, %label62 ]
  %op136 = phi i32 [ 0, %label39 ], [ %op64, %label62 ]
  %op137 = phi i32 [ %op133, %label39 ], [ %op140, %label62 ]
  %op45 = icmp slt i32 %op136, 2
  %op46 = zext i1 %op45 to i32
  %op47 = icmp ne i32 %op46, 0
  br i1 %op47, label %label48, label %label49
label48:                                                ; preds = %label43
  br label %label52
label49:                                                ; preds = %label43
  %op51 = add i32 %op132, 1
  br label %label34
label52:                                                ; preds = %label48, %label68
  %op138 = phi i32 [ %op134, %label48 ], [ %op107, %label68 ]
  %op139 = phi i32 [ 0, %label48 ], [ %op109, %label68 ]
  %op140 = phi i32 [ %op137, %label48 ], [ %op141, %label68 ]
  %op54 = icmp slt i32 %op139, 2
  %op55 = zext i1 %op54 to i32
  %op56 = icmp ne i32 %op55, 0
  br i1 %op56, label %label57, label %label62
label57:                                                ; preds = %label52
  br i1 %op61, label %label65, label %label68
label62:                                                ; preds = %label52
  %op64 = add i32 %op136, 1
  br label %label43
label65:                                                ; preds = %label57
  %op67 = add i32 %op140, 1
  br label %label68
label68:                                                ; preds = %label57, %label65
  %op141 = phi i32 [ %op140, %label57 ], [ %op67, %label65 ]
  %op109 = add i32 %op139, 1
  br label %label52
}