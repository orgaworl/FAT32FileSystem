#include<iostream>
#include <ctime>
#include<string>
#include <csignal>
#include<cstdio>
#include<cstdint>
#include<cstring>
#include <fstream>
#include<map>
#include<unordered_map>
#include<set>
#include<list>
#include <valarray>

#include"Disk.h"
using namespace std;
#define INPUT_BUF_LEN 1024
#define DISK_MAXLEN 2560
char Disk[DISK_MAXLEN];
#include<vector>

int testFunction() {
    fstream fstr("/debug/diskFile", ios::binary);

    disk dsk(Disk);
    char *path = new char[32];

    //mkdir≤‚ ‘

    dsk.Command_mkdir((char *) ("/home"));
    dsk.Command_mkdir((char *) ("/boot"));
    dsk.Command_mkdir((char *) ("/etc"));
    dsk.Command_mkdir((char *) ("/usr"));
    dsk.Command_mkdir((char *) ("/bin"));


    dsk.Command_mkdir((char *) ("/home/a/a1"));
    dsk.Command_mkdir((char *) ("/home/a/a2"));
    dsk.Command_mkdir((char *) ("/home/a/a3"));
    dsk.Command_mkdir((char *) ("/home/b"));
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
    strcpy(path, "/home/b/file");
    char data[128] = "When love beckons to you, follow him,Though his ways are hard and steep.";
    int dataLength = strlen(data);

    //–¥
    dsk.Command_open(path, "w+", filePoi);
    printf("Write data:\n\"%s\"\n\n", data);
    dsk.Command_write(filePoi, data, dataLength);
    dsk.Command_close(filePoi);

    //∂¡
    char buf[128];
    dsk.Command_open(path, "r", filePoi);
    dsk.Command_read(filePoi, buf, 128);
    dsk.Command_close(filePoi);

    printf("Read data:\n\"%s\"\n", buf);

    printf("\n\n\n\n");


    fstr.close();
    return 0;
}

void signalHandler(int signum) {
    cout << "Interrupt signal (" << signum << ") received.\n";
    printf("Quit by user\n");
    exit(signum);
}

#define ORIGINAL_INPUT_MAX_LENGTH 1024

//
//
// class shellCommandHandler {
// public:
// 	static char retBuf[1024];
// 	static map<string,int(*)(disk*dsk)>handleTable;
// 	shellCommandHandler() {
// 		handleTable["touch"]=handle_touch;
// 		handleTable["mkdir"]=handle_mkdir;
// 		handleTable["ls"]=handle_ls;
// 		handleTable["cd"]=handle_cd;
// 		handleTable["cat"]=handle_cat;
// 		handleTable["echo"]=handle_echo;
// 		// handleTable[">>"]=handle_append;
// 		// handleTable[">"]=handle_write;
// 		handleTable["rm"]=handle_rm;
// 		handleTable["mv"]=handle_mv;
// 	}
// 	static void doOperation(disk*dsk, string command) {
// 		if(handleTable.find(command)==handleTable.end()) {
// 			strcpy(retBuf,"not supported command!\n");
// 		}
// 		else {
// 			//handleTable[command](dsk);
// 		}
// 	}
//
// 	static int handle_cd(disk*dsk) {
// 		// retBuf.clear();
// 		// char tmpBuf[32];
// 		// scanf("%s",tmpBuf);
// 		return 0;
// 	}
// 	static int handle_ls(disk*dsk) {
// 		retBuf[0]=0;
// 		char path[32];
// 		scanf("%s",path);
// 		dsk->Command_ls(path);
//
// 		return 0;
// 	}
// 	static int handle_mkdir(disk*dsk) {
// 		retBuf[0]=0;
// 		char path[32];
// 		scanf("%s",path);
// 		dsk->Command_mkdir(path);
// 		return 0;
// 	}
// 	static int handle_rm(disk*dsk) {
// 		retBuf[0]=0;
// 		char path[32];
// 		scanf("%s",path);
// 		dsk->Command_delete(path);
// 		return 0;
// 	}
// 	static int handle_append(disk*dsk) {
// 		return 0;
// 	}
// 	static int handle_write(disk*dsk) {
// 		return 0;
// 	}
// 	static int handle_mv(disk*dsk) {
// 		retBuf[0]=0;
// 		return 0;
// 	}
// 	static int handle_echo(disk*dsk) {
// 		retBuf[0]=0;
// 		char path[32];
// 		scanf("%s",path);
// 		strcpy(retBuf,path);
//
//
// 		// //open≤‚ ‘
// 		// FileStream filePoi;
// 		// strcpy(path,"/home/b/file");
// 		// char data[128] = "When love beckons to you, follow him,Though his ways are hard and steep.";
// 		// int dataLength = strlen(data);
// 		//
// 		// //–¥
// 		// dsk.Command_open(path,"w+",filePoi);
// 		// printf("Write data:\n\"%s\"\n\n",data);
// 		// dsk.Command_write(filePoi,data,dataLength);
// 		// dsk.Command_close(filePoi);
//
//
// 		return 0;
// 	}
// 	static int handle_cat(disk*dsk) {
// 		retBuf[0]=0;
// 		char path[32];
// 		scanf("%s",path);
//
// 		//read
// 		FileStream filePoi;
// 		dsk->Command_open(path, "r", filePoi);
// 		dsk->Command_read(filePoi, retBuf, 128);
// 		dsk->Command_close(filePoi);
//
// 		return 0;
// 	}
// 	static int handle_touch(disk*dsk) {
// 		// retBuf[0]=0;
// 		// char path[32];
// 		// scanf("%s",path);
// 		// dsk.Command_touch(path);
// 		return 0;
// 	}
//
//
// protected:
//
//
// };

