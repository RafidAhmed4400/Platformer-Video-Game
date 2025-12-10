# **Platformer Game**

A side-scrolling 2D platformer built in C++ using SDL2, OpenGL/GLSL, and SDL_mixer. This project features multi-level gameplay, scene management, physics-based movement, and audio integration.

<br> <br>

**Features:**

- Multi-scene system (Menu, Levels, Win/Lose screens)

- Physics-based movement and jumping

- Side-scrolling camera system

- Tile-based map system

- Background music and sound effects

- Fixed timestep game loop for consistent physics

<br> <br>

**Technologies Used:**

- C++

- SDL2

- OpenGL

- GLSL Shaders

- SDL_mixer

- GLM

<br> <br>

**Controls:**

**Player:**

- Left Arrow – Move Left

- Right Arrow – Move Right

- Spacebar – Jump

<br> <br>

**Extra Mechanics:**

- Limited lives system

- Scene transitions between levels

- Win/Lose conditions based on player performance

<br> <br>

**Game Rules:**

- Reach the end of each level to progress.

- Falling off the map or hitting hazards reduces lives.

- Running out of lives results in a game over.

<br> <br>

**Project Structure:**

- /project-root

- main.cpp

- Entity.h / Entity.cpp

- Map.h / Map.cpp

- Scene.h / Scene.cpp

- LevelA.h / LevelA.cpp

- LevelB.hpp / LevelB.cpp

- LevelC.hpp / LevelC.cpp

- Menu.h / Menu.cpp

- Win.h / Win.cpp

- Lose.h / Lose.cpp

- shaders/

- vertex_textured.glsl

- fragment_textured.glsl

- assets/

- sprites/

- tilemaps/

- audio/

<br> <br>

**How to Run:**

**Prerequisites:**

- Install SDL2, SDL_mixer, OpenGL, and GLM.

**Build (example using g++):**

- g++ main.cpp *.cpp -lSDL2 -lSDL2_mixer -lOpenGL -ldl -o platformer

Run:

- ./platformer
