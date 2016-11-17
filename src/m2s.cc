/*
 *  Multi2Sim
 *  Copyright (C) 2013  Rafael Ubal (ubal@ece.neu.edu)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include "m2s.h"

#include <cstdlib>
#include <iostream>
#include <sys/time.h>

#include <dram/System.h>
#include <memory/Mmu.h>
#include <memory/Manager.h>
#include <memory/System.h>
#include <network/System.h>
#include <lib/cpp/CommandLine.h>
#include <lib/cpp/IniFile.h>
#include <lib/cpp/Misc.h>
#include <lib/cpp/Terminal.h>
#include <lib/esim/Engine.h>
#include <lib/esim/Trace.h>



extern "C"
{
#include <visual/common/visual.h>
}
//
// Configuration options
//

// Context configuration file
std::string m2s_context_config;

// Debug information in CUDA runtime
std::string m2s_cuda_debug;

// Event-driven simulator debugger
std::string m2s_debug_esim;

// Inifile debugger
std::string m2s_debug_inifile;

// Call stack debugger
std::string m2s_debug_callstack;

// Maximum simulation time
long long m2s_max_time = 0;

// Binary file for OpenCL runtime
std::string m2s_opencl_binary;

// Debug information in OpenCL runtime
std::string m2s_opencl_debug;

// List of OpenCL devices for runtime
std::string m2s_opencl_devices;

// Trace file
std::string m2s_trace_file;

// Visualization tool input file
std::string m2s_visual_file;



//
// Global variables
//

// Number of iterations in the main simulation loop
long long m2s_loop_iterations = 0;


unsigned access_identifier=0;




//
// Functions
//


void Multi2Sim::WelcomeMessage(std::ostream &os)
{
	// Compute simulation ID
	struct timeval tv;
	gettimeofday(&tv, NULL);
	unsigned min_id = misc::StringAlnumToInt("10000");
	unsigned max_id = misc::StringAlnumToInt("ZZZZZ");
	unsigned id = (tv.tv_sec * 1000000000 + tv.tv_usec)
			% (max_id - min_id + 1)
			+ min_id;
	std::string alnum_id = misc::StringIntToAlnum(id);
	
	// Blue color
	misc::Terminal::Blue(os);

	// Print welcome message in the standard error output
	os << '\n' << "; Multi2Sim " << VERSION << " - ";
	os << "A Simulation Framework for CPU-GPU Heterogeneous Computing\n";
	os << "; Please use command 'm2s --help' for a list of command-line options.\n";
	os << "; Simulation alpha-numeric ID: " << alnum_id << '\n';
	os << '\n';
	
	// Reset terminal color
	misc::Terminal::Reset(os);
}


void Multi2Sim::RegisterOptions()
{
	// Set error message
	misc::CommandLine *command_line = misc::CommandLine::getInstance();
	command_line->setErrorMessage("\nPlease type 'm2s --help' for a list of "
			"valid Multi2Sim command-line options.");

	// Set help message
	command_line->setHelp("Syntax:"
			"\n\n"
			"$ m2s [<options>] [<exe>] [<args>]"
			"\n\n"
			"Multi2Sim's command line can take a program "
			"executable <exe> as an argument, given as a binary "
			"file in any of the supported CPU architectures, and "
			"optionally followed by its arguments <args>. The "
			"following list of command-line options can be used "
			"for <options>:");
	

	//
	// General Multi2Sim Options
	//
	
	// Set category for following options
	command_line->setCategory("default", "General Multi2Sim Options");

	// Debugger for event-driven simulator
	command_line->RegisterString("--esim-debug <file>",
			m2s_debug_esim,
			"Dump debug information related with the event-driven "
			"simulation engine.");
	
	// Debugger for Inifile parser
	command_line->RegisterString("--inifile-debug <file>",
			m2s_debug_inifile,
			"Dump debug information about all processed INI files "
			"into the specified path.");
	
	// Maximum simulation time
	command_line->RegisterInt64("--max-time <time> (default = 0)",
			m2s_max_time,
			"Maximum simulation time in seconds. The simulator "
			"will stop once this time is exceeded. A value of 0 "
			"(default) means no time limit.");
	
	// Trace file
	command_line->RegisterString("--trace <file>",
			m2s_trace_file,
			"Generate a trace file with debug information on the "
			"configuration of the modeled CPUs, GPUs, and memory "
			"system, as well as their dynamic simulation. The "
			"trace is a compressed plain-text file in format. The "
			"user should watch the size of the generated trace as "
			"simulation runs, since the trace file can quickly "
			"become extremely large.");
	
	// Visualization tool input file
	command_line->RegisterString("--visual <file>",
			m2s_visual_file,
			"Run the Multi2Sim Visualization Tool. This option "
			"consumes a file generated with the '--trace' option "
			"in a previous simulation. This option is only "
			"available on systems with support for GTK 3.0 or "
			"higher.");




        // Register the remaining options
        mem::Mmu::RegisterOptions();
        mem::Manager::RegisterOptions();
        mem::System::RegisterOptions();
        dram::System::RegisterOptions();
        net::System::RegisterOptions();
}


void Multi2Sim::ProcessOptions()
{
	// Event-driven simulator debugger
	if (!m2s_debug_esim.empty())
		esim::Engine::setDebugPath(m2s_debug_esim);

	// Inifile debugger
	if (!m2s_debug_inifile.empty())
		misc::IniFile::setDebugPath(m2s_debug_inifile);

	// Trace file
	if (!m2s_trace_file.empty())
	{
		esim::TraceSystem *trace_system = esim::TraceSystem::getInstance();
		trace_system->setPath(m2s_trace_file);
	}

	// Visualization
	if (!m2s_visual_file.empty())
		visual_run(m2s_visual_file.c_str());

        // Process the remaining options
        mem::Mmu::ProcessOptions();
        mem::Manager::ProcessOptions();
        mem::System::ProcessOptions();
        dram::System::ProcessOptions();
        net::System::ProcessOptions();
		
}


void Multi2Sim::MainLoop()
{
// This part has to move to INIT too
	// Activate signal handler
	esim::Engine *esim = esim::Engine::getInstance();
	esim->EnableSignals();

	// Simulation loop
	while (!esim->hasFinished())
	{
		m2s_loop_iterations++;
		if (m2s_max_time > 0
				&& !(m2s_loop_iterations & ((1 << 17) - 1))
				&& esim->getRealTime() > m2s_max_time * 1000000)
			esim->Finish("MaxTime");
	}

	// Process all remaining events
	esim->ProcessAllEvents();

	// Restore default signal handlers
	esim->DisableSignals();
}


void Multi2Sim::DumpStatisticsSummary(std::ostream &os = std::cerr)
{
        // No summary dumped if no simulation was run
        if (m2s_loop_iterations < 2)
                return;

        // Print in blue
        misc::Terminal::Blue(os);

        // Header
        os << '\n' << ";\n"
                        << "; Simulation Statistics Summary\n"
                        << ";\n"
                        << "\n";

        // Calculate real time in seconds
        esim::Engine *esim_engine = esim::Engine::getInstance();
        double time_in_seconds = (double) esim_engine->getRealTime() / 1.0e6;

        // General statistics
        os << "[ General ]\n";
        os << misc::fmt("RealTime = %.2f [s]\n", time_in_seconds);
        os << "SimEnd = " << esim_engine->getFinishReason() << '\n';

        // General detailed simulation statistics
        if (esim_engine->getTime())
        {
                long long cycles = esim_engine->getCycle();
                os << misc::fmt("SimTime = %.2f [ns]\n", esim_engine->getTime() / 1000.0);
                os << misc::fmt("Frequency = %d [MHz]\n", esim_engine->getFrequency());
                os << misc::fmt("Cycles = %lld\n", cycles);
        }

        // End
        os << '\n';

        // Reset terminal color
        misc::Terminal::Reset(os);
}

void Multi2Sim::DumpReports()
{
	// Dumping memory report
	if (mem::System::hasInstance())
	{
		mem::System *mem_system = mem::System::getInstance();

		// Dump the memory report
		mem_system->DumpReport();
	}

	// Dumping network report
	if (net::System::hasInstance())
	{
		// Dump the network report
		net::System *net_system = net::System::getInstance();
		net_system->DumpReport();

		// Dump the network data for static visualization
		net_system->StaticGraph();

		// Dump the network routing table
		net_system->DumpRoutes();
        }
}

int Multi2Sim::checkProccessedEvents()
{
    total_witness=0; // Redundant way to check witness
    std::map<int, a_access>::iterator it=accesses_list.begin();
    while ( it!=accesses_list.end())
    {
        if(*it->second.access_witness==0)
        {
            // FIXME: Has to change VPI objects or pass information
            std::cout << "Access id: "<<it->first
                      << " to address " << it->second.access_address
                      << " finished accessing"<< '\n';

            free(it->second.access_witness);
            accesses_list.erase(it++); // Erase and increment iterator to check next access
        }
        else
        {
            // Redundant way to check witness
            total_witness = total_witness + *it->second.access_witness;
            ++it;
        }
    }
    // Success
    return 0;
}


int Multi2Sim::MainProgram(int argc, char **argv)
{
	// Print welcome message in standard error output
	WelcomeMessage(std::cerr);

	// Read command line
	RegisterOptions();

	// Process command line. Return to C version of Multi2Sim if a
	// command-line option was not recognized.
	misc::CommandLine *command_line = misc::CommandLine::getInstance();
	command_line->Process(argc, argv, false);
	
	// Process command line
        ProcessOptions();

	// Initialize memory system, only when '--mem-sim' command-line 
	// options have been processed.
	mem::System *memory_system = mem::System::getInstance();
	if (mem::System::isStandAlone())
	{
		// We need to load the network configuration file prior to
		// parsing memory config file. The memory config file searches
		// for the external network if it cannot find the network
		// in the memory configuration file sections.
		net::System *net_system = net::System::getInstance();
		net_system->ReadConfiguration();

		// Parse the memory configuration file
		memory_system->ReadConfiguration();

		// Memory stand alone
                if (memory_system->isRandomInjectionMode())
                {
                        memory_system->RandomInjectionRun();
                }
                else if (memory_system->isUsingTraceFile())
                {
                        memory_system->TraceFileRun();
                }
	}

	// Initialize network system, only if the option --net-sim is used
	if (net::System::isStandAlone())
	{
		net::System *net_system = net::System::getInstance();
		net_system->ReadConfiguration();
                net_system->StandAlone();
	}

	// Initialize dram system, only if the option --dram-sim is used
	if (dram::System::isStandAlone())
	{
		dram::System *dram_system = dram::System::getInstance();
		dram_system->ReadConfiguration();
		dram_system->Run();
	}
	
	// Main simulation loop
	MainLoop();

	// FIXME Statistics summary
	// This is the *other* part of Finalize
        DumpStatisticsSummary();

	// Reports
	DumpReports();

	// Success
	return 0;
}


// Only called by the VPI
void Multi2Sim::m2sInitialize(char input_arguments[])
{
    // Print welcome message in standard error output
    WelcomeMessage(std::cerr);

    // Read command line
    RegisterOptions();
//    mem::Mmu::RegisterOptions();
//    mem::Manager::RegisterOptions();
//    mem::System::RegisterOptions();
//    dram::System::RegisterOptions();
//    net::System::RegisterOptions();

    // FIXME must read from input arguments
    char mystring [] ="m2s --mem-debug debug-info.txt  --trace trace-info.gz --mem-report report-info.txt --mem-sim --mem-config mem-config";

    // Create the arrays to parse VPI input arguments
    int* my_argc = (int*) malloc(sizeof(int));
    char**my_argv = (char**)(malloc(strlen(mystring)*sizeof(char)));
    getVpiArguments(my_argc, my_argv, mystring);

    // Process command line. Return to C version of Multi2Sim if a
    // command-line option was not recognized.
    misc::CommandLine *command_line = misc::CommandLine::getInstance();
    command_line->Process(*my_argc, my_argv, false);

    // Process command line
    ProcessOptions();
//    mem::Mmu::ProcessOptions();
//    mem::Manager::ProcessOptions();
//    mem::System::ProcessOptions();
//    dram::System::ProcessOptions();
//    net::System::ProcessOptions();

    // Initialize memory system, only when '--mem-sim' command-line
    // options have been processed.
    mem::System *memory_system = mem::System::getInstance();
    if (1)
    {
            // We need to load the network configuration file prior to
            // parsing memory config file. The memory config file searches
            // for the external network if it cannot find the network
            // in the memory configuration file sections.
            net::System *net_system = net::System::getInstance();
            net_system->ReadConfiguration();

            // Parse the memory configuration file
            memory_system->ReadConfiguration();

            // Memory stand alone
            //memory_system->StandAlone();
    }

    // Initialize network system, only if the option --net-sim is used
    if (1)
    {
            net::System *net_system = net::System::getInstance();
            net_system->ReadConfiguration();
            //net_system->StandAlone();
    }

    // Initialize dram system, only if the option --dram-sim is used
    if (dram::System::isStandAlone())
    {
            dram::System *dram_system = dram::System::getInstance();
            dram_system->ReadConfiguration();
            dram_system->Run();
    }

    free(my_argc);
    free(my_argv);

}

// Only called by the VPI
void Multi2Sim::m2sReset()
{
    std::cout<<"M2S::vpiReset()"<<std::endl;
}

// Only called by the VPI
void Multi2Sim::m2sFinalize()
{
    std::cout<<"M2S::vpiFinalize()"<<std::endl;
    esim::Engine *esim_engine = esim::Engine::getInstance();
    // Lets finish all off
    esim_engine->ProcessAllEvents();
    // Here finish the esim
    esim_engine->Finish("MaxTime");

    // This is the *other* part of Finalize
    //DumpStatisticsSummary();

    // Reports
    DumpReports();

}

// Only called by the VPI
void Multi2Sim::m2sAccess(const unsigned int &mod
                         , const unsigned int &type
                         , const unsigned int &address)
{
    std::cout<<"M2S::vpiAccess()"		<<std::endl
             <<"\tmod = "	    << mod	<<std::endl
             <<"\ttype = "    << type	<<std::endl
             <<"\taddress = " << address	<<std::endl;

    // mod-l1-0
    // Get the right module
    mem::System *mem_system = mem::System::getInstance();
    std::cout<<"Current cycle: "<<mem_system->getCycle()<<std::endl;

    mem::Module *module = mem_system->getModule("mod-l1-0");
     if(module == NULL)
     {
         std::cout<<"NUll pointer"<<std::endl;
     }
     else
     {
         // Send the packet
         if (module->canAccess(address))
         {
             // Get the type of access based on the number
             mem::Module::AccessType the_type = mem::Module::AccessInvalid;
             switch (type) {
             case 0:
                 the_type = mem::Module::AccessInvalid;
                 break;
             case 1:
                 the_type = mem::Module::AccessLoad;
                 break;
             case 2:
                 the_type = mem::Module::AccessStore;
                 break;
             case 3:
                 the_type = mem::Module::AccessNCStore;
                 break;
             default:
                 the_type = mem::Module::AccessInvalid;
                 break;
             }

             // new witness
             int *current_witness = (int *)malloc(sizeof(int));

             *current_witness = -1;
             // Insert to access_map_list
             mem::a_access current_access = {
                 access_identifier,
                 address,
                 the_type,
                 "test",
                 current_witness
             };

             mem_system->accesses_list.emplace(access_identifier,current_access);


             // Perform the access
             module->Access(mem_system->accesses_list.at(access_identifier).access_type
                            ,mem_system->accesses_list.at(access_identifier).access_address
                            ,mem_system->accesses_list.at(access_identifier).access_witness);
             ++access_identifier;

         }

     }
}

void Multi2Sim::m2sStep()
{
    std::cout<<"M2S::vpiStep()"<<std::endl;

    // Get current cycle and check max cycles
    mem::System *mem_system = mem::System::getInstance();
    std::cout<<"___ cycle "<<mem_system->getCycle()<<" ___"<<std::endl;

    // Proccess events
    esim::Engine *esim_engine = esim::Engine::getInstance();
    esim_engine->ProcessEvents();

    // Give feedback to VPI interface
    checkProccessedEvents();
}


void Multi2Sim::LoadProgram(const std::vector<std::string> &arguments
                           , const std::vector<std::string> &environment
                           , const std::string &current_directory
                           , const std::string &stdin_file_name
                           , const std::string &stdout_file_name)
{

}


int main(int argc, char **argv)
{
	// Main exception handler
	try
	{
		// Run main program
                return Multi2Sim::getInstance().MainProgram(argc, argv);
	}
	catch (misc::Exception &e)
	{
		e.Dump();
		return 1;
	}
}

