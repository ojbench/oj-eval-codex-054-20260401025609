# Problem 054 - Regulus爱求导

**ACMOJ Problem ID**: 2998

## Table of Contents

- [Problem 054 - Regulus爱求导](#problem-054-regulus爱求导)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
    - [Background](#background)
  - [Assignment Description](#assignment-description)
    - [Grade Composition](#grade-composition)
  - [Assignment Requirements](#assignment-requirements)
    - [Input Format](#input-format)
    - [Output Format](#output-format)
    - [Samples](#samples)
    - [Data Constraints](#data-constraints)
  - [Per-Testcase Resource Limits](#per-testcase-resource-limits)
  - [Test Data](#test-data)
  - [Submission Requirements](#submission-requirements)
    - [OJ Git Repository Compilation Process](#oj-git-repository-compilation-process)
    - [Git Configuration Requirements](#git-configuration-requirements)
    - [Submission Guidelines](#submission-guidelines)
    - [Evaluation Notes](#evaluation-notes)
    - [Academic Integrity](#academic-integrity)

## Introduction

### Background

Enhanced symbolic differentiation: parse and differentiate expressions with sin/cos, fractions, nested operations.

## Assignment Description

### Grade Composition

| Grading Component | Percentage |
| :--: | :--: |
| Pass **2998. Regulus爱求导** | 80% |
| Code Review | 20% |

Here are several points that need clarification:

- In the Code Review, we will **strictly examine your code style and repository organization structure, etc.**

- This assignment provides some sample data for testing, stored in the `/workspace/data/054/data_test/` directory. Note that these are not the test cases on the Online Judge. Passing all local test cases does not guarantee that you will pass the OJ tests.

- Besides the provided sample data, we also encourage you to design your own test data based on your program logic to assist debugging.

## Assignment Requirements

### Problem Description

感谢ACMOJ 1714.

Regulus 是一个被数学分析和编译器折磨的孩子。

这天，他再也不想求导了。他要你帮他写一个求导自动机。

为了方便叙述，这里先给出几个简单的定义（和数学上的定义可能有所不同）：

1. 项：一个形如 $a x^b \sin^c x \cos^d x$ 的式子 ($a \in \mathbb{Z}, a \neq 0, b, c, d \in \mathbb{N}$)
2. 多项式：一个多项式由若干个项通过加减运算得到
3. 分式：若 $p(x)$ 与 $q(x)$ 为两个多项式，则称 $\frac{p(x)}{q(x)}$ 为一个分式
4. 表达式：经若干个分式加减乘除得到的式子称为表达式（可以包含括号）

现在，Regulus 会丢给你一个**表达式** $f(x)$，你需要输出两个**分式** $g(x), h(x)$，使得 $f(x) = g(x), f'(x) = h(x)$

为保证答案的唯一性，我们做出如下规定：

1. 若 $f = \frac{p_1}{q_1}$ 和 $g = \frac{p_2}{q_2}$ 是两个分式，定义分式的加法 $f + g := \frac{p_1 \times q_2 + p_2 \times q_1}{q_1 \times q_2}$
2. 若 $f = \frac{p_1}{q_1}$ 和 $g = \frac{p_2}{q_2}$ 是两个分式，定义分式的减法 $f - g := \frac{p_1 \times q_2 - p_2 \times q_1}{q_1 \times q_2}$
3. 若 $f = \frac{p_1}{q_1}$ 和 $g = \frac{p_2}{q_2}$ 是两个分式，定义分式的乘法 $f \times g := \frac{p_1 \times p_2}{q_1 \times q_2}$
4. 若 $f = \frac{p_1}{q_1}$ 和 $g = \frac{p_2}{q_2}$ 是两个分式，定义分式的除法 $f / g := \frac{p_1 \times q_2}{q_1 \times p_2}$

这四种运算之后，不需要对分式的上下进行约分；为了使答案唯一，我们规定经过约分后的分式和原来的分式是不相等的。

例如，若要计算 $\frac{1+x}{x^2} + \frac{1}{x}$，令 $p_1 = 1 + x, q_1 = x^2, p_2 = 1, q_2 = x$，根据分式的加法的定义：

$$
\frac{1+x}{x^2} + \frac{1}{x} = \frac{p_1 \times q_2 + p_2 \times q_1}{q_1 \times q_2} = \frac{(1+x) \times x + 1 \times x^2}{x^2 \times x} = \frac{2x^2 + x}{x^3}
$$

$\frac{2x^2 + x}{x^3}$ 就是最终答案，你不应该把它约分为 $\frac{2x + 1}{x^2}$

下面给出求导的运算法则。这部分与数学上相同。

1.  $C' = 0$ ($C$ 是常数), $(x^n)' = nx^{n-1} (n \ge 1)$, $(\cos x)' = -\sin x$, $(\sin x)' = \cos x$
2.  若 $f$ 和 $g$ 是两个分式，定义加法求导法则 $(f + g)' = f' + g'$
3.  若 $f$ 和 $g$ 是两个分式，定义减法求导法则 $(f - g)' = f' - g'$
4.  若 $f$ 和 $g$ 是两个分式，定义乘法求导法则 $(f \times g)' = f' \times g + g' \times f$
5.  若 $f$ 和 $g$ 是两个分式，定义除法求导法则 $(f / g)' = (f' \times g - g' \times f) / (g \times g)$
6.  链式求导法则：$[f(g(x))]' = f'(g(x)) \times g'(x)$ （如果你不理解这个公式，你只需要知道 $(\sin^n x)' = n \sin^{n-1} x \cos x$ 和 $(\cos^n x)' = -n \sin x \cos^{n-1} x$ 就行了）

此外，我们规定，你需要先将表达式通过分式的四则运算转化为分式，再对分式进行求导。

例如，要计算 $\frac{\frac{1}{x}}{1+x}$ 的导数，令 $f = \frac{1}{x}$ 和 $g = \frac{1+x}{1}$ 为两个分式，根据分式的除法的定义：

$$
\frac{f}{g} = \frac{1 \times 1}{x \times (x + 1)} = \frac{1}{x^2 + x}
$$

再运用除法求导法则，

$$
\left( \frac{1}{x^2 + x} \right)' = \frac{0 \times (x^2 + x) - (2x + 1) \times 1}{(x^2 + x) \times (x^2 + x)} = \frac{-2x - 1}{x^4 + 2x^3 + x^2}
$$

而不是直接运用除法求导法则：

$$
\left( \frac{\frac{1}{x}}{1 + x} \right)' = \frac{f' \times g - g' \times f}{g \times g} = \frac{-\frac{1+x}{x^2} - \frac{1}{x}}{(1 + x) \times (1 + x)} = \frac{\frac{-2x^2 - x}{x^3}}{x^2 + 2x + 1} = \frac{-2x^2 - x}{x^5 + 2x^4 + x^3}
$$

请注意，虽然不用进行约分（包括不要在分子分母同时*(-1)），但是合并同类项这一化简是需要进行的。例如 $\sin x + 2 \sin x$ 应写为 $3 \sin x$

另外，你不需要考虑诸如 $\sin^2 x + \cos^2 x = 1$ 的化简。换言之，你不用考虑除了本题给定公式之外的任何公式。

为保证答案的唯一性，你需要按照如下格式输出答案：

1.  答案应形为 `(s1)/(s2)` 的形式，其中 `s1`, `s2` 分别为分式的分子和分母。当 `s1` 的项数超过 1 项时，应该保留 `s1` 外面的括号，否则不应添加括号，`s2` 同理；`s1` 与 `s2` 内部不应出现任何的除号与括号
2.  `s1` 与 `s2` 内部应当是由加号或减号隔开的若干个项（但不应出现类似于 `x - -5` (x 减去负 5) 这样的写法，而应写成 `x + 5`）。如果第一项的系数是正数，不应该带有符号。项的系数和字母之间不应该有乘号。如果项的系数绝对值为 1 且该项不是常数项时，只需要保留符号。指数用符号 `^` 表达。如果某个指数为 0，应当省略掉指数和相应的符号；如果指数为 1，应当省略掉指数。项之间以项的 `x` 上方指数，`sin x` 上方指数，`cos x` 上方指数为第一、第二、第三关键字降序排列。（如 $\frac{-1 \times x^0 \sin^1 x \cos^3 x + 3 \times \sin^2 x}{3x}$ 应当输出 `(3sin^2x-sinxcos^2x)/3x`）
3.  特殊的，若 `s1 = 0`，你只需要输出一个 `0`；若 `s2 = 1`，你只需要输出一个 `s1`，且不用在 `s1` 外面打上括号，无论 `s1` 有几项。
4.  你的输出不应该包含除数字，加、减、除号（不应该有乘号）、括号、三个标识符 (`x`, `sinx`, `cosx`) 和 `^` 之外的其他字符。

保证输入数据是一个合法的符合正常书写规范的表达式，并且不会出现除数字，加、减、乘、除号、括号、三个标识符 (`x`, `sinx`, `cosx`) 和 `^` 之外的其他字符。组成表达式的项的书写规范和输出格式中要求的相同。运算过程中所有数字不会超过 int 类型存储的范围。负号只会出现在分子或分母的第一个字符或左括号后的第一个字符（会出现 `9 / -6x` 这样的数据）

```
import sys

class Term:
    """项：表示 a * x^b * sin^c(x) * cos^d(x)"""
    __slots__ = ('a', 'b', 'c', 'd')
    
    def __init__(self, a: int = 0, b: int = 0, c: int = 0, d: int = 0):
        # 你可以用 Term(1, 2, 3, 4) 创建一个表示 x^2 sin^3 x cos^4 x 的 term
        self.a = a
        self.b = b
        self.c = c
        self.d = d

    def __eq__(self, other) -> bool:
        # 相等表示是同类项（仅比较指数 b, c, d）
        return self.b == other.b and self.c == other.c and self.d == other.d

    def __ne__(self, other) -> bool:
        return not self.__eq__(other)


class Poly:
    """多项式：由若干 Term 组成"""
    __slots__ = ('terms',)

    def __init__(self, size_or_terms=None):
        if isinstance(size_or_terms, int):
            self.terms = [Term() for _ in range(size_or_terms)]
        elif isinstance(size_or_terms, list):
            # 从 term 列表初始化
            self.terms = size_or_terms[:]
        else:
            # 默认构造
            self.terms = []

    def __len__(self) -> int:
        return len(self.terms)

    def simplify(self) -> None:
        """合并同类项"""
        # TODO: 实现合并同类项（先排序，再合并）
        pass

    def __add__(self, other: 'Poly') -> 'Poly':
        """重载加法"""
        # 创建新 Poly，复制左右操作数的项，再 simplify
        result_terms = self.terms[:] + other.terms[:]
        result = Poly(result_terms)
        result.simplify()
        return result

    def __sub__(self, other: 'Poly') -> 'Poly':
        """重载减法"""
        # 复制 self 的项，复制 other 的项但系数取反
        negated_other = [Term(-t.a, t.b, t.c, t.d) for t in other.terms]
        result_terms = self.terms[:] + negated_other
        result = Poly(result_terms)
        result.simplify()
        return result

    def __mul__(self, other: 'Poly') -> 'Poly':
        """重载乘法"""
        # TODO: 实现多项式乘法（项两两相乘，指数相加，系数相乘）
        # 注意：不要修改 self 或 other 的数据
        pass

    def __copy__(self) -> 'Poly':
        """深拷贝 terms"""
        return Poly([Term(t.a, t.b, t.c, t.d) for t in self.terms])

    def derivate(self) -> 'Poly':
        """对多项式求导，返回新 Poly"""
        # TODO: 对每个 term 求导（应用乘积法则展开为最多三项）
        # 注意：以只读方式访问 self
        pass


class Frac:
    """分式：p / q，其中 p, q 为 Poly"""
    __slots__ = ('p', 'q')

    def __init__(self, arg1=None, arg2=None):
        if arg1 is None:
            # 默认构造
            self.p = Poly()
            self.q = Poly()
        elif isinstance(arg1, int):
            # Frac(int x) -> x / 1
            self.p = Poly([Term(arg1, 0, 0, 0)])
            self.q = Poly([Term(1, 0, 0, 0)])
        elif isinstance(arg1, Term):
            # Frac(Term _p) -> _p / 1
            self.p = Poly([Term(arg1.a, arg1.b, arg1.c, arg1.d)])
            self.q = Poly([Term(1, 0, 0, 0)])
        elif isinstance(arg1, Poly) and isinstance(arg2, Poly):
            # Frac(Poly _p, Poly _q)
            self.p = arg1.__copy__()
            self.q = arg2.__copy__()
        else:
            raise TypeError("Invalid Frac constructor arguments")

    def __add__(self, other: 'Frac') -> 'Frac':
        # p1/q1 + p2/q2 = (p1*q2 + p2*q1) / (q1*q2)
        new_p = self.p * other.q + other.p * self.q
        new_q = self.q * other.q
        return Frac(new_p, new_q)

    def __sub__(self, other: 'Frac') -> 'Frac':
        # TODO: 实现分式减法
        pass

    def __mul__(self, other: 'Frac') -> 'Frac':
        # TODO: 实现分式乘法
        pass

    def __truediv__(self, other: 'Frac') -> 'Frac':
        # TODO: 实现分式除法
        pass

    def derivate(self) -> 'Frac':
        """对分式求导：(p/q)' = (p' * q - q' * p) / (q * q)"""
        # TODO: 实现分式求导
        pass

    def output(self) -> None:
        """按题目要求格式输出分式"""
        # TODO: 实现输出逻辑（包括括号、系数、指数、三角函数等）
        pass


def get_number(s: str, l: int, r: int) -> int:
    """从 s[l:r] 中解析整数，若无数字则返回 ±1"""
    # 若无数字，根据是否有负号返回 -1 或 1
    # TODO: 实现与 C++ get_number 完全一致的逻辑
    pass


def get_term(s: str, l: int, r: int) -> Term:
    """从 s[l:r] 中解析一个项"""
    # 扫描 'x'（非 trig 内）、'n'（sin）、'c'（cos）
    # TODO
    pass


def dfs(s: str, l: int, r: int) -> Frac:
    """递归下降解析表达式为分式"""
    # TODO: 实现表达式解析（处理括号、+ - * / 优先级）
    pass


def solve(s: str, n: int) -> None:
    """主逻辑：解析表达式，输出原式和导数"""
    # TODO: 调用 dfs，然后输出 f(x) 和 f'(x)
    pass


def main():
    expr = sys.stdin.readline().strip()
    n = len(expr)
    solve(expr, n)


if __name__ == "__main__":
    main()
```

### Input Format

一行一个字符串，表示要求导的表达式

### Output Format

第一行一个字符串，表示将表达式化简为分式后的结果

第二行一个字符串，表示表达式求导的结果

### Samples

#### Sample 1

**Input:**
```
-1+x+x^2+3x^3+sinx+cos^2x+sin^4x-5cos^8x
```

**Output:**
```
3x^3+x^2+x+sin^4x+sinx-5cos^8x+cos^2x-1
9x^2+2x+4sin^3xcosx+40sinxcos^7x-2sinxcosx+cosx+1
```

#### Sample 2

**Input:**
```
-xsinxcos^2x*x^3sinx
```

**Output:**
```
-x^4sin^2xcos^2x
2x^4sin^3xcosx-2x^4sinxcos^3x-4x^3sin^2xcos^2x
```

#### Sample 3

**Input:**
```
1/(1-1/(1-sinx+x))
```

**Output:**
```
(x-sinx+1)/(x-sinx)
(cosx-1)/(x^2-2xsinx+sin^2x)
```

### Data Constraints

对于 100% 的数据，读入的表达式项的数目不超过 50，所有的 $|a|, b, c, d \le 10$

| 测试点编号 | * 出现次数 | / 出现次数 | 不出现括号 | 性质一 | 性质二 |
| :---: | :---: | :---: | :---: | :---: | :---: |
| 1 | 0 | 0 | ✓ | ✓ | ✓ |
| 2 | 0 | 0 | ✓ | | ✓ |
| 3 | 0 | 0 | ✓ | | |
| 4 | ≤ 50 | 0 | ✓ | ✓ | ✓ |
| 5 | ≤ 50 | 0 | ✓ | | ✓ |
| 6 | ≤ 50 | 0 | ✓ | | |
| 7 | ≤ 50 | ≤ 50 | ✓ | ✓ | ✓ |
| 8 | ≤ 50 | ≤ 50 | ✓ | | ✓ |
| 9 | ≤ 50 | ≤ 50 | ✓ | | |
| 10 | ≤ 50 | ≤ 1 | | ✓ | ✓ |
| 11 | ≤ 50 | ≤ 1 | | | ✓ |
| 12 | ≤ 50 | ≤ 1 | | | |
| 13 | ≤ 50 | ≤ 1 | | ✓ | ✓ |
| 14 | ≤ 50 | ≤ 1 | | | ✓ |
| 15 | ≤ 50 | ≤ 1 | | | |
| 16 | ≤ 50 | ≤ 50 | | | |
| 17 | ≤ 50 | ≤ 50 | | | |
| 18 | ≤ 50 | ≤ 50 | | | |
| 19 | ≤ 50 | ≤ 50 | | | |
| 20 | ≤ 50 | ≤ 50 | | | |

- **性质一**：表达式中不出现三角函数
- **性质二**：每一项的 $b, c, d$ 三个指数中至多有一个不为 0

## Per-Testcase Resource Limits

- **Time Limit (per test case)**: 3000 ms
- **Memory Limit (per test case)**: 256 MiB
- **Disk Usage**: No disk usage is permitted.

## Test Data

The test data for this problem is located at `/workspace/data/054/data_test/`.

## Submission Requirements

### OJ Git Repository Compilation Process

For Git compilation, we will first clone the repository using a command similar to:
```bash
git clone <repo_url> . --depth 1 --recurse-submodules --shallow-submodules --no-local
```

Then we check if there is a `CMakeLists.txt` file. If it exists, we run (if not, a warning message will be displayed):
```bash
cmake .
```

Finally, we check if there is any of `GNUmakefile`/`makefile`/`Makefile` (if cmake was run previously, this will be the generated Makefile). If it exists, we run (if not, a warning message will be displayed):
```bash
make
```

After this process is complete, we will use the `code` file in the project root directory as the compilation result.

A `CMakeLists.txt` file is provided in the project. You can use or modify it as needed. The local environment has gcc-13 and g++-13 available.

### Git Configuration Requirements

**IMPORTANT**: You must create a `.gitignore` file in your project root directory to avoid OJ evaluation conflicts.

The `.gitignore` file should include at least the following entries:

```gitignore
CMakeFiles/
CMakeCache.txt
```

### Submission Guidelines

- The submitted code must be able to compile successfully through the above compilation process
- The compiled executable file name must be `code`
- The program needs to be able to read data from standard input and write results to standard output
- Please ensure the code runs correctly within the given time and space limits
- **You must use C++ or C language** to implement this assignment

### Evaluation Notes

- The evaluation system will test your program using the provided test data
- The program output must exactly match the expected output (including format)
- Exceeding time or memory limits will be judged as the corresponding error type
- Please pay attention to the overall time performance of your code and the time complexity of each part of your algorithm.

### Academic Integrity

If any violations are found during evaluation or code review (including but not limited to using unconventional methods to pass test cases), your final score may be significantly reduced or become **0 points**.
