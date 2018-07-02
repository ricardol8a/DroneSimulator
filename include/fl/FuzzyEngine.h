#ifndef FUZZYENGINE_H
#define FUZZYENGINE_H

#include <vector>
#include <cassert>
#include "fl/LinguisticValue.h"
#include "fl/LinguisticVariable.h"
#include "fl/FuzzyInput.h"
#include "fl/FuzzyRuleToken.h"
#include "fl/FuzzyRule.h"
#include "fl/FuzzyReasoner.h"

using namespace std;
using namespace fl;

namespace fl{
	class CFuzzyEngine
	{
		//CFuzzyReasoner instance
		CFuzzyReasoner* fr;
		vector <CLinguisticVariable*> h_linguisticVariables;
		vector <CFuzzyInput*> h_fuzzyInputs;
	public:
		CFuzzyEngine(void)
		{
			//Creating
			fr = new CFuzzyReasoner();
		}
		
		~CFuzzyEngine(void)
		{
			//Releasing
			delete fr;
			for(unsigned int i = 0; i < h_linguisticVariables.size(); i++)
				delete h_linguisticVariables[i];
			for(unsigned int i = 0; i < h_fuzzyInputs.size(); i++)
				delete h_fuzzyInputs[i];
		}
		
		void AddFuzzyRule(CFuzzyRule* rule)
		{
			fr->AddFuzzyRule(rule);
		}
		
		void AddFuzzyInput(CFuzzyInput* fuzzyInput)
		{
			h_fuzzyInputs.push_back(fuzzyInput);
		}
		
		void AddLinguisticVariables(CLinguisticVariable* p_linguisticVariable)
		{
			h_linguisticVariables.push_back(p_linguisticVariable);
		}

		CLinguisticVariable* FindLinguisticVariableByName(string variableName)
		{
			unsigned int i = 0;
			while(i != h_linguisticVariables.size())
			{
				if(h_linguisticVariables[i]->ToString() == variableName)
					break;
				i++;
			}
			return h_linguisticVariables[i];
			//vector<CLinguisticVariable*>::iterator iterator = h_linguisticVariables.begin();
			//while(iterator != h_linguisticVariables.end())
			//{
			//	if((*iterator)->ToString() == variableName)
			//		break;
			//	iterator++;
			//}
			//return (*iterator);
		}
		
		double CalculateFuzzyRules(vector <double> inputs)
		{
			assert(h_fuzzyInputs.size() == inputs.size());
			for(unsigned int i = 0; i != h_fuzzyInputs.size(); i++)
				h_fuzzyInputs[i]->SetInputValue(inputs[i]);
			CLinguisticVariable* lVar = fr->CalculateFuzzyRules(h_fuzzyInputs);
			assert(lVar != NULL);
			double output = fr->Defuzzy(lVar); 
			ResetTokenValues();
			return output;
		}

		void ResetTokenValues()
		{
			//Set all linguistic values to -1
			unsigned int i = 0;
			while(i != h_linguisticVariables.size())
			{
				//Repeat until the end
				h_linguisticVariables[i]->ResetLinguisticValues();
				i++;
			}
			//vector <CLinguisticVariable*>::iterator iterator = h_linguisticVariables.begin();
			//while(iterator != h_linguisticVariables.end())
			//{
			//	//Repeat until the end
			//	CLinguisticVariable* temp = (*iterator);
			//	temp->ResetLinguisticValues();
			//	iterator++;
			//}
		}

		double GetA_ByName(string variableName, string valueName)
		{
			CLinguisticVariable* temp = FindLinguisticVariableByName(variableName);
			return temp->FindLinguisticValueByName(valueName)->GetA();
		}

		double GetB_ByName(string variableName, string valueName)
		{
			CLinguisticVariable* temp = FindLinguisticVariableByName(variableName);
			return temp->FindLinguisticValueByName(valueName)->GetB();
		}

		double GetC_ByName(string variableName, string valueName)
		{
			CLinguisticVariable* temp = FindLinguisticVariableByName(variableName);
			return temp->FindLinguisticValueByName(valueName)->GetC();
		}

		double GetD_ByName(string variableName, string valueName)
		{
			CLinguisticVariable* temp = FindLinguisticVariableByName(variableName);
			return temp->FindLinguisticValueByName(valueName)->GetD();
		}

		double GetLinguisticVariableValue(string variableName, string valueName)
		{
			CLinguisticVariable* temp = FindLinguisticVariableByName(variableName);
			assert(temp != NULL);
			return temp->FindLinguisticValueByName(valueName)->GetLinguisticValue();
		}
	};
}

#endif
