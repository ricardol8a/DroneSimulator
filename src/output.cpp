//Headers
#include "output.hpp"

extern txt Info;

txt Info;
#if(!MODEL)
NodeHandle* NHO;
Publisher vel_pub;
Publisher rst_pub;
Publisher tkf_pub;
Publisher lnd_pub;
#endif

void ntf::start(void)
{
	cfg setinfo(string(CONFIG) + string("info.cfg"));
	Info = txt(setinfo);
	#if(!MODEL)
	NHO = new NodeHandle();
	vel_pub = NHO->advertise<geometry_msgs::Twist>("cmd_vel", 1);
	rst_pub = NHO->advertise<std_msgs::Empty>("ardrone/reset", 1);
	tkf_pub = NHO->advertise<std_msgs::Empty>("ardrone/takeoff", 1);
	lnd_pub = NHO->advertise<std_msgs::Empty>("ardrone/land", 1);
	#endif
	atexit(finish);
}

void ntf::finish(void)
{
	#if(!MODEL)
	delete NHO;
	#endif
}

void ntf::cli(string text)
{
	while(text.back() == 10)
		text.pop_back();
	cout << text << endl;
}

void ntf::file(string text, string name)
{
	fstream box;
	box.open(string(DATA) + name + string(".txt"), fstream::out);
	box << text << endl;
	box.close();
}

#if(VIDEO)
void ntf::gui(string text, ivec2 position)
{
	glRasterPos2i(position.x, position.y);
	while(text.back() == 10)
		text.pop_back();
	for (unsigned int j = 0; j < text.size(); j++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[j]);
}
#endif

#if(!MODEL)
void ntf::socket0(int state)
{
	std_msgs::Empty msg;
	switch(state)
	{
		case 0:
			rst_pub.publish(msg);
			break;
		case 1:
			tkf_pub.publish(msg);
			break;
		case 2:
			lnd_pub.publish(msg);
			break;
	}
}

void ntf::socket1(dvec4 cmd_vel)
{
	geometry_msgs::Twist msg;
	msg.linear.x = cmd_vel.x;
	msg.linear.y = cmd_vel.y;
	msg.linear.z = cmd_vel.z;
	msg.angular.z = cmd_vel.w;
	vel_pub.publish(msg);
}

#endif
