#ifndef VEC2_H
#define VEC2_H

// ------------------------------------------------

typedef struct Vec2  // 2D vector Structure
{
  float x;
  float y;
} Vec2;

// ------------------------------------------------

Vec2 initializeVec2(); // Initialize a vec2 with random (x,y) coordonates with malloc

Vec2 mulscal(float e, Vec2 Vec); // Return the e*Vec with e a scalar and Vec a 2D vector

Vec2 sumvec(Vec2 Vec1, Vec2 Vec2); // Return the sum vector of two 2D vectors

Vec2 soustvec(Vec2 Vect1, Vec2 Vect2); // Return the difference between two 2D vectors (Vect1-Vect2)

float PrScal(Vec2 Vect1, Vec2 Vect2); // Return the scalar product of two 2D vectors

float norm(Vec2 Vec); // Return the scalar norm of a 2D vector

// ------------------------------------------------


#endif