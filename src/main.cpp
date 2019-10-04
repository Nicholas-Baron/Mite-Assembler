
#include "cmdargs.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::vector<std::string> read_file(const std::string & name) {
	std::vector<std::string> to_ret{};

	std::ifstream file{name};
	while (file.good()) {
		std::string temp;
		std::getline(file, temp);
		to_ret.emplace_back(std::move(temp));
	}

	return to_ret;
}

std::vector<std::string> remove_comments(std::vector<std::string> && lines) {
	for (auto & line : lines) {
		line = line.substr(0, line.find_first_of(';'));
	}

	return std::move(lines);
}

int main(int arg_count, char ** args) {
	auto options = parse_command_line(arg_count, args);

	if (options->show_help) { return 0; }

	std::cout << "Reading " << options->input_file << " ..." << std::endl;

	auto input = read_file(options->input_file);

	input = remove_comments(std::move(input));

	for (const auto & line : input) { std::cout << line << std::endl; }
}
