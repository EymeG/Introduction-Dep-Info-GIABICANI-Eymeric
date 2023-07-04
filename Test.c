#include "Vec2.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
    Vec2 vecteur1=initializeVec2();
    Vec2 vecteur2=initializeVec2();
    vecteur1.x=1.0;
    vecteur1.y=0.0;
    vecteur2.x=0.0;
    vecteur2.y=1.0;
    Vec2 vecres=sumvec(vecteur1,vecteur2);
    printf("xres=%f  yres=%f \n", vecres.x,vecres.y);
    vecres=soustvec(vecteur1,vecteur2);
    printf("xres=%f  yres=%f \n", vecres.x,vecres.y);
    printf("norm= %f\n", norm(vecres));
    vecres=mulscal(3.33,vecres);
    printf("xres=%f  yres=%f \n", vecres.x,vecres.y);
    printf("%f \n", PrScal(vecteur1,vecres));
}