#include "element.h"

Element::Element()
{
    E = 0;
    v = 0;
    t = 0;
    A = 0;
    for(int i=0;i<6;i++){
        delta[i] = 0;
        if(i<3){
            epson[i] = 0;
            sigma[i] = 0;
            eq_a[i] = 0;
            eq_b[i] = 0;
            eq_c[i] = 0;
        }
    }
}
