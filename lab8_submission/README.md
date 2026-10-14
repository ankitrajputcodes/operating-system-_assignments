# Lab 8 Submission — Memory Management in xv6

**Student:** Ankit Singh  
**Roll number:** 2401MC15

This submission implements the four questions fully described in the supplied `OS_Lab_Assignment-8.pdf`: virtual-to-physical translation, physical-memory statistics, page-table printing, and lazy heap allocation. The PDF's final checklist also names `cowtest`, `protecttest`, `shmtest`, and `pagerepl` (Questions 5–8), but the attached PDF does not include those questions' descriptions. Those parts are therefore not claimed as implemented; please provide the missing pages if they are required.

## Contents

- `tests/`: `vatest.c`, `memtest.c`, `vmprinttest.c`, and `lazytest.c`, plus the shared `meminfo.h` interface.
- `diffs/xv6_lab8.diff`: kernel and syscall changes against MIT `xv6-public` commit `eeb7b415dbcb12cc362d0783e41c3d1f44066b17`.
- `logs/`: QEMU serial output for each test.
- `screenshots/`: readable PNG renderings of those captured serial sessions.
- `Lab8_write_up.pdf`: design notes and observed results for Questions 1–4.

## Apply and run

Start from the xv6-public revision used for the Lab 7 submission:

```sh
git clone https://github.com/mit-pdos/xv6-public.git xv6-public
cd xv6-public
git checkout eeb7b415dbcb12cc362d0783e41c3d1f44066b17
cp /path/to/lab8_submission/tests/* .
git apply /path/to/lab8_submission/diffs/xv6_lab8.diff
make TOOLPREFIX=i686-elf- QEMU=qemu-system-i386 \
  CFLAGS='-fno-pic -static -fno-builtin -fno-strict-aliasing -O2 -Wall -MD -ggdb -m32 -Werror -Wno-error=array-bounds -Wno-error=infinite-recursion -Wno-error=unused-but-set-variable -fno-omit-frame-pointer -fno-stack-protector -fno-pie -no-pie' \
  qemu-nox CPUS=1
```

At the xv6 shell, run `vatest`, `memtest`, `vmprinttest`, and `lazytest`.

## Results at a glance

- `vatest` reports PDI/PTI/offset and P/W/U flags, rejects `0x7fffffff`, and shows a forked child mapping the same global VA to a different PA.
- `memtest` observes free pages fall from 56,789 to 56,787 after two heap pages are touched; after the child is reaped, free pages return to 56,787.
- `vmprinttest` identifies the stack guard as a present PTE with `U=0`, and shows the added heap mappings and the mapping removed by shrink.
- `lazytest` reserves 32 MB without reducing free pages, allocates on sparse touches, exercises kernel `write` and `read` buffers, checks a forked sparse page, and shows out-of-range and guard-page faults kill only the child.
