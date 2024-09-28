#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;	// 如果不写这句，后面的mutex、cout等前面都要加std::
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