#include <ros/ros.h>
#include <unistd.h>
#include <iostream>
#include <termios.h>
#include <sensor_msgs/image_encodings.h>
#include <nav_msgs/Odometry.h>
#include <bebop_msgs/Ardrone3PilotingStateAltitudeChanged.h>
#include <bebop_msgs/CommonCommonStateBatteryStateChanged.h>
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <stdio.h>
#include <stdlib.h>


using namespace std;
//Variables
int intOp,t;
uint8_t battery;
float vx,vy,vz,vaz;
geometry_msgs::Twist emma_cmd;

//Funciones
void c_vel (float lin_x, float lin_y, float lin_z, float ang_z);
//Funciones Callback
void opt_callback(const std_msgs::Int32::ConstPtr& msg1){
	intOp=msg1->data;
}

void vy_callback(const std_msgs::Float32::ConstPtr& msg2){
	vy=msg2->data;
}

void vx_callback(const std_msgs::Float32::ConstPtr& msg3){
	vx=msg3->data;
}

void vz_callback(const std_msgs::Float32::ConstPtr& msg4){
	vz=msg4->data;
}

void vaz_callback(const std_msgs::Float32::ConstPtr& msg4){
	vaz=msg4->data;
}

void battery_callback(const bebop_msgs::CommonCommonStateBatteryStateChanged::ConstPtr& msg2){
	battery=msg2->percent;
}


int main(int argc, char** argv)
{
  ros::init(argc, argv, "MCN");
  ros::NodeHandle nodo_;
  ros::Publisher takeoff_pub_ = nodo_.advertise<std_msgs::Empty>("/bebop/takeoff", 1);
  ros::Publisher land_pub_ = nodo_.advertise<std_msgs::Empty>("/bebop/land", 1);
  ros::Publisher fb_pub = nodo_.advertise<geometry_msgs::Twist>("/bebop/cmd_vel",1);
  //teclado	
  ros::Subscriber tec_sub = nodo_.subscribe("/copt",10,opt_callback);
  ros::Subscriber battery_ = nodo_.subscribe("/bebop/states/common/CommonState/BatteryStateChanged",1,battery_callback);
  //Nodos de velocidad
  ros::Subscriber vy_sub = nodo_.subscribe("/controlY",10,vy_callback); 
  //ros::Subscriber vy_sub = nodo_.subscribe("/vely",10,vy_callback);  
  ros::Subscriber vx_sub = nodo_.subscribe("/controlX",10,vx_callback); 
  ros::Subscriber vz_sub = nodo_.subscribe("/controlZ",10,vz_callback); 
  ros::Subscriber vaz_sub = nodo_.subscribe("/controlAZ",10,vaz_callback);
  //ros::Subscriber My_sub = nodo_.subscribe("/MY",1,My_callback);								
  	
  std_msgs::Empty takeoff_cmd;
  std_msgs::Empty land_cmd;	

  //Hover

  while(ros::ok()){
	switch(intOp){
		case 49: 
			//Tecla 1
			// if(t==0){
			// 	std::cout<<"Inicio prueba 1 "<<"B = "<<(int)battery<<" % "<<"Take off"<< endl;
			// 	takeoff_pub_.publish(takeoff_cmd);
			// 	ros::Duration(6).sleep();
			// 	t++;
			// }
			// else{	
			// 	std::cout<<"Prueba Pelota "<<"B = "<<(int)battery<<" % "<< "Velocidad "<< vy << endl;
			// 	c_vel(vx,vy,vz,0.0);
			// 	fb_pub.publish(emma_cmd)0.05:;
			// }

			std::cout<<"Prueba Pelota "<<"B = "<<(int)battery<<" % "<< "Velocidad "<< vy << endl;
			c_vel(vx,vy,vz,0.0);
			fb_pub.publish(emma_cmd);
		break;
		case 50:
			//Tecla 2
			std::cout<<"Take Off "<<"B "<<(int)battery<<" % \n";
			takeoff_pub_.publish(takeoff_cmd);
		break;
		case 51:
			//Tecla 3
			std::cout<<"Land "<<"B "<<(int)battery<<" % \n";
			land_pub_.publish(land_cmd);
		break;
		case 52:
			//Tecla 4
			std::cout<<"Hover "<<"B "<<(int)battery<<" % \n";
			c_vel(0.0,0.0,0.0,0.0);
			fb_pub.publish(emma_cmd);
		break;
		default:	
			//Cualquier tecla 
			std::cout<<"Hover "<<"B "<<(int)battery<<" % \n";
			c_vel(0.0,0.0,0.0,0.0);
			fb_pub.publish(emma_cmd);
		break;
	}      	
	ros::spinOnce();
	//ros::spin();
  }
  return 0;	
}

void c_vel (float lin_x, float lin_y, float lin_z, float ang_z){
	emma_cmd.linear.x = lin_x;
  	emma_cmd.linear.y = lin_y;
  	emma_cmd.linear.z = lin_z;
  	emma_cmd.angular.z = ang_z;
	
}
	
