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

struct Whack : Comp {
    SETBIT;
    f64 dmg;
};

struct Dog : Mob, Hp {
    incl(Mob, Hp);
    Dog(str iname = "no name") : Mob(), Hp() {
        name = iname;
        maxhp = 100.0;
        hp = maxhp;
    }
};

i32 main() {
    InitCompSys();
    Mob* mob = new Dog("Rex");
    if (auto* temp = has<Hp>(mob)) {
        cout << temp->hp << endl;
        temp->takeDmg(18.0);
        cout << temp->hp << endl;
        //cout << mob->name << endl;
    }
    if (auto* temp = has<Whack>(mob)) {
        cout << temp->dmg << endl;
    }

    delete mob;
}