#include "typedefs.h"
#include <vector>
#include <memory>
#include <iostream>
#include <typeinfo>

using namespace std;

struct Comp {
    virtual ~Comp() {};
    virtual const type_info& getType() const {
        return typeid(Comp);
    }
};

struct Hp : Comp {
    inline static u64 id = 1;
    f64 hp = 100.0;
};

struct Mob : Comp {
    inline static u64 id = 2;
};

struct Dog : Mob, Hp {
    vector<u64> comps = {1, 2};

    const type_info& getType() const override {
        return typeid(Dog);
    }
};

template <typename T>
T* castFull(Comp* ptr) {
    return static_cast<ptr->getType()*>(ptr);
    if (ptr && ptr->getType() == typeid(T)) {
        return static_cast<T*>(ptr);
    }
    cout << "fail\n";
    return nullptr;
}


int main() {
    Mob* mob = new Dog();

    Hp* dog = castFull<Hp>(mob);

    cout << dog->id << endl;

    if (dog) {
        cout << dog->hp << endl;
    }
        
}