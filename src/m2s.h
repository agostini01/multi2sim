#ifndef M2S_H
#define M2S_H

#include <cstdlib>
#include <iostream>
#include <lib/esim/Engine.h>
#include <string.h>

#include <memory/System.h>

class Multi2Sim
{
/*Singleton approach for this class-------------------------------------------*/
public:
    static Multi2Sim& getInstance()
    {
	static Multi2Sim instance;
	return instance;
    }
private:
    Multi2Sim(){}
    Multi2Sim(Multi2Sim const&);
    void operator=(Multi2Sim const&);

public:
    void WelcomeMessage(std::ostream &os);

    // Initialize multi2sim with the proper arguments
    void m2sInitialize(char input_arguments[]);

    // Reset m2s bringing it to its initial state
    void m2sReset();

    // Tells m2s to process events scheduled to next clock cycle
    // m2sStep will change a verilog object allowing the "hdl cores" to access
    // some addresses depending on m2s memory model.
    void m2sStep();

    // Enquee an access event
    void m2sAccess(const unsigned int & mod
		  ,const unsigned int & type
		  ,const unsigned int & address
		  );

    // Finilize simulation and generate dump files
    void m2sFinalize();

    int MainProgram(int argc, char **argv);

private:
    // Load a program from the command line
    void LoadProgram(const std::vector<std::string> &arguments,
		     const std::vector<std::string> &environment = {},
		     const std::string &current_directory = "",
		     const std::string &stdin_file_name = "",
		     const std::string &stdout_file_name = ""
		    );

    // Load programs from context configuration file
    void LoadPrograms();

    void RegisterOptions();

    void ProcessOptions();


    void RegisterRuntimes();


    void RegisterDrivers();


    void MainLoop();


    void DumpStatisticsSummary(std::ostream &os);

    void DumpReports();

    void Cleanup();


    // VPI specific
    struct a_access
    {
        unsigned access_id;
        unsigned access_address;
        mem::Module::AccessType access_type;
        const char* access_module_name;
        int* access_witness;

    };

   // VPI specific
   std::map<int, a_access> accesses_list;

   //
   int total_witness=-10000;


   int checkProccessedEvents();



    const std::string mem_config_0 =
		    "[CacheGeometry geo-l1]\n"
		    "Sets = 16\n"
		    "Assoc = 2\n"
		    "BlockSize = 64\n"
		    "Latency = 2\n"
		    "Policy = LRU\n"
		    "Ports = 2\n"
		    "\n"
		    "[CacheGeometry geo-l2]\n"
		    "Sets = 4\n"
		    "Assoc = 4\n"
		    "BlockSize = 128\n"
		    "Latency = 20\n"
		    "Policy = LRU\n"
		    "Ports = 4\n"
		    "\n"
		    "; 4 l1 cpu caches\n"
		    "\n"
		    "[Module mod-l1-0]\n"
		    "Type = Cache\n"
		    "Geometry = geo-l1\n"
		    "LowNetwork = net0\n"
		    "LowNetworkNode = n0\n"
		    "LowModules = mod-l2-0\n"
		    "\n"
		    "[Module mod-l1-1]\n"
		    "Type = Cache\n"
		    "Geometry = geo-l1\n"
		    "LowNetwork = net0\n"
		    "LowNetworkNode = n1\n"
		    "LowModules = mod-l2-0\n"
		    "\n"
		    "[Module mod-l1-2]\n"
		    "Type = Cache\n"
		    "Geometry = geo-l1\n"
		    "LowNetwork = net1\n"
		    "LowNetworkNode = n3\n"
		    "LowModules = mod-l2-1\n"
		    "\n"
		    "[Module mod-l1-3]\n"
		    "Type = Cache\n"
		    "Geometry = geo-l1\n"
		    "LowNetwork = net1\n"
		    "LowNetworkNode = n4\n"
		    "LowModules = mod-l2-1\n"
		    "\n"
		    "; 1 l2 cpu cache\n"
		    "\n"
		    "[Module mod-l2-0]\n"
		    "Type = Cache\n"
		    "Geometry = geo-l2\n"
		    "HighNetwork = net0\n"
		    "HighNetworkNode = n2\n"
		    "LowNetwork = net2\n"
		    "LowNetworkNode = n6\n"
		    "LowModules = mod-mm\n"
		    "\n"
		    "[Module mod-l2-1]\n"
		    "Type = Cache\n"
		    "Geometry = geo-l2\n"
		    "HighNetwork = net1\n"
		    "HighNetworkNode = n5\n"
		    "LowNetwork = net2\n"
		    "LowNetworkNode = n7\n"
		    "LowModules = mod-mm\n"
		    "\n"
		    "; 1 mm\n"
		    "\n"
		    "[Module mod-mm]\n"
		    "Type = MainMemory\n"
		    "BlockSize = 128\n"
		    "Latency = 200\n"
		    "HighNetwork = net2\n"
		    "HighNetworkNode = n8\n"
		    "\n"
		    "\n"
		    "[Entry core-0]\n"
		    "Arch = x86\n"
		    "Core = 0\n"
		    "Thread = 0\n"
		    "DataModule = mod-l1-0\n"
		    "InstModule = mod-l1-0\n"
		    "\n"
		    "[Entry core-1]\n"
		    "Arch = x86\n"
		    "Core = 1\n"
		    "Thread = 0\n"
		    "DataModule = mod-l1-1\n"
		    "InstModule = mod-l1-1\n"
		    "\n"
		    "[Entry core-2]\n"
		    "Arch = x86\n"
		    "Core = 2\n"
		    "Thread = 0\n"
		    "DataModule = mod-l1-2\n"
		    "InstModule = mod-l1-2\n"
		    "\n"
		    "[Entry core-3]\n"
		    "Arch = x86\n"
		    "Core = 3\n"
		    "Thread = 0\n"
		    "DataModule = mod-l1-3\n"
		    "InstModule = mod-l1-3\n";

