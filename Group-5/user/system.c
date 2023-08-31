/***********************************************************************
* This file contains functions linking the MPX userland to kernel space.
* You should not need to make any modifications to this file.
***********************************************************************/

#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#include <mpx/serial.h>
#include <mpx/vm.h>

#include <memory.h>
#include <processes.h>
#include <sys_req.h>

/* For R3: How many times each process prints its message */
#define RC_1 1
#define RC_2 2
#define RC_3 3
#define RC_4 4
#define RC_5 5

/* For R3: Constant strings used by all R3 processes */
static char dispatched[] = " dispatched\r\n";
static char after_exit[] = " ran after it was terminated!\r\n";

/* For R5: Pointers to student provided functions */
/* DO NOT SET MANUALLY, CALL sys_set_heap_functions() !!! */
static void * (*malloc_function)(size_t) = NULL;
static int (*free_function)(void *) = NULL;

/***********************************************************************/
/* Issue a request to the kernel. */
/***********************************************************************/
int sys_req(op_code op, ...)
{
	device dev = 0;
	char *buffer = NULL;
	size_t len = 0;

	if (op == READ || op == WRITE) {
		va_list ap;
		va_start(ap, op);
		dev = va_arg(ap, device);
		buffer = va_arg(ap, char *);
		len = va_arg(ap, size_t);
		va_end(ap);
	}

	int ret = 0;
	__asm__ volatile("int $0x60" : "=a"(ret) : "a"(op), "b"(dev), "c"(buffer), "d"(len));

	if (ret == -1 && (op == READ || op == WRITE)) {
		return (op == READ)
			? serial_poll(dev, buffer, len)
			: serial_out(dev, buffer, len);
	}

	return ret;
}

/***********************************************************************/
/* This causes R5 to go into full effect, replacing default heap functions
 * with those implemented by students. */
/***********************************************************************/
void sys_set_heap_functions(void * (*alloc_fn)(size_t), int (*free_fn)(void *))
{
	malloc_function = alloc_fn;
	free_function = free_fn;
}

/* Allocate memory using the student function if available, fallback to kmalloc(). */
void *sys_alloc_mem(size_t size)
{
	return malloc_function ? malloc_function(size) : kmalloc(size, 0, NULL);
}

/* Free memory if a student function is available, otherwise NOP. */
int sys_free_mem(void *ptr)
{
	return free_function ? free_function(ptr) : -1;
}

/***********************************************************************/
/* The Idle process */
/* You must create a System process of lowest priority for this in R3. */
/***********************************************************************/
void sys_idle_process(void)
{
	char msg[] = "IDLE PROCESS EXECUTING.\r\n";
	
	for (;;) {
		sys_req(WRITE, COM1, msg, sizeof(msg));
		sys_req(IDLE);
	}
}

/***********************************************************************/
/* Code common to all R3 processes */
/* DO NOT TRY TO CREATE A PROCESS FOR THIS FUNCTION!!! */
/***********************************************************************/
static void r3_proc(int iterations, const char *procname)
{
	for (int i = 0; i < iterations; i++) {
		sys_req(WRITE, COM1, procname, strlen(procname));
		sys_req(WRITE, COM1, dispatched, strlen(dispatched));
		sys_req(IDLE);
	}
	sys_req(EXIT);
	for (;;) {
		sys_req(WRITE, COM1, procname, strlen(procname));
		sys_req(WRITE, COM1, after_exit, strlen(after_exit));
		sys_req(EXIT);
	}
}

/***********************************************************************/
/* BEGIN R3 PROCESSES -- Create one process per function */
/***********************************************************************/
void proc1(void)
{
	r3_proc(RC_1, __func__);
}

void proc2(void)
{
	r3_proc(RC_2, __func__);
}

void proc3(void)
{
	r3_proc(RC_3, __func__);
}

void proc4(void)
{
	r3_proc(RC_4, __func__);
}

void proc5(void)
{
	r3_proc(RC_5, __func__);
}

/***********************************************************************/
/* BEGIN R6 PROCESSES -- Create processes with these functions to test R6 functionality */
/***********************************************************************/
void comwrite(void)
{
	char msg[] = "comwrite is writing a message.\r\n";
	sys_req(WRITE, COM1, msg, sizeof(msg));
	sys_req(EXIT);
}

void comread(void)
{
	char prompt[] = "comread: please input a message> ";
	sys_req(WRITE, COM1, prompt, sizeof(prompt));

	char buf[100] = { 0 };
	int nread = sys_req(READ, COM1, buf, sizeof(buf));
	sys_req(WRITE, COM1, buf, nread);

	sys_req(EXIT);
}

void iocom25(void)
{
	char msg[] = "iocom25 is writing a message.\r\n";

	for (int i = 0; i < 25; i++) {
		sys_req(WRITE, COM1, msg, sizeof(msg));
		sys_req(IDLE);
	}

	sys_req(EXIT);
}

void iocom(void) {
	char msg[] = "iocom is writing a message.\r\n";
	sys_req(WRITE, COM1, msg, sizeof(msg));

	char msg2[] = "iocom is still writing.\r\n";
	for (;;) {
		sys_req(IDLE);
		sys_req(WRITE, COM1, msg2, sizeof(msg2));
	}

	// this point should never be reached
	sys_req(EXIT);
}
