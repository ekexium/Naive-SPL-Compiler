# Naive-SPL-Compiler
A naive SPL compiler implementation in C++.
## 依赖
  - LLVM 6.0

并相应修改CMakeLists.txt中INC_DIR和LINK_DIR

  - gcc/clang
## 编译
### Flex & Bison

```bash
  flex -o lexer.cpp --yylineno lexer.l
  bison -d -o parser.cpp parser.y
```

### build

```bash
cmake .
make
```

## 运行

`./splc input.spl`

## 输出

- `output.ll`

LLVM IR。

- `output.s`
      
本机汇编代码。
可以通过 `gcc -o test.out output.s`生成本机可执行文件。

- `mips.s`

MIPS汇编, target = mips-apple-darwin17.6.0。

## Checklist
- [x] 数组 一维
- [x] record实现
- [x] 常量不可被重新赋值
- [ ] 类型检查？
- [x] 函数调用
- [x] 乘除
- [x] 加减区分 real integer
- [x] var: call by reference
- [x] write, writeln
- [ ] read
- [ ] 变量自动赋初值
- [x] Factor: T_NOT_FACTOR T_MINUS_FACTOR
- [ ] 倒序改正序访问
- [ ] 主程序返回值

## 功能

1. SPL基本语法
2. 可视化AST

dependency: graphviz
```bash
dot -Tpng -o ast.png ast.dot
```

3. 代码生成(default target和MIPS)

default target可在本机编译运行，检查正确性。
MIPS为实验要求。

4. 超简单的错误提示。