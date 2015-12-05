#include <stdint.h>
#include "video.h"
#include "mem.h"
#include "string.h"
#include "types.h"

#ifndef TASK_H
#define TASK_H

typedef struct {
	uint64_t gs;
	uint64_t fs;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;

	//iretq hook
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;

	uint64_t base;
} __attribute__((packed)) context_t;

// typedef struct task_t {
// 	struct task_t *next;
// 	struct task_t *join;

// 	void *stack;

// 	char *name;
// 	pid_t pid;
// 	task_state_t state;
// 	uint8_t console;
// } task_t;

void task_init();
task_t *task_create(task_entry_point func, const char* name, int argc, char** argv);
void task_remove(task_t *task);
void task_schedule_removal(task_t *task);

void task_ready(task_t *task);
void task_pause(task_t *task);
void task_sleep(task_t *task, uint64_t ms);
uint64_t task_join(task_t *task, task_t *other);

void task_atomic(task_t *task);
void task_unatomic(task_t *task);
uint64_t task_getatomic();
void task_decquantum();
uint64_t task_getquantum();

void task_setconsole(task_t *task, console_t console);
task_t* task_get_current();

task_t* task_get_for_console(console_t console);

void task_next();
task_t* task_get_first();

void task_foreach(void (*func)(task_t*));

void task_set_foreground(task_t *task, console_t console);
task_t* task_get_foreground(console_t console);

task_t* task_find_by_pid(pid_t pid);

void task_errno(uint8_t n);

#endif