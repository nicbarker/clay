# Playdate console example

This example uses a modified version of the document viewer application from the Clay video demo. The Playdate console has a very small black and white screen, so some of the fixed sizes and styles needed to be modified to make the application usable on the console. The selected document can be changed using up/down on the D-pad, and the selected document can be scrolled with the crank.

## Building

You need to have the (Playdate SDK)[https://play.date/dev/] installed to be able to build this example. Once it's installed you can build it by adding -DCLAY_INCLUDE_PLAYDATE_EXAMPLES=ON when initialising a directory with cmake.

e.g.

```
cmake -DCLAY_INCLUDE_PLAYDATE_EXAMPLES=ON cmake-build-debug
```

And then build it:

```
cmake --build cmake-build-debug
```

The pdx file will be located at examples/playdate-project-example/clay_playdate_example.pdx. You can then open it with the Playdate simulator.

## Building for the playdate device

By default building this example will produce a pdx which can only run on the Playdate simulator application. To build a pdx that can run on the Playdate hardware you need to set the toolchain to use armgcc, toolchain file to the arm.cmake provided in the playdate SDK and make sure to disable the other examples. The Playdate hardware requires threads to be disabled which is not compatible with some of the other examples.

e.g. To setup the cmake-build-release directory for device builds:

```
cmake -DTOOLCHAIN=armgcc -DCMAKE_TOOLCHAIN_FILE=/Users/mattahj/Developer/PlaydateSDK/C_API/buildsupport/arm.cmake -DCLAY_INCLUDE_ALL_EXAMPLES=OFF -DCLAY_INCLUDE_PLAYDATE_EXAMPLES=ON -B cmake-build-release
```

And then build it:

```
cmake --build cmake-build-release
```
