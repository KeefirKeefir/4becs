#include "typedefs.h"
#include <iostream>
#include <unordered_map>
#include <cstdlib>

using namespace std;
using u64bit = uint64_t; // the rightmost 16 bits are and index, the leftmost are a bitfield
namespace getbitns {
    u64 index;
}
u64bit getbit() {
    getbitns::index = 0;
    static u64 bit = 0;

    u64bit fullbit = 0;

    if (getbitns::index == 0xFFFF) {
        cerr << "too many components";
        return 0;
    }
    if (bit == 48) {
        bit = 1;
        getbitns::index++;
    }

    fullbit = getbitns::index;
    fullbit |= 1 << (16 + bit);
    bit++;
    return fullbit;
}


struct Comp {
    virtual unordered_map<u64bit, void*> getComps() {};
    virtual u64bit* getMask() {};

    template <typename C>
    bool has() {
        if ((getMask()[C::b & 0xFFFF] & (C::b & 0xFFFFFFFFFFFF0000)) != 0) {
            return true;
        }
        return false;
    }

    template <typename C>
    C* getComp() {
        if (has<C>()) {
            return static_cast<C*>(getComps()[C::b]);
        }  
        return nullptr;
    }
    virtual ~Comp() = default;
};

struct Str : Comp {}; //wrapper to separate comps from structs

struct A : Comp {
    inline static u64bit b = getbit();
    i32 num = 14;
};

struct B : Comp {
    inline static u64bit b = getbit();
    f64 hp = 100.0;
};

template <typename T>
void setmaskbit(u64bit* mask) {
    u64 idx = T::b & 0xFFFF;
    mask[idx] |= T::b & 0xFFFFFFFFFFFF0000;
}

struct S1 : Str {
    inline static u64bit* rawmask = nullptr;

    unordered_map<u64bit, void*> comps = {};


    u64bit* getMask() override {
        return rawmask;
    }

    unordered_map<u64bit, void*> getComps() override {
        return comps;
    }

    S1 () {
        if (rawmask) { free(rawmask); }
        if (!(rawmask = reinterpret_cast<u64bit*>(calloc(getbitns::index * sizeof(u64bit), 0)))) {
            cerr << "malloc failed, constructor fail" << endl;
        }
        setmaskbit<A>(rawmask);
        setmaskbit<B>(rawmask);

        comps[A::b] = static_cast<void*>(new A());
        comps[B::b] = static_cast<void*>(new B());
    }

    ~S1() {
        free(rawmask);
        for (auto& [key, ptr] : comps) {
            delete static_cast<Comp*>(ptr);
        }
    }
};


int main() {
    cout << "test1\n";
    Str* str = new S1();
    cout << "test2\n";
    cout << (A::b & 0xFFFF) << endl << (B::b & 0xFFFF) << endl;
    cout << (A::b & 0xFFFFFFFFFFFF0000) << endl << (B::b & 0xFFFFFFFFFFFF0000) << endl;

    if (B* b = str->getComp<B>()) {
        cout << "test3\n";
        cout << b->hp << endl;
    }
    if (A* a = str->getComp<A>()) {
        cout << "test4\n";
        cout << a->num << endl;
    }
    cout << "end";
}