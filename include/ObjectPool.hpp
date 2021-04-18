#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <stack>

#define INIT_OBJ_COUNT 800000 // initial object count 800K
#define AUG_OBJ_COUNT 200000 // object count for every augmentation 200K

template <class T>
class ObjectPool{
public:
    std::stack<T*> pool;
    
    ObjectPool(){
        // create initial objects for pool
        for (int i = 0; i < INIT_OBJ_COUNT; i++){
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
            for (int i = 0; i < AUG_OBJ_COUNT; i++){
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
