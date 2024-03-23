#include<iostream>
#include<thread>
#include<string>
#include<mutex>


class Log;
static Log* log=nullptr;
static std::once_flag once;
class Log{
    public:
    Log(){};
    Log(const Log& log)=delete;
    Log& operator=(const Log& log)=delete;
    static Log& getInstance(){
        //懒汉模式  饿汉模式：用到的时候才构造
        std::call_once(once,init);
        //init()
        return *log;
    }
    static void init(){
        std::cout<<__TIME__<<"init"<<std::endl;
        if(log==nullptr) log=new Log;
    }

    void printlog(std::string msg){
         std::cout<<__TIME__<<msg<<std::endl;
    }
    private:
    
};

void printerror(){
    Log::getInstance().printlog("hello");
}

int main(){
    //std::thread t(functionname,args...)
    std::thread t1(printerror);
    std::thread t2(printerror); 
    std::thread t3(printerror);
    t1.join();
    t2.join();
    t3.join();


    return 0;
}