//Header
#include "filter.hpp"

vector<string> filters = {"kalman", "custom", "mean", "median"};

ftr::ftr(cfg config)
{
	type = find(filters.begin(), filters.end(), config.getValueString("filter")) - filters.begin();
	if(type == filters.size())
		throw "Error: Unknow Filter Type";
	if(!type)
	{
		a = vector<dvec3>(1, dvec3(0.5));
		b = vector<dvec3>(1, dvec3(0.5));
	}
	else
	{
		vector<double> aux = config.getVectorDouble("inputconstant");
		for(unsigned int i = 0; i != aux.size(); i++)
			a.push_back(dvec3(aux[i]));
		aux = config.getVectorDouble("outputconstant");
		for(unsigned int i = 0; i != aux.size(); i++)
			b.push_back(dvec3(aux[i]));
		if(type > 1)
		{
			if(type > 2)
			{
				a = vector<dvec3>(a.size(), dvec3(0.0));
				if(a.size() % 2)
					a[a.size() / 2] = dvec3(1.0);
				else
				{
					a[(a.size() - 1) / 2] = dvec3(0.5);
					a[a.size() / 2] = dvec3(0.5);
				}
			}
			else
				a = vector<dvec3>(a.size(), dvec3(1.0 / static_cast<double>(a.size())));
			b = vector<dvec3>(b.size(), dvec3(0.0));
		}
	}
	in = vector<dvec3>(a.size(), dvec3());
	out = vector<dvec3>(b.size(), dvec3());
}

void ftr::update(vector<dvec3> acoefficient, vector<dvec3> bcoefficient)
{
	if((acoefficient.size() != a.size()) || (bcoefficient.size() != b.size()))
		throw "Error: Mismatch Vector Size";
	a = acoefficient;
	b = bcoefficient;
}

void ftr::update(vector<double> acoefficient, vector<double> bcoefficient)
{
	if((acoefficient.size() != a.size()) || (bcoefficient.size() != b.size()))
		throw "Error: Mismatch Vector Size";
	for(unsigned int i = 0; i != a.size(); i++)
		a[i] = dvec3(acoefficient[i]);
	for(unsigned int i = 0; i != b.size(); i++)
		b[i] = dvec3(bcoefficient[i]);
}

void ftr::update(dvec3 acoefficient, dvec3 bcoefficient)
{
	vector<dvec3> auxa(1, acoefficient);
	vector<dvec3> auxb(1, bcoefficient);
	update(auxa, auxb);
}

void ftr::handle(vector<dvec3> input, vector<dvec3> output)
{
	if((input.size() != in.size()) || (output.size() != out.size()))
		throw "Error: Mismatch Vector Size";
	in = input;
	out = output;
}

void ftr::handle(dvec3 input, dvec3 output)
{
	in = vector<dvec3>(in.size(), input);
	out = vector<dvec3>(out.size(), output);
}

dvec3 ftr::calcule(dvec3 input, dvec3 state)
{
	dvec3 output = dvec3();
	for(unsigned int i = 0; i != 3; i++)
	{
		vector<double> inputs(in.size(), 0.0);
		vector<double> outputs(out.size(), 0.0);
		inputs.front() = input[i];
		for(unsigned int j = 1; j != in.size(); j++)
			inputs[j] = in[j - 1][i];
		for(unsigned int j = 0; j != in.size(); j++)
			in[j][i] = inputs[j];
		for(unsigned int j = 0; j != out.size(); j++)
			outputs[j] = out[j][i];
		if(!type)
			outputs.back() = state[i];
		else if(type > 2)
			sort(inputs.begin(), inputs.end());
		for(unsigned int j = 0; j != a.size(); j++)
			output[i] += a[j][i] * inputs[j];
		for(unsigned int j = 0; j != b.size(); j++)
			output[i] += b[j][i] * outputs[j];
		out.front()[i] = output[i];
		for(unsigned int j = 1; j != out.size(); j++)
			out[j][i] = outputs[j - 1];
	}
	return output;
}
