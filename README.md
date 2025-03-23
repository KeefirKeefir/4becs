# Component systems for C++

Two component systems for C++.

The bitfield ECS is an ECS which uses bitfields and umaps to lookup pointers to components instance.

The dynamic cast speed wrapper basically makes dynamic casting more effiecient by only casting when a success is guaranteed.

Both systems use bitfields to check for the existence of components.

The systems have very simple semantics (thanks to macros) for developer ease of use.

Free to use

