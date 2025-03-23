#include <unordered_map>
#include <typeindex>

struct A { virtual ~A() {} };

struct B : virtual A {

};

struct C : virtual A {

};
struct D : B, C {};

struct ObjectRegistry {
    std::unordered_map<std::type_index, void*> bases;

    template <typename T>
    void register_base(T* ptr) {
        bases[typeid(T)] = ptr;
    }

    template <typename T>
    T* get_base() {
        return static_cast<T*>(bases[typeid(T)]);
    }
};

int main() {
    D d;
    ObjectRegistry registry;
    registry.register_base<B>(&d);
    registry.register_base<C>(&d);

    B* b = registry.get_base<B>();
    C* c = registry.get_base<C>();  // No casting, just lookup

    return 0;
}
