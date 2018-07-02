#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <string>
#include <map>
#include <list>
#include <vector>
#include <cstdio>
#include <cstdlib>
using namespace std;

class cfg {
	public:
		cfg(void){};
		
		cfg(string namefile)
		{
			FILE* file = fopen(namefile.data(), "r");
			if (!file)
				throw "Error: File No Found";
			char buff[1024];
			string line;
			string name, value;
			list<cfg*> groupStack;
			groupStack.push_front(this);
			while (fgets(buff, 1024, file))
			{
				line = trim(buff);
				if (line.length() && (line[0] != '#'))
				{
					if (line.length() > 1)
					{
						vector<string> section = split(line, '=');
						if ((section.size() != 2) || (section.front() == ""))
							throw "Error: Wrong Configuration File";
						name = section.front();
						value = section.back();
						if (value != "")
							groupStack.front()->symbols[name] = value;
						else
							groupStack.front()->groups[name] = cfg();
					}
					else if ((line == "{") && value == "")
						groupStack.push_front(&(groupStack.front()->groups[name]));
					else if ((line == "}") && (value != "") && (groupStack.size() > 1))
						groupStack.pop_front();
					else
						throw "Error: Wrong Configuration File";
				}
			}
			groupStack.pop_front();
			if (groupStack.size() > 0)
				throw "Error: Wrong Configuration File";
			fclose(file);
		}
				
		~cfg(void){};
		
		string getValueString(string name)
		{
			map<string, string>::iterator i = symbols.find(name);
			if (i == symbols.end()) 
				throw "Error: Missing Configuration Value";
			return i->second;
		}

		vector<string> getVectorString(string name)
		{
			string val = getValueString(name);
			return split(val, ',');
		}
		
		string getValueVectorString(string name, unsigned int number)
		{
			vector<string> val = getVectorString(name);
			return val[number];
		}

		bool getValueBool(string name)
		{
			string val = getValueString(name);
			return string2bool(val);
		}
    
		vector<bool> getVectorBool(string name)
		{
			vector<string> val = getVectorString(name);
			vector<bool> vec(val.size());
			for(unsigned int i = 0; i < val.size(); i++)
				vec[i] = string2bool(val[i]);
			return vec;
		}
		
		bool getValueVectorBool(string name, unsigned int number)
		{
			vector<string> val = getVectorString(name);
			return string2bool(val[number]);
		}

		double getValueDouble(string name)
		{
			string val = getValueString(name);
			return string2double(val);
		}
    
		vector<double> getVectorDouble(string name)
		{
			vector<string> val = getVectorString(name);
			vector<double> vec(val.size());
			for(unsigned int i = 0; i < val.size(); i++)
				vec[i] = string2double(val[i]);
			return vec;
		}
		
		double getValueVectorDouble(string name, unsigned int number)
		{
			vector<string> val = getVectorString(name);
			return string2double(val[number]);
		}

		int getValueInt(string name)
		{
			string val = getValueString(name);
			return string2int(val);
		}
    
        	vector<int> getVectorInt(string name)
		{
			vector<string> val = getVectorString(name);
			vector<int> vec(val.size());
			for(unsigned int i = 0; i < val.size(); i++)
				vec[i] = string2int(val[i]);
			return vec;
		}
		
		int getValueVectorInt(string name, unsigned int number)
		{
			vector<string> val = getVectorString(name);
			return string2int(val[number]);
		}

		inline map<string, string>& getSymbols()
		{
			return symbols;
		}

		inline cfg getGroup(string name)
		{
			return groups[name];
		}

		inline map<string, cfg>& getGroups(void)
		{
			return groups;
		}
		
		inline unsigned int getTotalGroups(void)
		{
			return groups.size();
		}
		
		inline bool getIfExistSymbol(string name)
		{
			map<string, string>::iterator i = symbols.find(name);
			if (i == symbols.end())
				return false;
			else
				return true;
		}
		
		inline bool getIfExistGroup(string name)
		{
			map<string, cfg>::iterator i = groups.find(name);
			if (i == groups.end())
				return false;
			else
				return true;
		}

	private:
	
		bool string2bool(string value)
		{
			if ( (value == "yes") || (value == "Yes") || (value == "YES") || (value == "true") || (value == "True") || (value == "TRUE"))
				return true;
			return false;
		}
		
		double string2double(string value)
		{
			return atof(value.c_str());
		}
		
		int string2int(string value)
		{
			return atoi(value.c_str());
		}
		
		vector<string> split(string line, char splitter)
		{
			vector<string> tokens;
			string token;
			for (unsigned int i = 0; i < line.size(); i++)
			{
				if(line.at(i) == splitter)
				{
					tokens.push_back(trim(token));
					token = "";
				}
				else
					token += line.at(i);
			}
			tokens.push_back(trim(token));
			return tokens;
		}
		
		string trim(string section)
		{
			while ( (section.length() > 0) && ( (section[0] == ' ') || (section[0] == '\t') ) )
				section = section.substr(1, string::npos);
			while ( (section.length() > 0) && ( (section[section.length() - 1] == ' ') || (section[section.length() - 1] == '\t') || (section[section.length() - 1] == '\n') || (section[section.length() - 1] == '\r') ) )
				section = section.substr(0, section.length() - 1);
			while ( (section.length() > 1) && (section[0] == '[') )
				section = section.substr(1, string::npos);
			while ( (section.length() > 1) && (section[section.length() - 1] == ']') )
				section = section.substr(0, section.length() - 1);
			return section;
		}
		
		map<string, string> symbols;

		map<string, cfg> groups;
};

#endif
