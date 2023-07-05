#include "Context.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

//----------------------------------------------------------------

Context* initializeContext(int capacity)
{
  Context* (context) = malloc(sizeof(Context));

  (context) -> num_particles = 0;
  (context) -> capacity_particles = capacity;
  (context) -> particles = malloc(capacity*sizeof(Particle));
  memset((context) -> particles,0,capacity*sizeof(Particle));

  (context) -> num_solids=0;
  (context) -> solids = malloc(capacity*sizeof(Solid));

  (context) -> num_ground_sphere = 5;
  (context) -> ground_spheres = malloc(((context) -> num_ground_sphere)*sizeof(SphereCollider));
  Vec2 p0 = {0.0f, 1.0f};
  Vec2 p1 ={4.5f,4.1f};
  Vec2 p2={-4.5f,4.1f};
  Vec2 p3 ={8.0f,-2.0f};
  Vec2 p4={-8.0f,-2.0f};

  (context) -> ground_spheres[0].center = p0;
  (context) -> ground_spheres[0].radius = 1.7f;
  (context) -> ground_spheres[1].center=p1;
  (context) -> ground_spheres[1].radius=1.7f;
  (context) -> ground_spheres[2].center=p2;
  (context) -> ground_spheres[2].radius=1.7f;
  (context) -> ground_spheres[3].center=p3;
  (context) -> ground_spheres[3].radius=2.0f;
  (context) -> ground_spheres[4].center=p4;
  (context) -> ground_spheres[4].radius=2.0f;

  (context) -> num_ground_line=1;
  (context) ->  ground_line = malloc(((context) -> num_ground_line)*sizeof(GroundCollider));
  Vec2 b0= {-16.0f, -2.0f};
  Vec2 b1={-16.0f,-2.0f};
  Vec2 b2={0.0f,1.0f};
  (context) -> ground_line[0].leftcoord=b0;
  (context) -> ground_line[0].rightcoord=b1;
  (context) -> ground_line[0].norm=b2;

  (context) -> num_ground_constraint=0;
  (context) -> ground_constraint=malloc((context -> capacity_particles)*(context -> num_ground_line+context -> num_ground_sphere)*sizeof(GroundConstraint));

  (context) -> num_dynamic_constraint=0;
  (context) -> inter_constraint=malloc((context -> capacity_particles)*(context -> capacity_particles)*sizeof(InteractionConstraint));

  (context) -> num_solid_constraint=0;
  (context) -> solid_constraint=malloc((context -> capacity_particles)*(context -> capacity_particles)*sizeof(SolidConstraint));
  
  return (context);
}

//----------------------------------------------------------------

void addParticle(Context* (context), float x, float y, float radius, float mass, int draw_id)
{
    assert((context) -> num_particles<(context) -> capacity_particles); // currently no resize in (context)
    (context) -> particles[(context) -> num_particles].position.x = x;
    (context) -> particles[(context) -> num_particles].position.y = y;
    (context) -> particles[(context) -> num_particles].velocity.x = 0.0F;
    (context) -> particles[(context) -> num_particles].velocity.y = 0.0F;
    (context) -> particles[(context) -> num_particles].inv_mass = 1.0F/mass;
    (context) -> particles[(context) -> num_particles].radius = radius;
    (context) -> particles[(context) -> num_particles].draw_id = draw_id;
    (context) -> particles[(context) -> num_particles].solid_id = -1;
    (context) -> num_particles += 1;
}

//----------------------------------------------------------------

void addSolid(Context* context, float x, float y, float radius, float mass, int partnumb, int distance, int draw_id){
  assert((context) -> num_particles<(context) -> capacity_particles);
  context -> solids[context -> num_solids].solidcomp = malloc(partnumb*sizeof(int));
  context -> solids[context -> num_solids].distance = distance;
  context -> solids[context -> num_solids].partnumb = partnumb;
  
  for(int i = 0; i < partnumb; i++){
    context -> solids[context -> num_solids].solidcomp[i] = context -> num_particles;
    addParticle(context, x+2*radius*i, y, radius, mass, draw_id);
    context -> particles[(context -> num_particles)-1].solid_id = context -> num_solids;
    context -> num_solids= context -> num_solids+1;
  }
}

//----------------------------------------------------------------

Particle getParticle(Context *context, int id)
{
  return (context) -> particles[id];
}

//----------------------------------------------------------------

SphereCollider getGroundSphereCollider(Context* (context), int id)
{
  return (context) -> ground_spheres[id];
}

//----------------------------------------------------------------

GroundCollider getGroundLine(Context* (context), int id)
{
  return (context) -> ground_line[id];
}

//----------------------------------------------------------------

Solid getSolid(Context* context, int id)
{
  return (context) -> solids[id];
}

//----------------------------------------------------------------

void setDrawId(Context* (context), int sphere_id, int draw_id)
{
  (context) -> particles[sphere_id].draw_id = draw_id;
}

