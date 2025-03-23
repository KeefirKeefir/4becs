#pragma once

#include <iostream>
#include <unordered_map>
#include <cstdlib>
#include <cstdint>

// RELEVANT KEYWORDS
// Comp - component base class
// COMP - macro to declare a component, put this into a component's member declarations
// Ent - entity base class
// incl(...) - takes in any number of component types, put this in the constructor of an entity
// Ent::comp<C>() - returns a pointer to a component of type C if it's in the the incl(...) list, or a nullptr
// Ent::has<C>() - return true if the component is in the incl list, false otherwise

// NOTES
// component and entity declarations aren't thread safe
// components must be declared before main(), or before any entity is instanced


// the rightmost 16 bits hold an index, the leftmost 48 bits are a bitfield
using u64bit = uint64_t;

// singleton to ensure all bits are generated before entities are
// do not declare new components after the main() function
// no not declare components after initialising a global entity
struct CompRegistry {
    static CompRegistry& instance() {
        static CompRegistry reg;
        return reg;
    }
    uint64_t getMaxIndex() { return maxIndex; }
    // generates a uniqe bit for each component
    u64bit getbit() {
        if (maxIndex == 0xFFFF) {
            std::cerr << "too many components";
            return 0;
        }
        u64bit fullbit = 0;
        if (bit == 48) {
            bit = 0;
            maxIndex++;
        }
        fullbit = maxIndex;
        fullbit |= 1ULL << (16 + bit);
        bit++;
        return fullbit;
    }
private:
    inline static uint64_t maxIndex = 0;
    inline static uint64_t bit = 0;
    CompRegistry() = default;
};

// component base class
struct Comp {
    virtual ~Comp() = default;
};

// put this in the destructor if you used incl(...) in the constructor
// frees the components
// Ent includes this by default, so you only need it for custom destructors
#define FREECOMPS \
    for (auto& [key, ptr] : compMap) delete static_cast<Comp*>(ptr); \
    free(compMask)

// entity base class
struct Ent {
    // check whether a component is included in an entity, return a bool
    template <typename C>
    bool has() {
        if (!compMask) {
            return false;
        }
        if ((compMask[C::b & 0xFFFF] & (C::b & 0xFFFFFFFFFFFF0000)) != 0) {
            return true;
        }
        return false;
    }
    // returns a pointer to a component, or a nullptr
    // checks if the component exists first, to avoid useless nullptrs being added to the umap
    template <typename C>
    C* comp() {
        if (has<C>()) {
            return static_cast<C*>(compMap[C::b]);
        }  
        return nullptr;
    }
    u64bit* compMask = nullptr;
    std::unordered_map<u64bit, void*> compMap = {};
    u64bit* getCompMask() { return compMask; }
    std::unordered_map<u64bit, void*> getCompMap() { return compMap; }
    virtual ~Ent() {
        FREECOMPS;
    }
};


// required for each component
// generate a unique bit for each component
#define COMP inline static u64bit b = CompRegistry::instance().getbit()

template <typename... Comps>
void setmaskbit(u64bit* mask) {
    ((mask[Comps::b & 0xFFFF] |= Comps::b & 0xFFFFFFFFFFFF0000), ...);
}

template <typename... Comps>
void setComps(std::unordered_map<u64bit, void*>& comps) {
    ((comps[Comps::b] = static_cast<void*>(new Comps())), ...);
}

// adds the desired components
// only meant to be used in the constructor
// unique to each instance, might be expandable to dynamic components
#define incl(...) \
    if (compMask) { free(compMask); } \
    if (!(compMask = reinterpret_cast<u64bit*>(calloc(CompRegistry::instance().getMaxIndex() + 1, sizeof(u64bit))))) { std::cerr << "malloc failed, constructor fail" << std::endl; } \
    setmaskbit<__VA_ARGS__>(compMask); \
    setComps<__VA_ARGS__>(compMap)


