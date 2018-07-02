//Headers
#include "inputs.hpp"

#ifndef GLUT_WHEEL_UP
#define GLUT_WHEEL_UP 3
#endif
#ifndef GLUT_WHEEL_DOWN
#define GLUT_WHEEL_DOWN 4
#endif
#define GLUT_KEY_DELETE 8
#define GLUT_KEY_INTRO 13
#define GLUT_KEY_BACKSPACE 32
#define GLUT_KEY_ESCAPE 27
#define GLUT_KEY_PLUS 43
#define GLUT_KEY_MINUS 45
#define GLUT_KEY_ZERO 48
#define GLUT_KEY_ONE 49
#define GLUT_KEY_TWO 50
#define GLUT_KEY_THREE 51
#define GLUT_KEY_FOUR 52
#define GLUT_KEY_FIVE 53
#define GLUT_KEY_SIX 54
#define GLUT_KEY_SEVEN 55
#define GLUT_KEY_EIGHT 56
#define GLUT_KEY_NINE 57

#if(VIDEO)
extern scn Scene;
#endif
extern sys System;
#if(!MODEL)
extern lnk Link;

lnk Link;
NodeHandle* NHI;
Subscriber nav_sub;
Subscriber tf2_sub;
#endif

void hid::start(void)
{
	#if(!MODEL)
	cfg setlink(string(CONFIG) + string("link.cfg"));
	Link = lnk(setlink);
	NHI = new NodeHandle();
	nav_sub = NHI->subscribe("ardrone/navdata", 1, socket0);
	tf2_sub = NHI->subscribe("tf", 250, socket1); //238 if ts = 1.0//24 if ts = 0.1//12 if ts = 0.05//5 if ts = 0.02
	#endif
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(extrakeyboard);
	//glutJoystickFunc(joystick, 50);
	atexit(finish);
}

void hid::finish(void)
{
	#if(!MODEL)
	delete NHI;
	#endif
}

void hid::mouse (int button, int state, int, int)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
		case GLUT_RIGHT_BUTTON:
		case GLUT_MIDDLE_BUTTON:
			break;
		case GLUT_WHEEL_DOWN:
			if (state == GLUT_UP)
				#if(VIDEO)
				Scene.move(1, 'x');
				#endif
			break;
		case GLUT_WHEEL_UP:
			if (state == GLUT_UP)
				#if(VIDEO)
				Scene.move(0, 'x');
				#endif
			break;
	}
	#if(VIDEO)
	glutPostRedisplay();
	#endif
}

void hid::keyboard (unsigned char key, int, int)
{
	switch (key)
	{
		case GLUT_KEY_DELETE:
			#if(VIDEO)
			Scene.shift();
			#endif
			break;
		case GLUT_KEY_INTRO:
			break;
		case GLUT_KEY_BACKSPACE:
			System.admin();
			break;
		case GLUT_KEY_ESCAPE:
			exit(EXIT_SUCCESS);
			break;
		case GLUT_KEY_ZERO:
		case GLUT_KEY_ONE:
		case GLUT_KEY_TWO:
		case GLUT_KEY_THREE:
		case GLUT_KEY_FOUR:
		case GLUT_KEY_FIVE:
		case GLUT_KEY_SIX:
		case GLUT_KEY_SEVEN:
		case GLUT_KEY_EIGHT:
		case GLUT_KEY_NINE:
			System.focus(key - 48);
			break;
	}
	#if(VIDEO)
	glutPostRedisplay();
	#endif
}

