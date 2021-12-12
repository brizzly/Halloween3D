# Halloween 3D 🎃
Halloween 3D Shooter OpenSource Game Engine

Work in progess !

Features :
Simple and easy to understand code in C++
OpenGL Rendering
Lights
Scene
Entities
Fontes
Files
Camera
Maths
Load mesh : OBJ, ASE (Textures, Camera, Lights, Brush)
Load Textures : PNG, JPEG, TGA


Homepage : https://jadegame.com/games/halloween/

Collision detection lib by Amir Geva : https://github.com/amirgeva/coldet

Audio lib by un4seen : https://www.un4seen.com/

PYRA Forum : https://pyra-handheld.com/boards/threads/the-commercial-game-halloween-fps-for-linux-is-now-the-open-source-project.99588/

Thanks to : gbudny, Farox, pocak100

for the impressive work on Linux/Pyra version of the game

Additional assets here : https://github.com/brizzly/Halloween3D_Assets


HOW TO BUILD
============

### *Linux :* 

```
In alloween.h :

//#define H_MAC
#define H_LINUX
//#define H_WINDOWS
```

cd Halloween3D/HalloweenSrc
mkdir Build
cd Build
cmake ..
make

### *macOS :*

```
In alloween.h :

#define H_MAC
//#define H_LINUX
//#define H_WINDOWS
```

Install lipjepg-turbo for Mac (non iOS) (arm64) binary dmg https://sourceforge.net/projects/libjpeg-turbo/files/2.1.2/libjpeg-turbo-2.1.2.dmg/download
Add Include path : /opt/libjpeg-turbo/include**/**
Add Lib path : /opt/libjpeg-turbo/include**/**

