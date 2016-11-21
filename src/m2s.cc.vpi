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
 * -----------------------------------------------------------------------------
 *  This file has been edited by Nicolas Bohm Agostini (n.b.agostini@gmail.com)
 *  with the intention of using multi2sim memory module in HDL simulations
 *
 */

#include "m2s.h"
#include <sys/time.h> // timeOfTheDay()
#include <lib/cpp/Terminal.h> // BlueTerm


#include <lib/cpp/IniFile.h>
#include <lib/esim/Engine.h>	//esim::
#include <memory/System.h>	//mem::
#include <network/System.h>	//net::
#include <arch/common/Arch.h>	//com::


#include <arch/x86/timing/Timing.h>
#include <arch/common/Arch.h>
#include <lib/cpp/IniFile.h>
#include <lib/cpp/Error.h>
#include <lib/esim/Engine.h>
#include <memory/System.h>
#include <network/System.h>

void Multi2Sim::WelcomeMessage(std::ostream &os)
{
    std::cout<<"M2S::WelcomeMessage Function"<<std::endl;

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

void Multi2Sim::m2sStep()
{
    std::cout<<"M2S::vpiStep()"<<std::endl;
}

void Multi2Sim::m2sInitialize()
{
    std::cout<<"M2S::vpiInitialize()"<<std::endl;
    WelcomeMessage(std::cerr);

    // Cleanup singleton instances
    Cleanup();

    // Load configuration files
    misc::IniFile ini_file_mem_1;
    misc::IniFile ini_file_x86;
    misc::IniFile ini_file_net_1;
    ini_file_mem_1.LoadFromString(mem_config_1);
    ini_file_x86.LoadFromString(x86_config);
    ini_file_net_1.LoadFromString(net_config_1);

    // Set up x86 timing simulator
    x86::Timing::ParseConfiguration(&ini_file_x86);
    x86::Timing::getInstance();

    // Set up network system
    net::System *network_system = net::System::getInstance();
    network_system->ParseConfiguration(&ini_file_net_1);

    // Set up memory system
    mem::System *memory_system = mem::System::getInstance();
    memory_system->ReadConfiguration(&ini_file_mem_1);

    // Get modules
//    mem::Module *module_l1_0 = memory_system->getModule("mod-l1-0");
//    mem::Module *module_l1_1 = memory_system->getModule("mod-l1-1");
//    mem::Module *module_l2_0 = memory_system->getModule("mod-l2-0");
//    mem::Module *module_l3 = memory_system->getModule("mod-l3");
//    mem::Module *module_mm = memory_system->getModule("mod-mm");
//    if(module_l1_0 == nullptr)
//	 std::cout<<"M2S::nullptr found!!!!"<<std::endl;
//    if(module_l1_1 == nullptr)
//	 std::cout<<"M2S::nullptr found!!!!"<<std::endl;
//    if(module_l2_0 == nullptr)
//	 std::cout<<"M2S::nullptr found!!!!"<<std::endl;
//    if(module_l3 == nullptr)
//	 std::cout<<"M2S::nullptr found!!!!"<<std::endl;
//    if(module_mm == nullptr)
//	 std::cout<<"M2S::nullptr found!!!!"<<std::endl;

//    //Set Block States
//    module_l1_0->getCache()->getBlock(0, 0)->setStateTag(mem::Cache::BlockExclusive, 0x0);
//    module_l2_0->getCache()->getBlock(0, 0)->setStateTag(mem::Cache::BlockExclusive, 0x0);
//    module_l3->getCache()->getBlock(0, 0)->setStateTag(mem::Cache::BlockExclusive, 0x0);
//    module_mm->getCache()->getBlock(0, 0)->setStateTag(mem::Cache::BlockExclusive, 0x0);
//    module_l2_0->setOwner(0, 0, 0, module_l1_0);
//    module_l3->setOwner(0, 0, 0, module_l2_0);
//    module_mm->setOwner(0, 0, 0, module_l3);
//    module_l2_0->setSharer(0, 0, 0, module_l1_0);
//    module_l3->setSharer(0, 0, 0, module_l2_0);
//    module_mm->setSharer(0, 0, 0, module_l3);

//    // EDIT HERE
//    // Accesses
//    int witness = -2;
//    module_l1_0->Access(mem::Module::AccessStore, 0x0, &witness);
//    module_l1_1->Access(mem::Module::AccessLoad, 0x200, &witness);

//    // Simulation loop
//    esim::Engine *esim_engine = esim::Engine::getInstance();
//    while (witness < 0)
//	    esim_engine->ProcessEvents();

//    // Check block
//    unsigned tag;
//    mem::Cache::BlockState state;
//    module_l1_0->getCache()->getBlock(0, 0, tag, state);
//    if(tag != 0x0)
//	std::cout<<"M2S::bad tag 1!!!!"<<std::endl;
//    if(state != mem::Cache::BlockInvalid)
//	std::cout<<"M2S::bad state 1!!!!"<<std::endl;

//    // Check block
//    module_l1_1->getCache()->getBlock(0, 0, tag, state);
//    if(tag != 0x200)
//	std::cout<<"M2S::bad tag 2!!!!"<<std::endl;
//    if(state != mem::Cache::BlockExclusive)
//	std::cout<<"M2S::bad state 2!!!!"<<std::endl;

//    // Check block
//    module_l2_0->getCache()->getBlock(0, 0, tag, state);
//    if(tag != 0x200)
//	std::cout<<"M2S::bad tag 3!!!!"<<std::endl;
//    if(state != mem::Cache::BlockExclusive)
//	std::cout<<"M2S::bad state 3!!!!"<<std::endl;

//    // Check block
//    module_l3->getCache()->getBlock(0, 0, tag, state);
//    if(tag != 0x0)
//	std::cout<<"M2S::bad tag 4!!!!"<<std::endl;
//    if(state != mem::Cache::BlockModified)
//	std::cout<<"M2S::bad state 4!!!!"<<std::endl;

//    // Check block
//    module_l3->getCache()->getBlock(8, 0, tag, state);
//    if(tag != 0x200)
//	std::cout<<"M2S::bad tag 5!!!!"<<std::endl;
//    if(state != mem::Cache::BlockExclusive)
//	std::cout<<"M2S::bad state 5!!!!"<<std::endl;

//    // Check block
//    module_mm->getCache()->getBlock(0, 0, tag, state);
//    if(tag != 0x0)
//	std::cout<<"M2S::bad tag 6!!!!"<<std::endl;
//    if(state != mem::Cache::BlockExclusive)
//	std::cout<<"M2S::bad state 6!!!!"<<std::endl;

//    // Check block
//    module_mm->getCache()->getBlock(8, 0, tag, state);
//    if(tag != 0x200)
//	std::cout<<"M2S::bad tag 7!!!!"<<std::endl;
//    if(state != mem::Cache::BlockExclusive)
//	std::cout<<"M2S::bad state 7!!!!"<<std::endl;

    std::cout<<"M2S::HERE!!!!"<<std::endl;
}

void Multi2Sim::m2sReset()
{
    std::cout<<"M2S::vpiReset()"<<std::endl;
}

void Multi2Sim::m2sFinalize()
{
    std::cout<<"M2S::vpiFinalize()"<<std::endl;
}

void Multi2Sim::m2sAccess(const unsigned int &mod
			 , const unsigned int &type
			 , const unsigned int &address)
{
    std::cout<<"M2S::vpiAccess()"		<<std::endl
	     <<"\tmod = "	    << mod	<<std::endl
	     <<"\ttype = "    << type	<<std::endl
	     <<"\taddress = " << address	<<std::endl;
}


void Multi2Sim::LoadProgram(const std::vector<std::string> &arguments
			   , const std::vector<std::string> &environment
			   , const std::string &current_directory
			   , const std::string &stdin_file_name
			   , const std::string &stdout_file_name)
{
}

void Multi2Sim::LoadPrograms()
{
}

void Multi2Sim::RegisterOptions()
{
}

void Multi2Sim::ProcessOptions()
{
}

void Multi2Sim::RegisterRuntimes()
{
}

void Multi2Sim::RegisterDrivers()
{
}

void Multi2Sim::MainLoop()
{
}

void Multi2Sim::DumpStatisticsSummary(std::ostream &os)
{
}

void Multi2Sim::DumpReports()
{
}

void Multi2Sim::Cleanup()
{
    esim::Engine::Destroy();

    net::System::Destroy();

    mem::System::Destroy();

    comm::ArchPool::Destroy();
}

int Multi2Sim::MainProgram(int argc, char **argv)
{
    // Initialize multi2sim with the proper arguments
    m2sInitialize();

    m2sAccess(0, 0, 10);
    m2sStep();

    m2sAccess(0, 0, 12);
    m2sStep();
    m2sAccess(0, 0, 14);
    m2sStep();

    return 0;
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
