/*
	LiquidFire Operating System
		(c) 2004-2020 Darklife Research
	
	Visit: http://darklife.org
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MYSELF		__func__

#define PAGE_COUNT	6
#define PAGE_SIZE	128

typedef signed char	liquid_pid_t;

static char memory_pool[PAGE_COUNT * PAGE_SIZE];
static liquid_pid_t page_dir[PAGE_COUNT];

void mm_dump_page_dir() {
	printf("%s -- { ", MYSELF);
	for(int k = 0; k < PAGE_COUNT; printf("%2d ", page_dir[k++]));
	printf(" }\n");
}

void mm_free(void *p) {
	if(!p) {
		printf("%s -- Trying to free a NULL pointer\n", MYSELF);
		return;
	} else {
		unsigned offset = (int) (p - (void *) &memory_pool);
		unsigned first_page = offset / PAGE_SIZE;
		int pid = page_dir[first_page];
		int last_pid = pid > 0 ? pid * -1 : pid;
		if(page_dir[first_page] == last_pid) {
			page_dir[first_page] = 0;
			return;
		}
		for(; page_dir[first_page] != last_pid; page_dir[first_page] = 0, ++first_page);
		page_dir[first_page] = 0;
	}
}

void *mm_alloc(liquid_pid_t pid, int size) {
	int npages = size / PAGE_SIZE + 1;
	for(unsigned first_page = 0, last_page; first_page < PAGE_COUNT; ++first_page) {
		for(last_page = 0; last_page < npages; ++last_page) {
			if((first_page + last_page) > PAGE_COUNT) {
				printf("%s -- Out of memory #2\n", MYSELF);
				return NULL;
			}
			if(page_dir[first_page + last_page]) {
				break;
			}
		}
		if(last_page == npages) {
			for(last_page = 0; last_page < npages; page_dir[first_page + last_page] = pid, ++last_page);
			page_dir[first_page + last_page - 1] = pid * -1;
			return &memory_pool[first_page * PAGE_SIZE];
		}
	}
	printf("%s -- Out of memory #1\n", MYSELF);
	return NULL;
}

void mm_init() {
	memset(memory_pool, 0, PAGE_COUNT * PAGE_SIZE);
	memset(page_dir, 0, PAGE_COUNT);
	printf("%s -- Starting MM engine\n", MYSELF);
}

int main() {
	mm_init();
	
	void *p1 = mm_alloc(1, 127);
	void *p2 = mm_alloc(2, 128);
	void *p3 = mm_alloc(3, 250);
	void *p4 = mm_alloc(4, 256);
	
	printf("\tp1 = %p\n", p1);
	printf("\tp2 = %p\n", p2);
	printf("\tp3 = %p\n", p3);
	printf("\tp4 = %p\n", p4);
	
	mm_dump_page_dir();
	
	mm_free(p1);
	mm_free(p2);
	mm_free(p4);
	
	mm_dump_page_dir();
	
	return 0;
}
