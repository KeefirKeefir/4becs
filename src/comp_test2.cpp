// test result: success
/* console: 
Comp0: idx=0, bit=1
Comp63: idx=0, bit=9223372036854775808
Comp64: idx=1, bit=1
Comp69: idx=1, bit=32
 */

#include "CompSys.hpp"
#include <string>
#include <iostream>
#include "typedefs.h"

using namespace std;
using str = std::string; 


#define DEFINE_COMPONENT(n) \
    struct Comp##n : Comp { \
        SETBIT; \
    }


DEFINE_COMPONENT(0);  DEFINE_COMPONENT(1);  DEFINE_COMPONENT(2);  DEFINE_COMPONENT(3);
DEFINE_COMPONENT(4);  DEFINE_COMPONENT(5);  DEFINE_COMPONENT(6);  DEFINE_COMPONENT(7);
DEFINE_COMPONENT(8);  DEFINE_COMPONENT(9);  DEFINE_COMPONENT(10); DEFINE_COMPONENT(11);
DEFINE_COMPONENT(12); DEFINE_COMPONENT(13); DEFINE_COMPONENT(14); DEFINE_COMPONENT(15);
DEFINE_COMPONENT(16); DEFINE_COMPONENT(17); DEFINE_COMPONENT(18); DEFINE_COMPONENT(19);
DEFINE_COMPONENT(20); DEFINE_COMPONENT(21); DEFINE_COMPONENT(22); DEFINE_COMPONENT(23);
DEFINE_COMPONENT(24); DEFINE_COMPONENT(25); DEFINE_COMPONENT(26); DEFINE_COMPONENT(27);
DEFINE_COMPONENT(28); DEFINE_COMPONENT(29); DEFINE_COMPONENT(30); DEFINE_COMPONENT(31);
DEFINE_COMPONENT(32); DEFINE_COMPONENT(33); DEFINE_COMPONENT(34); DEFINE_COMPONENT(35);
DEFINE_COMPONENT(36); DEFINE_COMPONENT(37); DEFINE_COMPONENT(38); DEFINE_COMPONENT(39);
DEFINE_COMPONENT(40); DEFINE_COMPONENT(41); DEFINE_COMPONENT(42); DEFINE_COMPONENT(43);
DEFINE_COMPONENT(44); DEFINE_COMPONENT(45); DEFINE_COMPONENT(46); DEFINE_COMPONENT(47);
DEFINE_COMPONENT(48); DEFINE_COMPONENT(49); DEFINE_COMPONENT(50); DEFINE_COMPONENT(51);
DEFINE_COMPONENT(52); DEFINE_COMPONENT(53); DEFINE_COMPONENT(54); DEFINE_COMPONENT(55);
DEFINE_COMPONENT(56); DEFINE_COMPONENT(57); DEFINE_COMPONENT(58); DEFINE_COMPONENT(59);
DEFINE_COMPONENT(60); DEFINE_COMPONENT(61); DEFINE_COMPONENT(62); DEFINE_COMPONENT(63);
DEFINE_COMPONENT(64); DEFINE_COMPONENT(65); DEFINE_COMPONENT(66); DEFINE_COMPONENT(67);
DEFINE_COMPONENT(68); DEFINE_COMPONENT(69);


struct SuperEntity : 
    Comp0, Comp1, Comp2, Comp3, Comp4, Comp5, Comp6, Comp7, Comp8, Comp9,
    Comp10, Comp11, Comp12, Comp13, Comp14, Comp15, Comp16, Comp17, Comp18, Comp19,
    Comp20, Comp21, Comp22, Comp23, Comp24, Comp25, Comp26, Comp27, Comp28, Comp29,
    Comp30, Comp31, Comp32, Comp33, Comp34, Comp35, Comp36, Comp37, Comp38, Comp39,
    Comp40, Comp41, Comp42, Comp43, Comp44, Comp45, Comp46, Comp47, Comp48, Comp49,
    Comp50, Comp51, Comp52, Comp53, Comp54, Comp55, Comp56, Comp57, Comp58, Comp59,
    Comp60, Comp61, Comp62, Comp63, Comp64, Comp65, Comp66, Comp67, Comp68, Comp69 {
    incl(
        Comp0, Comp1, Comp2, Comp3, Comp4, Comp5, Comp6, Comp7, Comp8, Comp9,
        Comp10, Comp11, Comp12, Comp13, Comp14, Comp15, Comp16, Comp17, Comp18, Comp19,
        Comp20, Comp21, Comp22, Comp23, Comp24, Comp25, Comp26, Comp27, Comp28, Comp29,
        Comp30, Comp31, Comp32, Comp33, Comp34, Comp35, Comp36, Comp37, Comp38, Comp39,
        Comp40, Comp41, Comp42, Comp43, Comp44, Comp45, Comp46, Comp47, Comp48, Comp49,
        Comp50, Comp51, Comp52, Comp53, Comp54, Comp55, Comp56, Comp57, Comp58, Comp59,
        Comp60, Comp61, Comp62, Comp63, Comp64, Comp65, Comp66, Comp67, Comp68, Comp69
    );
    SuperEntity() {}  // Default constructor
};



// Test function to verify component detection
void testComponents(Comp* entity) {
    // Check components at start, near 64, and past 64
    if (auto* c0 = has<Comp0>(entity)) {
        std::cout << "Comp0: idx=" << Comp0::b.idx << ", bit=" << Comp0::b.bit << "\n";
    }
    if (auto* c63 = has<Comp63>(entity)) {
        std::cout << "Comp63: idx=" << Comp63::b.idx << ", bit=" << Comp63::b.bit << "\n";
    }
    if (auto* c64 = has<Comp64>(entity)) {
        std::cout << "Comp64: idx=" << Comp64::b.idx << ", bit=" << Comp64::b.bit << "\n";
    }
    if (auto* c69 = has<Comp69>(entity)) {
        std::cout << "Comp69: idx=" << Comp69::b.idx << ", bit=" << Comp69::b.bit << "\n";
    }
}

int main() {
    InitCompSys();  // Required to resize masks
    Comp0* entity = new SuperEntity();
    testComponents(entity);
    delete entity;
    return 0;
}