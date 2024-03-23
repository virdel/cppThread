# c++ 多线程编程

'''

//创建线程

std::thread t(functionname,args...)

t.join()  主程序等待线程结束

t.joinable() 是否可以调用join() 函数
'''


### 线程函数中的数据未定义错误

std::ref  传递引用类型


用智能指针解决
外部对象调用私有成员函数可以使用友元


### 互斥量解决多线程数据共享问题
数据竞争问题
mutex
写操作之前加锁，操作完解锁
mtx.lock()

mtx.unlock()

## 互斥量死锁


## lock guard unique lock
std::lock_guard:
当构造函数被调用时，互斥量会被自动锁定
析构函数调用自动解锁
lock guard 不能复制或者移动，只能在作用域中出现
'''
    std::lock_guard<std::mutex> lg(mtx);

'''

std::unique guard:
对互斥量更加灵活管理，包括延迟加锁、条件变量、超时等。
 

## std::call once
单例模式
饿汉模式：用到的时候才构造
懒汉模式：直接构造好
std::once_flag

std::call_once(once,function_name)


## condition variable 
生产者与消费者模型
std::condition_variable:
1. 创建一个std::condition_variable对象
2. 创建一个互斥锁，保护共享资源的访问
3. 在需要等待条件变量的地方
    使用std::unique_lock<std::mutex>对象锁定互斥锁
    调用std::condition_variable::wait()、std::condition_variable::wait_for()、
    std::condition_variable::wait_until()
    函数等待条件变量
4. 在其他线程中需要通知等待的线程时，调用
    std::condition_variable::notify_on()、std::condition_variable::notify_all()
    通知等待的线程


## c++实现跨平台线程池
维护一个线程数组和任务队列，让线程完成队列里的任务
完美转发：
'''
template<typename T>
void perfect_forward(T&& arg) {
    another_function(std::forward<T>(arg));
}

// 假设我们有一个函数可以处理不同类型和值类别的参数
void another_function(int& lvalue) {
    // ...
}

void another_function(int&& rvalue) {
    // ...
}

// 使用完美转发
int main() {
    int x = 10;
    perfect_forward(x); // 转发左值引用
    perfect_forward(20); // 转发右值引用

    return 0;
}
'''