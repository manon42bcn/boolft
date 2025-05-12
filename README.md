# Ready, set, Bool!



1. **Bit, Booleans and Touring Machine**
2. **Bitwise Arithmetic and Gray Code**
3. **Logical Expression Evaluation in Reverse Polish Notation (RPN) and Truth Tables**
4. **Finite Set Operations** (power set generation and set-based logic)
5. **Space-Filling Curves** (Morton/Z-order encoding and decoding)
6. **Recommended Videos** 

---

## 1. Bit, Booleans and Touring Machine

https://en.wikipedia.org/wiki/Turing_machine
https://en.wikipedia.org/wiki/Bitwise_operation

The Turing machine, introduced by Alan Turing in 1936, is a foundational model in the theory of computation. It describes an abstract device that manipulates symbols on an infinite tape according to a set of rules. Despite its simplicity, the Turing machine is capable of simulating the logic of any computer algorithm, making it a universal model for computation.

At the heart of both the Turing machine and modern computers lies the manipulation of binary data-sequences of 0s and 1s. In practical terms, modern computers process this binary data using bitwise operations. Bitwise operations, such as AND, OR, XOR, NOT, and bit shifts, operate directly on the individual bits of binary numbers. These operations are fundamental to the functioning of digital circuits, data encoding, cryptography, and low-level system programming.

The connection between the Turing machine and bitwise operations is rooted in their shared foundation: both work with symbols (or bits) and apply simple, well-defined rules to manipulate them. While the Turing machine operates at a highly abstract level-reading and writing symbols on a tape-modern computers implement these concepts in hardware, using logic gates and circuits to perform bitwise operations at incredible speeds.

In essence, any bitwise operation that can be performed by a modern computer can, in theory, be simulated by a Turing machine. Conversely, the operations performed by a Turing machine can be implemented in hardware using bitwise logic. This interplay highlights the universality of the Turing machine model and demonstrates how the fundamental operations of computation-whether in theory or practice-are built upon the manipulation of bits.

In summary, the Turing machine provides the theoretical framework for computation, while bitwise operations represent the practical tools used by modern computers to process information at the lowest level. Together, they illustrate the deep connection between computational theory and the physical realities of digital computing.

## 2. Bitwise Arithmetic and Gray Code

**Purpose:** Perform addition, multiplication, and Gray-code conversion using only bitwise logic.

### 2.1 Full Adder

**Logic Block Diagram:**

```
  a -----┐      ┌─ sum = a⊕b⊕cin
         │ XOR ─┤                  → sum
  b -----┘      └─ carry1 = a∧b
                   │
  cin ----------AND┤
                   └─ carry2 = cin⊕(a⊕b)

  carry_out = carry1 ∨ carry2
```

**Truth Table:**

```
 a b cin | sum carry
 0 0  0  |  0    0
 0 0  1  |  1    0
 0 1  0  |  1    0
 0 1  1  |  0    1
 1 0  0  |  1    0
 1 0  1  |  0    1
 1 1  0  |  0    1
 1 1  1  |  1    1
```

### 2.2 Shift-and-Add Multiplier

**Process:**

```
 result = 0
 for i from 0 to n-1:
   if B[i] == 1:
     result = ADD(result, A << i)
```

*(ADD uses the full-adder chain illustrated above.)*

### 2.3 Gray Code Conversion

**Sequence for 3 bits:**

```
 Binary: 000 001 010 011 100 101 110 111
 Gray : 000 001 011 010 110 111 101 100
```

**Diagram:**

```
  000
   |
  001
   |
  011
   |
  010   — etc. (each step flips one bit)
```

Formulas:

```
 gray = binary ⊕ (binary >> 1)
 binary[i] = gray[i] ⊕ binary[i+1]
```

---

## 3. Logical Operations in RPN and Truth Tables

**Purpose:** Evaluate boolean expressions and generate their truth tables.

### 3.1 RPN Evaluation Diagram

Expression: `AB&|` (meaning `(A ∧ B) ∨ C`)

```
Tokens: A  B  AND  C  OR
Stack:         []
Read A → push A    [A]
Read B → push B    [A, B]
Read AND → pop B, A; push (A∧B) → [(A∧B)]
Read C → push C  [(A∧B), C]
Read OR → pop C, (A∧B); push ((A∧B)∨C) → [Result]
```

### 3.2 Truth Table Example

For `(A ∧ B) ∨ C`:

```
 A B C | A∧B | (A∧B)∨C
 0 0 0 |  0   |   0
 0 0 1 |  0   |   1
 0 1 0 |  0   |   0
 0 1 1 |  0   |   1
 1 0 0 |  0   |   0
 1 0 1 |  0   |   1
 1 1 0 |  1   |   1
 1 1 1 |  1   |   1
```

---

## 4. Finite Set Operations

**Purpose:** Generate power sets and perform set-based logic.

### 4.1 Power Set via Bitmasking

Given set `S = {a, b, c}`, n=3:

```
 m=0 (000) → {}
 m=1 (001) → {c}
 m=2 (010) → {b}
 m=3 (011) → {b, c}
 m=4 (100) → {a}
 ...
 m=7 (111) → {a, b, c}
```

### 4.2 Set Operations Diagram

```
   A = {1,2,3}   B = {2,3,4}

 A∪B = {1,2,3,4}
 A∩B = {2,3}
 A\B = {1}
 A⊕B = {1,4}
```

---

## 5. Space-Filling Curves: Morton (Z-order)

**Purpose:** Transform 2D coordinates into a single dimension while preserving locality.

### 5.1 Concept Diagram

A 4×4 grid traversed in Z-order:

```
  y=3  [10] [11] [14] [15]
  y=2  [ 8] [ 9] [12] [13]
  y=1  [ 2] [ 3] [ 6] [ 7]
  y=0  [ 0] [ 1] [ 4] [ 5]
        x=0   1    2    3
```

The path follows:

```
  start→(0,0)->(1,0)->(0,1)->(1,1)---
                    |                
             (2,0)->(3,0)->(2,1)->(3,1)
                    |                
      (0,2)->(1,2)->(0,3)->(1,3)--- 
                    |                
      (2,2)->(3,2)->(2,3)->(3,3) end
```

### 5.2 Bit Interleaving Diagram

For 2-bit x and y:

```
  x = x1 x0    y = y1 y0
            ↓ interleave
  Morton = x1 y1 x0 y0
```

*Example:* x=2 (10₂), y=1 (01₂) → Morton=1 0 0 1₂ = 9.

---

## 6. Recommended Videos

* Touring Machine: https://www.youtube.com/watch?v=iaXLDz_UeYY (es)
* What is a number: https://www.youtube.com/watch?v=H9pMUV4leQg (es)
* Russell's Paradox: https://www.youtube.com/watch?v=ymGt7I4Yn3k (en)
* Gödel's Incompleteness Theorem: https://www.youtube.com/watch?v=8MGbGsihe3k (es)
* Sets: https://www.youtube.com/watch?v=EkFX9jUJPKk (en)
* Bitwise Operators: https://www.youtube.com/watch?v=igIjGxF2J-w (en)

---
