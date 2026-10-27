# Lab 9 Submission - xv6 Virtual Memory

**Student:** Ankit Singh  
**Roll number:** 2401MC15

This submission implements the three questions shown in Assignment 9 for MIT `xv6-public` revision `eeb7b415dbcb12cc362d0783e41c3d1f44066b17`.

## Contents

- `diffs/xv6_lab9.diff`: documented kernel and syscall changes, based on the Lab 8 patch.
- `tests/pttest.c`: prints process mappings at startup and after growing/touching four heap pages.
- `tests/faulttest.c`: reserves 32 pages, touches five distinct pages, verifies the handled fault count, and checks an invalid child access.
- `tests/pagerel.c`: runs FIFO and LRU for a frame count and optional reference values; with no reference arguments it uses the assignment string and prints the 3/4/5-frame comparison.
- Lab 8 tests and `meminfo.h`, which provide shared lazy-allocation and memory-stat checks.
- `logs/` and `screenshots/`: captured QEMU serial output for the three questions, plus a lazy-allocation regression log.
- `Lab9_write_up.pdf`: one-page explanation of the changes and observed results.

## Apply and run

Use the xv6-public revision above, then apply the patch and copy the test sources into the xv6 root:

```sh
git checkout eeb7b415dbcb12cc362d0783e41c3d1f44066b17
git apply /path/to/lab9_submission/diffs/xv6_lab9.diff
cp /path/to/lab9_submission/tests/* .
make TOOLPREFIX=i686-elf- QEMU=qemu-system-i386 \
  CFLAGS='-fno-pic -static -fno-builtin -fno-strict-aliasing -O2 -Wall -MD -ggdb -m32 -Werror -Wno-error=array-bounds -Wno-error=infinite-recursion -Wno-error=unused-but-set-variable -fno-omit-frame-pointer -fno-stack-protector -fno-pie -no-pie' \
  qemu-nox CPUS=1
```

At the xv6 shell run `pttest`, `faulttest`, `lazytest`, and `pagerel`. `pagerel 3` runs the supplied assignment sequence for three frames; `pagerel 3 7 0 1 ...` accepts a custom sequence.

## Observed results

- `pttest` showed 2 valid user pages at startup and 6 after touching four newly reserved pages.
- `faulttest` reported exactly 5 handled lazy faults; an out-of-range child was killed while its parent continued.
- `pagerel` reported 15 FIFO faults and 12 LRU faults with 3 frames. The 3/4/5-frame fault counts were FIFO 15/10/9 and LRU 12/8/7.
- `lazytest` confirmed reservation itself used no physical pages, sparse touches allocated on demand, and invalid/guard accesses killed only the child.
