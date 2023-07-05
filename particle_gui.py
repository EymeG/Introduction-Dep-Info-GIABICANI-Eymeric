import ctypes
import pathlib
import time
import os
import sys
from tkinter import *
from ctypes import *

##################################################
# Initialize C bindings
##################################################

# Load the shared library into ctypes
if sys.platform == 'win32':
  libname = os.path.join(pathlib.Path().absolute(), "libparticle.dll")
else : 
  libname = os.path.join(pathlib.Path().absolute(), "libparticle.so")
c_lib = ctypes.CDLL(libname)

# Define python equivalent class
class SPHERE_COLLIDER(Structure):
    _fields_ = [("center", c_float*2),
                ("radius", c_float),
                ("draw_id",c_int)]
    
class GROUND_COLLIDER(Structure):
    _fields_= [("leftcoord", c_float*2),
               ("rightcoord",c_float*2),
               ("norm",c_float*2),
               ("draw_id",c_int)]
    
class GROUND_CONSTRAINT(Structure):
    _fields_=[("closestpoint",c_float*2),
              ("norm",c_float*2),
              ("particle_id",c_int)]

class INTERACTION_CONSTRAINT(Structure):
    _fields_=[("norm",c_float*2),
              ("particle_id",c_int),
              ("contact_id",c_int)]

class SOLID_CONSTRAINT(Structure):
    _fields_=[("norm",c_float*2),
              ("particle_id",c_int),
              ("contact_id",c_int)]

class PARTICLE(Structure):
    _fields_ = [("position", c_float*2),
                ("next_pos", c_float*2),
                ("velocity", c_float*2),
                ("inv_mass", c_float),
                ("radius",   c_float),
                ("solid_id", c_int),
                ("draw_id",  c_int),
                ]

class SOLID(Structure):
    _fields_ = [("solidcomp", POINTER(PARTICLE)),
                ("distance", c_float),
                ("partnumb",c_int)]
    
class CONTEXT(Structure):
    _fields_ = [("num_particles", c_int),
                ("capacity_particles", c_int),
                ("particles", POINTER(PARTICLE)),
                ("num_solids",c_int),
                ("solids",POINTER(SOLID)),
                ("num_ground_sphere", c_int),
                ("ground_spheres",POINTER(SPHERE_COLLIDER)),
                ("num_ground_line",c_int),
                ("ground_line",POINTER(GROUND_COLLIDER)),
                ("num_ground_constraint",c_int),
                ("ground_constraint", POINTER(GROUND_CONSTRAINT)),
                ("num_solid_constraint",c_int),
                ("solid_constraint", POINTER(SOLID_CONSTRAINT))]

# ("pos", c_float*2) => fixed size array of two float


# Declare proper return types for methods (otherwise considered as c_int)
c_lib.initializeContext.restype = POINTER(CONTEXT) # return type of initializeContext is Context*
c_lib.getParticle.restype = PARTICLE # return type of getParticle is Particle
c_lib.getGroundSphereCollider.restype = SPHERE_COLLIDER # return type of getGroundSphereCollider is SphereCollider
c_lib.getGroundLine.restype = GROUND_COLLIDER # return type of getGroundLine is GroundCollider
c_lib.getSolid.restype= SOLID

# WARNING : python parameter should be explicitly converted to proper c_type of not integer.
# If we already have a c_type (including the one deriving from Structure above)
# then the parameter can be passed as is.

##################################################
# Class managing the UI
##################################################

