
#include "cmdargs.hpp"

#include "clara/clara.hpp"

#include <iostream>
#include <memory>

std::unique_ptr<cmd_args> parse_command_line(int arg_count, char ** args) {
	cmd_args to_ret{};

	auto cli = clara::Help(to_ret.show_help)
			   | clara::Arg(to_ret.input_file, "input file")("File to read")
			   | clara::Opt(to_ret.output_file,
							"output file")["-o", "--output"]("File to write to")
			   | clara::Opt(to_ret.show_debug)["-v"]("Show the debug printing");

	auto result = cli.parse(clara::Args(arg_count, args));

	if (not result) {
		std::cerr << "Could not parse command line arguments: "
				  << result.errorMessage() << std::endl;
		std::exit(1);
	}

	if (to_ret.show_help) { std::cout << cli << std::endl; }

	return std::make_unique<cmd_args>(std::move(to_ret));
}
