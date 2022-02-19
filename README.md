# ogl-renderer
A personal implementation of an OpenGL renderer, made in an effort to learn the ins and outs of graphics programming.

# Setup Dependencies
Project relies on assimp and glfw from external sources, available from the following repositories respectively:
- https://github.com/assimp/assimp
- https://github.com/glfw/glfw

Build both dependencies using CMake. VS project files search for libs with the relative path `..\dependencies\`, so setting the CMake install prefixes to `..\dependencies\Assimp` and `..\dependencies\GLFW` respectively should do the trick.

## Assimp
Project was built using version 5.3.1 which is available from the repository as a tag. Newer versions should work if available but are likely not tested by me.

Don't forget to add the installed `bin` directory to PATH so that VS can find the `.dll` files for runtime linking.

## GLFW
Project was built using version 3.3.6 which is available from the repository as a tag. Newer versions should work if available but are likely not tested by me.
