#include<iostream>
#include<thread>
#include<string>
#include<mutex>
#include<condition_variable>
#include<queue>

std::queue<int> g_queue;
std::condition_variable g_cv;
std::mutex mtx;
 
void Producer(){
    for(int i=0;i<100;i++){
        {
        std::unique_lock<std::mutex> lock(mtx);
        g_queue.push(i);
        g_cv.notify_one();
        //通知消费者取任务
        std::cout<<"task: "<<i<<std::endl;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(10000));
    }
}

void consumer(){
    while(1){
        std::unique_lock<std::mutex> lock(mtx);
        //如果队列为空就等待
        g_cv.wait(lock,[]{
            return !g_queue.empty();
        });
        int value= g_queue.front();
        g_queue.pop();
        std::cout<<"processing task: "<<value<<std::endl;
    }
}

int main(){

    std::thread t1(Producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();

    return 0;
}