    const std::string mem_config_1 =
		    "[ CacheGeometry geo-l1 ]\n"
		    "Sets = 4\n"
		    "Assoc = 1\n"
		    "BlockSize = 64\n"
		    "Latency = 1\n"
		    "Policy = LRU\n"
		    "Ports = 2\n"
		    "\n"
		    "[ CacheGeometry geo-l2 ]\n"
		    "Sets = 8\n"
		    "Assoc = 1\n"
		    "BlockSize = 64\n"
		    "Latency = 2\n"
		    "Policy = LRU\n"
		    "Ports = 4\n"
		    "\n"
		    "[ CacheGeometry geo-l3 ]\n"
		    "Sets = 16\n"
		    "Assoc = 1\n"
		    "BlockSize = 64\n"
		    "Latency = 4\n"
		    "Policy = LRU\n"
		    "Ports = 8\n"
		    "\n"
		    "\n"
		    "; L1 Caches\n"
		    "\n"
		    "[ Module mod-l1-0 ]\n"
		    "Type = Cache\n"
		    "Geometry = geo-l1\n"
		    "LowNetwork = net-0\n"
		    "LowNetworkNode = n0\n"
		    "LowModules = mod-l2-0\n"
		    "\n"
		    "[ Module mod-l1-1 ]\n"
		    "Type = Cache\n"
		    "Geometry = geo-l1\n"
		    "LowNetwork = net-0\n"
		    "LowNetworkNode = n1\n"
		    "LowModules = mod-l2-0\n"
		    "\n"
		    "[ Module mod-l1-2 ]\n"
		    "Type = Cache\n"
		    "Geometry = geo-l1\n"
		    "LowNetwork = net-1\n"
		    "LowNetworkNode = n0\n"
		    "LowModules = mod-l2-1\n"
		    "\n"
		    "[ Module mod-l1-3 ]\n"
		    "Type = Cache\n"
		    "Geometry = geo-l1\n"
		    "LowNetwork = net-1\n"
		    "LowNetworkNode = n1\n"
		    "LowModules = mod-l2-1\n"
		    "\n"
		    "\n"
		    "; L2 Caches\n"
		    "\n"
		    "[ Module mod-l2-0 ]\n"
		    "Type = Cache\n"
		    "Geometry = geo-l2\n"
		    "HighNetwork = net-0\n"
		    "HighNetworkNode = n2\n"
		    "LowNetwork = net-2\n"
		    "LowNetworkNode = n0\n"
		    "LowModules = mod-l3\n"
		    "\n"
		    "[ Module mod-l2-1 ]\n"
		    "Type = Cache\n"
		    "Geometry = geo-l2\n"
		    "HighNetwork = net-1\n"
		    "HighNetworkNode = n2\n"
		    "LowNetwork = net-2\n"
		    "LowNetworkNode = n1\n"
		    "LowModules = mod-l3\n"
		    "\n"
		    "\n"
		    "; L3 Cache\n"
		    "\n"
		    "[ Module mod-l3 ]\n"
		    "Type = Cache\n"
		    "Geometry = geo-l3\n"
		    "HighNetwork = net-2\n"
		    "HighNetworkNode = n2\n"
		    "LowNetwork = net-3\n"
		    "LowNetworkNode = n0\n"
		    "LowModules = mod-mm\n"
		    "\n"
		    "\n"
		    "; Main Memory\n"
		    "\n"
		    "[ Module mod-mm ]\n"
		    "Type = MainMemory\n"
		    "BlockSize = 64\n"
		    "DirectorySize = 32\n"
		    "DirectoryAssoc = 1\n"
		    "Latency = 8\n"
		    "HighNetwork = net-3\n"
		    "HighNetworkNode = n1\n"
		    "\n"
		    "\n"
		    "; Cores\n"
		    "\n"
		    "[ Entry core-0 ]\n"
		    "Arch = x86\n"
		    "Core = 0\n"
		    "Thread = 0\n"
		    "Module = mod-l1-0\n"
		    "\n"
		    "[ Entry core-1 ]\n"
		    "Arch = x86\n"
		    "Core = 1\n"
		    "Thread = 0\n"
		    "Module = mod-l1-1\n"
		    "\n"
		    "[ Entry core-2 ]\n"
		    "Arch = x86\n"
		    "Core = 2\n"
		    "Thread = 0\n"
		    "Module = mod-l1-2\n"
		    "\n"
		    "[ Entry core-3 ]\n"
		    "Arch = x86\n"
		    "Core = 3\n"
		    "Thread = 0\n"
		    "Module = mod-l1-3\n";

