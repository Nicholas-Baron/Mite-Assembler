
#include "cmdargs.hpp"
#include "program_data.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

[[nodiscard]] std::vector<std::string> read_file(const std::string & name) {
	std::ifstream file{name};
	if (not file) {
		std::cerr << "Could not read file " << name << std::endl;
		return {};
	}

	std::vector<std::string> to_ret{};
	std::string				 temp;
	while (std::getline(file, temp)) {
		if(not temp.empty()) to_ret.emplace_back(std::move(temp));
	}

	return to_ret;
}

// This function uses a streaming paradigm
// It takes ownership of its parameters and does some modification over them
// Remove empty lines, comments,
std::vector<std::string> cleanup_lines(std::vector<std::string> && lines) {
	static constexpr const char * whitespace = " \t\n\v\f";

	// remove empty lines
	// Check if there is at least one empty line to remove
	auto to_remove
		= std::find_if(lines.begin(), lines.end(),
					   [](const auto & item) { return item.empty(); });
	while (to_remove != lines.end()) {
		// Remove that element and start looking from the element after the
		// removed
		to_remove
			= std::find_if(lines.erase(to_remove), lines.end(),
						   [](const auto & item) { return item.empty(); });
	}

	for (auto & line : lines) {
		line = line.substr(0, line.find_first_of(';'));  // delete comments
		line = line.substr(
			line.find_first_not_of(whitespace));  // remove left side whitespace
		line = line.substr(0, line.find_last_not_of(whitespace)
								  + 1);  // remove right side whitespace
	}

	// remove empty lines after doing the other processing (could have generated
	// some other lines) Check if there is at least one empty line to remove
	to_remove = std::find_if(lines.begin(), lines.end(),
							 [](const auto & item) { return item.empty(); });
	while (to_remove != lines.end()) {
		// Remove that element and start looking from the element after the
		// removed
		to_remove
			= std::find_if(lines.erase(to_remove), lines.end(),
						   [](const auto & item) { return item.empty(); });
	}

	return std::move(lines);
}

int main(int arg_count, char ** args) {
	auto options = parse_command_line(arg_count, args);

	if (options->show_help) { return 0; }

	std::cout << "Reading " << options->input_file << "..." << std::endl;

	auto input = read_file(options->input_file);

	std::cout << "Input size is " << input.size() << std::endl;

	input = cleanup_lines(std::move(input));

	if (options->output_file.empty()) {
		options->output_file = options->input_file.replace(
			options->input_file.size() - 3, 3, "csv");
	}

	std::cout << "Output is "
			  << (isalnum(options->output_file.front())
					  ? options->output_file.c_str()
					  : "standard out")
			  << std::endl;

	std::ofstream out_file{options->output_file};
	if (not out_file) {
		std::cerr << "Could not open " << options->output_file << std::endl;
		return 1;
	}

	if (raw_instructions(input)) {
		std::cout << "Found a raw program, no formatting needed." << std::endl;

		for (const auto & line : input) { out_file << line << std::endl; }

		return 0;
	}

	auto prog_data = parse(std::move(input));

	if (options->show_debug) {
		std::cout << "Variable list:\n";
		for (const auto & var : prog_data.memory) {
			std::cout << var << std::endl;
		}

		std::cout << "\nInstructions:\n";
		for (const auto & inst : prog_data.instructions) {
			std::cout << inst << std::endl;
		}

		std::cout << "\nLabel Mapping:\n";
		for (const auto & label : prog_data.labels) {
			std::cout << label.first << " -> " << label.second.first;
			if (label.second.second != unused_value) {
				std::cout << ", " << label.second.second;
			}
			std::cout << std::endl;
		}
	}
	auto output = assemble(std::move(prog_data));

	std::cout << "Outputting " << output.size() << " lines" << std::endl;
	for (const auto & line : output) {
		std::cout << line << std::endl;
		out_file << line << std::endl;
	}
}
