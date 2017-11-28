# island-life-game
OpenGL game made as result of Introduction to Computer Graphics classes.

### Requirenments
**OpenGL**

**GLUT**

**GLEW**  

Install glew lib:
> $sudo apt-get install libglew-dev

**GLM**

Install glm lib:
> $sudo apt-get install libglm-dev

**GLFW**

> git clone https://github.com/glfw/glfw && cd glfw && mkdir -p build && cd build && cmake -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ .. && make -j4 && sudo make install

**FreeType**

Download FreeType 2.8.1 lib:
>https://sourceforge.net/projects/freetype/files/

Copy _freetype_ directory  and _ft2build.h_ to /usr/include/

**SOIL**

Install the soil lib:
> $sudo apt-get install libsoil-dev


- - - -

### Compile

> make all

- - - -

### Execute

> make run

- - - -

### Clean

> make clean
