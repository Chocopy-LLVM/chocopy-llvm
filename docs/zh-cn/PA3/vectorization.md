# 向量化

- [向量化](#向量化)
  - [LLVM 中的实现](#llvm-中的实现)
    - [LLVM指令和VExt一一对应](#llvm指令和vext一一对应)

## LLVM 中的实现
LLVM Auto Vectorization要干的事：
1. Loop Find: 利用强连通分量算法先做Dominator Tree，用找到的头节点精炼loop
2. Vectorization: 
    1. Pattern Matching for x in range(4): a[x] = a[x] + 1 && a=b+c d=f+e g=h+i j=k+m
    2. replace the current instruction with vector

现在Riscv Vextension 设计还有点问题，这边设置VLEN是256bit 可以放4个32bit int，其他倍数由Pass成员变量决定，可以由编译器参数传入。如果超过VLEN大小，就需要舍入。

### LLVM指令和VExt一一对应
|  RVV指令    |LLVM指令                             |
|---------------------------------------------------------------------|:---------------------------------------------------------|
|  vsetivli        zero, 4, e32, mf2, ta, mu &&  vle32.v v8, (a0)     | %2 = load <4 x i32>, ptr %0                              |
|  vadd.vi v8, v8, 1                                                  |  %3 = add nsw <4 x i32> %2, <i32 1, i32 1, i32 1, i32 1> |
|  vse32.v v8, (a0)                                                   |  store <4 x i32> %3, ptr %0                              |
| vsetivli        zero, 1, e32, mf2, ta, mu && vslidedown.vi   v8, v8, 1 |  %4 = extractelement <4 x i32> %3, i64 1                 |
|  vmv.x.s a1, v8                                                     |  %4 = extractelement <4 x i32> %3, i64 0                 |
| vld.x.s a1, v8                                                      |  %4 = insertelement <4 x i32> %3, i64 1                  |




