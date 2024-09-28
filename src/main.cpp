#include<iostream>
#include <ctime>
#include<string>
#include <csignal>
#include<cstdio>
#include<cstdint>
#include<cstring>
#include"Disk.h"
using namespace std;
#define DISK_MAXLEN 2560
char Disk[DISK_MAXLEN];

int testFunction() {
	disk dsk(Disk);
	char* path = new char[32];

	//mkdir≤‚ ‘

	dsk.Command_mkdir((char*)("/home"));
	dsk.Command_mkdir((char*)("/boot"));
	dsk.Command_mkdir((char*)("/etc"));
	dsk.Command_mkdir((char*)("/usr"));
	dsk.Command_mkdir((char*)("/bin"));


	dsk.Command_mkdir((char*)("/home/a/a1"));
	dsk.Command_mkdir((char*)("/home/a/a2"));
	dsk.Command_mkdir((char*)("/home/a/a3"));
	dsk.Command_mkdir((char*)("/home/b"));
	printf("\n\n");
	//test ls
	dsk.Command_ls("/");
	dsk.Command_ls("/home");
	dsk.Command_ls("/home/a");
	dsk.Command_ls("/home/a/a1");


	printf("\n\n\n");
	//test delete
	strcpy(path, "/home/a/a1");
	dsk.Command_delete(path);
	dsk.Command_ls("/home/a");
	dsk.Command_ls("/home/a/a1");


	strcpy(path, "/home/a");
	dsk.Command_delete(path);
	dsk.Command_ls("/home/a");
	dsk.Command_ls("/home/a/a1");


	printf("\n\n\n");
	//open≤‚ ‘
	FileStream filePoi;
	strcpy(path,"/home/b/file");
	char data[128] = "When love beckons to you, follow him,Though his ways are hard and steep.";
	int dataLength = strlen(data);

	//–¥
	dsk.Command_open(path,"w+",filePoi);
	printf("Write data:\n\"%s\"\n\n",data);
	dsk.Command_write(filePoi,data,dataLength);
	dsk.Command_close(filePoi);

	//∂¡
	char buf[128];
	dsk.Command_open(path, "r", filePoi);
	dsk.Command_read(filePoi, buf, 128);
	dsk.Command_close(filePoi);

	printf("Read data:\n\"%s\"\n",buf);


	printf("\n\n\n\n");
	return 0;
}
void signalHandler( int signum )
{
	cout << "Interrupt signal (" << signum << ") received.\n";
	printf("Quit by user\n");
	exit(signum);

}
#define ORIGINAL_INPUT_MAX_LENGTH 1024
void simpleShell() {
	signal(SIGINT, signalHandler);
	char commandBuf[ORIGINAL_INPUT_MAX_LENGTH];
	while(true) {
		fgets(commandBuf,ORIGINAL_INPUT_MAX_LENGTH,stdin);



	}
}
int main()
{
	testFunction();
	//simpleShell();
	return 0;
}

