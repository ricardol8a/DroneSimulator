#ifndef LINGUISTICVALUE_H
#define LINGUISTICVALUE_H

#include <cstdlib>
#include <sstream>

using namespace std;

namespace fl{
	class LinguisticValue
	{
	private:
		//Trapezoid borders
		double A;
		double B;
		double C;
		double D;
		//Linguistic value name
		string s_valueName;
		//Value
		double linguisticValue; 
	public:
		//Constructor
		LinguisticValue(string nValueName, double nA, double nB, double nC, double nD)
		{
			A = nA;
			B = nB;
			C = nC;
			D = nD;
			//We don't want to div x / 0 right?
			if((A == B) && (B == C) && (C == D))
			{
				A += -1e-6;
				D +=  1e-6;
			}
			s_valueName = nValueName;
			linguisticValue = -1;
		}

		//Destructor
		~LinguisticValue(void)
		{

		}

		double CalculateLinguisticValue(double input)
		{
			//Normalize variables
			if(input < A)
				input = A;
			else if(input > D)
				input = D;
			//Calculating Trapezoidal function
			if ((B <= input) && (input <= C))
				return 1;
			else if((A < input) && (input < B))
				return (input - A) / (B - A);
			else if ((C < input) && (input < D))
				return (D - input) / (D - C);
			else
				return 0;
		}


		//Setters
		void SetLinguisticValue(double nLinguisticValue){linguisticValue = nLinguisticValue;}
		//Getters
		double GetLinguisticValue(){
			if(linguisticValue == -1)
				linguisticValue = 0;
			return linguisticValue;
		}
		double GetA(){return A;}
		double GetB(){return B;}
		double GetC(){return C;}
		double GetD(){return D;}
		string ToString(){return s_valueName;}
	};
}

#endif
