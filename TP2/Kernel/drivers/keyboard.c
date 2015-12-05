//TODO: fix caps-shift for SO

#include <stdint.h>
#include "video.h"
#include "keyboard.h"
#include "mem.h"
#include "types.h"
#include "msgqueue.h"
#include "input.h"
#include "kernel.h"
#include "string.h"

#define FIRST_BIT_ON(c) (0x80 | c)

typedef struct {
	char scancode;
	char ascii;
	char caps;
} scancode_t;

scancode_t keyboard_scancodes[][256] = {
	{	//USA
		{0x00 , NOCHAR, NOCHAR}, //empty,
		{0x01 , NOCHAR, NOCHAR}, //esc
		{0x02 , '1', '!'},
		{0x03 , '2', '@'},
		{0x04 , '3', '#'},
		{0x05 , '4', '$'},
		{0x06 , '5', '%'},
		{0x07 , '6', '^'},
		{0x08 , '7', '&'},
		{0x09 , '8', '*'},
		{0x0A , '9', '('},
		{0x0B , '0', '"'},
		{0x0C , '-', '_'},
		{0x0D , '=', '+'},
		{0x0E , NOCHAR, NOCHAR}, //backspace
		{0x0F , NOCHAR, NOCHAR}, //tab
		{0x10 , 'q', 'Q'},
		{0x11, 'w', 'W'},
		{0x12, 'e', 'E'},
		{0x13, 'r', 'R'},
		{0x14, 't', 'T'},
		{0x15, 'y', 'Y'},
		{0x16, 'u', 'U'},
		{0x17, 'i', 'I'},
		{0x18, 'o', 'O'},
		{0x19, 'p', 'P'},
		{0x1a, '[', '{'},
		{0x1b, ']', '}'},
		{0x1c, '\n', '\n'},//enter
		{0x1d, NOCHAR, NOCHAR},//left ctrl
		{0x1e, 'a', 'A'},
		{0x1f, 's', 'S'},
		{0x20, 'd', 'D'},
		{0x21, 'f', 'F'},
		{0x22, 'g', 'G'},
		{0x23, 'h', 'H'},
		{0x24, 'j', 'J'},
		{0x25, 'k', 'K'},
		{0x26, 'l', 'L'},
		{0x27, ';', ':'},
		{0x28, '\'', '"'},
		{0x29, '`', '~'},
		{0x2a, NOCHAR, NOCHAR},//left shift
		{0x2b, '\\', '|'},
		{0x2c, 'z', 'Z'},
		{0x2d, 'x', 'X'},
		{0x2e, 'c', 'C'},
		{0x2f, 'v', 'V'},
		{0x30, 'b', 'B'},
		{0x31, 'n', 'N'},
		{0x32, 'm', 'M'},
		{0x33, ',', '<'},
		{0x34, '.', '>'},
		{0x35, '/', '?'},
		{0x36, NOCHAR, NOCHAR},//right shift
		{0x37, '*', NOCHAR},//keypad *
		{0x38, NOCHAR, NOCHAR},//left alt
		{0x39, ' ', ' '},
		{0x3a, NOCHAR, NOCHAR},//caps
		{0x3b, NOCHAR, NOCHAR},//f1
		{0x3c, NOCHAR, NOCHAR},//f2
		{0x3d, NOCHAR, NOCHAR},//f3
		{0x3e, NOCHAR, NOCHAR},//f4
		{0x3f, NOCHAR, NOCHAR},//f5
		{0x40, NOCHAR, NOCHAR},//f6
		{0x41, NOCHAR, NOCHAR},//f7
		{0x42, NOCHAR, NOCHAR},//f8
		{0x43, NOCHAR, NOCHAR},//f9
		{0x44, NOCHAR, NOCHAR},//f10
		{0x45, NOCHAR, NOCHAR},//num
		{0x46, NOCHAR, NOCHAR},//scroll
		{0x47, '7', NOCHAR},//keypad 7
		{0x48, '8', NOCHAR},//keypad 8
		{0x49, '9', NOCHAR},//keypad 9
		{0x4a, '-', NOCHAR},//keypad -
		{0x4b, NOCHAR, NOCHAR},//keypad 4
		{0x4c, '5', NOCHAR},//keypad 5
		{0x4d, NOCHAR, NOCHAR},//keypad 6
		{0x4e, '+', NOCHAR},//keypad +
		{0x4f, '1', NOCHAR},//keypad 1
		{0x50, '2', NOCHAR},//keypad 2
		{0x51, '3', NOCHAR},//keypad 3
		{0x52, '0', NOCHAR},//keypad 0
		{0x53, '.', NOCHAR},//keypad 0
		{0x57, NOCHAR, NOCHAR},//f11
		{0x58, NOCHAR, NOCHAR}//f12
	},
	{	//LATIN
		{0x00 , NOCHAR, NOCHAR}, //empty,
		{0x01 , NOCHAR, NOCHAR}, //esc
		{0x02 , '1', '!'},
		{0x03 , '2', '"'},
		{0x04 , '3', '#'},
		{0x05 , '4', '$'},
		{0x06 , '5', '%'},
		{0x07 , '6', '&'},
		{0x08 , '7', '/'},
		{0x09 , '8', '('},
		{0x0A , '9', ')'},
		{0x0B , '0', '='},
		{0x0C , '-', '?'},
		{0x0D , NOCHAR, NOCHAR},
		{0x0E , NOCHAR, NOCHAR}, //backspace
		{0x0F , NOCHAR, NOCHAR}, //tab
		{0x10 , 'q', 'Q'},
		{0x11, 'w', 'W'},
		{0x12, 'e', 'E'},
		{0x13, 'r', 'R'},
		{0x14, 't', 'T'},
		{0x15, 'y', 'Y'},
		{0x16, 'u', 'U'},
		{0x17, 'i', 'I'},
		{0x18, 'o', 'O'},
		{0x19, 'p', 'P'},
		{0x1a, NOCHAR, NOCHAR},
		{0x1b, '+', '*'},
		{0x1c, '\n', '\n'},//enter
		{0x1d, NOCHAR, NOCHAR},//left ctrl
		{0x1e, 'a', 'A'},
		{0x1f, 's', 'S'},
		{0x20, 'd', 'D'},
		{0x21, 'f', 'F'},
		{0x22, 'g', 'G'},
		{0x23, 'h', 'H'},
		{0x24, 'j', 'J'},
		{0x25, 'k', 'K'},
		{0x26, 'l', 'L'},
		{0x27, ';', ':'},
		{0x28, '\'', '"'},
		{0x29, '|', NOCHAR},
		{0x2a, NOCHAR, NOCHAR},//left shift
		{0x2b, '}', ']'},
		{0x2c, 'z', 'Z'},
		{0x2d, 'x', 'X'},
		{0x2e, 'c', 'C'},
		{0x2f, 'v', 'V'},
		{0x30, 'b', 'B'},
		{0x31, 'n', 'N'},
		{0x32, 'm', 'M'},
		{0x33, ',', ';'},
		{0x34, '.', ':'},
		{0x35, '-', '_'},
		{0x36, NOCHAR, NOCHAR},//right shift
		{0x37, '*', NOCHAR},//keypad *
		{0x38, NOCHAR, NOCHAR},//left alt
		{0x39, ' ', ' '},
		{0x3a, NOCHAR, NOCHAR},//caps
		{0x3b, NOCHAR, NOCHAR},//f1
		{0x3c, NOCHAR, NOCHAR},//f2
		{0x3d, NOCHAR, NOCHAR},//f3
		{0x3e, NOCHAR, NOCHAR},//f4
		{0x3f, NOCHAR, NOCHAR},//f5
		{0x40, NOCHAR, NOCHAR},//f6
		{0x41, NOCHAR, NOCHAR},//f7
		{0x42, NOCHAR, NOCHAR},//f8
		{0x43, NOCHAR, NOCHAR},//f9
		{0x44, NOCHAR, NOCHAR},//f10
		{0x45, NOCHAR, NOCHAR},//num
		{0x46, NOCHAR, NOCHAR},//scroll
		{0x47, '7', NOCHAR},//keypad 7
		{0x48, '8', NOCHAR},//keypad 8
		{0x49, '9', NOCHAR},//keypad 9
		{0x4a, '-', NOCHAR},//keypad -
		{0x4b, '4', NOCHAR},//keypad 4
		{0x4c, '5', NOCHAR},//keypad 5
		{0x4d, '6', NOCHAR},//keypad 6
		{0x4e, '+', NOCHAR},//keypad +
		{0x4f, '1', NOCHAR},//keypad 1
		{0x50, '2', NOCHAR},//keypad 2
		{0x51, '3', NOCHAR},//keypad 3
		{0x52, '0', NOCHAR},//keypad 0
		{0x53, NOCHAR, NOCHAR}, //keypad 0
		{0x54, '.', NOCHAR},//keypad 0
		{0x55, '.', NOCHAR},//keypad 0
		{0x56, '<', '>'},//keypad 0

		{0x57, NOCHAR, NOCHAR},//f11
		{0x58, NOCHAR, NOCHAR}//f12
	}
};

