#include <gnc_functions.hpp>
//just to test
int main(int argc, char** argv)
{
	//initializing ros 
	ros::init(argc, argv, "drdo_test_node");
	ros::NodeHandle drdo_test_node;

    //initialize control publisher/subscribers
    init_publisher_subscriber(drdo_test_node);

    //waiting for Flight Controller connection
	wait4connect();

	//waiting for mode to switch to GUIDED
	wait4start();

	//create local reference frame 
	initialize_local_frame();

    //request takeoff takeoff(float takeOffHieght)
	takeoff(15);


    //Waypoints
	std::vector<gnc_api_waypoint> waypointList;
	gnc_api_waypoint nextWayPoint;
	nextWayPoint.x = 0;
	nextWayPoint.y = 0;
	nextWayPoint.z = 15;
	nextWayPoint.psi = 0;
	waypointList.push_back(nextWayPoint);
	nextWayPoint.x = 5;
	nextWayPoint.y = 0;
	nextWayPoint.z = 15;
	nextWayPoint.psi = -90;
	waypointList.push_back(nextWayPoint);
	nextWayPoint.x = 5;
	nextWayPoint.y = 5;
	nextWayPoint.z = 15;
	nextWayPoint.psi = 0;
	waypointList.push_back(nextWayPoint);
	nextWayPoint.x = 0;
	nextWayPoint.y = 5;
	nextWayPoint.z = 15;
	nextWayPoint.psi = 90;
	waypointList.push_back(nextWayPoint);
	nextWayPoint.x = 0;
	nextWayPoint.y = 0;
	nextWayPoint.z = 15;
	nextWayPoint.psi = 180;
	waypointList.push_back(nextWayPoint);


    //specify control loop rate. We recommend a low frequency to not over load the FCU with messages. Too many messages will cause the drone to be sluggish
	ros::Rate rate(2.0);
	int counter = 0;
	while(ros::ok())
	{
		ros::spinOnce();
		rate.sleep();
		if(check_waypoint_reached() == 1)
		{
			if (counter < waypointList.size())
			{
				set_destination(waypointList[counter].x,waypointList[counter].y,waypointList[counter].z, waypointList[counter].psi);
				counter++;	
			}else{
				//land after all waypoints are reached
				land();
			}	
		}	
		
	}
	return 0;
}
