#ifndef FUZZYRULETOKEN_H
#define FUZZYRULETOKEN_H

#include <vector>
#include "fl/FuzzyInput.h"
#include "fl/LinguisticVariable.h"

using namespace std;
using namespace fl;

namespace fl{

	//ENUM operation
	enum Operation{
		AND,OR,EMPTY
	};
	class CFuzzyRuleToken
	{
		//Is there operator before token
		Operation operation;
		//Is there negation for this token
		bool b_negation;
		//Pointer to linguistic variable
		CLinguisticVariable* p_linguisticVariable;
		//Name of Linguistic value
		string linguisticValueName;

	public:

		//Constructor
		CFuzzyRuleToken(bool nNegation, 
			Operation nOperation, 
			CLinguisticVariable* nLinguisticVariable, 
			string nLinguisticValueId)
		{
			b_negation = nNegation;
			operation = nOperation;
			p_linguisticVariable = nLinguisticVariable;
			linguisticValueName = nLinguisticValueId;
		}

		~CFuzzyRuleToken(void)
		{
			
		}

		//Calculate token value
		double CalculateTokenOutputValue(vector <CFuzzyInput*> h_fuzzyInput)
		{
			//Find linguistic variable
			unsigned int i = 0;
			while(i != h_fuzzyInput.size())
			{
				if(h_fuzzyInput[i]->GetVariableName() == p_linguisticVariable->ToString())
					break;
				i++;
			}
			double input = h_fuzzyInput[i]->GetInputValue();
			//vector <CFuzzyInput*> :: iterator iterator = h_fuzzyInput.begin();
			//while(iterator != h_fuzzyInput.end())
			//{
			//	if((*iterator)->GetVariableName() == p_linguisticVariable->ToString())
			//		break;
			//	iterator++;
			//}
			//double input = (*iterator)->GetInputValue();
			//Calculate linguistic value
			return p_linguisticVariable->CalculateLinguisticValueByName(linguisticValueName, input);
		}

		void UpdateTokenValue(double newVal)
		{
			//Just update token value
			p_linguisticVariable->UpdateLinguisticValueByName(linguisticValueName, newVal);
		}

		//GETTERS
		string GetLinguisticValueName(){return linguisticValueName;}
		CLinguisticVariable* GetLinguisticVariable(){return p_linguisticVariable;}

		bool IsOutput(){ return p_linguisticVariable->IsOutputVariable();}
		bool IsNegated(){return b_negation;}
		bool IsFirst(){
			if(operation == EMPTY) 
				return true;
			return false;
		}

		bool IsAndOperator(){
			if(operation == AND) 
				return true;
			return false;
		}

		bool IsOrOperator(){
			if(operation == OR) 
				return true;
			return false;
		}
	};
}

#endif
