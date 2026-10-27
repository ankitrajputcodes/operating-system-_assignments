#ifndef XV6_MEMINFO_H
#define XV6_MEMINFO_H
struct meminfo {
  int total_pages;
  int free_pages;
  int process_pages;
  int page_table_pages;
};
#endif
