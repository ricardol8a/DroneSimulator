#ifndef FUZZYREASONER_H
#define FUZZYREASONER_H

#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cmath>
#include "fl/FuzzyInput.h"
#include "fl/FuzzyCalculator.h"
#include "fl/LinguisticVariable.h"
#include "fl/FuzzyRule.h"

using namespace std;
using namespace fl;

namespace fl{
	//Main class for calculating FuzzyLogic
	class CFuzzyReasoner
	{
		//Vector of Fuzzy rules
		vector<CFuzzyRule*> v_fuzzyRulesVector; 
	public:

		//Constructor
		CFuzzyReasoner(void)
		{

		}

		~CFuzzyReasoner(void)
		{
			//We don't like memory leaks, so release all rules
			for(unsigned int i = 0; i < v_fuzzyRulesVector.size(); i++)
				delete v_fuzzyRulesVector[i];
		}

		//Add fuzzy rule
		void AddFuzzyRule(CFuzzyRule* p_fuzzyRule)
		{
			v_fuzzyRulesVector.push_back(p_fuzzyRule);
		}

		//Start reasoner for fuzzy rulles
		CLinguisticVariable* CalculateFuzzyRules(vector <CFuzzyInput*> h_fuzzyInputs)
		{
			//Reset all values

			CFuzzyRuleToken* outputObject = NULL;
			//Calculate all fuzzy rules
			for(unsigned int i = 0; i < v_fuzzyRulesVector.size(); i++)
			{
				//Glede na izhodno vrednost izhodne spremeljivke naredi update izhodnega objekta
				outputObject = FuzzyRuleReasoner(v_fuzzyRulesVector[i], h_fuzzyInputs);
			}
			//Return output object
			return outputObject->GetLinguisticVariable();
		}

		//Start fuzzy rule reasoner
		CFuzzyRuleToken* FuzzyRuleReasoner(CFuzzyRule* fuzzyRule, vector <CFuzzyInput*> h_fuzzyInputs)
		{
			double resultVal = 0;
			for(unsigned int i = 0; i < fuzzyRule->GetFuzzyRuleTokens().size(); i++)
			{
				//Get fuzzy rule token
				CFuzzyRuleToken* token = fuzzyRule->GetFuzzyRuleTokens()[i];

				if(token->IsOutput())
				{
					//Update output object
					token->UpdateTokenValue(resultVal);
					//Return result token
					return token;
				}
				else
				{
					double tokenVal = token->CalculateTokenOutputValue(h_fuzzyInputs);
					token->UpdateTokenValue(tokenVal);
					if(token->IsNegated())
						tokenVal = 1 - tokenVal;	//Negate value
					if(i == 0)
						resultVal = tokenVal;		//Set value
					else if(token->IsOrOperator())
						resultVal = CFuzzyCalculator::OR(resultVal, tokenVal); //OR operator
					else if(token->IsAndOperator())
						resultVal = CFuzzyCalculator::AND(resultVal, tokenVal); //AND operator

				}
			}
			//This won't happends saso
			assert(NULL);
			return NULL;
		}
		//Defuzzyfication
		double Defuzzy(CLinguisticVariable *output)
		{
			//For every output value
			vector<LinguisticValue*> valuesList = output->GetLinguisticValuesList();

			double upEqualation = 0;
			double downEqualation = 0;
			//Calculating defuzzy value
			for(unsigned int i = 0; i < valuesList.size(); i++)
			{
				LinguisticValue* val = valuesList.at(i);
				upEqualation += val->GetLinguisticValue()
					* CalculateTrapezoidBalance(val->GetA(), val->GetB(), val->GetC(), val->GetD()); 
				downEqualation += val->GetLinguisticValue();
			}
			//Return output value of system
			if(downEqualation == 0)
				return 0;
			return upEqualation / downEqualation;
		}

		//Calculating surface of trapezoid 
		double CalculateTrapezoidBalance(double A, double B, double C, double D)
		{
			return (C * C + D * D + C * D - A * A - B * B - A * B) / (3 * (C + D - A - B));
		}
	};
}

#endif
