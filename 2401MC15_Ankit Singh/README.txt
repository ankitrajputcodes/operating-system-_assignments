Name: Ankit Singh
Roll number: 2401MC15
Set: Operating System Lab Examination (MA3105), 17 November 2026

Question 1:
Added an eight-entry kernel semaphore table protected by a spinlock. The sem_wait
implementation sleeps while the count is zero and sem_post increments the count
and wakes waiting processes. Added syscall plumbing and synctest.c, which prints
the strict ABC sequence for five rounds.

Question 2:
Added a free physical page counter to kalloc/free and the freepages system call.
Added va2pa, which walks the calling process page table and rejects unmapped,
kernel-only, or inaccessible pages. memtest.c exercises both calls and verifies
the copied global page and the post-wait free-page count.

Validation:
The changed C files pass i386-targeted Clang syntax checks. Full xv6/QEMU
execution could not be performed on this Mac because the i386-jos-elf cross
compiler is not installed; QEMU is present. No Git commit or push was made.
