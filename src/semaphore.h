#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;	// �����д��䣬�����mutex��cout��ǰ�涼Ҫ��std::
class Semaphore
{
private:
    int count;
    mutex m;
    condition_variable cv;
public:
    Semaphore(int count_);
    void P();
    void V();
};