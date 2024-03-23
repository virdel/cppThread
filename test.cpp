#include<iostream>
#include<vector>
#include<queue>
#include<mutex>
#include<thread>
#include<condition_variable>
#include<functional>



using namespace std;
class ThreadPool{
private:
    queue<function<void()>> tasks;
    vector<thread> threads;
    mutex mtx;
    condition_variable condition;
    bool stop;

public:
    ThreadPool(int n):stop(false){
        for(int i=0;i<n;i++){
            threads.emplace_back(
                [this](){
                    while(1)
                    {
                        unique_lock<mutex> lock(mtx);
                        condition.wait(lock,[this]{
                            return !this->tasks.empty()||this->stop;
                        });
                        if(this->stop&&this->tasks.empty()){
                            return ;
                        }
                        function<void()> task(move(this->tasks.front()));
                        this->tasks.pop();
                        lock.unlock();
                        task();
                    }

                    
                }
            );

        }
    }

    ~ThreadPool(){
        {
            unique_lock<mutex> lock(mtx);
            stop=true;
        }
        condition.notify_all();
        for(auto& t:threads){
            t.join();
        }
    }

    template<class F,class... Args>
    void enqueue(F&& f,Args&&... args){
        function<void()> task(bind(std::forward<F>(f),std::forward<Args> (args)...));
        {
            unique_lock<mutex> lock(mtx);
            tasks.emplace(move(task));
        }
        condition.notify_one();
    }
};

int main(){
    ThreadPool p(5);
    for(int i=0;i<1000;i++){
        cout<<"add task:"<<i<<endl;

        p.enqueue([](int k){
            cout<<"processing task:"<<k<<endl;
        },i);
    }

    return 0;


}