static keyboard_distrib keyboard_distribution = KEYBOARD_USA;

static kstatus keyboard_status = {.caps = FALSE, .ctrl = FALSE, .alt =  FALSE, .shift = FALSE};

static msgqueue_t* kbdqueue;
static task_t *kbdtask;

static dka_catch* dka_catched_scancodes[MAX_KBD_HANDLERS] = {NULL};

static void keyboard_caps_handler(uint64_t s) {
	keyboard_status.caps = !keyboard_status.caps;
}

static void keyboard_shift_handler(uint64_t s) {
	keyboard_status.shift = !keyboard_status.shift;
}

static void keyboard_backspace_handler(uint64_t s) {

	if (input_size() == 0) {
		kdebug("Cola vacia. No se desencola nada.\n");
		return;
	}

	screen_t *screen = get_screen(video_current_console());

	if (screen->column == 0) {
		screen->column = SCREEN_WIDTH - 1;
		screen->row--;
	} else {
		screen->column--;
	}

	input_undo();

	video_write_char_at(video_current_console(), ' ', screen->row, screen->column);

	video_update_cursor();
}

static inline bool keyboard_is_scode_in_range(uint64_t range, uint64_t scode) {

	uint32_t low, high;

	// kdebug("Full rango: 0x");
	// kdebug_base(range, 16);
	// _kdebug("\t");

	low = (uint32_t) range;
	high = (uint32_t) (range >> 32);

	// kdebug("Analizando rango: 0x");
	// kdebug_base(low, 16);
	// _kdebug(" <= ");
	// kdebug_base(scode, 16);
	// _kdebug(" <= 0x");
	// kdebug_base(high, 16);

	// if (low <= scode && scode <= high) {
	// 	_kdebug("\t TRUE");
	// } else {
	// 	_kdebug("\t FALSE");
	// }

	// kdebug_nl();

	return (low <= scode && scode <= high);
}

