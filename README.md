# carvr
carvr implements image resizing through [seam carving](http://www.youtube.com/watch?v=6NcIJXTlugc).  It is optimized for speed and has a very minimal interface.

![Demo](http://mattkeeter.com/projects/carvr/demo.gif)

# Dependancies
You can install all of the dependancies through [Homebrew](http://brew.sh/):  
```brew install opencv wxmac cmake imagemagick libicns```

 * `opencv` and `wxmac` are used for image processing and UI respectively
 * `cmake` is used to generate a Makefile
 * `imagemagick` and `libicns` are used to generate the application icon (don't judge me)

# Building
Type `make` in the root directory (there's a top-level Makefile that delegates all of the hard work to CMake).

# Download
Alternatively, download a prebuild .app.
