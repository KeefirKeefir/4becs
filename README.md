# cppComposition
a composition library/framework for C++


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