static bool keyboard_run_handlers(uint64_t scode) {

	bool catched = FALSE;

	for (uint32_t i = 0; i < MAX_KBD_HANDLERS; i++) {

		if (dka_catched_scancodes[i] == NULL) {
			continue;
		}

		bool is_wildcard = (dka_catched_scancodes[i]->flags & KEYBOARD_WILDCARD);
		bool is_range = (dka_catched_scancodes[i]->flags & KEYBOARD_RANGE);
		bool is_all_consoles = (dka_catched_scancodes[i]->flags & KEYBOARD_ALLCONSOLES);
		bool is_console_equal;

		if (dka_catched_scancodes[i]->task == NULL) {
			is_console_equal = (video_current_console() == KERNEL_CONSOLE);

		} else {
			is_console_equal = (video_current_console() == dka_catched_scancodes[i]->task->console);
		}

		if (!is_wildcard) {
			if (is_range) {
				if (!keyboard_is_scode_in_range(dka_catched_scancodes[i]->scancode, scode)) {
					continue;
				}
			} else {
				if (dka_catched_scancodes[i]->scancode != scode) {
					continue;
				}
			}
		}

		if (!is_all_consoles && !is_console_equal) {
			continue;
		}

		kdebug("Ejecutando handler \"");
		_kdebug(dka_catched_scancodes[i]->name);
		_kdebug("\" en consola: ");
		kdebug_base((dka_catched_scancodes[i]->task == NULL) ? KERNEL_CONSOLE : dka_catched_scancodes[i]->task->console, 10);
		kdebug_nl();

		uint64_t cr3 = 0;

		if (dka_catched_scancodes[i]->task != NULL) {
			cr3 = dka_catched_scancodes[i]->task->cr3;
		}

		kbd_run_handler(dka_catched_scancodes[i]->handler, scode, cr3);

		//dka_catched_scancodes[i]->handler(scode);

		if (dka_catched_scancodes[i]->flags & KEYBOARD_IGNORE) {
			continue;
		} else {
			catched = TRUE;
		}
	}

	if (catched) {
		return TRUE;
	}


	return FALSE;
}

static  __attribute__ ((noreturn)) uint64_t keyboard_task(int argc, char** argv) {

	while (TRUE) {

		uint64_t *scancodeptr, scancode;
		int reps;
		char c;

		while (msgqueue_size(kbdqueue) == 0);

		scancodeptr = msgqueue_deq(kbdqueue);
		scancode = *scancodeptr;
		free(scancodeptr);

		switch (scancode) {
		case 0xE0:
			reps = 1;
			break;

		case 0xE1:
			reps = 2;
			break;

		default:
			reps = 0;
			break;
		}

		for (int i = 0; i < reps; i++) {
			scancodeptr = msgqueue_deq(kbdqueue);

			scancode = (scancode << 8 ) | (*scancodeptr);
			free(scancodeptr);
		}

		if (keyboard_run_handlers(scancode)) {
			kdebug("Scancode atrapado por un handler\n");
			continue;
		}

		scancode_t t = keyboard_scancodes[keyboard_distribution][scancode];

		if (t.ascii == NOCHAR) {
			//kdebug("Recibido caracter NOCHAR\n");
			continue;
		}

		if (keyboard_status.caps || keyboard_status.shift) {
			c = t.caps;
		} else {
			c = t.ascii;
		}

		input_add(c);

		if (c != '\n') {
			video_write_char(video_current_console(), c);
			video_update_cursor();
		}

	}
}

