// 4becs Entity Component System
// version 1.3.0

#pragma once

#include <iostream>
#include <unordered_map>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <memory>

// newer and safer version of 4becs with better macros and dynamic components
// 4becs is meant for larger components, to replace the need for inheritance

// RELEVANT KEYWORDS

// comp(T) - macro, declares a component struct with CompBase:Comp as the baseclass
// Ent - entity baseclass

// Functions

// bool hasComp<C>(Ent*) - takes in an Ent pointer and returns true if the component type C is included in the Ent
// C* getComp<C>(Ent*) - returns a pointer to an Ent's component bases on the input type C, otherwise returns a nullptr
// C* addComp<C>(Ent*)
// void addComps<...>(Ent*)
// void remvComp<C>(Ent*)
// C* overwriteComp<C>(Ent*)
// C* resetComp<C>(Ent*)

// NOTES

// add components in the constructor or after an object is created
// Entities need to derive from Ent to have components
// Components need to be declared with the comp(T) macro 



namespace becs{
    // the amount of bits used to store the index
    // this should be in a range of 2-5, but technically can be from 0 to 63
    // a value of 2 will allow for a max of 248 components
    // 3 - 488; 4 - 976; 5 - 1920;
    constexpr uint64_t IDX_BITS = 2; 
    constexpr uint64_t MAX_IDX = (1ULL << becs::IDX_BITS) - 1;
    // the amount of bits in each sub-bitfield
    constexpr uint64_t BF_BITS = 64 - becs::IDX_BITS; 

    // the rightmost {becs::IDX_BITS} bits hold an index, the leftmost {becs::BF_BITS} bits are a bitfield
    using u64BitID = uint64_t;
}

namespace becs {
    // gives out a unique becs::u64BitID to each component 
    struct CompRegistry {
        CompRegistry() = default;
        static CompRegistry& instance() {
            static CompRegistry reg;
            return reg;
        }
        becs::u64BitID getBit() {
            static uint64_t idx = 0;
            static uint64_t bit = 0;
        
            becs::u64BitID fullbit = 0;
            if (bit == becs::BF_BITS) {
                bit = 0;
                idx++;
                assert(idx <= becs::MAX_IDX && "too many components, modify becs::IDX_BITS in 4becs.hpp");
            }
            fullbit = idx;
            fullbit |= 1ULL << (becs::IDX_BITS + bit);
            bit++;
            //std::cout << idx << " " << becs::MAX_IDX << "\n";
            return fullbit;
        }
    };

}

// comp(Typename) to create a component
#define comp(T) struct T : becs::CompBase<T>

namespace becs {
    // component baseclass
    // needed for freeing and resetting components
    struct Comp {
        virtual ~Comp() = default;
        virtual void reset(){};
    };

    // template so that each Comp has a unique bit
    template <typename T>
    struct CompBase : Comp {
        inline static const becs::u64BitID BitIdentifier = becs::CompRegistry::instance().getBit();
    };
}


// entity base class
struct Ent {
    becs::u64BitID* compMask = (becs::u64BitID*)(calloc(becs::MAX_IDX + 1, sizeof(becs::u64BitID)));
    std::unordered_map<becs::u64BitID, std::unique_ptr<becs::Comp>> compMap = {};
    ~Ent() {
        free(compMask);
    }
};

// checks whether a component is included in an entity
// returns a bool
template <typename C>
bool hasComp(Ent* ent) {
    if (!ent->compMask) return false;
    return (ent->compMask[C::BitIdentifier & becs::MAX_IDX] & (C::BitIdentifier & (~becs::MAX_IDX))) != 0;
}

// returns a pointer to a component, or a nullptr
// checks if the component exists first, to avoid useless nullptrs being added to the umap
// the returned pointer should quickly go out of scope, as it has use-after-free risk
template <typename C>
C* getComp(Ent* ent) {
    if (hasComp<C>(ent)) {
        return static_cast<C*>(ent->compMap[C::BitIdentifier].get());
    }
    return nullptr;
}

// adds a component if the Ent doesn't already have it
// returns a pointer to that Comp
template <typename C>
C* addComp(Ent* ent) {
    if (!hasComp<C>(ent)) {
        ent->compMask[C::BitIdentifier & becs::MAX_IDX] |= (C::BitIdentifier & (~becs::MAX_IDX));
        ent->compMap[C::BitIdentifier] = std::make_unique<C>();
    };
    return static_cast<C*>(ent->compMap[C::BitIdentifier].get());
}

// add multiple Comps at once
template <typename... C>
void addComps(Ent* ent) {
    (addComp<C>(ent), ...);
}

// remove a component if it exists
template <typename C>
void remvComp(Ent* ent) {
    if (!hasComp<C>(ent)) return;
    ent->compMask[C::BitIdentifier & becs::MAX_IDX] &= ~(C::BitIdentifier & (~becs::MAX_IDX));
    ent->compMap.erase(C::BitIdentifier);
}

// overwrite/recreate a component
// returns a pointer to the new Comp or a nullptr if the Comp didn't exist
template <typename C>
C* overwriteComp(Ent* ent) {
    if (!hasComp<C>(ent)) return nullptr;
    ent->compMap[C::BitIdentifier] = std::make_unique<C>();
    return static_cast<C*>(ent->compMap[C::BitIdentifier].get());
}

// reset a component and return it
// calls a virtual reset() method
template <typename C>
C* resetComp(Ent* ent) {
    if (!hasComp<C>(ent)) return nullptr;
    C* comp = static_cast<C*>(ent->compMap[C::BitIdentifier].get());
    comp->reset();
    return comp;
}

// shorthand for addComps<...>(this)
// use in constructors
#define incl(...) addComps<__VA_ARGS__>(this)