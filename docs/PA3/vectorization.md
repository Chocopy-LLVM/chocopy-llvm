# Vectorization

- [Vectorization](#vectorization)
  - [Implementation in LLVM](#implementation-in-llvm)
    - [LLVM instructions and VExt correspond one by one](#llvm-instructions-and-vext-correspond-one-by-one)

## Implementation in LLVM
LLVM Auto Vectorization does the following.
1. Loop Find: Use the strongly connected component algorithm to first do the Dominator Tree and refine the loop with the found head node
2. Vectorization: 
    1. Pattern Matching for x in range(4): a[x] = a[x] + 1 && a=b+c d=f+e g=h+i j=k+m
    2. replace the current instruction with vector

Now Riscv Vextension design is still a bit problematic, this side set VLEN is 256bit can put four 32bit int, other multiples by Pass member variable decision, can be passed by the compiler parameters. If it exceeds the VLEN size, it needs to be rounded.

### LLVM instructions and VExt correspond one by one
| RVV directive |LLVM directive |
|---------------------------------------------------------------------|:---------------------------------------------------------|
| vsetivli zero, 4, e32, mf2, ta, mu && vle32.v v8, (a0) | %2 = load <4 x i32>, ptr %0 |
| vadd.vi v8, v8, 1 | %3 = add nsw <4 x i32> %2, <i32 1, i32 1, i32 1, i32 1, i32 1> |
| vse32.v v8, (a0) | store <4 x i32> %3, ptr %0 |
| vsetivli zero, 1, e32, mf2, ta, mu && vslidedown.vi v8, v8, 1 | %4 = extractelement <4 x i32> %3, i64 1 |
| vmv.x.s a1, v8 | %4 = extractelement <4 x i32> %3, i64 0 |
| vld.x.s a1, v8 | %4 = insertelement <4 x i32> %3, i64 1 |

