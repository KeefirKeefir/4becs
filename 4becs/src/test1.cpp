
#include "4becs.hpp"
#include <iostream>
#include "typedefs.h"

using namespace std;

comp(Hp) {
    f64 hp = 100.0;
};

struct Dog : Ent {
    string name = "Joe";

    Dog() {
        addComp<Hp>(this);
    }
};

int main() {
    auto* dog = new Dog();

    if (auto* hp = getComp<Hp>(dog)) {
        cout << hp->hp;
    }

    delete dog;
}