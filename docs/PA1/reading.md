# Extended Readings

This section is an extended reading related to Experiment 2 for those who have the ability to expand their horizons and is not required as a lab or course requirement.

## Different generators

Bison is not the only parser generator, and it is not even the best one to use. We recommend that students learn more about other generators, just in case.

There are several perspectives that can be examined.

1. what kind of grammar is supported?
2. what is the target language?
3. how is it implemented?
4. what kind of lexer is supported?
5. what is the efficiency?

And so on. In fact, there is a [comparison page](https://en.wikipedia.org/wiki/Comparison_of_parser_generators) on Wikipedia.

## Handwritten parsers

Although the parser generator works very well: just pour in the grammar and it will automatically generate a lot of code. But there are several drawbacks.

1. a parse tree is generated instead of an abstract syntax tree. This requires more manual work for conversion afterwards. 2.
2. error reporting and error recovery can be complicated. 3.
3. if the generator lacks essential features or bugs, it can cause a lot of trouble.

In the real world, people often write parsers by hand to avoid these drawbacks. In practice, a **top-down** parser, or a **recursive descent**, or an LL is often chosen for ease of error reporting, etc.

In the past, it was often thought that top-down parsing could not handle left recursion. In fact, there is a technique called Pratt parser
technique that solves this problem. It is a simple variant of recursive descent that is easy to understand, yet quite powerful and well suited to handle expressions (recursive, operators with union). Two reference articles are given here for interested students to read.

1. [Simple Top-Down Parsing in Python](http://effbot.org/zone/simple-top-down-parsing.htm) (Python)
2. [Simple but Powerful Pratt Parsing](https://matklad.github.io/2020/04/13/simple-but-powerful-pratt-parsing.html) (
   Rust)
3. [Pratt Parsing Index](https://www.oilshell.org/blog/2017/03/31.html) (a survey article)

In short, top-down parsers are the most commonly used in practice and are very well written after all. For this purpose, we recommend that students do the following exercises.

1. write a JSON parser in your preferred language using the recursive descent method. JSON is the most commonly used Internet data interchange format, and its grammar is available on its website [json.org](http://json.org).
2. Write a quadratic calculator in your favorite language using the Pratt parser method and try to provide user-friendly error reporting.

## Parser combinators

A parser combinator is a higher-order function that can combine multiple parsers into a single parser. What does this mean? What are the applications?

First, you need to define what a "parser" is here. In this case, the parser takes a string and returns the parsed output ** and ** the rest of the string. So, the parser in this case is actually a function.

As an example, suppose there is a lexical parser (which is actually a parser, but accepts a regular grammar) `number` and `identifier`.

```
number("123abc") ==> (Some(123), "abc") Note that the integer 123 is returned here instead of the string
number("abc123") ==> (None, "abc123") fails to identify, so the output for number is None
identifier("abc123") ==> (Some("abc123"), "") Here the string "abc123" is returned
```

(`Some(x)` means the parsing was successful and the output is `x`; `None` means the parser failed.)

Assuming that this language is a calculator language, it supports `2*x` in terms of `2x`. So `factor` can be a numeric followed by an identifier. Wouldn't it be nice if there was a way to combine `number` and `identifier`? We introduce the following combinators.

1. `seq(p,q)`: indicates that the input is passed through p and q in order and the result of both is output; if one of the steps fails, the whole `seq(p,q)` also fails.
2. `or(p,q)`: means try p first, and return the result if it succeeds, otherwise try q next, or fail.

Then it is possible to define

```
factor = or( 
  seq(number,identifier).map { Expr.Mul(Expr.Const(#1), Expr.Val(#2)) }
  number.map(Expr.Const)
)
```

(the above `.map(...) ` is used to convert numeric values such as strings or numbers into abstract syntax tree nodes. (It can be analogous to the action code in the grammar file.)

Based on what was said above, we can surmise that it behaves as

```
factor("123") = (Some(Expr.Const(123)), "")
factor("2x") = (Some(Expr.Mul(Expr.Const(2), Expr.Val("x"))), "")
```

It's easy to see that the process of combining functions into large functions is the process of combining small parsers into large parsers, and it's natural to embed the logic you want into them. What is more interesting is that the compiler is fully aware of the type of each function.

It follows that parser combinators are a programming technique rather than a parsing technique (which is implicit in the implementation of combinators), and using this technique allows for greater modularity in the code and catches errors at compile time in more strongly typed languages. In addition, the code can be just as clean and tidy as using the parser generator, even though the code is completely handwritten. If you are interested, be sure to try it out in your favorite high-level language, or write a set of combinators by hand.

## More Parsing Techniques

The parsing techniques described in the textbook are very useful, but they are not the whole story of parsing. For example.

1. the [Earley parser](https://en.wikipedia.org/wiki/Earley_parser) which can handle dichotomous grammars and left recursion.
2. the [Packrat parser](https://en.wikipedia.org/wiki/Parsing_expression_grammar) for linear time.
3. the O(n³|G|) time [CYK algorithm](https://en.wikipedia.org/wiki/CYK_algorithm) designed using dynamic programming ideas.
4. Even in 2020, when parsing is already considered a solved problem, algorithms such as [Pika parser](https://arxiv.org/abs/2005.06444) are being proposed.

Of course, it is enough to know the names of these algorithms, but they are probably not used in practice.

## Is there a one-and-done solution?

A smart student might ask the question: Is it possible to write a random grammar, have the machine automatically check whether the grammar is binary, convert it into a very efficient grammar representation, and finally generate the code automatically?

Unfortunately, the answer is no. To understand the principle behind it, further study of the theory is required (and the proof is a bit tedious). Here, we list (in very loose language) some conclusions related to context-independent grammar. Note: "undecidable" means
"It is impossible to write such a program", doesn't it sound very middle-class :-p

1. the duality of context-independent grammars is undecidable.
2. Checking whether a grammar accepts any string is undecidable.
3. it is undecidable to check whether two grammars accept the same language.
4. it is undecidable whether the intersection of languages accepted by two CFGs is empty or not.

Also, despite the conclusion in 2 above, "checking if the grammar accepts no strings" is decidable, and can be easily done by converting the CFG to Chomsky normal form.

(Smart students may start to think about what the conclusion of these questions is for regular languages ......)

Here is a famous problem [Post correspondence problem](https://en.wikipedia.org/wiki/Post_correspondence_problem) to illustrate that sometimes human intuition is very unreliable.

Given two lists of strings of the same length a[1], a[2], a[3], ... , a[n] and b[1], b[2], b[3], ... , b[n], answer: does there exist a column of subscripts i[1], i[2], ... , i[k] such that a[i[1]] a[
i[2]] ... a[i[k]] = b[i[1]] b[i[2]] ... b[i[k]]?

To help you get a sense of this, here's a reread of the example from Wikipedia.

| a₁  | a₂  | a₃  |
| --- | --- | --- |
| a   | ab  | bba |

| b₁  | b₂  | b₃  |
| --- | --- | --- |
| baa | aa  | bb  |

For this set of inputs, the problem is solvable because a₃a₂a₃a₁ = b₃b₂b₃b₁.

Although we may not think of an efficient approach for a while, our intuition tells us that it seems possible to go to the violent enumeration and compare them one by one ......

**However**
, this problem is impossible to solve mechanically! It is impossible to write a program to determine this problem. what is going on with PCP undecidable? PCP believe that we are all familiar with it, but PCP undecidable is what is going on, let me take you through it. PCP undecidable, in fact, is the downtime problem undecidable, you may be surprised how PCP undecidable it? But that's the way it is, and I'm very surprised.

## Online Analysis

At this point, we can write parser, but it is not enough in engineering practice. For example, in order to provide accurate real-time error reporting, auto-completion, and code indentation, IDEs need to provide syntax trees as soon as the user edits the code. Simply using lab2
simple offline parser is completely inadequate for this purpose. When editing code, most of the time the code is syntactically or even lexically incorrect, and various error scenarios must be taken into account and guaranteed not to mess up the code. In addition, when providing automatic indentation, errors in the back should not affect the indentation of the code in the front. Another problem is that offline parsing requires building the syntax tree from scratch, which is costly. Inspired by this need for "online parsing", a number of useful works have emerged, such as.

1. [tree-sitter](https://github.com/tree-sitter/tree-sitter): incremental parser framework that always maintains the complete syntax tree in memory. 2.
2. [Auto-indentation with incomplete information](https://arxiv.org/ftp/arxiv/papers/2006/2006.03103.pdf): Operator-based
   precedence parser-based framework for code indentation, supporting local forward parsing. Although it does not maintain a complete syntax tree, it is fast enough due to the small amount of parsing at a time.
