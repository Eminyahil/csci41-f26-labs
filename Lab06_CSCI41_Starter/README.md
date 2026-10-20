# CSCI 41 - Lab 06: Abstract Data Types

Implement `IntSet` in `src/int_set.cpp`. Read `include/int_set.h` first: it
says what every function must do.

## The rules

- **Own the memory yourself.** Store the values in an array you allocate with
  `new[]` and free with `delete[]`. No `std::vector`, no `std::set`, no other
  container.
- **Do not change `include/int_set.h`.**
- **Write all three of the destructor, the copy constructor and the copy
  assignment operator.** The class owns memory, so the compiler's versions
  are wrong.

## Build and run

CLion: open this folder, pick the target `lab06_tests`, run it.

```bash
mkdir -p build
cmake -S . -B build
cmake --build build
./build/lab06_tests
```

`./build/lab06` runs the demo in `src/main.cpp`, which is not graded.

## Grading

- The 12 public checks in `tests/test_lab06.cpp` are a self-check, not your
  grade. Every *kind* of case the private tests look at appears here at least
  once, so if a whole category is broken you will see it before you submit.
- **Private tests: 25 tests, 4 points each, 100 total.**
- Graded builds use `-fsanitize=address`. A leak or a double free will be
  caught and will cost you, because this is the lab where memory is the
  point.

An unimplemented function throws, which fails only its own tests.

## Before you write anything

Two minutes with a pen, for yourself. Not collected.

Write down what each operation costs on this representation, in terms of
`n = size()`:

| operation | cost | why |
|---|---|---|
| `contains` | | |
| `insert` (value already present) | | |
| `insert` (new value, room spare) | | |
| `insert` (new value, array full) | | |
| `remove` | | |
| `size`, `isEmpty`, `capacity` | | |

That table is the design. If you cannot fill it in, you do not yet know what
you are building.

## Two things the lectures have not covered

**Size is not capacity.** `size()` is how many values you are holding.
`capacity()` is how many you could hold before the array has to be replaced.
They are different numbers and only one of them is part of the idea of a set.

**Why double the capacity, rather than adding one?** Growing means allocating
a new array and copying everything across, which costs `n`. If you grow by
one every time, you pay that on *every* insert. If you double, the array has
to be replaced only when it is full, which gets rarer and rarer as it gets
bigger -- you grow at 4, 8, 16, 32 and so on. Adding 100 values costs a
handful of copies instead of a hundred.

## `capacity()` is a deliberate abstraction leak

A set means "each value at most once, in no particular order." Capacity is
not part of that idea at all; it is an artifact of storing the values in an
array. A set built some other way might have no meaningful capacity.

It is in the interface anyway, so the tests can check that growth works and
so you can watch it happen. That is a real engineering trade, made for
testability, and it is worth knowing you are making it rather than doing it
by accident. The handout asks you what it would mean for a differently-built
`IntSet`.

## If you get stuck

- `git status`-style habit for code: run the public tests after every small
  change, so you always know what you just improved or broke.
- Implement `indexOf` first. Three other functions become one line each.
- For `remove`, remember a set has no order. You do not need to shift
  everything down -- ask what is allowed to move.
- If your copy constructor compiles but the deep-copy test fails, you almost
  certainly copied the pointer instead of the values.
- Build with `-fsanitize=address` yourself while developing. It will tell you
  about a double free immediately instead of at grading time.
