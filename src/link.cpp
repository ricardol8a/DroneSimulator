//Header
#include "link.hpp"

#if(!MODEL)
extern sys System;

vector<string> frames = {"odo", "tag", "mix"};

lnk::lnk(cfg config)
{
	type = find(frames.begin(), frames.end(), config.getValueString("frame")) - frames.begin();
	if(type == frames.size())
		throw "Error: Unknow Frame Type";
	status = 0;
	alpha = config.getValueDouble("alpha");
	if(abs(alpha) >= 1.0)
		throw "Error: Alpha Out of Bound";
	vector<dmat4> aux = {dmat4(0.0), dmat4(0.0), dmat4(0.0)};
	for (unsigned int i = 0; i != 2; i++)
		data.push_back(aux);
	add(dvec3(0.0, 0.0, 2.25), dquat(0.5, -0.5, 0.5, 0.5), true, 2);
}

void lnk::add(int state)
{
	status = state;
}

void lnk::add(dvec3 position, dquat orientation, bool select, int number)
{
	if(!select && (number == 0))
		orientation = eul2qua(qua2eul(orientation) * dvec3(1.0, 1.0, -1.0));
	data[static_cast<int>(select)][number] = mat4_cast(orientation) + dmat4(dvec4(), dvec4(), dvec4(), dvec4(position, 0.0));
}

void lnk::sync(void)
{
	vector<dmat4> aux = {dmat4(), dmat4()};
	if(type - 1)
	{
		if(data.front()[0] != dmat4(0.0) && (data.front()[1] == dmat4(0.0)) && (status == 4))
			add(dvec3(inverse(data.front()[0])[3]), quat_cast(inverse(data.front()[0])), false, 1);
		for(unsigned int i = 0; i != data.front().size(); i++)
			aux.front() = data.front()[i] * aux.front();
	}
	if(type)
	{
		for(unsigned int i = 0; i != data.back().size(); i++)
			aux.back() = aux.back() * data.back()[i];
	}
	if(!(((type - 1) && (aux.front() == dmat4(0.0))) || ((type) && (aux.back() == dmat4(0.0)))))
	{
		if(type - 1)
			aux.front() = mat4_cast(eul2qua(qua2eul(quat_cast(aux.front())) * dvec3(1.0, 1.0, -1.0))) + dmat4(dvec4(), dvec4(), dvec4(), dvec4(dvec3(aux.front()[3]), 0.0));
		if(type)
			aux.back() = inverse(aux.back());
		if(type > 1)
		{
			static vector<dmat4> input = aux;
			static vector<dmat4> output = {dmat4(0.0), aux.back()};
			output.front() = (0.5 + alpha / 2.0) * (aux.front() - input.front()) + alpha * output.front();
			output.back() = (0.5 - alpha / 2.0) * (aux.back() + input.back()) + alpha * output.back();
			input.front() = aux.front();
			input.back() = aux.back();
			aux.push_back(output.front() + output.back());
		}
		System.assign(((status == 3) || (status == 4) || (status == 8)), dvec3(aux[type][3]), quat_cast(aux[type]));
	}
}

#endif
