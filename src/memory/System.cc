/*
 *  Multi2Sim
 *  Copyright (C) 2012  Rafael Ubal (ubal@ece.neu.edu)
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

#include <fstream>
#include <cmath>
#include <random>

#include <lib/cpp/CommandLine.h>
#include <lib/cpp/Misc.h>
#include <lib/esim/Engine.h>
#include <lib/esim/Event.h>
#include <lib/esim/FrequencyDomain.h>

#include "System.h"


namespace mem
{


// Configuration options
std::string System::config_file;
std::string System::debug_file;
std::string System::report_file;
bool System::help = false;

int System::frequency = 1000;
long long System::max_cycles = 100000;
long long System::access_counts = 200;
double System::injection_rate = 0.001;
double System::ratio = 0.2;

long long System::sanity_check_interval = 0;
long long System::last_sanity_check = 0;

esim::Trace System::trace;

misc::Debug System::debug;

std::unique_ptr<System> System::instance;


bool System::sim_mem_vpi = false;
bool System::sim_mem_stand_alone = false;
unsigned int System::access_identifier = 0;
unsigned int System::total_witness =0;
std::map<int, a_access> System::accesses_list;
bool System::sim_mem_stand_alone_random = false;
std::string System::input_memory_trace_file;

System *System::getInstance()
{
	// Instance already exists
	if (instance.get())
		return instance.get();

	// Create instance
	instance = misc::new_unique<System>();
        return instance.get();
}


System::System()
{
	//
	// NMOESI events
	//

	// Create frequency domain
	esim::Engine *esim_engine = esim::Engine::getInstance();
	frequency_domain = esim_engine->RegisterFrequencyDomain("Memory");

	event_load = esim_engine->RegisterEvent("load",
			EventLoadHandler,
			frequency_domain);
	event_load_lock = esim_engine->RegisterEvent("load_lock",
			EventLoadHandler,
			frequency_domain);
	event_load_action = esim_engine->RegisterEvent("load_action",
			EventLoadHandler,
			frequency_domain);
	event_load_miss = esim_engine->RegisterEvent("load_miss",
			EventLoadHandler,
			frequency_domain);
	event_load_unlock = esim_engine->RegisterEvent("load_unlock",
			EventLoadHandler,
			frequency_domain);
	event_load_finish = esim_engine->RegisterEvent("load_finish",
			EventLoadHandler,
			frequency_domain);

	event_store = esim_engine->RegisterEvent("store",
			EventStoreHandler,
			frequency_domain);
	event_store_lock = esim_engine->RegisterEvent("store_lock",
			EventStoreHandler,
			frequency_domain);
	event_store_action = esim_engine->RegisterEvent("store_action",
			EventStoreHandler,
			frequency_domain);
	event_store_unlock = esim_engine->RegisterEvent("store_unlock",
			EventStoreHandler,
			frequency_domain);
	event_store_finish = esim_engine->RegisterEvent("store_finish",
			EventStoreHandler,
			frequency_domain);

	event_nc_store = esim_engine->RegisterEvent("nc_store",
			EventNCStoreHandler,
			frequency_domain);
	event_nc_store_lock = esim_engine->RegisterEvent("nc_store_lock",
			EventNCStoreHandler,
			frequency_domain);
	event_nc_store_writeback = esim_engine->RegisterEvent("nc_store_writeback",
			EventNCStoreHandler,
			frequency_domain);
	event_nc_store_action = esim_engine->RegisterEvent("nc_store_action",
			EventNCStoreHandler,
			frequency_domain);
	event_nc_store_miss= esim_engine->RegisterEvent("nc_store_miss",
			EventNCStoreHandler,
			frequency_domain);
	event_nc_store_unlock = esim_engine->RegisterEvent("nc_store_unlock",
			EventNCStoreHandler,
			frequency_domain);
	event_nc_store_finish = esim_engine->RegisterEvent("nc_store_finish",
			EventNCStoreHandler,
			frequency_domain);

	event_find_and_lock = esim_engine->RegisterEvent("find_and_lock",
			EventFindAndLockHandler,
			frequency_domain);
	event_find_and_lock_port = esim_engine->RegisterEvent("find_and_lock_port",
			EventFindAndLockHandler,
			frequency_domain);
	event_find_and_lock_action = esim_engine->RegisterEvent("find_and_lock_action",
			EventFindAndLockHandler,
			frequency_domain);
	event_find_and_lock_finish = esim_engine->RegisterEvent("find_and_lock_finish",
			EventFindAndLockHandler,
			frequency_domain);

	event_evict = esim_engine->RegisterEvent("evict",
			EventEvictHandler,
			frequency_domain);
	event_evict_invalid = esim_engine->RegisterEvent("evict_invalid",
			EventEvictHandler,
			frequency_domain);
	event_evict_action = esim_engine->RegisterEvent("evict_action",
			EventEvictHandler,
			frequency_domain);
	event_evict_receive = esim_engine->RegisterEvent("evict_receive",
			EventEvictHandler,
			frequency_domain);
	event_evict_process = esim_engine->RegisterEvent("evict_process",
			EventEvictHandler,
			frequency_domain);
	event_evict_process_noncoherent = esim_engine->RegisterEvent("evict_process_noncoherent",
			EventEvictHandler,
			frequency_domain);
	event_evict_reply = esim_engine->RegisterEvent("evict_reply",
			EventEvictHandler,
			frequency_domain);
	event_evict_reply = esim_engine->RegisterEvent("evict_reply",
			EventEvictHandler,
			frequency_domain);
	event_evict_reply_receive = esim_engine->RegisterEvent("evict_reply_receive",
			EventEvictHandler,
			frequency_domain);
	event_evict_finish = esim_engine->RegisterEvent("evict_finish",
			EventEvictHandler,
			frequency_domain);

	event_write_request = esim_engine->RegisterEvent("write_request",
			EventWriteRequestHandler,
			frequency_domain);
	event_write_request_receive = esim_engine->RegisterEvent("write_request_receive",
			EventWriteRequestHandler,
			frequency_domain);
	event_write_request_action = esim_engine->RegisterEvent("write_request_action",
			EventWriteRequestHandler,
			frequency_domain);
	event_write_request_exclusive = esim_engine->RegisterEvent("write_request_exclusive",
			EventWriteRequestHandler,
			frequency_domain);
	event_write_request_updown = esim_engine->RegisterEvent("write_request_updown",
			EventWriteRequestHandler,
			frequency_domain);
	event_write_request_updown_finish = esim_engine->RegisterEvent("write_request_updown_finish",
			EventWriteRequestHandler,
			frequency_domain);
	event_write_request_downup = esim_engine->RegisterEvent("write_request_downup",
			EventWriteRequestHandler,
			frequency_domain);
	event_write_request_downup_finish = esim_engine->RegisterEvent("write_request_downup_finish",
			EventWriteRequestHandler,
			frequency_domain);
	event_write_request_reply = esim_engine->RegisterEvent("write_request_reply",
			EventWriteRequestHandler,
			frequency_domain);
	event_write_request_finish = esim_engine->RegisterEvent("write_request_finish",
			EventWriteRequestHandler,
			frequency_domain);

	event_read_request = esim_engine->RegisterEvent("read_request",
			EventReadRequestHandler,
			frequency_domain);
	event_read_request_receive = esim_engine->RegisterEvent("read_request_receive",
			EventReadRequestHandler,
			frequency_domain);
	event_read_request_action = esim_engine->RegisterEvent("read_request_action",
			EventReadRequestHandler,
			frequency_domain);
	event_read_request_updown = esim_engine->RegisterEvent("read_request_updown",
			EventReadRequestHandler,
			frequency_domain);
	event_read_request_updown_miss = esim_engine->RegisterEvent("read_request_updown_miss",
			EventReadRequestHandler,
			frequency_domain);
	event_read_request_updown_finish = esim_engine->RegisterEvent("read_request_updown_finish",
			EventReadRequestHandler,
			frequency_domain);
	event_read_request_downup = esim_engine->RegisterEvent("read_request_downup",
			EventReadRequestHandler,
			frequency_domain);
	event_read_request_downup_finish = esim_engine->RegisterEvent("read_request_downup_finish",
			EventReadRequestHandler,
			frequency_domain);
	event_read_request_reply = esim_engine->RegisterEvent("read_request_reply",
			EventReadRequestHandler,
			frequency_domain);
	event_read_request_finish = esim_engine->RegisterEvent("read_request_finish",
			EventReadRequestHandler,
			frequency_domain);

	event_invalidate = esim_engine->RegisterEvent("invalidate",
			EventInvalidateHandler,
			frequency_domain);
	event_invalidate_finish = esim_engine->RegisterEvent("invalidate_finish",
			EventInvalidateHandler,
			frequency_domain);

	event_message = esim_engine->RegisterEvent("message",
			EventMessageHandler,
			frequency_domain);
	event_message_receive = esim_engine->RegisterEvent("message_receive",
			EventMessageHandler,
			frequency_domain);
	event_message_action = esim_engine->RegisterEvent("message_action",
			EventMessageHandler,
			frequency_domain);
	event_message_reply = esim_engine->RegisterEvent("message_reply",
			EventMessageHandler,
			frequency_domain);
	event_message_finish = esim_engine->RegisterEvent("message_finish",
			EventMessageHandler,
			frequency_domain);

	event_flush = esim_engine->RegisterEvent("flush",
			EventFlushHandler,
			frequency_domain);
	event_flush_finish = esim_engine->RegisterEvent("flush_finish",
			EventFlushHandler,
			frequency_domain);

	// Local memory event driven simulation

	event_local_load = esim_engine->RegisterEvent("local_load",
			EventLocalLoadHandler,
			frequency_domain);
	event_local_load_lock = esim_engine->RegisterEvent("local_load_lock",
			EventLocalLoadHandler,
			frequency_domain);
	event_local_load_finish = esim_engine->RegisterEvent("local_load_finish",
			EventLocalLoadHandler,
			frequency_domain);

	event_local_store = esim_engine->RegisterEvent("local_store",
			EventLocalStoreHandler,
			frequency_domain);
	event_local_store_lock = esim_engine->RegisterEvent("local_store_lock",
			EventLocalStoreHandler,
			frequency_domain);
	event_local_store_finish = esim_engine->RegisterEvent("local_store_finish",
			EventLocalStoreHandler,
			frequency_domain);

	event_local_find_and_lock = esim_engine->RegisterEvent("local_find_and_lock",
			EventLocalFindAndLockHandler,
			frequency_domain);
	event_local_find_and_lock_port = esim_engine->RegisterEvent("local_find_and_lock_port",
			EventLocalFindAndLockHandler,
			frequency_domain);
	event_local_find_and_lock_action = esim_engine->RegisterEvent("local_find_and_lock_action",
			EventLocalFindAndLockHandler,
			frequency_domain);
	event_local_find_and_lock_finish = esim_engine->RegisterEvent("local_find_and_lock_finish",
			EventLocalFindAndLockHandler,
			frequency_domain);
}


void System::RegisterOptions()
{
	// Get command line object
	misc::CommandLine *command_line = misc::CommandLine::getInstance();

	// Category
	command_line->setCategory("Memory System");


        // VPI simulator
        command_line->RegisterBool("--mem-sim-vpi",
                        sim_mem_vpi,
                        "Runs a memory simulation using a input access, "
                        "for the memory configuration file (option "
                        "'--mem-config')");

	// Stand-alone simulator
	command_line->RegisterBool("--mem-sim",
			sim_mem_stand_alone,
			"Runs a memory simulation using a command trace or input access, "
			"for the memory configuration file (option "
			"'--mem-config')");

	// Debug information
	command_line->RegisterString("--mem-debug <file>", debug_file,
			"Debug information related with the memory system, "
			"including caches and coherence protocol.");

	// Configuration INI file for memory system
	command_line->RegisterString("--mem-config <file>", config_file,
			"Memory configuration file. For a detailed description "
			"of the sections and variables supported for this file "
			"use option '--mem-help'.");
	
	// Help message for memory system
	command_line->RegisterBool("--mem-help",
			help,
			"Print help message describing the memory configuration"
			" file, passed in option '--mem-config <file>'.");
	command_line->setIncompatible("--mem-help");

	// Option for producing the report
	command_line->RegisterString("--mem-report <file>", report_file,
			"File for a report on the memory hierarchy, including "
			"cache hits, misses evictions, etc. This option must "
			"be used together with detailed simulation of any "
			"CPU/GPU architecture.");

	// Option sanity check
	command_line->RegisterInt64("--mem-sanity-check <interval>",
			sanity_check_interval,
			"This option performs a sanity check for the underlying "
			"coherency protocol in constant periods equal to the interval, "
			"to examine its consistency and correctness. The simulation "
			"fails if the correctness is not maintained.");

        // Option trace file
        command_line->RegisterString("--mem-sim-trace <file>", input_memory_trace_file,
                        "Trace file containing sequential list of inputs to access. "
                        "Used alongside '--mem-sim'.");

        // Option random injection
        command_line->RegisterBool("--mem-sim-random ",
                        sim_mem_stand_alone_random,
                        "Run random injections instead of a trace file. "
                        "Used alongside '--mem-sim'.");
}

void System::ProcessOptions()
{
	// Show help message
	if (help)
	{
		std::cerr << help_message;
		exit(0);
	}

        // Stand-Alone VPI requires config file
        if (sim_mem_vpi && config_file.empty())
                throw Error(misc::fmt("Option --mem-sim-vpi requires "
                                " --mem-config option "));


	// Stand-Alone requires config file
	if (sim_mem_stand_alone && config_file.empty())
		throw Error(misc::fmt("Option --mem-sim requires "
				" --mem-config option "));

        if (sim_mem_stand_alone
            && !sim_mem_stand_alone_random
            && input_memory_trace_file.empty())
                throw Error(misc::fmt("Option --mem-sim require option "
                                      "--mem-sim-trace or --mem-sim-random"));

        if (sim_mem_stand_alone
            && sim_mem_stand_alone_random
            && !input_memory_trace_file.empty())
                throw Error(misc::fmt("Cannot use both options --mem-sim-trace "
                                      "and --mem-sim-random"));

	// Debug file
	debug.setPath(debug_file);
}


void System::RandomInjectionRun()
{
	// Random Number generation setup
	std::random_device rd;	
	std::mt19937 rng(rd());
	std::uniform_int_distribution<unsigned> random_generator(0, 0xfffff);

	// Number of modules in level 1
	int l1_caches = 0;

	// Get the number of modules in the level 1
	for (auto &module : modules)
		if (module->getLevel() == 1)
			l1_caches++;

	// Initiate a list of double for injection time
	auto inject_time = misc::new_unique_array<double>(l1_caches);

	// Set the witness value to the number of events
	int witness = -1 * access_counts;

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

void System::Access(const unsigned int &mod, const unsigned int &type, const unsigned int &address)
{
    std::string module_name ="mod-l1-";
    module_name.append(std::to_string(mod));
    mem::Module *module = getModule(module_name);

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
            a_access current_access = {
                access_identifier,
                address,
                the_type,
                "test",
                current_witness
            };
            accesses_list.emplace(access_identifier,current_access);


            // Perform the access
            module->Access( accesses_list.at(access_identifier).access_type
                            ,accesses_list.at(access_identifier).access_address
                            ,accesses_list.at(access_identifier).access_witness);
            ++access_identifier;
        }
    }
}


int System::Step()
{
    // Proccess events
    esim::Engine *esim_engine = esim::Engine::getInstance();
    esim_engine->ProcessEvents();

    // update the access map table
    checkProccessedEvents();

    return 0;
}



int System::checkProccessedEvents()
{

    total_witness=0; // Redundant way to check witness
    std::map<int, mem::a_access>::iterator it=accesses_list.begin();
    while ( it!=accesses_list.end())
    {
        if(*it->second.access_witness==0)
        {

            // FIXME: Has to change VPI objects or pass information
            std::cout << "============ Access id: "<<it->first
                      << " to address " << it->second.access_address
                      << " finished accessing ============="<< '\n';

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

int System::Finalize(){
    esim::Engine *esim_engine = esim::Engine::getInstance();
    // Lets finish all off
    esim_engine->ProcessAllEvents();
    // Here finish the esim
    esim_engine->Finish("MaxTime");

    return 0;
}

void System::TraceFileRun()
{
        if (input_memory_trace_file.empty())
                return;

        const auto& input_file_path = input_memory_trace_file;
        std::ifstream input_file(input_file_path);
        if (!input_file)
                throw Error(misc::fmt("%s: cannot open file for read",
                                      input_file_path.c_str()));

        // parse trace file into this vector
        /* trace file format:
           ----------------
           trace_file ::= line *
           line ::= access | '#' ...
           access ::= [ delay ] module type addr
           delay ::= '+'INT
           module ::= INT
           type ::= INT
           addr ::= INT
        */
        struct TraceLine {
                unsigned int type, address;
                unsigned int module, cycle;
                inline TraceLine (unsigned int t, unsigned int a,
                                  unsigned int m, unsigned int c) :
                        type(t), address(a), module(m), cycle(c)
                {
                }
        };
        std::vector<TraceLine> accesses;
        unsigned int cycle = 0;
        misc::StringError str_err;
        unsigned int line_num;

        for (line_num = 1; !input_file.eof(); line_num++)
        {
                std::string line;
                std::getline(input_file, line);
                misc::StringTrim(line);

                // comment or empty line
                if (line.empty()
                    || line[0] == '#')
                        continue;
                // tokenize words
                std::vector<std::string> words;
                misc::StringTokenize(line, words);
                if (words.empty())
                        continue;

                // position of "module" identifier in words
                // e.g.  +1 2 1 0x1000    mod_pos = 1
                //          ^
                //       2 2 0x2000       mod_pos = 0
                //       ^
                unsigned int mod_pos = 0;

                // parse "+DELAY"
                if (words[0].size() > 1 && words[0][0] == '+') {
                        mod_pos = 1;

                        int delay = misc::StringToInt(words[0], str_err);
                        if (str_err != misc::StringErrorOK) break;
                        if (delay < 0)
                                throw Error(misc::fmt("invalid trace file: line %d: "
                                                      "invalid negative cycles",
                                                      line_num));
                        cycle += delay;
                }

                // check # of words
                if (words.size() > (mod_pos + 3))
                        throw Error(misc::fmt("invalid trace file: line %d: "
                                              "too many integers on line",
                                              line_num));
                if (words.size() < (mod_pos + 3))
                        throw Error(misc::fmt("invalid trace file: line %d: "
                                              "missing some parameters on line",
                                              line_num));

                // parse "MOD TYPE ADDR"
                unsigned int module = misc::StringToInt(words[mod_pos], str_err);
                if (str_err != misc::StringErrorOK) break;
                unsigned int type = misc::StringToInt(words[mod_pos + 1], str_err);
                if (str_err != misc::StringErrorOK) break;
                unsigned int address = misc::StringToInt(words[mod_pos + 2], str_err);
                if (str_err != misc::StringErrorOK) break;
                // insert into list
                accesses.emplace_back(type, address, module, cycle);
        }

        if (str_err != misc::StringErrorOK)
                throw Error(misc::fmt("invalid trace file: line %d: "
                                      "invalid number format",
                                      line_num));

        // run the parsed trace file
        cycle = 0;
        for (auto& acc : accesses)
        {
                // wait for correct cycle #
                while (cycle < acc.cycle)
                {
                        Step();
                        cycle++;
                }
                Access(acc.module, acc.type, acc.address);
        }
        // finish
        Finalize();
}


