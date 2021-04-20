#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <stack>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>


template <class T>
class ObjectPool{
public:
    static const int initObjCount = 3000000; // initial object count 3M
    static const int augObjCount = 1000000; // object count for every augmentation 1M
    std::stack<T*> pool;
    
    ObjectPool(){
        // create initial objects for pool
        // time the initialization
        auto startTime = std::chrono::steady_clock::now(); // start time
        for (int i = 0; i < initObjCount; i++){
            pool.push(new T());
        }
        std::cout << "Constructed object pool with " << initObjCount << " objects in: "
                  << std::setprecision(4) << std::chrono::duration<double>(std::chrono::steady_clock::now() - startTime).count() << " sec"
                  << std::endl;
        return;
    }

    ~ObjectPool(){
        while(!pool.empty()){
            auto tmp = pool.top();
            delete tmp;
            pool.pop();
        }
        return;
    }

    T* pop(){
        if (pool.empty()){ // create objects if pool is empty
            for (int i = 0; i < augObjCount; i++){
                pool.push(new T());
            }
        }

        // take an object out from pool and reuse
        auto obj = pool.top();
        pool.pop();

        return obj;
    }

    void push(T* obj){
        pool.push(obj);
        return;
    }

};

#endif