class ParticleUI :
    def __init__(self) :
        # create drawing context
        self.context = c_lib.initializeContext(300)
        self.width = 1000
        self.height = 1000

        # physical simulation will work in [-world_x_max,world_x_max] x [-world_y_max,world_y_max]
        self.world_x_max = 10 
        self.world_y_max = 10 
        # WARNING : the mappings assume world bounding box and canvas have the same ratio !

        self.window = Tk()

        # create simulation context...
        self.canvas = Canvas(self.window,width=self.width,height=self.height)
        self.canvas.pack()

        # Initialize drawing, only needed if the context is initialized with elements,
        # otherwise, see addParticle
        for i in range(self.context.contents.num_particles):
            sphere = c_lib.getParticle(self.context, i)
            draw_id = self.canvas.create_oval(*self.worldToView( (sphere.position[0]-sphere.radius,sphere.position[1]-sphere.radius) ),
                                              *self.worldToView( (sphere.position[0]+sphere.radius,sphere.position[1]+sphere.radius) ),
                                              fill="orange")
            c_lib.setDrawId(self.context, i, draw_id) 
        
        for i in range(self.context.contents.num_ground_sphere):
            sphere = c_lib.getGroundSphereCollider(self.context, i)
            draw_id = self.canvas.create_oval(*self.worldToView( (sphere.center[0]-sphere.radius,sphere.center[1]-sphere.radius) ),
                                              *self.worldToView( (sphere.center[0]+sphere.radius,sphere.center[1]+sphere.radius) ),
                                              fill="blue")
            c_lib.setDrawIdSphereCollider(self.context, i, draw_id) 
        
        for i in range(self.context.contents.num_ground_line):
            line = c_lib.getGroundLine(self.context, i)
            draw_id = self.canvas.create_line(*self.worldToView( (line.rightcoord[0],line.rightcoord[1]) ),
                                              *self.worldToView( (line.leftcoord[0],line.leftcoord[1]) ),
                                              fill="black",width=4)
            c_lib.setDrawIdGroundCollider(self.context, i, draw_id) 

        # Initialize Mouse and Key events
        self.canvas.bind("<Button-1>", lambda event: self.mouseCallback(event))
        self.window.bind("<Key>", lambda event: self.keyCallback(event)) # bind all key
        self.window.bind("<Escape>", lambda event: self.enterCallback(event)) 
        # bind specific key overide default binding

    def launchSimulation(self) :
        # launch animation loop
        self.animate()
        # launch UI event loop
        self.window.mainloop()



    def animate(self) :
        """ animation loop """
        # APPLY PHYSICAL UPDATES HERE !
        for i in range(6) :
            c_lib.updatePhysicalSystem(self.context, c_float(0.016/float(6)), 1) #TODO can be called more than once..., just devise dt
            
        for i in range(self.context.contents.num_particles):
            sphere = c_lib.getParticle(self.context, i)
            self.canvas.coords(sphere.draw_id,
                               *self.worldToView( (sphere.position[0]-sphere.radius,sphere.position[1]-sphere.radius) ),
                               *self.worldToView( (sphere.position[0]+sphere.radius,sphere.position[1]+sphere.radius) ) )
            

            
            
        
        
        self.window.update()
        self.window.after(16, self.animate)

    # Conversion from worl space (simulation) to display space (tkinter canvas)
    def worldToView(self, world_pos) :
        return ( self.width *(0.5 + (world_pos[0]/self.world_x_max) * 0.5),
                 self.height *(0.5 - (world_pos[1]/self.world_y_max) * 0.5)) 
    def viewToWorld(self, view_pos) :
        return ( self.world_x_max * 2.0 * (view_pos[0]/self.width - 0.5) ,
                 self.world_y_max * 2.0 * (0.5-view_pos[1]/self.height))  

    def addParticle(self, screen_pos, radius, mass) :
        (x_world, y_world) = self.viewToWorld(screen_pos)
        # min max bounding box in view coordinates, will be propertly initialized 
        # in the canvas oval after the first call to animate
        #b_min = self.worldToView( (x_world-radius,y_world-radius) )
        #b_max = self.worldToView( (x_world+radius,y_world+radius) )
        draw_id = self.canvas.create_oval(0,0,0,0,fill="red")
        c_lib.addParticle(self.context, 
                        c_float(x_world), c_float(y_world), 
                        c_float(radius), c_float(mass),
                        draw_id)
    
    def addSolid(self, screen_pos, radius, mass, distance, partnumb):
        (x_world,y_world)= self.viewToWorld(screen_pos)
        draw_id=self.canvas.create_oval(0,0,0,0, fill="orange")
        c_lib.addSolid(self.context,c_float(x_world), c_float(y_world),c_float(radius), c_float(mass),draw_id)

    # All mouse and key callbacks

    def mouseCallback(self, event):
        self.addParticle((event.x,event.y), 0.5, 0.5)
    
    def keyCallback(self, event): # Add a solid at mouse pointer position if any key except enter is pressed (NON FONCTIONNEL, les particules s'ajoutent néanmoins mais décollent instantanément par collision)
        self.addSolid((event.x,event.y),0.6,0.5,0.55,3)

    def enterCallback(self, event):
        self.window.destroy()


gui = ParticleUI()
gui.launchSimulation()

