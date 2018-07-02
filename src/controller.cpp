//Header
#include "controller.hpp"

vector<string> controllers = {"pid", "fuzzy"};

ctr::ctr(cfg config)
{
	type = find(controllers.begin(), controllers.end(), config.getValueString("controller")) - controllers.begin();
	if(type == controllers.size())
		throw "Error: Unknow Controller Type";
	if(config.getVectorDouble("proportionalconstant").size() == 1)
		kp = dvec3(config.getValueDouble("proportionalconstant"));
	else
		kp = dvec3(config.getValueVectorDouble("proportionalconstant", 0), config.getValueVectorDouble("proportionalconstant", 1), config.getValueVectorDouble("proportionalconstant", 2));
	if(config.getVectorDouble("derivativeconstant").size() == 1)
		kd = dvec3(config.getValueDouble("derivativeconstant"));
	else
		kd = dvec3(config.getValueVectorDouble("derivativeconstant", 0), config.getValueVectorDouble("derivativeconstant", 1), config.getValueVectorDouble("derivativeconstant", 2));
	if(type)
	{
		ki = dvec3();
		if(config.getVectorDouble("outputconstant").size() == 1)
			ko = dvec3(config.getValueDouble("outputconstant"));
		else
			ko = dvec3(config.getValueVectorDouble("outputconstant", 0), config.getValueVectorDouble("outputconstant", 1), config.getValueVectorDouble("outputconstant", 2));
		FuzzyEngine = new CFuzzyEngine();
		string item, subitem;
		for(unsigned int i = 0; true; i++)
		{
			item = "atribute" + to_string(i);
			if(!config.getIfExistGroup(item))
				break;
			cfg setatribute = config.getGroup(item);
			string name = setatribute.getValueString("name");
			if(!(name == "Output"))
				FuzzyEngine->AddFuzzyInput(new CFuzzyInput(name, 0.0));
			CLinguisticVariable* linguisticVariable = new CLinguisticVariable((name == "Output"), name);
			for(unsigned int j = 0; true; j++)
			{
				subitem = "subatribute" + to_string(j);
				if(!setatribute.getIfExistGroup(subitem))
					break;
				cfg setsubatribute = setatribute.getGroup(subitem);
				linguisticVariable->AddLinguisticValue(new LinguisticValue(setsubatribute.getValueString("name"), setsubatribute.getValueVectorDouble("vertex", 0), setsubatribute.getValueVectorDouble("vertex", 1), setsubatribute.getValueVectorDouble("vertex", 2), setsubatribute.getValueVectorDouble("vertex", 3)));
			}
			FuzzyEngine->AddLinguisticVariables(linguisticVariable);
		}
		for(unsigned int i = 0; true; i++)
		{
			item = "rule" + to_string(i);
			if(!config.getIfExistSymbol(item))
				break;
			vector<string> sentence = config.getVectorString(item);
			bool negated = false;
			string leftSide;
			string rightSide;
			Operation operation = EMPTY; 
			CFuzzyRule* rule = new CFuzzyRule();
			for(unsigned int i = 1; i < sentence.size(); i++)
			{
				if((sentence[i] == "AND") || (sentence[i] == "OR"))
				{
					if(sentence[i] == "AND")
						operation = AND;
					else
						operation = OR;
				}
				else if((sentence[i] == "IS") || (sentence[i] == "ISNOT"))
				{
					leftSide = sentence[i - 1];
					if(sentence[i] == "ISNOT")
						negated = true;
					rightSide = sentence[++i];
					CLinguisticVariable* linguisticVariable = FuzzyEngine->FindLinguisticVariableByName(leftSide);
					if(linguisticVariable == NULL)
						throw "Error: Wrong Configuration Value";
					rule->AddTokenToRule(new CFuzzyRuleToken(negated, operation, linguisticVariable, rightSide));
					operation = EMPTY;
					negated = false;
				}
			}
			FuzzyEngine->AddFuzzyRule(rule);
		}
	}
	else
	{
		if(config.getVectorDouble("integralconstant").size() == 1)
			ki = dvec3(config.getValueDouble("integralconstant"));
		else
			ki = dvec3(config.getValueVectorDouble("integralconstant", 0), config.getValueVectorDouble("integralconstant", 1), config.getValueVectorDouble("integralconstant", 2));
		ko = dvec3(1.0);
	}
}

void ctr::update(double proportional, double integral, double derivative, double output)
{
	kp = dvec3(proportional);
	kd = dvec3(derivative);
	if(type)
		ko = dvec3(output);
	else
		ki = dvec3(integral);
}

void ctr::update(dvec3 proportional, dvec3 integral, dvec3 derivative, dvec3 output)
{
	kp = proportional;
	kd = derivative;
	if(type)
		ko = output;
	else
		ki = integral;
}

dvec3 ctr::calcule(dvec3 error, dvec3 ierror, dvec3 derror)
{
	dvec3 result;
	for(unsigned int i = 0; i != 3; i++)
	{
		if(type)
		{
			vector<double> aux = {kp[i] * error[i], kd[i] * derror[i]};
			result[i] = FuzzyEngine->CalculateFuzzyRules(aux);
		}
		else
			result[i] = kp[i] * error[i] + ki[i] * ierror[i] + kd[i] * derror[i];
	}
	return ko * result;
}
