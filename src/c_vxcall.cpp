#include <ros/ros.h>
#include <unistd.h>
#include <iostream>
#include <cmath>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <stdio.h>
//Variables
//ARDrone m=1.477
//Bebop m=0.450
//Bebop2 m=0.500
const float  m = 0.45, g = 9.81;
float x, xd, dx, wx, xg = 0.0, dxg = 0.0, wxg = 0.0, x0=0.0,  dx0 = 0.0, wx0 = 0.0, xg0 = 0.0, dxg0 = 0.0, wxg0 = 0.0, ux=0.0, vx=0.0, e, ab;

using namespace std;
//Funciones
void odom_callback(const nav_msgs::Odometry::ConstPtr& msg){
	x=msg->pose.pose.position.x;
}

void ref_callback(const std_msgs::Float32::ConstPtr& msg1){
	xd=msg1->data;	
}

int main (int argc, char **argv){
	ros::init(argc, argv, "c_vx");
	ros::NodeHandle node_obj;
	//Publisher
	ros::Publisher vx_publisher=node_obj.advertise<std_msgs::Float32>("/velx",10);
	//Suscriber
	ros::Subscriber odom = node_obj.subscribe("/bebop/odom",10,odom_callback);
    ros::Subscriber ref = node_obj.subscribe("/x_ref",10,ref_callback);
	//Radio de la pelota
	ros::Rate loop_rate(10);
	std::cout<<"Velocidad x \n";
	//FILE * PlotData = fopen("velx.txt", "w");
	while (ros::ok()){
		//Inicio de control de velocidad en x
			ux = 0.05*(xd-x)-0.2*(x-x0)-0.1*wxg;
			//ux = 0.05*(xd-x)-0.2*(x-x0);
			if(ux>0.7){	
				ux=0.7;
			}
			if(ux<-0.7)
			{
				ux=-0.7;
			}
			//Observador
 			xg = xg0 + 0.001*dxg0 + pow(0.001,2)*ux/(2.0*m) + tanh(x0-xg0);
			dxg = dxg0 + 0.001*ux/m + tanh(x0-xg0);
			wx = x0-xg0;
			wxg = wxg0 + tanh(wx0-wxg0);
		//Fin de control de velocidad en x	
		cout << "VelX" << vx << "\n";
		vx=ux;
		std_msgs::Float32 msg;
		msg.data=vx;
		vx_publisher.publish(msg);
		//Escribir Archivo
		//fprintf(PlotData, "%f %f %f \n",xd,x,ux);
		//Reinicio de variables
		x0 = x; xg0 = xg; wx0 = wx; wxg0 = wxg;
		ros::spinOnce();
	}	
	return 0;	
}
