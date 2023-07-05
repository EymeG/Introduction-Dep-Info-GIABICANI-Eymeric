#pragma once

#include "Vec2.h"
#include "Particle.h"
#include "Constraint.h"

// ------------------------------------------------

typedef struct Context {

  // Particles
  // Moving particles registration;
  int num_particles;
  int capacity_particles;
  Particle* particles;

  // Solids
  // Moving solids registration;

  int num_solids;
  Solid* solids;

  // Ground colliders 
  // Static spheres registration;
  int num_ground_sphere;
  SphereCollider* ground_spheres;

  // Ground line
  // Grounds and Walls registration;
  int num_ground_line;
  GroundCollider* ground_line;

  // Ground Constraint
  // Static Constraints registration;
  int num_ground_constraint;
  GroundConstraint* ground_constraint;
  
  // Dynamic Constraint;
  // Dynamic Constraints registration;
  int num_dynamic_constraint;
  InteractionConstraint* inter_constraint;

  // Solid Constraint;
  // Solid Constraints registration;
  int num_solid_constraint;
  SolidConstraint* solid_constraint;
  
} Context;


Context* initializeContext(int capacity); // Initialize the simulation context

void addParticle(Context* (context), float x, float y, float radius, float mass, int draw_id); // Add a particle to the simulation Context 

void addSolid(Context* context, float x, float y, float radius, float mass, int draw_id, int partnumb, int distance); // Add Solid data to the simulation Context (NON FONCTIONNEL)

//---------------------------

// GET OBJECTS METHODS

Particle getParticle(Context* context, int id); // Return the particle's data from context whose particle_id is id.

SphereCollider getGroundSphereCollider(Context* (context), int id); // Return the SphereCollider's data from context whose SphereCollider_id is id.

GroundCollider getGroundLine(Context* (context), int id); // Return the GroundCollider's data from context whose GroundCollider_id is id.

Solid getSolid(Context* context, int id); // Return the Solid data from context whose Solid_id is id

// ------------------------------------------------

// SET DRAW_ID METHODS

void setDrawId(Context* (context), int sphere_id, int draw_id); //Set the draw_id argument of the particle whose id is sphere_id in the context to the value draw_id

void setDrawIdSphereCollider(Context* (context), int sphere_id, int draw_id); //Set the draw_id argument of the groundSphere whose id is sphere_id in the context to the value draw_id

void setDrawIdGroundCollider(Context* (context), int line_id, int draw_id); //Set the draw_id argument of the groundSphere whose id is sphere_id in the context to the value draw_id

// ------------------------------------------------

// CHECK_CONTACT METHODS

void checkContactWithPlane(Context* context,int particle_id); // Check if the particle from context whose particle_id is particle_id is in contact with a plane of context and save Constraint data if True

void checkContactWithSphere(Context* (context),int particle_id); // Check if the particle from context whose particle_id is particle_id is in contact with a SphereCollider of context and save Constraint data if True

void checkDynamicConstraint(Context* context,int particle_id); // Check if the particle from context whose particle_id is particle_id is in contact with another particle of context and save Constraint data if True

void checkSolidConstraint(Context* context, int particle_id);  // Check if a particle from context whose particle_id is particle_id is part of a solid and save Constraint data if True

// ------------------------------------------------

void updatePhysicalSystem(Context* (context), float dt, int num_constraint_relaxation);

// ------------------------------------------------
// Methods below are called by updatePhysicalSystem
// ------------------------------------------------

void applyExternalForce(Context* (context), float dt); // Add the effect of external force to position of all particles from context
void dampVelocities(Context* (context));
void updateExpectedPosition(Context* (context), float dt); 
void addDynamicContactConstraints(Context* (context)); // Update the dynamic Contact Constraints register
void addStaticContactConstraints(Context* (context)); // Update the static Contact Constraints register 
void addSolidConstraint(Context* (context)); // Update the Solid Constraints register (NON TESTE)
void projectConstraints(Context* (context)); // Calculate and add the effect of Contact constraint to position of all particles from context
void updateVelocityAndPosition(Context* (context), float dt); // Update velocity and position of all context's particles after calculation of all constraints and external forces effects.
void applyFriction(Context* (context));
void deleteContactConstraints(Context* (context)); // Reset all constraint counter from context

// ------------------------------------------------

