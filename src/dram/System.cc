/*
 * Multi2Sim
 * Copyright (C) 2014 Agamemnon Despopoulos (agdespopoulos@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <vector>
#include <algorithm>
#include <iostream>

#include <lib/cpp/CommandLine.h>
#include <lib/cpp/Misc.h>
#include <lib/cpp/String.h>
#include <lib/esim/Engine.h>

#include "Address.h"
#include "Bank.h"
#include "Controller.h"
#include "Channel.h"
#include "Scheduler.h"
#include "System.h"


namespace dram
{

//
// Configuration Options
//

std::string debug_file;

std::string activity_file;

std::string config_file;

bool System::stand_alone = false;

bool System::help = false;

int System::frequency = 667;


//
// Static variables
//

misc::Debug System::debug;

misc::Debug System::activity;

std::unique_ptr<System> System::instance;

esim::FrequencyDomain *System::frequency_domain(nullptr);

esim::Event *System::event_command_return(nullptr);

const char *System::err_config_note =
		"Please run 'm2s --dram-help' or consult the Multi2Sim Guide for "
		"a description of the DRAM system configuration file format.";

const std::string System::help_message =
		"Option '--dram-config <file>' is used to configure the DRAM system. The\n"
		"configuration file is a plain-text file in the IniFile format. The DRAM\n"
		"system is comprised of one or more memory controllers.\n"
		"\n"
		"The following sections and variables can be used in the DRAM system\n"
		"configuration file:\n"
		"\n"
		"Section [General] defines global parameters affecting the entire DRAM\n"
		"system.\n"
		"\n"
		"  Frequency = <value>  (Default = 1000)\n"
		"      Frequency of the DRAM system in MHz.\n"
		"\n"
		"Section [MemoryController <name>] defines a generic DRAM device. This section is\n"
		"used to define the modes of the DRAM, the size of the componants and the timings.\n"
		"The default configuration results in an 8GB DRAM device with typical timings for \n"
		"a DDR3 device running at 1600MHz.\n"
		"\n"
		"  PagePolicy = {Open|Closed} (Default = Open) \n"
		"      Policy that dictates whether the row of a bank remains open or closed.\n"
		"  SchedulingPolicy = {OldestFirst|BankRoundRobin} (Default = OldestFirst)\n"
		"      Policy that determines which bank is allowed to execute a command.\n"
		"  NumChannels = <num> (Default =  1)\n"
		"      Number of channels in the DRAM system.\n"
		"  NumRanks = <num> (Default = 2)\n"
		"      Number of ranks in each channel.\n"
		"  NumBanks = <num> (Default = 8)\n"
		"      Number of banks in each rank.\n"
		"  NumRows = <num> (Default = 1024)\n"
		"      Number of rows in each bank.\n"
		"  NumColumns = <num> (Default = 1024)\n"
		"      Number of columns in each row.\n"
		"  NumBits = <num> (Default = 8)\n"
		"      Number of bits in each column.\n"
		"  tRC = <num> (Default = 49)\n"
		"  tRRD = <num> (Default = 5)\n"
		"  tRP = <num> (Default = 11)\n"
		"  tRFC = <num> (Default = 128)\n"
		"  tCCD = <num> (Default = 4)\n"
		"  tRTRS = <num> (Default = 1)\n"
		"  tCWD = <num> (Default = 5)\n"
		"  tWTR = <num> (Default = 6)\n"
		"  tCAS = <num> (Default = 11)\n"
		"  tRCD = <num> (Default = 11)\n"
		"  tOST = <num> (Default = 1)\n"
		"  tRAS = <num> (Default = 28)\n"
		"  tWR = <num> (Default = 12)\n"
		"  tRTP = <num> (Default = 6)\n"
		"  tBURST = <num> (Default = 4)\n";


System *System::getInstance()
{
	// Instance already exists
	if (instance.get())
		return instance.get();

	// Create instance
	instance.reset(new System());
	return instance.get();
}


void System::Destroy()
{
    instance = nullptr;
}


System::System()
{
	debug << "Dram simulator initialized\n";
}


int System::getCapacity()
{
	int num_controllers = 1;
	int num_channels = 1;
	int num_ranks = 1;
	int num_banks = 1;
	int num_rows = 1;
	int num_columns = 1;

	// Retrieve number of controllers
	num_controllers = controllers.size();

	// Find the largest of each component size because one address format
	// must be able to decode to any location in the system.
	// Controllers can potentially each have different sizes for everything
	for (auto const &controller : controllers)
	{
		num_channels = std::max(num_channels, 
				controller->getNumChannels());
		num_ranks = std::max(rank_size, controller->getNumRanks());
		num_banks = std::max(bank_size, controller->getNumBanks());
		num_rows = std::max(row_size, controller->getNumRows());
		num_columns = std::max(column_size, 
				controller->getNumColumns());
	}

	return num_controllers * num_channels * num_ranks * num_banks
		* num_rows * num_columns;
}


void System::RegisterOptions()
{

	//
	// FIXME: These options are removed, in order to be excluded from
	// the m2s --help. However, later when we hook up the DRAM model to
	// the memory system, it will come back.
	// 
	// FIXME 2: A whole --dram-trace option should be added as an 
	// input to the stand-alone DRAM. Otherwise, the stand-alone
	// does not make any sense. It cannot be actions, as part of the
	// configuration file.
	//
	// FIXME 3: The debug and debug_activity files should be combined
	// into one. It does not make sense to have both of them as two
	// separate file.  
	// Get command line object
	misc::CommandLine *command_line = misc::CommandLine::getInstance();

	// Category
	command_line->setCategory("DRAM");

	// Debugger for dram
	command_line->RegisterString("--dram-debug <file>",
			debug_file,
			"Dump debug information related with the DRAM "
			"simulation.");

	// Activity log for dram
	command_line->RegisterString("--dram-debug-activity <file>",
			activity_file,
			"Dump debug information related with DRAM activity "
			"during simulation.");

	// Dram system configuration
	command_line->RegisterString("--dram-config <file>",
			config_file,
			"DRAM configuration file. Memory controllers and "
			"their components can be defined here.");

	// Help message for dram configuration
	command_line->RegisterBool("--dram-help",
			help,
			"Print help message describing the network configuration"
			" file, passed in option '--dram-config <file>'.");

	// Stand-alone simulator
	command_line->RegisterBool("--dram-sim",
			stand_alone,
			"Runs a DRAM simulation using the actions provided "
			"in the DRAM configuration file (option "
			"'--dram-config').");

	// FIXME add an option to read the read/write ratio
	// --dram-read-write-ratio

	// FIXME add an option to set the access count
	// --dram-access-count

	// FIXME add an option to set the injection rate
	// --dram-injection-rate

	// FIXME add an option for max cycle
	// --dram-max-cycle
}


void System::ProcessOptions()
{
	// DRAM help
	if (help)
	{
		std::cerr << help_message;
		exit(1);
	}

	// Debugger
	if (!debug_file.empty())
		setDebugPath(debug_file);

	// Activity Debugger
	if (!activity_file.empty())
		setActivityDebugPath(activity_file);

	// Stand-Alone requires config file
	if (stand_alone && config_file.empty())
		throw Error(misc::fmt("Option --dram-sim requires "
				" --dram-config option "));
}


void System::ReadConfiguration()
{
	// Load network configuration file
	if (!config_file.empty())
	{
		// Load and parse the configuration file
		misc::IniFile ini_file(config_file);
		ParseConfiguration(&ini_file);
	}
}


void System::ParseConfiguration(misc::IniFile *ini_file)
{
	// Debug 
	System::debug << ini_file->getPath() << ": Loading DRAM "
		"Configuration file\n";

	// Get the frequency
	frequency = ini_file->ReadInt("General", "Frequency", frequency);
	if (!esim::Engine::isValidFrequency(frequency))
		throw Error(misc::fmt("%s: The value for 'Frequency' "
				"must be between 1MHz and 1000GHz.\n%s",
				ini_file->getPath().c_str(),
				err_config_note));

	// Register frequency domain
	esim::Engine *esim = esim::Engine::getInstance();
	frequency_domain = esim->RegisterFrequencyDomain(
			"frequency_domain", frequency);

	// Create events used by the entire system
	event_command_return = esim->RegisterEvent("command_return",
			Controller::CommandReturnHandler, frequency_domain);

	// Iterate through each section.
	// Parse it if it is a MemoryController section.
	int num_controller = 0;
	for (int i = 0; i < ini_file->getNumSections(); i++)
	{
		std::string section_name = ini_file->getSection(i);
		if (misc::StringPrefix(section_name, "MemoryController"))
		{
			controllers.emplace_back(new Controller(num_controller,
					ini_file, section_name));
			num_controller++;
		}
	}

	// Check for invalid variables
	ini_file->Check();

	// All controllers and all the memory hierarchy under them has been
	// made, so now calculate the sizes of address components.
	GenerateAddressSizes();
}


void System::Run()
{
	// Get the simulation engine.
	esim::Engine *engine = esim::Engine::getInstance();

	// FIXME implement a similar unifrom injection model as presented in 
	// file src/memory/System,cc:StandAlone()
	// Run a simulation with 1000 cycles.

	// 1) Random Number generation setup for the address
	// 2) get the number of events that has to be created
	// 3) get the cycle
	// 4) loop forever
	// 	5) exit only if dram-max-cycle is reached, or all events are processed
	// 	and we reached the dram-access-count
	// 	6) randomly inject an access of type read/write (based on their ratio) and 
	//		based on injection rate itself
	// 7) exit gracefully, like dram-sim or net-sim

	// Get current cycle and check max cycles
	esim::Engine *esim_engine = esim::Engine::getInstance();

	// Loop from the beginning to the end the simulation
	while (witness < 0)
	{
		long long cycle = System::getInstance()->getCycle();
		if (cycle >= max_cycles)
			break;

		// Traverse all nodes to check if some nodes need injection
		for (int i = 0; i < (int) modules.size(); i++)
		{
			// Get the iterator to the module
			auto it = modules.begin();
			std::advance(it, i);

			// Get the module
			Module *module = it->get();

			// If the module is not level 1, continue
			if (module->getLevel() != 1)
				continue;

			// Check turn for next injection
			if (inject_time[i] > cycle)
				continue;

			// Perform the access injection
			while (inject_time[i] < cycle)
			{
				// Schedule next injection
				inject_time[i] += net::System::RandomExponential(
						injection_rate);

				// Find a random address
				unsigned random_address = random_generator(rng);

				// Keep updating the random address until the
				// cache module serves that address
				while (module->ServesAddress(random_address) != true)
					random_address = random_generator(rng);

				// Send the packet
				if (module->canAccess(random_address))
				{
					// Get the type of access based on the
					// ratio
					Module::AccessType type = random() <
						ratio ? Module::AccessStore : 
						Module::AccessLoad;

					// Perform the access
					module->Access(type, random_address,
							&witness);

				}
			}
		}

		// Next cycle
		debug << misc::fmt("___ cycle %lld ___\n", cycle);	
		esim_engine->ProcessEvents();
	}

	// Lets finish all off
	esim_engine->ProcessAllEvents();

	// Here finish the esim
	esim_engine->Finish("MaxTime");

}


int System::getNextCommandId()
{
	next_command_id++;
	return next_command_id;
}


void System::AddRequest(std::shared_ptr<Request> request)
{
	// Decode the address and move the request to the correct controller.
	Address *address = request->getAddress();
	controllers[address->getPhysical()]->AddRequest(request);

	// Debug
	debug << misc::fmt("[%lld] Adding request for 0x%llx to controller %d\n",
			frequency_domain->getCycle(), address->getEncoded(),
			address->getPhysical());
}


int System::Log2(unsigned num)
{
	// Find the index of the highest set bit of num by shifting num to the
	// right repeatedly until it equals 0.
	int result = 0;
	while (num >>= 1)
		result++;
	return result;
}


void System::GenerateAddressSizes()
{
	// Set physical size based on number of controllers in the system.
	physical_size = Log2(controllers.size());

	// Find the largest of each component size because one address format
	// must be able to decode to any location in the system.
	// Controllers can potentially each have different sizes for everything
	// but all sizes under the controller are uniform.
	for (auto const &controller : controllers)
	{
		logical_size = std::max(logical_size,
				Log2(controller->getNumChannels()));
		rank_size = std::max(rank_size,
				Log2(controller->getNumRanks()));
		bank_size = std::max(bank_size,
				Log2(controller->getNumBanks()));
		row_size = std::max(row_size,
				Log2(controller->getNumRows()));
		column_size = std::max(column_size,
				Log2(controller->getNumColumns()));
	}
}


void System::Read(long long address)
{
	if (address < 0 || address > getCapacity())
		throw misc::Error("Invalid Address");

	// Create the request object that will be inserted into the queue.
	std::shared_ptr<Request> request = std::make_shared<Request>();

	// Set the request's address
	request->setEncodedAddress(address);

	// Set the request's type.
	request->setType(RequestRead);

	// Add request to the system
	System *dram = System::getInstance();
	dram->AddRequest(request);
}


void System::Write(long long address)
{
	if (address < 0 || address > getCapacity())
		throw misc::Error("Invalid Address");

	// Create the request object that will be inserted into the queue.
	std::shared_ptr<Request> request = std::make_shared<Request>();

	// Set the request's address
	request->setEncodedAddress(address);

	// Set the request's type.
	request->setType(RequestWrite);

	// Add request to the system
	System *dram = System::getInstance();
	dram->AddRequest(request);
}


void System::Dump(std::ostream &os) const
{
	
	// Print header
	os << "\n\n--------------------\n\n";
	os << "Dumping DRAM system\n";

	// Print controllers in the system
	os << misc::fmt("%d Controllers\nController dump:\n",
			(int) controllers.size());
	for (auto &controller : controllers)
		controller->dump(os);
}


}  //namespace dram
