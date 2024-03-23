#include<iostream>
#include<thread>
#include<string>
#include<mutex>
#include<condition_variable>
#include<queue>
#include<vector>
#include<functional>

class ThreadPool{
public:
    ThreadPool(int n):stop(false){
        for(int i=0;i<n;i++){
            threads.emplace_back([this](){
                while(1){
                    std::unique_lock<std::mutex>   lock(mtx);
                    condition.wait(lock,[this]{
                        return !tasks.empty()||stop;
                    }) ;
                    if(stop&&tasks.empty()){
                        return ;
                    }
                    std::function<void()> task=std::move(tasks.front());
                    tasks.pop();
                    lock.unlock();
                    task();
                }
            });
        }
    }
    ~ThreadPool(){
        {   
            std::unique_lock<std::mutex> lock(mtx); 
            stop=true;
        }
        condition.notify_all();
        for(auto& t:threads ){
            t.join();
        }

    }
    template<class F,class... Args>
    void enqueue(F&& f,Args&&... args){
        {
            std::function<void()> task=std::bind(std::forward<F>(f),std::forward<Args> (args)...);
            std::unique_lock<std::mutex> lock(mtx);
            tasks.emplace(std::move(task));
        }
        condition.notify_one();
    }

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable condition;
    bool stop;
};

int main(){
    
    ThreadPool p(5);
    for(int i=0;i<100;i++){
        p.enqueue([](int k){
            std::cout<<k<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        },i);
    }


    return 0;
}