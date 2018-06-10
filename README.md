# Naive-SPL-Compiler
A naive SPL compiler implementation in C++.
## 依赖
  - LLVM 6.0
  - gcc/clang
## 编译
### Flex & Bison

```bash
  flex -o lexer.cpp --yylineno lexer.l
  bison -d -o parser.cpp parser.y
```

### build

```bash
cd cmake-build-debug
make
```

## 运行

`splc input.spl`

## 输出

- `output.ll`

LLVM IR。

- `output.s`
      
本机汇编代码。
可以通过 `gcc -o test.out output.s`生成本机可执行文件。

- `mips.s`

MIPS汇编, target = mips-apple-darwin17.6.0。

- ~~数组的实现~~

- ~~record实现~~

- ~~常量被重新赋值？~~

- 类型检查？

- ~~函数调用~~

- 乘除现在有问题？加减区分一下double int？ — 乘除的问题应该大概是类型检查？

- ~~Var val~~

- ProcStmt 除了write的部分 — write 有bug 不管了

- 变量自动赋初值

- ~~Factor: T_NOT_FACTOR T_MINUS_FACTOR~~ 假装实现 NOT没有测试过

- 倒序改正序访问

- ~~test4 — reference 的 reference有问题~~

- ~~test5 — undefined type?~~

- 枚举不写了吧

  

  
