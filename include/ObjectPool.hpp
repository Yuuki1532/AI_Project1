#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <stack>

#define INIT_OBJ_COUNT 800000 // initial object count 800K
#define AUG_OBJ_COUNT 200000 // object count for every augmentation 200K

template <class T>
class ObjectPool{
public:
    std::stack<T*> pool;
    
    TreePool(){
        // create initial objects for pool
        for (int i = 0; i < INIT_OBJ_COUNT; i++){
            pool.push(new T());
        }
        return;
    }

    ~TreePool(){
        while(!pool.empty()){
            auto tmp = pool.top();
            delete tmp;
            pool.pop();
        }
        return;
    }

    void recycle(T* node){
        pool.push(node);
        return;
    }

};

#endif
