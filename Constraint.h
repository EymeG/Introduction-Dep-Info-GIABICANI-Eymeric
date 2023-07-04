#ifndef CONSTRAINT_H_
#define CONSTRAINT_H_

#include "Context.h"

// ------------------------------------------------

typedef struct SphereCollider // Registration of fundamental data of a static sphere in a 2D space
{
  Vec2 center; // center point of the static sphere
  float radius; // radius of the static sphere
  int draw_id;  // id used to identify drawing element associated to the static sphere;
} SphereCollider;


typedef struct GroundCollider // Registration of fundamental data of a static plane in a 2D space
{
  Vec2 leftcoord; //Left and right points of the plane line allowing drawing with tkinter methods in python.
  Vec2 rightcoord; // 
  Vec2 norm; // normal vector to the plane
  int draw_id; // id used to identify drawing element associated to the plane
} GroundCollider;

typedef struct GroundConstraint // Registration of fundamental data of an interaction constraint between a particle and a static object (GroundCollider or sphere collider)
{
  Vec2 closestpoint; // Closest point of the static object to the particle identify by particle_id 
  Vec2 norm; // Normal vector to the static object
  int particle_id; // Identification id of the particle to which the constraint effect will be added
} GroundConstraint;

typedef struct InteractionConstraint // Registration of fundamental data of an interaction constraint between particles
{
  Vec2 norm; // Normal vector going from one particle to the other
  int particle_id; // Id of the particle to which the constraint effect will be added
  int contact_id; // Id of the particle that collide with the particle identify by particle_id
  float C; // Usefull informations for constraint effect calculation
} InteractionConstraint;


// ------------------------------------------------


#endif
