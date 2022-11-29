# Creating a Zephyr Application

## The /src folder
The /src folder is found is the <`workspace-folder`>/<`application-folder`>/<`application`>/`src`. The /src folder house the main.c script, this folder is where west looks for the c script to build into a binary that is housed in the /build folder. This binary file will get flashed to the board using
```
west flash
```
## The /.west folder
the /.west folder hold a config file that points west to the <`application-folder`> and passes along other useful information to the west application. This config file will only ever need to be editing if you change the application name, as west will need to know the new location
## Device Trees
The device tree syntax and other useful information can be read at [here](https://docs.zephyrproject.org/1.14.0/guides/dts/index.html#:~:text=Device%20Tree%20provides%20a%20unified%20description%20of%20a,is%20also%20used%20to%20describe%20Zephyr-specific%20configuration%20information.) for our application however all you must know is there are two areas west looks to build our device tree. One is in the <`workspace-folder`>/<`application-folder`>/boards/<`board-folder`>/<`board`>.dts. Here is where the default device tree file lives. This file is provided to us by zephyr and we will not edit this file. The other area where west will look to add to the final device tree used in our application is <`workspace-folder`>/<`application-folder`>/<`application`>/boards/<`board`>.overlay. At compilation time the overlay will be added to the device tree, thus allowing us to edit this overlay to expose addition hardware. In the example provided just GPIO2 is exposed, however you can expose any hardware on the board through this overlay file.
## CMakeCache.txt
If changes were made to a file structure after the project was already built you can get conflicting cache errors that the compiler will throw at you. This is because CMake generates a cache file in different referenced directories called `CMakeChache.txt` In this situation you can just remove the `CMakeCache.txt` and it should resolve any cache issues.
## Building and Flashing Custom Apps
When building your custom application the same process in the getting started guide can be followed with some slight modifications. To build your application from the <`application-folder`> run
```
west build -b rpi_pico <application> -- -DOPENOCD=/usr/local/bin/openocd -DOPENOCD_DEFAULT_PATH=/usr/local/share/openocd/scripts -DRPI_PICO_DEBUG_ADAPTER=picoprobe
```
After the build process is finished you are now ready to flash your board. To flash the board with your built binaries run
```
sudo -s
. /home/<username>/zephyrproject/venv/bin/activate
west flash