
struct Comp {
};

struct Hp : Comp {};
struct Mob : Comp {};
struct Giant : Comp {};
struct Drivable : Comp {};

struct Dog : Mob, Hp {

};

struct Elephant : Mob, Giant {

};

struct Car : Mob, Hp, Giant, Drivable {

};


int main() {
    Mob* mob = new Dog();

    // Hp* hp = fastcast<Hp>(mob);
    // T* ptr = fastcast<T>(mob); // where T inherits from Comp
}