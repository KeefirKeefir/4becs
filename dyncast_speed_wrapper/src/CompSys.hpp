#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <typeinfo>

// RELEVANT KEYWORDS,
// the first four are mandatory to properly set up this system, 
// and has() is the only way to make use of this system
// Comp - all components need to inherit from Comp, 
// Do not make a Comp* pointer, it won't work
// SETBIT - all components must include this, 
// gives the component a unique bit in the bitmap used to check for components
// incl(...) - all inherited components of a struct must be declared in incl
// !!
// InitCompSys - must be called once at the beginning of main() or before any calls to has()
// !!
// has() - returns a dynamically casted pointer to the input component, or a nullptr
// uses the bitmap to compare a component's bit (from SETBIT) and a structs bitmask (from incl)

// NOTES
// this system isn't intende for use with any other inheritance than
// a component inheriting from Comp, and structs inheriting from components
// it might work with typical inheritance stuff, but this system is meant to replace that


// a component's bit, held in the component
struct Compbit {
    uint64_t idx;
    uint64_t bit;
    Compbit(uint64_t newidx, uint64_t newbit) : idx(newidx), bit(newbit) {}
};

// Singleton, required so that every SETBIT is done before any incl()
struct CompSys {
    static CompSys& Instance() {
        static CompSys instance;
        return instance;
    }

    Compbit getbit() {
        Compbit newbit(bitindex, bitcounter);
        bitcounter <<= 1;
        if (bitcounter == 0) {
            bitindex ++;
            bitcounter = 1;
        }
        return newbit;
    }

    uint64_t bitindex = 0;
    static std::vector<std::vector<uint64_t>*> compBitMasks;


private:
    CompSys() = default;
    uint64_t bitcounter = 1;
};
inline std::vector<std::vector<uint64_t>*> CompSys::compBitMasks;

// create the component's bit identifier
#define SETBIT inline static Compbit b = CompSys::Instance().getbit()


//component base
struct Comp {
    virtual std::vector<uint64_t> mask() const { return {}; }
    virtual ~Comp() = default;
};

// returns a pointer dynamically cast to the given component
// compares the given object's mask and the given component's type's BIT
template <typename C>
C* has(Comp* ptr) {

    std::vector<uint64_t> bits = ptr->mask();

    if ((bits[C::b.idx] & C::b.bit) != 0) {
        return dynamic_cast<C*>(ptr);
    }
    return nullptr;
}


// MACRO: DON'T WORRY ABOUT FN DECL NOT DEFINED
// creates the mask and mask getter mask()
// the has() function uses this mask and the given component's type's BIT
#define incl(...) \
    inline static std::vector<uint64_t> m_mask = combineMasks<__VA_ARGS__>(); \
    std::vector<uint64_t> mask() const override { return m_mask; } \
    static struct MaskRegistrar {   \
        MaskRegistrar() {           \
            CompSys::compBitMasks.push_back(&m_mask); \
        }                            \
    } maskRegistrar;

template <typename... Comps>
inline static std::vector<uint64_t> combineMasks() {
    std::vector<uint64_t> combinedMask;

    combinedMask.resize(CompSys::Instance().bitindex + 1);

    ((combinedMask[Comps::b.idx] |= Comps::b.bit), ...);

    return combinedMask;
}

// must be called at the beginning of main or once somewhere in the program before any call to has()
inline void InitCompSys() {
    for (auto* maskPtr : CompSys::compBitMasks) {
       maskPtr->resize(CompSys::Instance().bitindex + 1);
    }
}



// EXAMPLE USAGE:

/* 
struct Health : Comp {
    SETBIT;

    double maxhp;
    double hp = maxhp;
    void takedmg(double amount) { 
        hp -= amount; 
    }
};

struct Entity : Comp {
    SETBIT;

    string name;
};

struct Dog : Entity, Health {
    incl(Entity, Health); 
    
    void bark() {
        cout << "Woof!\n";
    }
    Dog(string iname) {

        name = iname;
        maxhp = 100.0;
        hp = maxhp;
    }
};

int main() {
    InitCompSys();

    Entity* dog1 = new Dog("Rex");

    cout << dog1->name << "\n";

    if (auto* ptr = has<Health>(dog1)) {
        ptr->takedmg(14.0);
        cout << ptr->hp << endl;
    }
    
    delete dog1;
}
 */