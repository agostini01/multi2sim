#ifndef M2S_H
#define M2S_H

#include <cstdlib>
#include <iostream>
#include <lib/esim/Engine.h>

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
    void m2sInitialize();

    // Reset m2s bringing it to its initial state
    void m2sReset();

    // Tells m2s to process events scheduled to next clock cycle
    void m2sStep();

    // Enquee and access event
    void m2sAccess(const unsigned int & mod
		  ,const unsigned int & type
		  ,const unsigned int & address
		  );

    // Finilize simulation and generate dump files
    void m2sFinalize();

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


    void DumpStatisticsSummary(std::ostream &os = std::cerr);

    void DumpReports();


    int MainProgram(int argc, char **argv);

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
};

#endif // M2S_H