//set<string>

void getPurePath(string &path) {
    if (path.back() == '"' && path.front() == '"') {
        path.erase(path.length() - 1);
        path.erase(0);
    }
}

unordered_map<string, int> handleTable{
    {"touch", 1},
    {"mkdir", 2},
    {"cd", 3},
    {"ls", 4},
    {"cat", 5},
    {"rm", 6},
    {"echo", 7},
    {">>", 8},
    {">", 9},
};

void simpleShell() {
    signal(SIGINT, signalHandler);
    char memDisk[4096];
    char retBuf[1024] = {0};
    disk dsk(memDisk);
    while (true) {
        printf("\033[32m root@root:/%s$  \033[0m", dsk.curPathStr);

        // input
        char commandBuf[ORIGINAL_INPUT_MAX_LENGTH];
        gets(commandBuf);
        printf("\n");
        // parse
        list<string> inputList;
        char *p;
        p = strtok(commandBuf, " ");
        while (p) {
            inputList.emplace_back(p);
            p = strtok(NULL, " ");
        }

        while (!inputList.empty()) {
            string command = inputList.front();
            inputList.pop_front();
            string singlePara = inputList.front();
            if(singlePara.empty()) {
                printf("invalid para\n");
                break;
            }

            inputList.pop_front();
            switch (handleTable[command]) {
                case 1: {
                    // touch
                    dsk.Command_touch(singlePara.c_str());
                    retBuf[0] = 0;
                    break;
                }
                case 2: {
                    // mkdir
                    dsk.Command_mkdir(singlePara.c_str());
                    retBuf[0] = 0;
                    break;
                }
                case 3: {
                    // cd
                    break;
                }
                case 4: {
                    //ls
                    dsk.Command_ls(singlePara.c_str());
                    break;
                }
                case 5: {
                    //cat
                    const char *path = singlePara.c_str();
                    FileStream filePoi;
                    dsk.Command_open(path, "r", filePoi);
                    dsk.Command_read(filePoi, retBuf, 1024);
                    dsk.Command_close(filePoi);
                    break;
                }
                case 6: {
                    //rm
                    dsk.Command_delete(singlePara.c_str());
                    retBuf[0] = 0;
                    break;
                }
                case 7: {
                    //echo
                    strcpy(retBuf, singlePara.c_str());
                    break;
                }
                case 8: {
                    // >>
                    const char *path = singlePara.c_str();
                    FileStream filePoi;
                    dsk.Command_open(path, "a+", filePoi);
                    //printf("Write data:\n\"%s\"\n\n",data);
                    dsk.Command_write(filePoi, retBuf, strlen(retBuf));
                    dsk.Command_close(filePoi);
                    retBuf[0] = 0;
                    break;
                }
                case 9: {
                    //>

                    const char *path = singlePara.c_str();
                    FileStream filePoi;
                    dsk.Command_open(path, "w+", filePoi);
                    //printf("Write data:\n\"%s\"\n\n",data);
                    dsk.Command_write(filePoi, retBuf, strlen(retBuf));
                    dsk.Command_close(filePoi);
                    retBuf[0] = 0;
                    break;
                }
                default: {
                    strcpy(retBuf, "command not found");
                    break;
                }
            }

        }
        printf("%s\n", retBuf);
    }
}

void printDocument() {
    cout << "You can absolute path or relative path." << endl
            << "The maximum length of a file name is 6 bytes now." << endl
            << "Current support command:"<< "  touch mkdir ls cat cat" << endl
            <<"------------------------------------"<<endl;
}

int main() {
    printDocument();
    //testFunction();
    simpleShell();
    return 0;
}
