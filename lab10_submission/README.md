# Lab 10 Submission - xv6 Memory Management and Address Translation

**Student:** Ankit Singh
**Roll number:** 2401MC15

This submission implements the two questions in Operating Systems Assignment 10 on the same MIT `xv6-public` revision used by Lab 9 (`eeb7b415dbcb12cc362d0783e41c3d1f44066b17`).

## Contents

- `diffs/xv6_lab10.diff`: kernel, syscall, Makefile, and test-program changes to apply after the Lab 9 patch.
- `tests/memtest.c`: allocates and touches four heap pages, displays allocator counters, releases the pages, and displays the counters again.
- `tests/vttest.c`: translates two mapped heap addresses and checks that an unmapped address returns `-1`.

## Apply

Start from the revision above, apply the Lab 9 patch, then apply this patch:

```sh
git apply /path/to/lab9_submission/diffs/xv6_lab9.diff
git apply --unidiff-zero /path/to/lab10_submission/diffs/xv6_lab10.diff
```

The patch adds `_memtest` and `_vttest` to the xv6 user-program list. At the xv6 shell, run `memtest` and `vttest` to inspect the reported values.

## Implementation notes

- The allocator tracks successful allocations, releases, current allocated pages, and free pages under its existing lock. Pages placed on the free list during allocator initialization are excluded from the release counter.
- `memstats()` prints the four counters from the kernel.
- `vtop(int)` uses `walkpgdir()` and returns the physical frame address plus the virtual-page offset. It returns `-1` for invalid or unmapped user addresses.

The source changes are provided; runtime output is not claimed in this submission.
