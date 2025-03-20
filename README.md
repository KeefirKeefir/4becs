# cppComposition

A composition-based library/framework for C++.

Simply `#include CompSys.hpp` to use it

Free to use

## Overview
This system provides a lightweight alternative to traditional inheritance, leveraging component-based design with bitmask-based component lookup and dynamic casting.

## Relevant Keywords, you won't need anything else
- **`Comp`** - Base class for all components. All components must inherit from `Comp`.
- **`SETBIT`** - Required in all components. Assigns a unique bit in the bitmap used for component lookup. (macro + function call)
- **`incl(...)`** - Declares which components are included in a struct, these must be the same as the components a struct inherits from. (macro + generic fn call)
- **`InitCompSys()`** - Must be called once at the beginning of `main()` or before any calls to `has()`.
- **`has<T>(entity)`** - Checks if an entity has a component of type `T`. Returns a dynamically casted pointer if found, otherwise `nullptr`. (generic fn)

## Usage Notes
- Do **not** create `Comp*` pointers directly; they will not function as expected.
- This system is **not** designed for traditional inheritance beyond components inheriting from `Comp`, (structs still need to inherit from their components ofc).
- `incl(...)` must be used to declare inherited components in a struct.

## Example
```cpp
#include "CompSys.hpp"

struct CompA : Comp { SETBIT; };
struct CompB : Comp { SETBIT; };

struct Entity : CompA, CompB { incl(CompA, CompB); };

int main() {
    InitCompSys();
    Entity e;
    if (has<CompA>(&e)) {
        std::cout << "Entity has CompA" << std::endl;
    }
}
```

