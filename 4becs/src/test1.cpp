
#include "4becs.hpp"
#include <iostream>
#include "typedefs.h"

using namespace std;

COMP(Hp) {
    f64 m_Hp = 100.0;
};

COMP(Armour) {
    f64 m_Armour = 50.0;
};

COMP(Touchable) {
    void (*m_Touch)(void*);
    void* m_Owner;
    void init(void (*fnptr)(void*), void* owner) {
        m_Touch = fnptr;
        m_Owner = owner;
    }
    void f_Touch() {
        m_Touch(m_Owner);
    }
};

struct Dog : Ent {
    string m_Name = "Joe";

    static void onTouch(void* self) {
        cout << "boop " << endl;
    }

    Dog() {
        //INCL(Hp);
        addComp<Touchable>(this)->init(&onTouch, this);
    }
};

int main() {
    auto* dog = new Dog();

    addComp<Hp>(dog);

    if (auto* hp = getComp<Hp>(dog)) {
        cout << hp->m_Hp<< endl;
    }

    auto* armour = addComp<Armour>(dog);
    cout << armour->m_Armour << endl;

    if (auto* touchable = getComp<Touchable>(dog)) {
        touchable->f_Touch();
    }

    remvComp<Touchable>(dog);
    if (auto* touchable = getComp<Touchable>(dog)) {
        touchable->f_Touch();
    }
    else {
        cout << "not touchable" << endl;
    }

    delete dog;
}