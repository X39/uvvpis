#include "emulator/vpu.h"
#include "core/binary.h"

#include <tclap/CmdLine.h>
#include <filesystem>

#include <sstream>
#include <iostream>

int main(int argc, char** argv)
{
	TCLAP::CmdLine cmd("Universal Videogame Virtual Processor Instruction Set Command Line Interface (UVVPIS CLI)");

	TCLAP::ValueArg<std::string> runArg("", "run", "Executes the given file, using the emulator.", false, "", "PATH");
	cmd.add(runArg);

	cmd.parse(argc, argv);
	if (argc <= 1) { std::cout << "Use --help [-h] to display command line options." << std::endl; }
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	if (!runArg.getValue().empty())
	{
		uvvpis::vpu vpu;
		uvvpis::binary bin;
		std::filesystem::path fpath(runArg.getValue());
		fpath = fpath.lexically_normal();
		std::cout << "Loading file '" << fpath << "'." << std::endl;
		auto res = bin.load(fpath.string());
		if (res != uvvpis::binary::result::ok)
		{
			std::cout << "Failed..." << std::endl;
		}
		else
		{
			for (;;)
			{
				auto inst = bin[vpu.registers().pc];
				vpu.execute(inst);
				std::cout << uvvpis::to_string(inst) << '\n';
				{
					std::stringstream sstream;
					vpu.print_registers(sstream, "   ");
					std::cout << sstream.str() << std::endl;
				}
				if (inst.op == uvvpis::opcode::HLT)
				{
					break;
				}
				if (inst.op == uvvpis::opcode::EXT)
				{
					switch (inst.argument)
					{
					default:
					case 0:
						std::string str;
						std::cout << '\n' << "---EXT halt requested. Press [ENTER] to continue.---" << std::endl;
						std::getline(std::cin, str);
					}
				}
			}
		}
	}
	return 0;
}