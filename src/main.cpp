
#include "cmdargs.hpp"

#include <iostream>

int main(int arg_count, char ** args) {
	auto options = parse_command_line(arg_count, args);

	if (options->show_help) { return 0; }

	std::cout << "Reading " << options->input_file << " ..." << std::endl;
}
