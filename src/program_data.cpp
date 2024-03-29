
#include "program_data.hpp"

#include <cctype>
#include <iostream>  // cerr

// this function first splits the lines between memory and instruction types
// secondly, it generates the labels table
program_data parse(std::vector<std::string> && raw_program) {
	program_data to_ret{};

	// separate each line into either a memory or instruction line
	for (auto && line : raw_program) {
		if (line.at(0) == '[') {
			// memory type line
			to_ret.memory.push_back(line);
		} else if (isalnum(line.at(0))) {
			// instruction type
			to_ret.instructions.push_back(line);
		} else if (line.at(0) != '"') {
			// unusual line
			std::cerr << "Unusual line found: " << line << std::endl;
		}
	}

	// populate the label table
	// first the variables
	for (const auto & variable : to_ret.memory) {
		// find the name and remove all initial whitespace
		auto name = variable.substr(variable.find(']') + 1);
		while (isspace(name.at(0))) { name = name.substr(1); }

		// get the number
		const auto number_start = variable.find_first_of("0123456789");
		const auto number_len   = variable.find(']') - number_start;
		short number = std::stoi(variable.substr(number_start, number_len));

		// get the initialization value
		auto value	 = unused_value;
		auto equal_pos = name.find('=');
		if (equal_pos != std::string::npos) {
			value = std::stoi(name.substr(equal_pos + 1));
			name  = name.substr(0, equal_pos - 1);
		}

		auto iter = to_ret.labels.find(name);
		if (iter == to_ret.labels.end()) {
			to_ret.labels.emplace(name, std::make_pair(number, value));
		} else {
			// duplicate name
			std::cerr << "Duplicate label found: " << iter->first << " at "
					  << iter->second.first << " and " << name << " at "
					  << number << std::endl;
		}
	}

	// now the line labels
	// these need numbers
	size_t number = 0;
	for (const auto & line : to_ret.instructions) {
		auto label_end = line.find(':');
		if (label_end == std::string::npos) {
			// continue looking at other lines
			number++;
			continue;
		}

		// get the label from the rest of the line
		auto label = line.substr(0, label_end);
		while (not isalnum(label.back())) {
			label = label.substr(0, label.size() - 1);
		}

		auto iter = to_ret.labels.find(label);
		if (iter == to_ret.labels.end()) {
			to_ret.labels.emplace(label, std::make_pair(number, unused_value));
		} else {
			// duplicate name
			std::cerr << "Duplicate label found: " << iter->first << " at "
					  << iter->second.first << " and " << label << " at "
					  << number << std::endl;
		}

		number++;
	}

	return to_ret;
}

// this function substitutes the labels as needed
std::vector<std::string> assemble(program_data && program) {
	std::vector<std::string> to_ret{};

	for (const auto & value : program.memory) {
		auto name_start = value.find_first_not_of("0123456789[] \t");
		auto name_len
			= (value.find_last_not_of("0123456789[] \t=") - name_start) + 1;

		auto name = value.substr(name_start, name_len);

		auto iter = program.labels.find(name);
		if (iter != program.labels.end()) {
			if (iter->second.second != unused_value) {
				to_ret.push_back(std::to_string(iter->second.first) + ','
								 + std::to_string(iter->second.second));
			}
		} else {
			std::cerr << "Could not find variable " << name << std::endl;
		}
	}

	int line_count = 0;
	for (const auto & line : program.instructions) {
		// first, remove the labels from the current lines
		// then, replace the old labeled lines

		std::string to_output;
		if (line.find(':') != std::string::npos) {
			to_output = line.substr(line.find(':'));
			while (not isalnum(to_output.front())) {
				to_output = to_output.substr(1);
			}
		} else {
			to_output = line;
		}

		auto start_of_label = to_output.find_last_of("\t ") + 1;
		auto end_of_label   = to_output.find_last_not_of("0123456789 \t") + 1;

		if (start_of_label < end_of_label
			and start_of_label != std::string::npos
			and end_of_label != std::string::npos) {
			auto label_len = end_of_label - start_of_label;
			auto label	 = to_output.substr(start_of_label, label_len);

			auto iter = program.labels.find(label);
			if (iter != program.labels.end()) {
				to_output.replace(start_of_label, label_len,
								  std::to_string(iter->second.first));
				// TODO: Replace with a check for whether the "label" is
				// actually an instruction
			} else if (not isupper(label.front())
					   and not isdigit(label.front())) {
				std::cerr << "Could not find label " << label << " from line "
						  << line_count << std::endl;
			}
		}

		to_ret.push_back(std::to_string(line_count++) + ',' + to_output);
	}

	return to_ret;
}
