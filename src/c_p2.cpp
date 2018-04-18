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
int intOp;
uint8_t battery;
int vx,my;
int F=1;
float h,x,x_des,vy,vyo,vaz;
geometry_msgs::Twist emma_cmd;

//Funciones
void c_vel (float lin_x, float lin_y, float lin_z, float ang_z);
//Funciones Callback
void altitude_callback(const bebop_msgs::Ardrone3PilotingStateAltitudeChanged::ConstPtr& msg2){
	h=msg2->altitude;
}

void opt_callback(const std_msgs::Int32::ConstPtr& msg1){
	intOp=msg1->data;
}

void My_callback(const std_msgs::Int32::ConstPtr& msg2){
	my=msg2->data;
}

void vy_callback(const std_msgs::Float32::ConstPtr& msg){
	vy=msg->data;
}

void vaz_callback(const std_msgs::Float32::ConstPtr& msg){
	vaz=msg->data;
}

void vyo_callback(const std_msgs::Float32::ConstPtr& msg){
	vyo=msg->data;
}

void battery_callback(const bebop_msgs::CommonCommonStateBatteryStateChanged::ConstPtr& msg2){
	battery=msg2->percent;
}

void odom_callback(const nav_msgs::Odometry::ConstPtr& msg){
	x=msg->pose.pose.position.x;
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
  ros::Subscriber Alt = nodo_.subscribe("/bebop/states/ardrone3/PilotingState/AltitudeChanged",1,altitude_callback);
  ros::Subscriber odom = nodo_.subscribe("/bebop/odom",1,odom_callback);
  //Nodos de velocidad
  ros::Subscriber Mx_sub = nodo_.subscribe("/MY",1,My_callback);
  ros::Subscriber vy_sub = nodo_.subscribe("/controlY",10,vy_callback); 
  ros::Subscriber vyo_sub = nodo_.subscribe("/vely",10,vyo_callback);
  ros::Subscriber vaz_sub = nodo_.subscribe("/velaz",10,vaz_callback); 
  //ros::Subscriber My_sub = nodo_.subscribe("/MY",1,My_callback);								
  	
  std_msgs::Empty takeoff_cmd;
  std_msgs::Empty land_cmd;	

  //Hover

  while(ros::ok()){
	switch(intOp){
		case 49: //Tecla 1
			switch(F){
				case 1:
					if(my < 300){
						std::cout<<"Prueba 2 "<<"B = "<<" % "<<"Avanzando F = "<< F << " " << vy << endl;
						c_vel(0.04,vy,0,vaz);
						fb_pub.publish(emma_cmd);
						F=1;
					}else{
						std::cout<<"Prueba 2 "<<"B = "<<(int)battery<<" % "<<"Hover F = "<< F << endl;
						c_vel(0,0,0,0);
						fb_pub.publish(emma_cmd);
						F=2;
					}
				break;
		
				case 2:
					std::cout<<"Prueba 2 "<<"B = "<<(int)battery<<" % "<<" Subiendo :) F = " << F << endl;
					c_vel(0,vyo,0.2,0);
					fb_pub.publish(emma_cmd);
					if(h > 1.50){ F=3; x_des = x_des + 0.75;}					
					else { F=2; x_des = x; }
				break;
		
				case 3:
					std::cout<<"Prueba 2 "<<"B = "<<(int)battery<<" % "<<" Pasando F = " << F << "X_Des = "<< x_des << endl;
					c_vel(0.05,vyo,0,vaz);
					fb_pub.publish(emma_cmd);
					if( x > x_des){ F=4; }
					else { F=3; }		
				break;
		
				case 4:
					std::cout<<"Prueba 2 "<<"B = "<<(int)battery<<" % "<<" Aterrizando F = " << F << endl;
					c_vel(0,0,0,0);
					fb_pub.publish(emma_cmd);
					ros::Duration(1).sleep();
					land_pub_.publish(land_cmd);
					F=1;
				break;
				default:
					std::cout<<"Hover "<<"B "<<(int)battery<<" % \n";
					c_vel(0.0,0.0,0.0,0.0);
					fb_pub.publish(emma_cmd);
				break;
				
			}
		break;
		case 50:// Tecla 2
			std::cout<<"Take Off "<<"B "<<(int)battery<<" % \n";
			takeoff_pub_.publish(takeoff_cmd);
		break;
		case 51: //Tecla 3
			std::cout<<"Land "<<"B "<<(int)battery<<" % \n";
			land_pub_.publish(land_cmd);
		break;
		case 52://Tecla 4
			std::cout<<"Hover "<<"B "<<(int)battery<<" % \n";
			c_vel(0.0,0.0,0.0,0.0);
			fb_pub.publish(emma_cmd);
		break;
		default:	
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
	