void System::DumpReport()
{
    // Dump report files
    if (!report_file.empty())
    {
        // Try to open the file
        std::ofstream f(report_file);
        if (!f)
            throw Error(misc::fmt("%s: cannot open file for write",
                                  report_file.c_str()));

        // Dump the memory report
        DumpReport(f);

        // For every internal network report in the same file
        for (auto &network : networks)
            network->DumpReport(f);
    }
}


void System::DumpReport(std::ostream &os) const
{
    // Dump introduction to the memory report file
    os << "; Report for caches, TLBs, and main memory\n";
    os << ";    Accesses - Total number of accesses - "
          "Reads, Writes, and NCWrites (non-coherent) \n";
    os << ";    Hits, Misses - Accesses resulting in hits/misses\n";
    os << ";    HitRatio - Hits divided by accesses\n";
    os << ";    Evictions - Invalidated or replaced cache blocks\n";
    os << ";    Retries - For L1 caches, accesses that were retried\n";
    os << ";    ReadRetries, WriteRetries, NCWriteRetries - Read/Write"
          " retried accesses\n";
    os << ";    Reads, Writes, NCWrites - Total read/write accesses\n";
    os << ";    BlockingReads, BlockingWrites, BlockingNCWrites - "
          "Reads/writes coming from lower-level cache\n";
    os << ";    NonBlockingReads, NonBlockingWrites, NonBlockingNCWrites -"
          " Coming from upper-level cache\n";
    os << "\n\n";

    // Dump report for each module
    for (auto &module : modules)
        module->DumpReport(os);
}