void hid::extrakeyboard (int key, int, int)
{
	switch (key)
	{
		case GLUT_KEY_F1:
			System.manual(1, 0, 'x');
			break;
		case GLUT_KEY_F2:
			System.manual(1, 1, 'x');
			break;
		case GLUT_KEY_F5:
			System.manual(1, 0, 'y');
			break;
		case GLUT_KEY_F6:
			System.manual(1, 1, 'y');
			break;
		case GLUT_KEY_F9:
			System.manual(1, 0, 'z');
			break;
		case GLUT_KEY_F10:
			System.manual(1, 1, 'z');
			break;
		case GLUT_KEY_F3:
			#if(MODEL)
			System.manual(0, 0, 'x');
			#endif
			break;
		case GLUT_KEY_F4:
			#if(MODEL)
			System.manual(0, 1, 'x');
			#endif
			break;
		case GLUT_KEY_F7:
			#if(MODEL)
			System.manual(0, 0, 'y');
			#endif
			break;
		case GLUT_KEY_F8:
			#if(MODEL)
			System.manual(0, 1, 'y');
			#endif
			break;
		case GLUT_KEY_F11:
			System.manual(0, 0, 'z');
			break;
		case GLUT_KEY_F12:
			System.manual(0, 1, 'z');
			break;
		case GLUT_KEY_DOWN:
			#if(VIDEO)
			Scene.move(1, 'y');
			#endif
			break;
		case GLUT_KEY_UP:
			#if(VIDEO)
			Scene.move(0, 'y');
			#endif
			break;
		case GLUT_KEY_LEFT:
			#if(VIDEO)
			Scene.move(1, 'z');
			#endif
			break;
		case GLUT_KEY_RIGHT:
			#if(VIDEO)
			Scene.move(0, 'z');
			#endif
			break;
		case GLUT_KEY_PAGE_UP:
		case GLUT_KEY_PAGE_DOWN: 
		case GLUT_KEY_HOME:
		case GLUT_KEY_END:
		case GLUT_KEY_INSERT:
			break;
	}
	#if(VIDEO)
	glutPostRedisplay();
	#endif
}

void hid::joystick(unsigned int buttonMask, int, int, int)
{
	switch (buttonMask)
	{
		case GLUT_JOYSTICK_BUTTONS:
		case GLUT_JOYSTICK_AXES:
		case GLUT_JOYSTICK_POLL_RATE:
		case GLUT_JOYSTICK_BUTTON_A:
		case GLUT_JOYSTICK_BUTTON_B:
		case GLUT_JOYSTICK_BUTTON_C:
		case GLUT_JOYSTICK_BUTTON_D:
			break;
	}
}

#if(!MODEL)
void hid::socket0(const ardrone_autonomy::Navdata& msg)
{
	Link.add(msg.state);
}

void hid::socket1(const tf2_msgs::TFMessageConstPtr& msg)
{
	const tf2_msgs::TFMessage& aux = *msg;
	if (aux.transforms.front().header.frame_id == "odom" && aux.transforms.front().child_frame_id == "ardrone_base_link")
		Link.add(dvec3(aux.transforms.front().transform.translation.x, aux.transforms.front().transform.translation.y, aux.transforms.front().transform.translation.z), dquat(aux.transforms.front().transform.rotation.w, aux.transforms.front().transform.rotation.x, aux.transforms.front().transform.rotation.y, aux.transforms.front().transform.rotation.z), false, 0);
	else if (aux.transforms.front().header.frame_id == "ardrone_base_link" && aux.transforms.front().child_frame_id == "ardrone_base_frontcam")
		Link.add(dvec3(aux.transforms.front().transform.translation.x, aux.transforms.front().transform.translation.y, aux.transforms.front().transform.translation.z), dquat(aux.transforms.front().transform.rotation.w, aux.transforms.front().transform.rotation.x, aux.transforms.front().transform.rotation.y, aux.transforms.front().transform.rotation.z), true, 0);
	else if (aux.transforms.front().header.frame_id == "ardrone_base_frontcam" && aux.transforms.front().child_frame_id == "ar_marker_3")
		Link.add(dvec3(aux.transforms.front().transform.translation.x, aux.transforms.front().transform.translation.y, aux.transforms.front().transform.translation.z), dquat(aux.transforms.front().transform.rotation.w, aux.transforms.front().transform.rotation.x, aux.transforms.front().transform.rotation.y, aux.transforms.front().transform.rotation.z), true, 1);
	else if (aux.transforms.front().header.frame_id == "ar_marker_3" && aux.transforms.front().child_frame_id == "world")
		Link.add(dvec3(aux.transforms.front().transform.translation.x, aux.transforms.front().transform.translation.y, aux.transforms.front().transform.translation.z), dquat(aux.transforms.front().transform.rotation.w, aux.transforms.front().transform.rotation.x, aux.transforms.front().transform.rotation.y, aux.transforms.front().transform.rotation.z), true, 2);
}

#endif
