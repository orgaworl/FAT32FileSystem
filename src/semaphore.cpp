#include <iostream>
#include <thread>
#include <mutex>
#include"semaphore.h"
#include <condition_variable>
using namespace std;


Semaphore::Semaphore(int count_) : count(count_){}
void Semaphore::P() {
     unique_lock<mutex> loc(m);
     if (--count < 0) {
         cv.wait(loc);
     }
 }
void Semaphore::V() {
     unique_lock<mutex> loc(m);
     if (++count <= 0) {
         cv.notify_one();
     }
}