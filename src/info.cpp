//Headers
#include "info.hpp"

extern sys System;
#if(VIDEO)
extern vector<box> Sector;
extern pln Plane;
#endif
extern ntf Output;
extern vector<reg> Register;

vector<string> name = {"t", "x", "y", "z", "R", "P", "Y", "vx", "vy", "vz", "vR", "vP", "vY", "ax", "ay", "az", "aR", "aP", "aY"};
vector<string> unit = {"s", "m", "m", "m", "°", "°", "°", "m/s", "m/s", "m/s", "°/s", "°/s", "°/s", "m/s²", "m/s²", "m/s²", "°/s²", "°/s²", "°/s²"};
vector<double> factor = {1.0, 1.0, 1.0, 1.0, 180.0 / M_PI, 180.0 / M_PI, 180.0 / M_PI, 1.0, 1.0, 1.0, 180.0 / M_PI, 180.0 / M_PI, 180.0 / M_PI, 1.0, 1.0, 1.0, 180.0 / M_PI, 180.0 / M_PI, 180.0 / M_PI};

txt::txt(cfg config)
{
	#if(MODEL && VIDEO)
	a = config.getValueBool("printdata");
	#else
	a = true;
	#endif
	b = config.getValueBool("storedata");
	#if(MODEL)
	if(a)
	{
		#endif
		stringstream buffer;
		time_t datetime = time(nullptr);
		buffer << "Date&Time: " << ctime(&datetime);
		Output.cli(buffer.str());
		buffer.str(string());
		buffer << "|" ;
		for (unsigned int k = 0; k < 7; k++)
			buffer << "  " << name[k] << "[" << unit[k] << "]" << "  " << "|";
		Output.cli(buffer.str());
		#if(MODEL)
	}
	#endif
}

void txt::print(void)
{
	#if(MODEL)
	if(a)
	{
		static string last;
		#endif
		stringstream buffer;
		buffer.setf(ios_base::right, ios_base::adjustfield);
		buffer.setf(ios_base::fixed, ios_base::floatfield);
		buffer.precision(3);
		buffer << "|";
		for (unsigned int k = 0; k < 7; k++)
			buffer << setw(8) << Register[System.n].status.back().at(k) * factor[k] << "|";
		if(Register.size() > 1)
			buffer << "   #" << System.n;
		#if(MODEL)
		if(last != buffer.str())
		#endif
			Output.cli(buffer.str());
		#if(MODEL)
		last = buffer.str();
	}
	#endif
}

void txt::store(void)
{
	if(b)
	{
		time_t datetime = time(nullptr);
		char filename[20];
		strftime(filename, sizeof(filename), "%Y-%m-%d-%H-%M-%S", localtime(&datetime));
		for(unsigned int i = 0; i < Register.size(); i++)
		{
			stringstream buffer;
			buffer.setf(ios_base::scientific, ios_base::floatfield);
			buffer.precision(7);
			for(unsigned int j = 0; j < (Register[i].status.size() + 1); j++)
			{
				for(unsigned int k = 0; k < 19; k++)
				{
					if(j)
						buffer << (Register[i].status)[j - 1][k] * factor[k];
					else
						buffer << name[k];
					if(k != 18)
						buffer << " ";
					else
						buffer << endl;
				}
			}
			Output.file(buffer.str(), string(filename) + string("-") + to_string(i));
		}
	}
}

#if(VIDEO)
void txt::render(void)
{
	clr Color;
	glColor4fv(Color.black);
	ivec2 p = Sector[0].x + Plane.x - Plane.d / 2;
	ivec2 q = p + Sector[0].d;
	ivec2 x;
	time_t datetime = time(nullptr);
	for (int i = 0; i < (9 + (Register.size() > 1)); i++)
	{
		stringstream buffer;
		buffer.setf(ios_base::fixed, ios_base::floatfield);
		buffer.precision(2);
		switch(i)
		{
			case 0:
				buffer << ctime(&datetime);
				x = ivec2(p.x + 10, q.y - 20);
				break;
			case 1:
				if (System.state)
					buffer << "<<Running>>";
				else
					buffer << " [Stopped] ";
				x = ivec2(q.x - 9 * 24 - 10, q.y - 20);
				break;
			case 2:
				buffer << "t = " << Register[System.n].status.back().at(0) * factor[0] << "s";
				x = ivec2(q.x - 9 * (buffer.str()).size() - 10, q.y - 20);
				break;
			case 3:
				buffer << "x = " << Register[System.n].status.back().at(1) * factor[1] << "m";
				x = ivec2(p.x + 10, p.y + 50);
				break;
			case 4:
				buffer << "y = " << Register[System.n].status.back().at(2) * factor[2] << "m";
				x = ivec2(p.x + 10, p.y + 30);
				break;
			case 5:
				buffer << "z = " << Register[System.n].status.back().at(3) * factor[3] << "m";
				x = ivec2(p.x + 10, p.y + 10);
				break;
			case 6:
				buffer << "roll = " << Register[System.n].status.back().at(4) * factor[4] << "\7";
				x = ivec2(q.x - 9 * (buffer.str()).size() - 10, p.y + 50);
				break;
			case 7:
				buffer << "pitch = " << Register[System.n].status.back().at(5) * factor[5] << "\7";
				x = ivec2(q.x - 9 * (buffer.str()).size() - 10, p.y + 30);
				break;
			case 8:
				buffer << "yaw = " << Register[System.n].status.back().at(6) * factor[6] << "\7";
				x = ivec2(q.x - 9 * (buffer.str()).size() - 10, p.y + 10);
				break;
			case 9:
				buffer << "#" << System.n;
				x = ivec2((p.x + q.x - 9 * (buffer.str()).size()) / 2, p.y + 30);
				break;
		}
		Output.gui(buffer.str(), x);
	}
}
#endif
