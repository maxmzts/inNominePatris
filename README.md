# In Nomine Patris

## Description
A 2D action RPG with roguelike elements. It uses a custom 2D engine built from scratch, featuring tile management for map design, collision handling, dynamic music, sound effects, visual effects, and a custom save system.

## Gameplay
The objective of the game is completing its 3 worlds. Each world has rooms with enemies and a boss at the end. Being either defeated or victorious will provide the player with karma, a persisten currency used to acquire permantent upgrades. 

The upgrade tree is split into two branches: **Absolution** and **Sin**. Choosing an upgrade from one branch will lock the corresponding upgrade on the same level from the opposite branch.  
*Note: The story is not yet implemented.*

## Installation

### Requirements
- Ubuntu 24.04 (Windows subsytems included)
- C++14 or higher
- SFML 2.5+
- TinyXML-2
- CMake 3.1+
- Clang
- VSCode

### Build Instructions

1. Having all the necessary requirements, you'll need to open a terminal and clone the repository:
```bash
git clone https://github.com/maxmzts/inNominePatris.git
```
2. Download the assets from the cloud:
``` bash
cd inNominePatris/src
./setup.sh
```
3. On completion, open the editor:
``` bash
code
```
4. When VSCode is running go to the top toolbar to `File` -> `Open Workspace from File...` and select the file named `project.code-workspace`.
5. Then press F7 to compile the project.
6. Finally, press F5 to run the build.

## Technology
The game is developed in **C++**, using **SFML** as the graphics library, **Clang** as the compiler, and **CMake** for build configuration.  
It also uses **TinyXML-2** for tilemap handling.

## Authors
- Máximo Martínez Torres — Enemy AI, Sound and Save System. 
- David González Moreno — Levels, Interaction management, Player and Tile management.
- Jorge Pelegrín Bru — Combat, Weapons, Upgrades and some sprites.  
- Estrella Domínguez — Game State and HUD.

Note: most assets are external resources found online, see more on CREDITS.md.

## AI-generated Music Notice

All background music used in this project was generated using SUNO AI, an AI-based music generation tool. This approach was chosen due to the academic nature of the project, limited resources, and the absence of dedicated composers within the team.

The generated tracks serve a decorative and illustrative purpose only and are not intended for commercial use.

We acknowledge the growing role of generative tools in creative workflows and remain committed to transparency regarding the origin of all assets used in this project.
