#ifndef _PROGRAM_DATA_
#define _PROGRAM_DATA_

#include <map>
#include <vector>
#include <string>

struct program_data{
	std::vector<std::string> instructions;
	std::vector<std::string> memory;
	std::map<std::string, int> labels;
};

// A raw instruction is defined as a line in the form "number,instruction"
// The assembler cannot work with these lines, as arranging to fit that number
// prevents the use of label substitution
inline bool raw_instructions(const std::vector<std::string>& program){

	for(const auto& line : program){
		if(line.find(',') != std::string::npos){ return true; }
	}
	
	return false;
}

// this function first splits the lines between memory and instruction types
// secondly, it generates the labels table
program_data parse(std::vector<std::string>&& raw_program);

// this function substitutes the labels as needed
std::vector<std::string> assemble(program_data&& program);

#endif

