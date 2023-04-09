
# A Minecraft clone made in C with OpenGL

I'm making this so I can learn OpenGL, multithreading, graphics programming in general and I to also get to know how infinite generation is made.
As a bonus I get to recreate minecraft. 

## Features (so far)

 -  Infinite world generation
 -  Multithreading via pthreads 
 -  Decent performance I guess

## Wanna compile it?
   Download glfw3 and the source code and compile.

   Link with the following libraries
   ### On Windows
   > -lgdi32 -lopengl32 -lglfw3 
   ### On Linux
   > -lglfw -lGLU -lGL -lpthread -lXrandr -lm -lXxf86vm -lXi -lXinerama -lX11 -lrt -ldl 


## Todo
   - [ ] Proper chunk generation (Currently its all random)
   - [ ] Block placing and destroying (Through raycasting)
   - [ ] Transparency
   - [ ] Lighting and fancy shading
   - [ ] Collisions and fps movement 
