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
          , const unsigned int & type
          , const unsigned int & address
          );

    void m2sAccess(const unsigned int &mod
          , const unsigned int &type
          , const unsigned int &address
          , const unsigned int &identifier
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
