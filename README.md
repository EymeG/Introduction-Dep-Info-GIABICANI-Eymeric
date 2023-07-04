# Windows 

If Windows 11 + WSL2 see Unix instructions

Case 1 : If you have an Anaconda environment to develop in python, everything required 
for the 2D simulation project should already be installed on your computer.
Case 2 : Similarly if you have access to python and gcc (usually because MinGW was installed at some point),
it is possible to follow the same steps.

Launch Anaconda Power Shell (case 1) or a regular power shell (case 2) 
and execute following build commands :
  gcc -c -fPIC particle.c -o particle.o
  gcc -fPIC -shared particle.o -o libparticle.dll
and to launch the python script :
  python3 particle_gui.py
=>  a tkinter window should open with a red sphere falling.

Note that if you do not have access to the gcc command in a powershell, it is also possible
to install gcc with mingw with the following package manager.
  https://chocolatey.org/install


# Unix (this will work directly on school computers)

To compile run the following build commands :
  gcc -c -fPIC particle.c -o particle.o
  gcc -fPIC -shared particle.o -o libparticle.so
Then launch the python script :
  python3 particle_gui.py
=>  a tkinter window should open with a red sphere falling.

If tkinter is not install, it can be installed using:
  sudo apt install python3-tk
  
  
# COMMAND LINE....  
gcc -c -fPIC particle.c Vec2.c Constraint.c;  gcc -fPIC -shared particle.o Vec2.o Constraint.o -o libparticle.so



