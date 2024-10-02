
#include"Disk.h"
#include"semaphore.h"
using namespace std;


int setTime(uint16_t& data_, uint16_t& time_)
{
	time_t now = time(0);
	tm* ltm = localtime(&now);
	ltm->tm_hour;
	//ʱ��: ���ʱ
	time_ = 0;
	time_ += (ltm->tm_sec / 2) & 0b11111;
	time_ = time_ << 5;
	time_ += ltm->tm_min & 0b111111;
	time_ = time_ << 5;
	time_ += ltm->tm_hour & 0b11111;


	//����: ������
	data_ = 0;
	data_ += (ltm->tm_mday) & 0b11111;
	data_ = data_ << 4;
	data_ += ltm->tm_mon & 0b1111;
	data_ = data_ << 7;
	data_ += ltm->tm_year & 0b1111111;



	return 1;
}
int printTime(uint16_t data_, uint16_t time_)
{
	unsigned int temp;
	temp = data_ & 0b1111111;
	printf(" %4d.", temp + 1900);
	data_ = data_ >> 7;
	temp = data_ & 0b1111;
	printf("%2d.", temp + 1);
	data_ = data_ >> 4;
	temp = data_ & 0b11111;
	printf("%2d ", temp);


	temp = time_ & 0b11111;
	printf("%2d:", temp);
	time_ = time_ >> 5;
	temp = time_ & 0b111111;
	printf("%2d:", temp);
	time_ = time_ >> 6;
	temp = time_ & 0b11111;
	printf("%2d    ", temp);
	return 1;
}
void printf_bin(uint8_t num)
{
	for (int i = 0; i < 8; i++)
	{
		if (num&0b10000000)
		{
			printf("1");
		}
		else
		{
			printf("0");
		}
		num =num<<1;
	}
}


