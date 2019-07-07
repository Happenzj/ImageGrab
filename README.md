# ImageGrab

## 1. Prerequisites

   1) ubuntu 16.04 + ROS Kinetic
   2) Opencv

## 2. Usage

    cd catkin_ws/src
    git clone https://github.com/Happenzj/ImageGrab
    cd ..
    catkin_make
    source devel/setup.bash
    roslaunch imagegrab imagegrab.launch
    
*Note There are some things in the launch file need to be modified before you use the package

1)You need to change the save_file_path to your own path, and then create two folders: image_0 and image_1

2)The names of the left and right image topics need to be changed to your own

3)Output image size

__INPUT:__

Left and right camera topics

__OUTPUT:__

Left and right camera images(default:640*360) and timestamps

## 3. Authors
    Zheng Jie





