
#include "program_data.hpp"

#include <cctype>
#include <iostream> // cerr

// this function first splits the lines between memory and instruction types
// secondly, it generates the labels table
program_data parse(std::vector<std::string>&& raw_program){
	
	program_data to_ret{};

	// separate each line into either a memory or instruction line
	for(auto&& line : raw_program){
		if(line.at(0) == '['){
			// memory type line
			to_ret.memory.push_back(line);
		}else if(isalnum(line.at(0))){
			// instruction type
			to_ret.instructions.push_back(line);
		}else{
			// unusual line
			std::cerr << "Unusual line found: " << line << std::endl;
		}
	}

	// populate the label table
	// first the variables
	for(const auto& variable : to_ret.memory){
		// find the name and remove all initial whitespace
		auto name = variable.substr(variable.find(']') + 1);
		while(isspace(name.at(0))){
			name = name.substr(1);
		}

		// get the number
		const auto number_start = variable.find_first_of("0123456789");
		const auto number_len = variable.find(']') - number_start;
		auto number = std::stoi(variable.substr(number_start, number_len));

		std::cout << "Found label " << name << std::endl;

		auto iter = to_ret.labels.find(name);
		if(iter == to_ret.labels.end()){
			to_ret.labels.emplace(name, number);
		}else{
			// duplicate name
			std::cerr << "Duplicate label found: " 
					  << iter->first << " at " << iter->second
					  << " and " << name <<" at " << number << std::endl;
		}
	}

	// now the line labels
	// these need numbers
	size_t number = 0;
	for(const auto& line : to_ret.instructions){
		auto label_end = line.find(':');
		if(label_end == std::string::npos){
			// continue looking at other lines
			number++;
			continue;
		}

		// get the label from the rest of the line
		auto label = line.substr(0, label_end);

		std::cout << "Found label " << label << std::endl;

		auto iter = to_ret.labels.find(label);
		if(iter == to_ret.labels.end()){
			to_ret.labels.emplace(label, number);
		}else{
			// duplicate name
			std::cerr << "Duplicate label found: " 
					  << iter->first << " at " << iter->second
					  << " and " << label <<" at " << number << std::endl;
		}

		number++;
	}

	return to_ret;
}

// this function substitutes the labels as needed
std::vector<std::string> assemble(program_data&& program){
	return {};
}

