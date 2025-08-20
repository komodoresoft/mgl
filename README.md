# 📦 Modern Legacy GL (MGL)

**MGL** is an OpenGL 1-style wrapper for modern OpenGL functions, keeping an old-school, lightweight interface similar to legacy OpenGL. Perfect for small engines, retro-style games, or learning purposes.<br>

![Build](https://img.shields.io/badge/build-kmak-lightgrey?style=flat-square&logo=c) ![Lang](https://img.shields.io/badge/language-C-blue?style=flat-square&logo=c) ![License](https://img.shields.io/badge/license-MIT-green?style=flat-square) ![Status](https://img.shields.io/badge/status-WIP-orange?style=flat-square)

## 📝 Example
This little snippet shows what the library allows you to do.

```c
mglMatrixMode(GL_PROJECTION);
mglLoadIdentity();
mgluPerspective(60.0, (float)width/(float)height, 0.1, 100.0);
mgluTranslatef(0.0, 0.0, -5.0);

mglMatrixMode(GL_MODELVIEW);
mglLoadIdentity();

mglBegin(GL_TRIANGLES);
    mglColor3f(1.0, 0.0, 0.0); mglVertex2f( 0.0, -1.0);
    mglColor3f(0.0, 1.0, 0.0); mglVertex2f(-0.5,  0.5);
    mglColor3f(0.0, 0.0, 1.0); mglVertex2f( 0.5,  0.5);
mglEnd();
```

## ⚙️  Prerequisites
To compile and use MGL, you need to install the following prerequisites:
- C compiler (GCC, Clang, MSVC)
- OpenGL headers and libraries
- [The KMAK Build System](https://komodoresoft.com/kmak/)

## 📦 Installation
To install the library and use it in your projects, use the following commands:

```bash
git clone https://github.com/komodoresoft/mgl
cd mgl
kmak make.kmk build_mgl
```

## 🧩 Compiling the Examples
To compile the examples you need [glfw3](https://www.glfw.org/).
- Once you downloaded the glfw3, copy the headers in `include\GLFW`
- Then, place the compiled libraries in `lib`
- Next, place `glfw3.dll` in `examples`
- Finally, you can compile the examples with:
```bash
kmak make.kmk build_tests
```
