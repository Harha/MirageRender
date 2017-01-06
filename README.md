Mirage Render
======

Mirage Render is a 3d renderer that aims for photorealistic results. Currently the software is in very early stage.

Build status and downloads
------------
| Build status | Build downloads |
| ------------ | --------------- |
| [![Build Status](http://harha.us.to:8080/buildStatus/icon?job=MirageRender_Linux_build)](http://harha.us.to:8080/job/MirageRender_Linux_build/) | [Linux x64](http://harha.us.to/jenkins/MirageRender_Linux/) |
| [![Build Status](http://harha.us.to:8080/buildStatus/icon?job=MirageRender_Windows_build)](http://harha.us.to:8080/job/MirageRender_Windows_build/) | [Windows x64](http://harha.us.to/jenkins/MirageRender_Windows/) |

Usage
-----

```
mirage --help
LOG | Main: MirageRender, version 0.0.28
LOG | Usage: mirage.exe --script scriptfilename.lua, folder for scripts is ./res/scripts/
      Available launch parameters:
      Show this help message: --help, -h
      Load a scene file: --script, -s
LOG | Main: atexit(dispose) Hook called.
```

Example images
--------------

![Dabrovic sponza test](images/dabrovic_sponza.png "Dabrovic sponza, test")

_a Dabrovic sponza scene rendered with indirect illumination._

![Crytek sponza test](images/sponza_crytek_textures.png "Crytek sponza, test")

_a Crytek sponza scene rendered with direct and indirect illumination._

![Cornell Box, Golden Dragon](images/golden_dragon.png "Cornell Box, glossy material test")

_a Cornell box scene with a glossy metallic floor, a golden dragon and two room surfaces acting as light sources._

![Mitsuba, Texture Mapping](images/texturemapping.png "Mitsuba, texture mapping test")

_a Mitsuba sphere, spot light source and a diffuse texture applied on the floor mesh._

![Cornell Box, caustics test](images/mirage_water3.png "Cornell Box, caustics test")

_a Cornell box scene rendered using monte carlo path tracing, caustics caused by water, indirect illumination._

![Cornell Box, gi test](images/mirage_cornellbox_plight.png "Cornell Box, gi test")

_a Cornell box scene rendered using monte carlo path tracing, direct & indirect illumination._