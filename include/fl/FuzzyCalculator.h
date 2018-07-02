#ifndef FUZZYCALCULATOR_H
#define FUZZYCALCULATOR_H

namespace fl
{
	class CFuzzyCalculator
	{
	public:
		//Implementation of fuzzy operators

		//Operator AND
		static double AND(double a, double b)
		{
			if(a < b)
				return a;
			return b;
		}

		//Operator OR
		static double OR(double a, double b)
		{
			if(a > b)
				return a;
			return b;
		}
	};
}

#endif
