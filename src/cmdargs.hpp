#ifndef _CMD_ARGS_
#define _CMD_ARGS_

#include <memory>	// unique_ptr
#include <string>

// Header to interface with clara

// A struct which contains every command line argument
struct cmd_args{
	std::string input_file;		// source file
	std::string output_file;	// destination file
};

std::unique_ptr<cmd_args> parse_command_line(int arg_count, char** args);

#endif

