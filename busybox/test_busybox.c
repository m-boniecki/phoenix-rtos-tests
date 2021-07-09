#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <time.h>

int main(int argc, char **argv)
{
	int ret;

	if ((ret = vfork()) < 0) {
		fprintf(stderr, "vfork function failed \n");
		return 1;
	}
	else if (!ret) {
		execv("/bin/posixsrv", argv);
		fprintf(stderr, "exec function failed \n");
		return 1;
	}
	printf("Posixsrv runned in background\n");
	usleep(500000);

	if ((ret = system("cd /bin/testsuite/ && export PATH=/bin:/sbin:/usr/bin:/usr/sbin && ./runtest")) < 0) {
		fprintf(stderr, "system function failed \n");
		return 1;
	}
	// system("echo -ne \"qwe\nasd\nzxc\n\" > input");
	// system("echo -ne \"asd\" | eval \"diff -u - input\"");

	printf("\nruntest done\n\n");

	return 0;
}
