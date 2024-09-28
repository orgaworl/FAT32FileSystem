#pragma once
#include"semaphore"
#include<iostream>
#include<cstdint>
#include<cstring>
#define FAT_NOT_USE  0x0000
#define FAT_RESERVE 0xFFF0
#define FAT_ERROE 0xFFF7
#define FAT_END 0xFFF8

#define STATUS_FIND 1
#define STATUS_NOT_USE 0
#define STATUS_RESERVED -1
#define STATUS_END -2
#define STATUS_BAD -3

#define FREE_BLOCK_NOT_FIND -1
#define FREE_BLOCK_FIND 1

#define PATH_NOT_EXIST 0
#define PATH_EXIST 1

#define INNER_FILE_NOT_FIND 0
#define INNER_FILE_FIND 1
#define DISK_MAXLEN 2560
#define DEBUG true

int setTime(uint16_t& data_, uint16_t& time_);
int printTime(uint16_t data_, uint16_t time_);
void printf_bin(int num);


const int FAT16DBR[8] = { 8,4,4,4,4,4,2,2 };
struct Block
{
	//32Byte
	char* data[32];

};
struct DicEntry
{
	char name[8];
	char extension[3];
	uint8_t attributes;
	uint8_t reserved[10];

	uint16_t time;
	uint16_t date;
	uint16_t firstBlockNum;
	int size;
};
struct FileStream
{
	bool write = 0;
	bool read = 0;


	int DirBlock = -1;  //目录项块
	int byteSize = 0;

	int curBlockOrd = -1; //块号
	int curByteOrd = 0;  //块内字节序号
};
class disk
{
public:
	int BLOCK_SIZE = 0;
	int PBP_SIZE = 0;
	int FAT_NUM = 0;
	int FAT_SIZE = 0;
	int ROOT_NUM = 0;
	int CATALOG_SIZE = 0;
	int HIDE_BLOCK_NUM = 0;


	int curPath;//指向目录项
	char* curPathStr;
	char* DBRpoi;
	uint16_t* FATpoi;
	char* ROOTpoi;
	char* ENDpoi;



	disk(char Disk[DISK_MAXLEN]);
	DicEntry* dir(int blockOrd);
	char* ord2addr(int blockOrd);
	int addr2ord(char* addr);
	int analysPath(const char* path, char**& pathSplit, int& startBlock, int& stepNum);
	int findFreeBlock();
	int nextBlockStatus(int blockOrdinal);
	int creatDirEnt(const char* dirName);
	int creatDirAndFile(const char* dirName);
	int creatEmptyFile(const char* path);
	int isDir(int DirEnt);
	int findFileInDir(int dirBlock, char* name, int& dstBlock, int& prevBlock);
	int checkPath(const char* path, int& dstBlock, int& prevBlock);
	int Command_mkdir(char* path);
	int Command_mkdir(char** pathSplit, int dirBlock, int step);
	int deleteDirFile(int block);
	int Command_delete(char* path);
	int Command_open(char* path, const char* mode, FileStream& file);
	int Command_write(FileStream& file, char* data, int dataLength);
	int Command_read(FileStream& file, char* buf, int bufLength);
	int Command_close(FileStream& file);
	int Command_ls(const char* path);
};