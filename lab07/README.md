# CSCI 41 - Lab 07: Linked Lists

A `CircularPlaylist`: a circular, doubly linked list of song titles with a
cursor marking the current song.

```bash
mkdir -p build
cmake -S . -B build
cmake --build build
./build/lab07_tests     # the public self-check
./build/lab07           # the demo, not graded
```

In CLion: open this folder, pick the `lab07_tests` target, run it.

## What you edit

`src/playlist.cpp`, and nothing else. `include/playlist.h` states exactly
what every function must do, including the empty playlist and the playlist
of one. Read it first.

## Three rules that will save you an evening

1. **Test the empty playlist and the playlist of one before anything else.**
   Almost every circular-list bug lives in those two cases. A ring of one
   points at *itself* in both directions.
2. **A ring has no `nullptr`,** so you cannot end a walk by looking for one.
   Use `do { ... } while (cur != start);`
3. **Move the cursor before you delete what it points at.** A cursor holding
   a freed address is the bug this lab exists to teach.

## Build it with sanitizers from the first function

```bash
g++ -std=c++17 -g -Wall -Wextra -D_GLIBCXX_ASSERTIONS \
    -fsanitize=address -Iinclude
    src/playlist.cpp tests/test_lab07.cpp -o t && ./t
```

**That is how it is graded.** A leak or a double free stops the test run and
costs you marks. `clear()` is where it will bite first: cut the ring open
before you free it.

## Grading

- The public checks in `tests/test_lab07.cpp` are a self-check, not your
  grade. Every KIND of case the private tests look at appears there at least
  once, so a broken category shows up before you submit.
- Private tests: 25 tests, 4 points each, 100 total, built the same way
  against your `src/playlist.cpp`.
- An unimplemented function throws, which fails only its own tests.

## Submitting

Your repository IS the submission. Put your work in a `lab07/` folder inside
your `csci41-f26-labs` repository, commit as you go, and push.

- At least three meaningful commits.
- You are graded on what is PUSHED when the deadline passes.
- No `build/` folder in the repository.
