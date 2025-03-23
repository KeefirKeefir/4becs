#include <iostream>
#include <unordered_map>

struct Comp {
    void* owner = nullptr; // points to the full object
};

struct Hp : virtual Comp { 
    double hp = 100.0; 
};
struct Mob : virtual Comp {};

struct Dog : Mob, Hp {
    Dog() {
        this->owner = this;   // each base sets owner to full object
    }
};

// type-safe retrieval
template <typename T>
T* getComp(Comp* base) {
    return static_cast<T*>(base->owner); // cast owner back to T*
}

int main() {
    Dog* dog = new Dog();
    Mob* mob = dog; // upcast to Mob*

    Hp* hp = getComp<Hp>(mob); // retrieve Hp component

    if (hp) {
        std::cout << "Successfully retrieved Hp component.\n" << "hp: " << hp->hp << std::endl;
    } else {
        std::cout << "Failed to retrieve Hp component.\n";
    }

    delete dog;
}
