#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <stack>



template <class T>
class ObjectPool{
public:
    static const int initObjCount = 800000; // initial object count 800K
    static const int augObjCount = 200000; // object count for every augmentation 200K
    std::stack<T*> pool;
    
    ObjectPool(){
        // create initial objects for pool
        for (int i = 0; i < initObjCount; i++){
            pool.push(new T());
        }
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
