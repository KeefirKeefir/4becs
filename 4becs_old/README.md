# cppComposition/4becs

Refined alternative to my bfECS, uses a regular array, although the max component amount has to be manually handled, though you'll get an error if there are problems.

4becs stands for 4-bit ECS, technically it's 2 to 5 bits (the actual limit 0 to 63 bits). The bits refer to how many bits are used to store the index of each bitfield.

Entity Component system which stores components as heap allocated instances and uses a combination of a bitfield and unordered map for lookups.

Simply `#include 4becs.hpp` to use it

Free to use

## FOLLOWING SECTIONS NOT YET UPDATED FOR 4becs

## Relevant Keywords, you won't need anything else
- **Comp** → Component base class.
- **COMP** → Macro to declare a component; put this inside a component's member declarations.
- **Ent** → Entity base class.
- **incl(...)** → Takes any number of component types; put this in an entity’s constructor.
- **Ent::comp<C>()** → Returns a pointer to a component of type `C` if present, otherwise `nullptr`.
- **Ent::has<C>()** → Returns `true` if the entity has component `C`, otherwise `false`.

## Example usage
```cpp
// ECSbf.hpp example

#include "ECSbf.hpp"
#include "typedefs.h"
#include <iostream>

using namespace std;

// declare a component with Comp and COMP
struct Mob : Comp {
    // COMP macro to set it as a component in the system
    COMP;
    u8 name[100];
};

struct Hp : Comp {
    COMP;
    f64 hp = 100;
};

// declare an entity with Ent and incl(...)
struct Dog : Ent {
    Dog() {
        // incl(...) macro to include components
        incl(Mob, Hp);
    }
};

int main() {
    Ent* s = new Dog();

    // access a component with Ent::comp<C>()
    if(auto* hp = s->comp<Hp>()) {
        cout << hp->hp << endl;
    }
}
```

