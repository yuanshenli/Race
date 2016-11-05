# `Planets` : A Quick Look at Graphics Concepts

This is an OpenFrameworks project that demonstrates the usage of OF graphical primitive objects, the concepts behind `ofPushMatrix()` and `ofPopMatrix()`, translation, rotation, etc.

Note: this is graphics only -- no sound here!

## To build

* Extract this directory and place it in your `$OF/apps/myapps/` directory, where `$OF` is the path to your OpenFrameworks directory. For example, this would end up in `/Users/tim/Desktop/of_v0.9.5_osx_release/apps/myApps/ofxPlanets/` on my OSX machine.

* Build the program.
    * On OSX, open up `planets.xcodeproj` and build the target for "*planets Release > My Mac (64 bit)*" (or Debug, or 32-bit, etc.)
    * On Linux, you can type `make` from the project directory. Alternately, if you want to use the [QtCreator IDE](https://www.qt.io/ide/), you can open up `planets.creator` and build.
    * Windows is untested, but you should be able to import the project into Visual Studio or QtCreator after you've followed the general OpenFrameworks installation directions.

* The compiled program should now be in your `ofxPlanets/bin` directory.

## Further reading

* [OpenFrameworks](http://openframeworks.cc/)
* [OpenFrameworks API / Documentation](http://openframeworks.cc/documentation/)
* [OpenFrameworks advanced graphics tutorial](http://openframeworks.cc/ofBook/chapters/advanced_graphics.html)
* [OpenFrameworks intro to vectors](http://openframeworks.cc/ofBook/chapters/stl_vector.html)