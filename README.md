Rim Lighting
============
Simple rim lighting in OpenGL and GLSL which highlight edges of objects. I
implemented it as a post processing effect in order to separate the rim
lighting shader code but it is arguably more efficient to do it in your
standard light shader.

+ Press `F1` to toggle free look control on/off.
+ Press `F2` to toggle _SPOOKY_ mode on/off.
+ Press `+` to increase rim width.
+ Press `-` to decrease rim width.
+ Press `1`, `2`, `3` or `4` to switch between rim colors.

License
-------
Licensed under GNU GPL v3.0.

Screenshot
----------
![scrot](https://github.com/mharrys/rim/raw/master/scrot.png "Screenshot")

How-to
------
You will need a C++11 compiler, GLM, GLEW, Assimp and SDL2. Consult SConstruct for
details.

Build and run

    $ scons
    $ cd bin
    $ ./bloom

Cleanup

    $ scons -c

References
----------
1. Renderman - Fake Rim-Light Surface Shader. [downloaded 2014-10-10]. Available from http://www.fundza.com/rman_shaders/surface/fake_rim/fake_rim1.html.
2. OpenGL Rim Shader. [downloaded 2014-10-10]. Available from http://www.roxlu.com/2014/037/opengl-rim-shader.