//----------------------------------------------------------------

void setDrawIdSphereCollider(Context* (context), int sphere_id, int draw_id)
{
  (context) -> ground_spheres[sphere_id].draw_id=draw_id;
}

//----------------------------------------------------------------

void setDrawIdGroundCollider(Context* (context), int line_id, int draw_id)
{
  (context) -> ground_line[line_id].draw_id=draw_id;
}

//----------------------------------------------------------------

void checkContactWithPlane(Context* context,int particle_id)
{ 

  for(int i =0; i < (context) -> num_ground_line; i++){
    Vec2 closestpoint = {context -> particles[particle_id].next_pos.x, context -> ground_line[i].rightcoord.y};

    if (PrScal(soustvec(context -> particles[particle_id].next_pos,closestpoint),context -> ground_line[i].norm)-(context -> particles[particle_id].radius)<0.0f){
      context -> ground_constraint[context -> num_ground_constraint].closestpoint=closestpoint;
      context -> ground_constraint[context -> num_ground_constraint].norm=context -> ground_line[i].norm;
      context -> ground_constraint[context -> num_ground_constraint].particle_id=particle_id;
      context -> num_ground_constraint=context -> num_ground_constraint+1;
    } 
  }
} 

//---------------------------------------------------------------- 

void checkContactWithSphere(Context* (context),int particle_id)
{
  if(context ->  num_ground_sphere == 0)return;
  for(int i=0; i < (context) -> num_ground_sphere; ++i){
    Vec2 VecTest=soustvec(context -> particles[particle_id].next_pos,context -> ground_spheres[i].center);
    float sdf= norm(VecTest)-(context -> ground_spheres[i].radius);
    if(sdf<context -> particles[particle_id].radius){
      VecTest=mulscal(1.0/norm(VecTest),VecTest);
      context -> ground_constraint[context -> num_ground_constraint].closestpoint=soustvec(context -> particles[particle_id].next_pos,mulscal(sdf,VecTest));
      context -> ground_constraint[context -> num_ground_constraint].norm=VecTest;
      context -> ground_constraint[context -> num_ground_constraint].particle_id=particle_id;
      context -> num_ground_constraint=context -> num_ground_constraint+1;
    } 
  }
}

//----------------------------------------------------------------

void checkDynamicConstraint(Context* context,int particle_id)
{
  if (context ->  num_particles == 0){return;}
  for(int j=0; j<context -> num_particles;j++){
    if(j != particle_id){
      Vec2 x_ij=soustvec(context -> particles[particle_id].next_pos,context -> particles[j].next_pos);
      float C= norm(x_ij)-(context -> particles[particle_id].radius+context -> particles[j].radius);
      if(C<0){
        context -> inter_constraint[context -> num_dynamic_constraint].contact_id=j;
        context -> inter_constraint[context -> num_dynamic_constraint].particle_id=particle_id;
        context -> inter_constraint[context -> num_dynamic_constraint].C=C;
        context -> inter_constraint[context -> num_dynamic_constraint].norm=x_ij;
        context -> num_dynamic_constraint=context -> num_dynamic_constraint+1;
      }
    }
  }
}

//----------------------------------------------------------------

void checkSolidConstraint(Context* context, int particle_id){
  if (context ->  num_particles == 0) return;
  if (context -> particles[particle_id].solid_id != -1){
    int i = context -> particles[particle_id].solid_id;
    int n = context -> solids[i].partnumb;
    for(int j = 0; j < n ; j++){
      if(context -> solids -> solidcomp[j] != particle_id){
        Vec2 x_ij=soustvec(context -> particles[particle_id].next_pos,context -> particles[context ->  solids -> solidcomp[j]].next_pos);
        float C= norm(x_ij)-(context -> solids[i].distance);
        if (C<0){
          context -> solid_constraint[context -> num_solid_constraint].contact_id=j;
          context -> solid_constraint[context -> num_solid_constraint].particle_id=particle_id;
          context -> solid_constraint[context -> num_solid_constraint].C=C;
          context -> solid_constraint[context -> num_solid_constraint].norm= x_ij;
          context -> num_solid_constraint = context -> num_solid_constraint + 1;
      }
      }
    }

  }
}

//----------------------------------------------------------------


void updatePhysicalSystem(Context* (context), float dt, int num_constraint_relaxation)
{
  applyExternalForce((context), dt);
  dampVelocities((context));
  updateExpectedPosition((context), dt);
  addDynamicContactConstraints((context));
  addStaticContactConstraints((context));
 
  for(int k=0; k<num_constraint_relaxation; ++k) {
    projectConstraints((context));
  }

  updateVelocityAndPosition((context), dt);
  applyFriction((context));

  deleteContactConstraints((context));
}

//----------------------------------------------------------------