    const std::string x86_config =
		    "[ General ]\n"
		    "Cores = 4\n"
		    "Threads = 1\n";

    const std::string net_config =
		    "[ Network.net0 ]\n"
		    "DefaultInputBufferSize = 1024\n"
		    "DefaultOutputBufferSize = 1024\n"
		    "DefaultBandwidth = 256\n"
		    "\n"
		    "[ Network.net1 ]\n"
		    "DefaultInputBufferSize = 1024\n"
		    "DefaultOutputBufferSize = 1024\n"
		    "DefaultBandwidth = 256\n"
		    "\n"
		    "[ Network.net2 ]\n"
		    "DefaultInputBufferSize = 1024\n"
		    "DefaultOutputBufferSize = 1024\n"
		    "DefaultBandwidth = 256\n"
		    "\n"
		    "[ Network.net0.Node.n0 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net0.Node.n1 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net0.Node.n2 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net1.Node.n3 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net1.Node.n4 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net1.Node.n5 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net2.Node.n6 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net2.Node.n7 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net2.Node.n8 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net0.Node.s0 ]\n"
		    "Type = Switch \n"
		    "\n"
		    "[ Network.net1.Node.s1 ]\n"
		    "Type = Switch \n"
		    "\n"
		    "[ Network.net2.Node.s2 ]\n"
		    "Type = Switch \n"
		    "\n"
		    "[ Network.net0.Link.l0 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n0\n"
		    "Dest = s0\n"
		    "\n"
		    "[ Network.net0.Link.l1 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n1\n"
		    "Dest = s0\n"
		    "\n"
		    "[ Network.net0.Link.l2 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n2 \n"
		    "Dest = s0\n"
		    "\n"
		    "[ Network.net1.Link.l3 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n3 \n"
		    "Dest = s1\n"
		    "\n"
		    "[ Network.net1.Link.l4 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n4 \n"
		    "Dest = s1\n"
		    "\n"
		    "[ Network.net1.Link.l5 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n5 \n"
		    "Dest = s1\n"
		    "\n"
		    "[ Network.net2.Link.l6 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n6 \n"
		    "Dest = s2\n"
		    "\n"
		    "[ Network.net2.Link.l7 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n7 \n"
		    "Dest = s2\n"
		    "\n"
		    "[ Network.net2.Link.l8 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n8 \n"
		    "Dest = s2\n";

