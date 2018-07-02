#ifndef LINGUISTCVARIABLE_H
#define LINGUISTCVARIABLE_H

#include <vector>
#include <cassert>
#include <map>
#include "fl/FuzzyCalculator.h"
#include "fl/LinguisticValue.h"

using namespace std;
using namespace fl;

namespace fl{

	class CLinguisticVariable
	{
	private:
		//Variable name
		string variableName;

		//Is variable input or output
		bool b_output;

		//Table of linguistic values
		map <string, LinguisticValue*> h_linguisticValuesMap;
	public:

		//Default constructor
		CLinguisticVariable(bool nOutput, string nVariableName)
		{
			b_output = nOutput;
			variableName = nVariableName;
		}

		//Destructor
		~CLinguisticVariable(void)
		{
			//Release all LinguisticValue objects
			map <string, LinguisticValue*> :: iterator iterator = h_linguisticValuesMap.begin();
			while(iterator != h_linguisticValuesMap.end())
			{
				//Repeat until the end
				LinguisticValue* temp = iterator->second;
				//Remove item from hash table
				h_linguisticValuesMap.erase(iterator);
				//Move to first item of hash table
				iterator = h_linguisticValuesMap.begin();
				//Delete LinguisticValue object
				delete temp;
			}
		}

		//Methods
		//Method add Linguistic value to hah map
		void AddLinguisticValue(LinguisticValue* p_linguisticValue)
		{
			h_linguisticValuesMap[p_linguisticValue->ToString()] = p_linguisticValue;
		}

		void ResetLinguisticValues()
		{
			//Just for reseting linguistic values
			map <string, LinguisticValue*> :: iterator iterator = h_linguisticValuesMap.begin();
			while(iterator != h_linguisticValuesMap.end())
			{
				//Repeat until the end
				LinguisticValue* temp = iterator->second;
				temp->SetLinguisticValue(-1);
				iterator++;
			}
		}

		LinguisticValue* FindLinguisticValueByName(string name)
		{
			//Fint linguistic value by name
			map <string, LinguisticValue*> :: iterator iterator = h_linguisticValuesMap.find(name);
			if(iterator == h_linguisticValuesMap.end())
			{
				//SASO, this should never happends!!!!!
				assert(NULL);
				return NULL;
			}
			//Return LinguisticValue
			return iterator->second;
		}

		vector<LinguisticValue*> GetLinguisticValuesList()
		{
			vector<LinguisticValue*> linguisticValuesList;

			map <string, LinguisticValue*> :: iterator iterator = h_linguisticValuesMap.begin();
			while(iterator != h_linguisticValuesMap.end())
			{
				//Repeat until the end
				LinguisticValue* temp = iterator->second;
				//Remove item from hash table
				linguisticValuesList.push_back(temp);
				iterator++;
			}
			return linguisticValuesList;
		}

		double CalculateLinguisticValueByName(string name, double input)
		{
			LinguisticValue* value = FindLinguisticValueByName(name);
			return value->CalculateLinguisticValue(input);
		}

		//Update Linguistic value by name
		void UpdateLinguisticValueByName(string name, double newVal)
		{
			//Find LinguisticValue
			LinguisticValue* value = FindLinguisticValueByName(name);

			//If there is value, we should perform operator OR
			if(value->GetLinguisticValue() != -1)
				value->SetLinguisticValue(CFuzzyCalculator::OR(value->GetLinguisticValue(), newVal));
			else
				value->SetLinguisticValue(newVal); //There is no value, just set newVal
		}


		//Getters
		bool IsOutputVariable(){return b_output;}
		string ToString(){return variableName;}
	};
}

#endif
