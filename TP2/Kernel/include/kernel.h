#ifndef KERNEL_H
#define KERNEL_H

#define KB(s) ((s) << 10)
#define MB(s) (KB(s) << 10)

#define STACK_SIZE KB(128)

#define FLAGS_INT_BIT (1<<9)

#define QUANTUM  	5

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

extern uint64_t pit_timer;
	
#define kdebug(str) (_kdebug("["), kdebug_base(pit_timer, 10), _kdebug(" ms] "__FILE__ ":" STR(__LINE__) ": " str))
#define kdebugs(str) (kdebug(""), _kdebug(str), kdebug_nl())

void _kdebug(const char* s);
void kdebug_char(char c);
void kdebug_base(uint64_t value, uint32_t base);
void kdebug_nl();

void active_screensaver();

void reschedule();

void intson();
void intsoff();
void halt();

//void gdt_init();

uint64_t get_ms_since_boot();

//Tareas del kernel
void screensaver_init();
void screensaver_set_wait(uint64_t s);
void screensaver_trigger();

uint64_t get_flags();

//Implementacion segun mtask
bool kset_ints(bool ints);

void kbd_run_handler(dka_handler handler, uint64_t scode, uint64_t cr3);

void switch_u2k();
void switch_k2u();

#endif
