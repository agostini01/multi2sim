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

void Multi2Sim::WelcomeMessage(std::ostream &os)
{
    std::cout<<"M2S::WelcomeMessage Function"<<std::endl;
}

void Multi2Sim::m2sStep()
{
    std::cout<<"M2S::vpiStep()"<<std::endl;
}

void Multi2Sim::m2sInitialize()
{
    std::cout<<"M2S::vpiInitialize()"<<std::endl;

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

int Multi2Sim::MainProgram(int argc, char **argv)
{
    return 0;
}

int main()
{
    return 0;
}