    const std::string net_config_1 =
		    "[ Network.net-0 ]\n"
		    "DefaultInputBufferSize = 256\n"
		    "DefaultOutputBufferSize = 256\n"
		    "DefaultBandwidth = 64\n"
		    "\n"
		    "[ Network.net-1 ]\n"
		    "DefaultInputBufferSize = 256\n"
		    "DefaultOutputBufferSize = 256\n"
		    "DefaultBandwidth = 64\n"
		    "\n"
		    "[ Network.net-2 ]\n"
		    "DefaultInputBufferSize = 256\n"
		    "DefaultOutputBufferSize = 256\n"
		    "DefaultBandwidth = 64\n"
		    "\n"
		    "[ Network.net-3 ]\n"
		    "DefaultInputBufferSize = 256\n"
		    "DefaultOutputBufferSize = 256\n"
		    "DefaultBandwidth = 64\n"
		    "\n"
		    "[ Network.net-0.Node.n0 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net-0.Node.n1 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net-0.Node.n2 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net-1.Node.n0 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net-1.Node.n1 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net-1.Node.n2 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net-2.Node.n0 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net-2.Node.n1 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net-2.Node.n2 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net-3.Node.n0 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net-3.Node.n1 ]\n"
		    "Type = EndNode\n"
		    "\n"
		    "[ Network.net-0.Node.s0 ]\n"
		    "Type = Switch\n"
		    "\n"
		    "[ Network.net-1.Node.s0 ]\n"
		    "Type = Switch\n"
		    "\n"
		    "[ Network.net-2.Node.s0 ]\n"
		    "Type = Switch\n"
		    "\n"
		    "[ Network.net-3.Node.s0 ]\n"
		    "Type = Switch\n"
		    "\n"
		    "[ Network.net-0.Link.l0 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n0\n"
		    "Dest = s0\n"
		    "\n"
		    "[ Network.net-0.Link.l1 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n1\n"
		    "Dest = s0\n"
		    "\n"
		    "[ Network.net-0.Link.l2 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n2\n"
		    "Dest = s0\n"
		    "\n"
		    "[ Network.net-1.Link.l0 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n0\n"
		    "Dest = s0\n"
		    "\n"
		    "[ Network.net-1.Link.l1 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n1\n"
		    "Dest = s0\n"
		    "\n"
		    "[ Network.net-1.Link.l2 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n2\n"
		    "Dest = s0\n"
		    "\n"
		    "[ Network.net-2.Link.l0 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n0\n"
		    "Dest = s0\n"
		    "\n"
		    "[ Network.net-2.Link.l1 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n1\n"
		    "Dest = s0\n"
		    "\n"
		    "[ Network.net-2.Link.l2 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n2\n"
		    "Dest = s0\n"
		    "\n"
		    "[ Network.net-3.Link.l0 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n0\n"
		    "Dest = s0\n"
		    "\n"
		    "[ Network.net-3.Link.l1 ]\n"
		    "Type = Bidirectional\n"
		    "Source = n1\n"
		    "Dest = s0\n";


    ///
    /// \brief getVpiArguments
    ///         creates typical argc and argv from a char array
    /// \param my_argc
    ///         int * to argc
    /// \param my_argv
    ///         char ** to argv
    /// \param mystring
    ///         input string
    ///
    /// It is necessary to allocate the pointers first, for this:
    ///
    ///     char mystring[] = "a string of input arguments"
    ///     //...
    ///     //...
    ///     int* my_argc = (int*) malloc(sizeof(int));
    ///     char**my_argv = (char**)(malloc(strlen(mystring)*sizeof(char)));
    ///
    void getVpiArguments(int* my_argc, char **my_argv, char *mystring)
    {
        char * pch;
        *my_argc = 0;
        pch = strtok (mystring," ");
        my_argv[*my_argc] = pch;

        while (pch != NULL)
        {
            ++*my_argc;
            pch = strtok (NULL, " ");
            my_argv[*my_argc] = pch;
        }
    }

};



#endif // M2S_H