disk::disk(char Disk[DISK_MAXLEN])
	{
		this->BLOCK_SIZE = 32;
		this->PBP_SIZE = 32;
		this->FAT_NUM = 0;
		this->FAT_SIZE = 160;
		this->ROOT_NUM = 16;
		this->CATALOG_SIZE = 32;
		this->HIDE_BLOCK_NUM = 0;

		this->DBRpoi = (Disk);
		this->FATpoi = (uint16_t*)(Disk + PBP_SIZE);
		this->ROOTpoi = (char*)(Disk + PBP_SIZE + FAT_SIZE);
		this->ENDpoi = &Disk[DISK_MAXLEN];
		//  1. д�� DBR ���ֵ�ֵ.
		int* composeSize = (int*)FAT16DBR; char* poi = &Disk[0];
		strcpy(poi, "FAT16");   poi += composeSize[0];
		*((uint32_t*)poi) = 32; poi += composeSize[1];
		*((uint32_t*)poi) = 32; poi += composeSize[2];
		*((uint32_t*)poi) = 1;  poi += composeSize[3];
		*((uint32_t*)poi) = 160; poi += composeSize[4];
		*((uint32_t*)poi) = 16; poi += composeSize[5];
		*((uint16_t*)poi) = 0;  poi += composeSize[6];
		*((uint16_t*)poi) = 0xAA55; poi += composeSize[7];
		//  2. ��ʼ�� FAT.
		int end1 = (PBP_SIZE + FAT_SIZE) / BLOCK_SIZE;
		int end2 = FAT_SIZE / 2;
		for (int i = 0; i < end1; i++) {
			FATpoi[i] = 0xFFF0;        //FFF0-FFF6 ����
		}
		for (int i = end1; i < end2; i++) {
			FATpoi[i] = 0x0000;         //δʹ��
		}
		//	3. ������(root) Ŀ¼�ļ�,���� "." �� ".." Ŀ¼��
		int rootBlock = addr2ord(ROOTpoi);
		int block1 = creatDirEnt(".");
		int block2 = creatDirEnt("..");
		FATpoi[block1] = block2;
		FATpoi[block2] = FAT_END;
		dir(block1)->firstBlockNum = rootBlock;
		dir(block2)->firstBlockNum = rootBlock;
		this->curPathStr = new char[240];
		curPathStr[0] = 0x0;
		this->curPath = rootBlock;
	}


	DicEntry* disk::dir(int blockOrd)
	{
		//DicEntry* temp = (DicEntry*)ord2addr(blockOrd);
		return (DicEntry*)ord2addr(blockOrd);

	}
	char* disk::ord2addr(int blockOrd)
	{
		return DBRpoi + BLOCK_SIZE * blockOrd;
	}
	int disk::addr2ord(char* addr)
	{
		return ((char*)addr - DBRpoi) / BLOCK_SIZE;
	}
	int disk::analysPath(const char* path, char**& pathSplit, int& startBlock, int& stepNum)
	{
		//�趨��ʼ��Ϊ ��Ŀ¼�ļ��׿�/��ǰĿ¼��

		//����·���ַ���,���Ϊ����ļ���
		int loopTime = strlen(path);
		stepNum = 1;
		for (int i = 0; i < loopTime; i++)
		{
			if (path[i] == '/')
			{
				stepNum += 1;
			}
		}
		if (path[loopTime - 1] == '/') { stepNum -= 1; }
		pathSplit = new char* [stepNum];




		int readOrd = 0;
		char temp = path[readOrd];
		int in = 0;
		for (int i = 0; i < stepNum; i++)
		{
			pathSplit[i] = new char[20];
			while (temp != '/' && temp != 0x0)
			{
				pathSplit[i][in] = temp;
				in++;
				temp = path[++readOrd];
			}
			pathSplit[i][in] = 0x0;
			temp = path[++readOrd];
			in = 0;
		}
		if (strcmp(pathSplit[0], ""))
		{//��Ŀ¼��ʼ
			startBlock = addr2ord(ROOTpoi);
			return 0;
		}
		startBlock = curPath;
		return 1;
	}
	int disk::findFreeBlock()
	{
		int FAT_EntryNum = FAT_SIZE / 2;

		for (int i = 0; i < FAT_EntryNum; i++)
		{//����FATÿһ��ֱ���ҵ�0x0000
			if (FATpoi[i] == FAT_NOT_USE)
			{
				return i;
			}
		}
		return FREE_BLOCK_NOT_FIND;
	}
	int disk::nextBlockStatus(int blockOrdinal)
	{
		//��ѯFAT��: ������ַȷ���Ŀ� ����һ�����״̬
		uint16_t nextBlockOrdinal = FATpoi[blockOrdinal];
		if (nextBlockOrdinal == 0) {
			return STATUS_NOT_USE;
		}
		if (nextBlockOrdinal == 0xFFF7)
		{
			return STATUS_BAD;
		}
		else if ((nextBlockOrdinal >= 0xFFF0 && nextBlockOrdinal <= 0xFFF6))
		{
			return STATUS_RESERVED;
		}
		else if ((nextBlockOrdinal >= 0xFFF8 && nextBlockOrdinal <= 0xFFFF))
		{
			return STATUS_END;
		}
		return STATUS_FIND;
	}

	int disk::creatDirEnt(const char* dirName)
	{
		//���شغ�
		int newBlock = findFreeBlock();
		FATpoi[newBlock] = FAT_END;
		strcpy(dir(newBlock)->name, dirName);
		strcpy(dir(newBlock)->extension, "");
		dir(newBlock)->attributes = 0b00010000;
		dir(newBlock)->firstBlockNum = FAT_END;
		setTime(dir(newBlock)->date, dir(newBlock)->time);
		dir(newBlock)->size = 0;


		return newBlock;
	}
	int disk::creatDirAndFile(const char* dirName)
	{
		//����Ŀ¼���ָ��Ŀ¼�ļ�
		// return Ŀ¼���
		//Ŀ¼�ļ�����"."".." ����δָ��ǰ�ļ���.

		//Ŀ¼��
		int newBlock = creatDirEnt(dirName);


		//����Ŀ¼�ļ� ,����Ŀ¼��: "." "..", ������
		int block1 = creatDirEnt(".");
		int block2 = creatDirEnt("..");
		FATpoi[block1] = block2;
		FATpoi[block2] = FAT_END;
		dir(newBlock)->firstBlockNum = block1;

		return newBlock;
	}
	int disk::creatEmptyFile(const char* path)
	{

		//1. ����Ŀ¼�ļ�
		char** pathSplit;
		int step;
		int dirBlock;
		int fileBlock;
		analysPath(path, pathSplit, dirBlock, step);
		dirBlock = Command_mkdir(pathSplit, dirBlock, step - 1);//����Ŀ¼��,ָ��Ŀ¼�ļ�


		//2. �����ļ�
		fileBlock = dir(dirBlock)->firstBlockNum;//Ŀ¼�ļ���һ��
		int _;
		int reVal = findFileInDir(fileBlock, pathSplit[step - 1], fileBlock, _);//�鿴ƥ��Ŀ¼�ļ����Ƿ��ж�Ӧ�ļ�
		if (reVal == INNER_FILE_FIND)
		{
			return -1;
		}
		else if (reVal == INNER_FILE_NOT_FIND)
		{
			//�����ļ�Ŀ¼���
			int newBlock = findFreeBlock();
			FATpoi[newBlock] = FAT_END;
			strcpy(dir(newBlock)->name, pathSplit[step - 1]);
			strcpy(dir(newBlock)->extension, "");
			dir(newBlock)->attributes = 0b00000000;
			setTime(dir(newBlock)->date, dir(newBlock)->time);
			dir(newBlock)->size = 0;
			FATpoi[fileBlock] = newBlock;
			//�����ļ���
			int fileBlock = findFreeBlock();
			FATpoi[fileBlock] = FAT_END;
			//����
			dir(newBlock)->firstBlockNum = fileBlock;//ƥ���ļ�����Ŀ¼��ָ�հ׿�
			return newBlock;
		}
		return -1;
	}


	int disk::isDir(int DirEnt)
	{
		//�ж�Ŀ¼���Ƿ�Ϊ
		return dir(DirEnt)->attributes & 0b00010000;
	}

	int disk::findFileInDir(int dirBlock, char* name, int& dstBlock, int& prevBlock)
	{
		//����Ŀ¼�ļ�������Ŀ¼��
		//dstBlock����ƥ��Ŀ¼������һ��

		//tempBlock�����ļ����ƶ�

		int tempBlock = dirBlock;//��һ��Ŀ¼�ļ���"."Ŀ¼�ʼ
		int findMark = 0;
		while (true)
		{
			if (strcmp(dir(tempBlock)->name, name) == 0) {
				findMark = 1;
				break;
			}
			if (FATpoi[tempBlock] == FAT_END) { break; }
			prevBlock = tempBlock;
			tempBlock = FATpoi[tempBlock];
		}
		dstBlock = tempBlock;

		if (findMark) { return INNER_FILE_FIND; }
		return INNER_FILE_NOT_FIND;
	}
	int disk::checkPath(const char* path, int& dstBlock, int& prevBlock)
	{
		//����ƥ��Ŀ¼����/����

		//curBlock�����ļ����ƶ�
		//innerBlock�����ļ����ƶ�
		//tempPrevָ��innerBlockǰһ��Ŀ¼��

		char** pathSplit;
		int step;
		int curBlock;
		analysPath(path, pathSplit, curBlock, step);

		int innerBlock = curBlock;//(stepΪ1�������,Ŀ�꼴Ϊ)
		int tempPrev = curBlock;

		//���Ѱ��,��������һĿ¼�ļ� .��
		int isFind = 0;
		for (int i = 1; i < step; i++)
		{
			//1. ����Ŀ¼Ŀ¼�ļ����Ƿ����ƥ��Ŀ¼��
			isFind = findFileInDir(curBlock, pathSplit[i], innerBlock, tempPrev);
			if (isFind == INNER_FILE_NOT_FIND)
			{
				return PATH_NOT_EXIST;
			}
			//2. ����Ŀ¼�ļ�
			if (i == step - 1) { break; }
			curBlock = dir(innerBlock)->firstBlockNum;
		}
		dstBlock = innerBlock;
		prevBlock = tempPrev;
		return PATH_EXIST;
	}
	int disk::Command_mkdir(const char* path)
	{
	//��ʼ��
		char** pathSplit;
		int step;
		int dirBlock;
		int fileBlock;
		analysPath(path, pathSplit, dirBlock, step);
		return Command_mkdir(pathSplit, dirBlock, step);
	}
	int disk::Command_mkdir(char** pathSplit, int dirBlock, int step)
	{
		//���Ѵ����򲻽����κβ���
		//���������ƥ��Ŀ¼��
		//fileBlockָ��ƥ��Ŀ¼��
		int fileBlock;
		//���Ѱ��
		int isFind = 0;
		for (int i = 1; i < step; i++)
		{
			//1. ����Ŀ¼�ļ� ���Ƿ����ƥ��Ŀ¼��
			int _;
			isFind = findFileInDir(dirBlock, pathSplit[i], fileBlock, _);
			if (isFind == INNER_FILE_NOT_FIND)
			{   //������ƥ�����򴴽� Ŀ¼���Ŀ¼�ļ�����
				int newBlock = creatDirAndFile(pathSplit[i]);
				int p = dir(newBlock)->firstBlockNum;
				int pp = FATpoi[p];
				dir(p)->firstBlockNum = newBlock;
				dir(pp)->firstBlockNum = dir(dirBlock)->firstBlockNum;
				FATpoi[fileBlock] = newBlock;
				FATpoi[newBlock] = FAT_END;
				fileBlock = newBlock;
			}
			//2. ����Ŀ¼�ļ�
			dirBlock = dir(fileBlock)->firstBlockNum;
		}
		return fileBlock;
	}

	int disk::deleteDirFile(int block)
	{
		//input :ɾ���ļ�Ŀ¼��
		//result: �ݹ�ɾ����Ŀ¼��ָ�� �ļ�


			//1. ɾ��Ŀ¼��ָ������
		if (isDir(block)) {
			//1.1 ָ��Ŀ¼�ļ���ݹ�ɾ��
			int poi = dir(block)->firstBlockNum;
			while (true)
			{
				int tempStore = poi;
				poi = FATpoi[poi];
				if (strcmp(".", dir(tempStore)->name) != 0 && strcmp("..", dir(tempStore)->name) != 0)
				{
					deleteDirFile(tempStore);
				}
				else
				{
					FATpoi[tempStore] = FAT_NOT_USE;
				}
				if (poi == FAT_END)
				{
					break;
				}

			}
		}
		else
		{
			//1.2 ָ��һ���ļ���ֱ��ɾ��
			int curB = dir(block)->firstBlockNum;
			int nextB;
			while (true)
			{
				nextB = FATpoi[curB];
				FATpoi[curB] = FAT_NOT_USE;
				if (nextBlockStatus(curB) != STATUS_FIND)
				{
					break;
				}
				curB = nextB;
			}
		}
		//2. ɾ��Ŀ¼��(����������)
		FATpoi[block] = FAT_NOT_USE;
		return 1;
	}
	int disk::Command_delete(const char* path)
	{
#ifdef DEBUG
		printf("sudo rm -rf %s\n\n",path);
#endif
		//�ݹ�ɾ��Ŀ¼��������
		// 
		//1. ��λҪɾ���ļ�Ŀ¼��
		int prevBlock;
		int curBlock;
		int findMark = checkPath(path, curBlock, prevBlock);
		if (findMark == PATH_NOT_EXIST)
		{
			return PATH_NOT_EXIST;
		}

		//2. ���Ŀ¼�ļ���Ŀ¼�������,ɾ��Ŀ¼��ָ���ļ�
		FATpoi[prevBlock] = FATpoi[curBlock];
		deleteDirFile(curBlock);
		FATpoi[curBlock] = FAT_NOT_USE;
		return 1;
	}

	Semaphore mutexSem(1);

	int disk::Command_open(const char* path, const char* mode, FileStream& file)
	{
#ifdef DEBUG
		printf("open %s\n", path);
#endif
		//ģʽ: r w a r+ w+ a+
		if(path[0]=='.') {
			char tmpBuf[1024];
			strcpy(tmpBuf,this->curPathStr);
			strcat(tmpBuf,&path[1]);
			path=tmpBuf;
		}
		mutexSem.P();
		int dstBlock, _;
		int status = checkPath(path, dstBlock, _);
		if (status == PATH_EXIST)
		{
			file.DirBlock = dstBlock; //ָ���ļ�Ŀ¼����
		}
		else if (status == PATH_NOT_EXIST && strcmp(mode, "w+") == 0)
		{
			dstBlock = creatEmptyFile(path);//�ļ���������ģʽΪw+,�����ļ�
			if (dstBlock == -1)
			{
				printf("error");
				return PATH_NOT_EXIST;
			}
			file.DirBlock = dstBlock;
		}
		else
		{
			return PATH_NOT_EXIST;
		}

		//������������
		if (mode[1] == '+' || mode[0] == 'r') { file.read = 1; };
		if (mode[1] == '+' || mode[0] == 'w' || mode[0] == 'a') { file.write = 1; };
		file.byteSize = dir(dstBlock)->size;
		if (mode[0] != 'a')
		{
			file.curBlockOrd = dir(dstBlock)->firstBlockNum;
			file.curByteOrd = 0;
		}
		else if(mode[0] == 'a')
		{
			file.curByteOrd = dir(dstBlock)->size % BLOCK_SIZE;
			int blockNum = (dir(dstBlock)->size - 1) / BLOCK_SIZE + 1;
		}
		
		return PATH_EXIST;
	}

	int disk::Command_write(FileStream& file,char* data, int dataLength)
	{
		file.byteSize += dataLength;
		int writeNum = 0;
		while (dataLength != writeNum)
		{
			char* addr = ord2addr(file.curBlockOrd);
			addr[file.curByteOrd] = data[writeNum];
			file.curByteOrd++;
			writeNum++;
			if (file.curByteOrd == BLOCK_SIZE)
			{
				file.curByteOrd = 0;
				if (nextBlockStatus(file.curBlockOrd) == STATUS_FIND)
				{
					file.curBlockOrd = FATpoi[file.curBlockOrd];
				}
				else
				{
					//�������ļ���
					int newBlock = findFreeBlock();
					FATpoi[file.curBlockOrd] = newBlock;
					FATpoi[newBlock] = FAT_END;
					file.curBlockOrd = newBlock;
				}

			}
		}
		return 1;
	}

	int disk::Command_read(FileStream& file,char* buf, int bufLength)
	{
		//���ļ��������ݴ�ŵ�buf����,Ҫ��bufLength>=���ݳ���,�������ȡ
		int totalLength = file.byteSize;
		int readNum = 0;
		if (bufLength < totalLength+1) {
			return 0;
		}
		while (readNum != totalLength)
		{
			char* addr = ord2addr(file.curBlockOrd);
			buf[readNum] = addr[file.curByteOrd];
			file.curByteOrd++;
			readNum++;
			if (file.curByteOrd == BLOCK_SIZE)
			{
				file.curByteOrd = 0;
				file.curBlockOrd = FATpoi[file.curBlockOrd];
			}
		}
		buf[readNum] = 0;
		return 1;
	}

	int disk::Command_close(FileStream& file)
	{
		int dirBlock = file.DirBlock;
		dir(dirBlock)->size = file.byteSize;
		setTime(dir(dirBlock)->date, dir(dirBlock)->time);
		file.byteSize = 0;
		file.curBlockOrd = 0;
		file.curByteOrd = 0;
		file.DirBlock = 0;
		file.read = 0;
		file.write = 0;

		mutexSem.V();
		return 1;
	}

	int disk::Command_ls(const char* path)
	{
		if(path[0]==0) {
			path=".";
		}
		//1. �ҵ�Ŀ¼�ļ�
		int dstBlock;
		int _;
		int pathExist = checkPath(path, dstBlock, _);
		if (pathExist == PATH_NOT_EXIST)
		{
			printf("ls: cannot access %s: No such file or directory\n\n",path);
			//printf(" %s NOT EXIST\n", path);
			return 0;
		}
		dstBlock = dir(dstBlock)->firstBlockNum;//Ŀ¼���ƶ���Ŀ¼�ļ��׿�

		//2.��ʾĿ¼�ļ�������Ŀ¼��
		
		while (true)
		{
			//printf("%2hx", dir(dstBlock)->attributes);
			printf_bin(dir(dstBlock)->attributes);
			cout << " " << dir(dstBlock)->size << "Byte ";
			printTime(dir(dstBlock)->date, dir(dstBlock)->time);
			cout << dir(dstBlock)->name << endl;
			if (nextBlockStatus(dstBlock) == STATUS_FIND)
			{
				dstBlock = FATpoi[dstBlock];
			}
			else
			{
				break;
			}
		}
		printf("\n");
		return 1;
	}

int disk::Command_touch(const char *path) {
		creatEmptyFile(path);
		return 1;
}
