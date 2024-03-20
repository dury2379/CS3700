#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <utility>
#include <vector>
#include <thread>
#include "math.h"

using namespace std;

constexpr long long value= 1000000;   
mutex myMutex;

void minmax(long long& min, long long& max, const int threadID, const vector<long long>& val,
   unsigned long long beg, unsigned long long end){
    long long localMin = val[beg];
    long long localMax = val[beg];
    for (auto it= beg; it <= end; ++it){
        if (val[it] < localMin)
        {
            localMin = val[it];
        }
        else if(val[it] > localMax) {
            localMax = val[it];
        }
    }
    cout << "Thread ID: " << threadID << ", local min: " << localMin << endl
         << "Thread ID: " << threadID << ", local max: " << localMax << endl;
    lock_guard<mutex> myLock(myMutex);
    if (localMin < min)
    {
        min = localMin;
    }
    if(localMax > max) {
        max = localMax;
    }
}

int main(){

  cout << endl;

  vector<long long> randValues;
  randValues.reserve(value);

  mt19937 engine (time(nullptr));
  uniform_int_distribution<> uniformDist(0, pow(10, 8));
  for ( long long i=0 ; i< value ; ++i)
     randValues.push_back(uniformDist(engine));
 
  long long min = randValues[0];
  long long max = randValues[0];

  auto start = chrono::system_clock::now();

  int threads = 4;
  thread t[threads];
  long long slice = value / threads;
  int startIdx=0;
  for (int i = 0; i < threads; ++i) {
    cout << "Thread[" << i << "] - slice ["
         << startIdx << ":" << startIdx+slice-1 << "]" << endl;
    t[i] = thread(minmax, ref(min), ref(max), i, ref(randValues), startIdx, startIdx+slice-1);
    startIdx += slice;
  }

  for (int i = 0; i < threads; ++i)
     t[i].join();

  chrono::duration<double> dur= chrono::system_clock::now() - start;
  cout << "Time for addition " << dur.count() << " seconds" << endl;
  cout << "Result: " << endl
       << "    Min: " << min << endl
       << "    Max: " << max << endl;

  cout << endl;

}
