// bfECS.hpp example

#include "bfECS.hpp"
#include "typedefs.h"
#include <iostream>

using namespace std;

struct Mob : Comp {
    COMP;
    u8 name[100];
};

struct Hp : Comp {
    COMP;
    f64 hp = 100;
};

struct Dog : Ent {
    Dog() {
        incl(Mob, Hp);
    }
};

int main() {
    Ent* s = new Dog();

    if(auto* hp = s->comp<Hp>()) {
        cout << hp->hp << endl;
    }
}