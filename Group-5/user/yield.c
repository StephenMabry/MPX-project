#include <sys_req.c>

void yield(){
	sys_req(IDLE);
}
