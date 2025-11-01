# 🧮 Binary Tree Traversal and BEDMAS Calculation

This project explores **binary expression trees** and the three main traversal methods:
- **Preorder Traversal**
- **Inorder Traversal**
- **Postorder Traversal**

It takes a **command-line BEDMAS expression** (with parentheses, variables, and floating-point numbers), constructs a **binary expression tree**, and displays the results of each traversal.

In addition, the program can **evaluate** the given expression by applying BEDMAS (operator precedence) rules. It accepts a second argument to substitute variable values and compute the final numeric result.

---

## ⚙️ Features

- Parses and builds a **binary tree** from an infix BEDMAS expression.  
- Supports **multi-character operands** (e.g., `x1`, `x2`, `5.12`).  
- Implements all three standard traversal algorithms:
  - **Preorder:** Root → Left → Right  
  - **Inorder:** Left → Root → Right  
  - **Postorder:** Left → Right → Root  
- Evaluates arithmetic expressions using recursive tree evaluation.  
- Replaces variables from the command line (e.g., `x1=2.5;x2=3.0;x3=4.0`).  
- Displays output clearly for all traversals and computed result.

---

## 🧠 Example

### Input:
```bash
bin/inorder '(((x1+5.12)*(x2-7.68))/x3)' && bin/postorder '(((x1+5.12)*(x2-7.68))/x3)' && bin/preorder '(((x1+5.12)*(x2-7.68))/x3)' && bin/calculate '(((x1+5.12)*(x2-7.68))/x3)' 'x1=1.25;x2=3.50;x3=1.00'
```

### Output:
```bash
Preorder Output: / * + x1 5.12 - x2 7.68 x3 
Inorder Output: (((x1+5.12)*(x2-7.68))/x3)
Postorder Output: x1 5.12 + x2 7.68 - * x3 / 
Result: -26.63
```

## 🧩 Project Structure
```
bin/                 # Contains executable files
src/
│
├── inorder.c        # Main source file with tree construction & traversals
├── preorder.c       # Preorder traversal program
├── postorder.c      # Postorder traversal program
└── calculate.c      # BEDMAS evaluation logic
Makefile         # Build automation for all targets
```

## 🧱 Compilation
To compile all executables:
```
make
```

## ▶️ Usage
Run the traversal programs individually, i.e.:
```bash
./bin/inorder "(((x1+5.12)*(x2-7.68))/x3)"
```
Run with variable substitution to evaluate, i.e:
```bash
./bin/calculate "(((x1+5.12)*(x2-7.68))/x3)" "x1=1.25;x2=3.50;x3=1.00"
```

## ✅ Assumptions and Constraints
[x] Negative numbers are not allowed in the second argument.
[x] Floating-point results are rounded to 2 decimal digits.
[x] Each variable must appear exactly once in the substitution list.
[x] The expression must be properly parenthesized to define order.