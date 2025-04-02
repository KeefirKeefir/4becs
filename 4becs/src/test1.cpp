
#include "4becs.hpp"
#include <iostream>
#include "typedefs.h"

using namespace std;


struct Hp : Comp {
    COMP;
    f64 hp = 100.0;

};

struct Dog : Ent {
    string name = "Joe";

    Dog() {
        incl(Hp);
    }
};

int main() {
    auto* dog = new Dog();

    if (auto* hp = get<Hp>(dog)) {
        cout << hp->hp;
    }
}