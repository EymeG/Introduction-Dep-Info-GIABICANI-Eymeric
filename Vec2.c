#include "Vec2.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// ------------------------------------------------

Vec2 initializeVec2()
{
    Vec2* retour= (Vec2*) malloc(sizeof(Vec2));
    return *(retour);
}

// ------------------------------------------------

Vec2 mulscal(float e, Vec2 Vec)
{
    Vec2 retour= initializeVec2();
    retour.x = e*Vec.x;
    retour.y = e*Vec.y;
    return retour;
}

// ------------------------------------------------

Vec2 sumvec(Vec2 Vect1, Vec2 Vect2)
{
    Vec2 retour= initializeVec2();
    retour.x=Vect1.x+Vect2.x;
    retour.y=Vect1.y+Vect2.y;
    return retour;
}

// ------------------------------------------------

Vec2 soustvec(Vec2 Vect1, Vec2 Vect2)
{
    Vec2 retour= initializeVec2();
    retour.x=Vect1.x-Vect2.x;
    retour.y=Vect1.y-Vect2.y;
    return retour;
}

// ------------------------------------------------

float PrScal(Vec2 Vect1, Vec2 Vect2)
{
    return Vect1.x*Vect2.x+Vect1.y*Vect2.y;
}

// ------------------------------------------------

float norm(Vec2 Vec)
{
    return sqrtf(Vec.x*Vec.x+Vec.y*Vec.y);
}