void keyboard_init() {

	kbdqueue = msgqueue_create(KEYBOARD_BUFFER_SIZE);

	//TODO caso de shift
	keyboard_catch(0x3A, keyboard_caps_handler, NULL, KEYBOARD_ALLCONSOLES, "caps");
	keyboard_catch(0x2A, keyboard_caps_handler, NULL, KEYBOARD_ALLCONSOLES, "caps");
	keyboard_catch(0x36, keyboard_caps_handler, NULL, KEYBOARD_ALLCONSOLES, "caps");
	keyboard_catch(FIRST_BIT_ON(0x2A), keyboard_caps_handler, NULL, KEYBOARD_ALLCONSOLES, "caps");
	keyboard_catch(FIRST_BIT_ON(0x36), keyboard_caps_handler, NULL, KEYBOARD_ALLCONSOLES, "caps");

	keyboard_catch(0x0E, keyboard_backspace_handler, NULL, KEYBOARD_ALLCONSOLES, "caps");

	kbdtask = task_create(keyboard_task, "keyboard", 0, NULL);

	task_setconsole(kbdtask, 0);
	task_ready(kbdtask);
}

void keyboard_irq_handler(uint64_t s) {
	msgqueue_add(kbdqueue, &s, sizeof(uint64_t));
}

void keyboard_change_console(console_t console) {
	kbdtask->console = console;
}

int32_t keyboard_catch(uint64_t scancode, dka_handler handler, task_t *task, uint64_t flags, char* name) {

	uint32_t index = 0;

	dka_catch* tmp = (dka_catch*)calloc(sizeof(dka_catch));

	tmp->scancode = scancode;
	tmp->handler = handler;
	tmp->task = task;
	tmp->flags = flags;

	tmp->name = malloc(strlen(name) + 1);
	strcpy(tmp->name, name);

	while (index < MAX_KBD_HANDLERS && dka_catched_scancodes[index] != NULL) {
		index++;
	}

	if (index == MAX_KBD_HANDLERS) {
		kdebug("Se lleno el almacen de handlers!\n");
		return -1;
	}

	dka_catched_scancodes[index] = tmp;


	if (task != NULL) {

		uint32_t i;

		for (i = 0; i < MAX_TASK_KBD_HANDLERS; i++) {
			if (task->kbdhandlers[i] == -1) {
				task->kbdhandlers[i] = index;
				break;
			}
		}

		if (i == MAX_KBD_HANDLERS) {
			kdebug("La tarea: '");
			_kdebug(task->name);
			_kdebug("' pid=");
			kdebug_base(task->pid, 10);
			_kdebug(" alcanzo el maximo de handlers del teclado. Abortando!");
			kdebug_nl();
			free(tmp->name);
			free(tmp);
			return -1;
		}

		kdebug("La tarea: '");
		_kdebug(task->name);
		_kdebug("' pid=");
		kdebug_base(task->pid, 10);
		_kdebug(" reservo el handler con indice: ");
		kdebug_base(index, 10);
		kdebug_nl();
	}

	return index;
}

void keyboard_clear_handler(uint32_t index) {

	task_t *task = dka_catched_scancodes[index]->task;

	if (task != NULL) {
		for (uint32_t i = 0; i < MAX_TASK_KBD_HANDLERS; i++) {
			if (task->kbdhandlers[i] == index) {
				task->kbdhandlers[i] = -1;
				break;
			}
		}

		kdebug("La tarea: '");
		_kdebug(task->name);
		_kdebug("' pid=");
		kdebug_base(task->pid, 10);
		_kdebug(" libero el handler con indice: ");
		kdebug_base(index, 10);
		kdebug_nl();
	}

	free(dka_catched_scancodes[index]->name);
	free(dka_catched_scancodes[index]);
	dka_catched_scancodes[index] = NULL;
}

void keyboard_clear_from_task(task_t *task) {

	for (uint32_t i = 0; i < MAX_TASK_KBD_HANDLERS; i++) {
		if (task->kbdhandlers[i] != -1) {
			keyboard_clear_handler(task->kbdhandlers[i]);
		}
	}
}

void keyboard_set_distribution(keyboard_distrib d) {
	keyboard_distribution = d;
}