void applyExternalForce(Context* (context), float dt)
{
  if ((context) -> num_particles == 0) return;
  Particle *p = (context) -> particles;
  float alpha= 0.3;

  for (int i = 0; i<(context) -> num_particles; ++i){
    float inv_mass=context -> particles[i].inv_mass;
    (p+i) -> velocity.y += -dt* 9.81;
    (p+i) -> velocity = sumvec((p+i) -> velocity, mulscal(-dt*alpha*inv_mass,(p+i) ->  velocity));
  }
}

//----------------------------------------------------------------

void dampVelocities(Context* (context))
{
}

//----------------------------------------------------------------

void updateExpectedPosition(Context* (context), float dt)

{
  if ((context) -> num_particles == 0) return;
  Particle *p = (context) -> particles;
  for (int i = 0; i<(context) -> num_particles; ++i){
    (p+i) -> next_pos=sumvec((p+i) -> position,mulscal(dt,(p+i)  -> velocity));

}
}

//----------------------------------------------------------------

void addDynamicContactConstraints(Context* (context))
{
  if ((context) -> num_particles == 0) return;
  for (int i = 0; i<(context) -> num_particles;++i){
    checkDynamicConstraint(context,i);
  }
}

//----------------------------------------------------------------

void addSolidConstraint(Context* (context))
{
  if ((context) -> num_particles == 0) return;
  for (int i = 0; i<(context) -> num_particles; i++){
    checkSolidConstraint(context,i);
  }
}

//----------------------------------------------------------------

void addStaticContactConstraints(Context* (context))
{
  if ((context) -> num_particles == 0) return;
  for (int i = 0; i<(context) -> num_particles; ++i){
    checkContactWithPlane(context, i);
    checkContactWithSphere(context,i);
  }
}

//----------------------------------------------------------------

void projectConstraints(Context* (context)) // Calculation and update of particles' positions of context due to constraint registration
{

// Calculation of constraints due to Static Objects (GroundColliders and SphereColliders of context)
  GroundConstraint* gc= context -> ground_constraint;
  for (int i = 0; i<(context) -> num_ground_constraint; ++i){
  GroundConstraint gc_i= gc[i];
  Particle part_i=context -> particles[gc_i.particle_id];
  Vec2 qc=soustvec(part_i.next_pos,mulscal(PrScal(soustvec(part_i.next_pos,gc_i.closestpoint),(gc_i).norm),(gc_i).norm));
  float C=PrScal(soustvec(part_i.next_pos,qc),(gc_i.norm))-part_i.radius;
  Vec2 delta_i=mulscal(-C , gc_i.norm);
  context -> particles[gc_i.particle_id].next_pos=sumvec(part_i.next_pos,delta_i);
  }
  

// Calculation of constraints due to moving Objects (with particles of context)
  InteractionConstraint* ic= context -> inter_constraint;
  for (int i = 0; i<(context) -> num_dynamic_constraint;i++){
    InteractionConstraint ic_i=ic[i];
    Particle part_i=context -> particles[ic_i.particle_id];
    Particle part_j=context -> particles[ic_i.contact_id];
    float C= ic_i.C;
    float sigma_i=part_i.inv_mass/(part_i.inv_mass+part_j.inv_mass)*C;
    Vec2 delta_i=mulscal(-sigma_i/norm(ic_i.norm),ic_i.norm);
    context -> particles[ic_i.particle_id].next_pos=sumvec(part_i.next_pos,delta_i);
  }
  
// Calculation of constraints due to particles interactions in solids (particles of context) (NON TESTE)
  SolidConstraint* sc = context -> solid_constraint;
  for (int i = 0; i<(context) -> num_solid_constraint; i++){
    SolidConstraint sc_i=sc[i];
    Particle part_i = context -> particles[sc_i.particle_id];
    Particle part_j = context -> particles[sc_i.contact_id];
    float C = sc_i.C;
    float sigma_i=part_i.inv_mass/(part_i.inv_mass+part_j.inv_mass)*C;
    Vec2 delta_i=mulscal(-sigma_i/norm(sc_i.norm),sc_i.norm);
    context -> particles[sc_i.particle_id].next_pos=sumvec(part_i.next_pos,delta_i);
  }

 }
    
//----------------------------------------------------------------

void updateVelocityAndPosition(Context* (context), float dt){
  if ((context) -> num_particles==0) {return;}
  Particle* p = (context) -> particles;
  for (int i = 0; i<(context) -> num_particles; ++i){
    (p+i) -> velocity=mulscal(1.0/dt, soustvec((p+i) -> next_pos,(p+i) -> position));
    (p+i) -> position=(p+i) -> next_pos;
  }
}

//----------------------------------------------------------------

void applyFriction(Context* (context)) // Friction effect on particle is added in applyExternalForce method.
{
}

//----------------------------------------------------------------

void deleteContactConstraints(Context* (context)) // Reset all constraint counter from context
{
  context -> num_ground_constraint = 0;
  context -> num_dynamic_constraint = 0;
  context -> num_solid_constraint = 0;
}