void System::SanityCheck()
{
    //
    // Top down Rules
    //

    // Get the blocks of each module in the highest level
    for (auto &module : modules)
    {
        // Get the associated cache
        Cache *cache = module->getCache();

        // Get every block of the cache
        for (unsigned set = 0; set < cache->getNumSets(); set++)
        {
            for (unsigned way = 0; way < cache->getNumWays(); way++)
            {
                // Get the block
                Cache::Block *block = cache->getBlock(set,way);

                // If the block is not valid, continue
                if (block->getState() == Cache::BlockInvalid)
                    continue;

                // Get the lower module for the top-down rules
                Module *lower_module = module->
                        getLowModuleServingAddress(
                            block->getTag());
                int lower_set;
                int lower_way;
                int lower_tag;
                Cache::BlockState lower_state = Cache::BlockInvalid;
				module->FindBlock(block->getTag(),
						lower_set,
						lower_way,
						lower_tag,
						lower_state);

				// Rule 1:
				// Lower level module should have the block
				// in an state other than Invalid.
				if (lower_state == Cache::BlockInvalid)
				{
					module->Dump();
					lower_module->Dump();
					throw Error(misc::fmt("Sanity check failed\n"
					"window %lld to %lld: The module %s "
					"has a block that is not found in "
					"module's approperiate lower module %s\n",
					(last_sanity_check - 1) * sanity_check_interval,
					last_sanity_check * sanity_check_interval,
					module->getName().c_str(),
					lower_module->getName().c_str()));
				}
			}
		}
	}

	//
	// Down Up Rules
	//

	//
	// Peer Rules
	//
}

}  // namespace mem

