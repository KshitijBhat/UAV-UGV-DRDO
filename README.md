# UAV guided UGV

## Presentation: (https://docs.google.com/presentation/d/14SBoSPBtUrPp43PPALbnM0FGVaOfveorzsXv51FzmBM/edit#slide=id.g11ed09688e2_0_10)[Link]

## Installation and setup
Create a catkin workspace by running the following commands
```
mkdir -p catkin_ws/src
cd catkin_ws/src
git clone https://github.com/dalalbhavya/uav-ugv.git
catkin build
source ../devel/setup.bash
```
To run the world make sure to include the path in $GAZEBO_MODEL_PATH environment variable

`src/InterIIT_DRDO/interiit22/models`

by running the command in terminal

```
echo $GAZEBO_MODEL_PATH
export GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:<your path>
```

To install PX4-Autopilot use the commands

```
git clone https://github.com/PX4/PX4-Autopilot.git --recursive
bash ./PX4-Autopilot/Tools/setup/ubuntu.sh
```

ROS Topics not publishing for depth camera
Solved by copying the links a new world file `drdo_world1_cam.world`

P.S. : This isssue is yet to be resolved in a more general way

To perform a general testing of the control code of drone follow

1. Open a new terminal and change the directory to `~/catkin_ws/src/iq_sim/scripts` and run

```
./startsitl.sh
```

2. Now open a new terminal and run (replace last digit for respective world (e.g. drdo_world2.launch for world2)
```
roslaunch interiit22 drdo_world1.launch
```

3. In a new terminal run

```
rosrun iq_gnc drdo_test
```

4. Now make the mode GUIDED by running the following command in MAVProxy Terminal

```
mode GUIDED
```

----
# **Approach**


## **Mapping and Geotagging**
We wrote code for manoeuvre the UAV over the terrain and capture rgbd images of small patches of terrain and created an orthomosaic map .
UAV used Ardupilot MissionPlanner for maneuvering and uses RTL command to return back to start position.

## **Road Detection** 

D-LinkNet is used to segment the roads obtained by rgbd camera images 
We skeletonize the output mask generated by the D-LinkNet model

For case of Plain Overlay images -
We use Depth Image based Plane Detection (DIPD) algorithm to distinguish the roads from the terrain .

## **Image Thresholding and Tracking of Car**

We are using image thresholding for determining orientation i.e. yaw of the prius or UGV.
We are using PID controller for tracking of the car.

## **UGV Movement and concerned ROS Topics & Nodes** : 

We created a ROS  node string_generator  for  converting array of waypoints into a string of commands  which is then published to prius_teleop node.
We use  cross product function for  detecting  turns. 

For  3  consecutive points ( x1 , y1 )  ,  ( x2  , y2 )   and   ( x3  , y3 )  ;

cross_product  =   V12 x V13  =   ( x1 - x2 )*( y3 - y2 ) -  ( x3 - x2 )*( y2 - y1 )

cross_product = 0    —>   go  straight

cross_product > 0    —>   left  turn

cross_product < 0    —>   right  turn

We created a ROS  node prius_teleop  for  utilizing  string of commands  for generating control  msgs  for  publishing to  /prius  topic .

/prius  ->  We use this topic for controlling motion of prius car . We publish values of throttle , steer , brake and shift_gears for maneuvering the car.

## **Spline Interpolation and Path Planning** :

We are using A* algorithm for generating waypoints through which UGV has to transverse to reach target location.
We are using Cubic splines for smoothening of turns. 



## References
1. https://docs.px4.io/master/en/simulation/ros_interface.html
2. https://openaccess.thecvf.com/content_cvpr_2018_workshops/w4/html/Zhou_D-LinkNet_LinkNet_With_CVPR_2018_paper.html
3. https://competitions.codalab.org/competitions/18467
4. https://github.com/Intelligent-Quads
5. https://github.com/OpenDroneMap/ODM
6. https://bdataanalytics.biomedcentral.com/articles/10.1186/s41044-018-0035-y
7. https://ardupilot.org/copter/docs/zigzag-mode.html
