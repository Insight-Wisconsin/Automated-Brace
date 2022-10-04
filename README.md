# Zephyr Setup

## Installing Ubunutu (Optional if already running linux)
In powershell open as adminstrator  
![Alt Image text](/pics/Run_Powershell_As_Admin.png?raw=true)  
Run to install WSL and Ubuntu
```
wsl --install -d ubuntu
```  
After installing Ubuntu and WSL follow the prompts in the launched terminal session until you get to a prompt similar to this:  
```
mrmartin@MaxsPC:~$
```  
## Installing Zephyr and Python dependecies
Update your system to the newest release     
```
sudo apt update  
sudo apt upgrade
```  
Create the directory to house all the zephyr scripts  
```
mkdir zephyrproject
```  
Add the Kitware Archive to the APT repoistories  
```
wget https://apt.kitware.com/kitware-archive.sh 
sudo bash kitware-archive.sh
```  
Install all the zephyr dependencies
```
sudo apt install --no-install-recommends git cmake ninja-build gperf \
ccache dfu-util device-tree-compiler wget \
python3-dev python3-pip python3-setuptools python3-tk python3-wheel xz-utils file \
make gcc gcc-multilib g++-multilib libsdl2-dev libmagic1
```  
Ensure that all that `cmake` is at least at 3.20.0, `Python` is at least 3.8, and `dtc` is at least 1.4.6  
```
cmake --version  
python3 --version  
dtc --version
```  
Download and create virtual env  
***NOTE: This virtual enviroment will be be the enviroment that we develop all of our code in so remember to activate it any time you do work***
```
sudo apt install python3-venv  
cd ~/zephyrproject  
python3 -m venv venv  
. ./venv/bin/activae
```  

Install `wheel` and `west` to get zephyr source code  
```
pip install wheel  
pip install west
```  
Get zephyr source code, and export the CMake package  
```
west init .  
west update  
west zephyr-export
```  
Install additional zephyr python dependencies  
```
pip install -r ~/zephyrproject/zephyr/scripts/requirments.txt
```
## Install Zephyr SDK
Download the latest Zephyr SDK bundle  
```
cd ~  
wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.15.0/zephyr-sdk-0.15.0_linux-x86_64.tar.gz  
wget -O - https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.15.0/sha256.sum | shasum --check --ignore-missing
```   
Extract Zephyr SDK bundle  
```
tar xvf zephyr-sdk-0.15.0_linux-x86_64.tar.gz
```  
Run the SDK setup script  
```
cd zephyr-sdk-0.15.0 
./setup.sh
```  
## PicoProbe
To set your RPi Pico to accept binary files, hold down the bootsel button (white push-button). It should show up as a usb storage device. From there drag and drop /binaries/picoprobe.uf2 onto the pi. The on board LED should turn solid green, if so SUCCESS!
## Building your first example
We will need install one more windows application to be able to see 
the picoprobe device in WSL, like before open a administrators powershell window and run  
```
winget install usbidpd
```
Additionally we will need to install some dependecies in WSL
```
sudo apt install linux-tools-5.4.0-77-generic hwdata
sudo update-alternatives --install /usr/local/bin/usbip usbip /usr/lib/linux-tools/5.4.0-77-generic/usbip 20
```
Change directories to the head directory and run the pico setup script
```
cd ~/
./picosetup.sh
```
Add the line below to your udev rules file by running
```
echo ATTRS{idVendor}=="2e8a", ATTRS{idProduct}=="0004", MODE="660", GROUP="plugdev", TAG+="uaccess" > /etc/udev/rules.d/60-openocd.rules
```
Now it is time to build our zephyr application
```
west build -b rpi_pico samples/basic/blinky -- -DOPENOCD=/usr/local/bin/openocd -DOPENOCD_DEFAULT_PATH=/usr/local/share/openocd/scripts -DRPI_PICO_DEBUG_ADAPTER=picoprobe
```
 After our application binaries have been build switch to a powershell window to now attach our picoprobe usb device to WSL by finding the bus id of the picoprobe RPi
 ```
usbipd wsl list
usbipd esl attach --busid <busid>
```
After this back in WSL flash the other pi and you should see the second pi's LED flash
```
sudo -s
. ./.venv/bin/activate
west flash
```
# Development Process
When starting development on a new portion of the project make sure to update you local repo and create a seperate branch for your work
```
git pull
git checkout -b maxwell-martin-fixing stuff
```
When you have made your changes commit your local changes and push to your remote branch
```
git commit
git push
```
After this make a merge request and then discuss with team lead to get code merged into the main repoistory