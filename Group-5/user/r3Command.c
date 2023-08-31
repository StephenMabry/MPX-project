#include <sys_req.h>
#include <processes.h>
#include <pcb.h>
#include <context.h>



void yield(){
	sys_req(IDLE);
}

void loadR3(){
	pcb* pc1 = pcb_setup("process_1",0,1);
	context* ctx = (context*) pc1->stackPtr;

	ctx->eax = 0x00;
	ctx->ebx = 0x00;
	ctx->ecx = 0x00;
	ctx->edx = 0x00;
	ctx->esi = 0x00;
	ctx->edi = 0x00;
	ctx->ebp = (uint32_t)pc1->stackPtr;
	ctx->esp = (uint32_t)pc1->stackPtr;
	ctx->eip = (uint32_t)proc1;
	ctx->cs = 0x08;
	ctx->ds = 0x10;
	ctx->es = 0x10;
	ctx->fs = 0x10;
	ctx->gs = 0x10;
	ctx->ss = 0x10;
	ctx->eflags = 0x0202;
	pcb_insert(pc1);

	pcb* pc2 = pcb_setup("process_2",0,1);
	ctx = (context*) pc2->stackPtr;
	
        ctx->eax = 0x00;
        ctx->ebx = 0x00;
        ctx->ecx = 0x00;
        ctx->edx = 0x00;
        ctx->esi = 0x00;
        ctx->edi = 0x00;
        ctx->ebp = (uint32_t) pc2->stackPtr;
        ctx->esp = (uint32_t) pc2->stackPtr;
        ctx->eip = (uint32_t) proc2;
        ctx->cs = 0x08;
        ctx->ds = 0x10;
        ctx->es = 0x10;
        ctx->fs = 0x10;
        ctx->gs = 0x10;
        ctx->ss = 0x10;
        ctx->eflags = 0x0202;
        pcb_insert(pc2);

	pcb* pc3 = pcb_setup("process_3",0,1);
	ctx = (context*) pc3->stackPtr;

	ctx->eax = 0x00;
	ctx->ebx = 0x00;
        ctx->ecx = 0x00;
        ctx->edx = 0x00;
        ctx->esi = 0x00;
        ctx->edi = 0x00;
        ctx->ebp = (uint32_t) pc3->stackPtr;
        ctx->esp = (uint32_t) pc3->stackPtr;
        ctx->eip = (uint32_t) proc3;
        ctx->cs = 0x08;
        ctx->ds = 0x10;
	ctx->es = 0x10;
        ctx->fs = 0x10;
        ctx->gs = 0x10;
        ctx->ss = 0x10;
        ctx->eflags = 0x0202;
	pcb_insert(pc3);

	pcb* pc4 = pcb_setup("process_4",0,1);
	ctx = (context*) pc4->stackPtr;

	ctx->eax = 0x00;
        ctx->ebx = 0x00;
        ctx->ecx = 0x00;
        ctx->edx = 0x00;
        ctx->esi = 0x00;
        ctx->edi = 0x00;
        ctx->ebp = (uint32_t) pc4->stackPtr;
        ctx->esp = (uint32_t) pc4->stackPtr;
        ctx->eip = (uint32_t) proc4;
        ctx->cs = 0x08;
        ctx->ds = 0x10;
        ctx->es = 0x10;
        ctx->fs = 0x10;
        ctx->gs = 0x10;
        ctx->ss = 0x10;
        ctx->eflags = 0x0202;
        pcb_insert(pc4);

	pcb* pc5 = pcb_setup("process_5",0,1);
	ctx = (context*) pc5->stackPtr;
	ctx->eax = 0x00;
	ctx->ebx = 0x00;
        ctx->ecx = 0x00;
	ctx->edx = 0x00;
        ctx->esi = 0x00;
        ctx->edi = 0x00;
        ctx->ebp = (uint32_t) pc5->stackPtr;
        ctx->esp = (uint32_t) pc5->stackPtr;
        ctx->eip = (uint32_t) proc5;
        ctx->cs = 0x08;
        ctx->ds = 0x10;
        ctx->es = 0x10;
        ctx->fs = 0x10;
        ctx->gs = 0x10;
        ctx->ss = 0x10;
        ctx->eflags = 0x0202;
	pcb_insert(pc5);
	
}
