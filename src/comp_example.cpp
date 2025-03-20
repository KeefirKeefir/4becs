// example usage of CompSys.hpp

#include "CompSys.hpp"
#include <string>
#include <iostream>
#include "typedefs.h"

using namespace std;
using str = std::string; 

struct Mob : Comp {
    SETBIT;

    str name;
};


struct Hp : Comp {
    SETBIT;

    f64 maxhp;
    f64 hp;

    void takeDmg(f64 amount) {
        hp -= amount;
        cout << "Ouch, hp: " << hp << endl;
    }
};

struct Dog : Mob, Hp {
    incl(Mob, Hp);

    Dog(str iname) {
        name = iname;
        maxhp = 100.0;
        hp = maxhp;
    }
};



i32 main() {
    InitCompSys();

    Mob* mob = new Dog("Rex");

    if (auto* temp = has<Hp>(mob)) {
        temp->takeDmg(18.0);
    }

    delete mob;
}