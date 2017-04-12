#include "ModuleSystem.h"
#include "RevLanguageMain.h"
#include "Parser.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RbSettings.h"
#include "Workspace.h"
#include "RlUserInterface.h"
#include "RbVersion.h"
#include <iostream>
#include <string>
#include <cstdlib>

#ifdef RB_MPI
#include <mpi.h>
#endif

RevLanguageMain::RevLanguageMain(void)
{

}


void RevLanguageMain::startRevLanguageEnvironment(std::vector<std::string> source_files)
{
    
    int pid = 0;
#ifdef RB_MPI
//    pid = MPI::COMM_WORLD.Get_rank();
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
#endif
    
    // load the modules
    try
    {
        RevLanguage::ModuleSystem::getModuleSystem().loadModules( RbSettings::userSettings().getModuleDir() );
    }    
    catch ( RbException e )
    {
        std::cout << e.getMessage() << std::endl;
    }


    // Print a nifty message
    RbVersion version = RbVersion();
    RevLanguage::UserInterface::userInterface().output(version.getHeader(), false);
    RevLanguage::UserInterface::userInterface().output("", false);
    
    RevLanguage::Workspace::globalWorkspace().initializeGlobalWorkspace();

    // process the command line arguments as source file names    
    std::string line;
    std::string command_line;
    int result = 0;

    for (unsigned int i =0 ; i < source_files.size(); i++)
    {
        line = "source(\"" + source_files[i] + "\")";
        
        // let only the master process print to the screen
        if ( pid == 0 )
        {
            std::cout << "> " << line << std::endl;
        }
        
        // Process the command line
        if ( result == 1 )
        {
            command_line += line;
        }
        else
        {
            command_line = line;
        }
        
        result = RevLanguage::Parser::getParser().processCommand(command_line, &RevLanguage::Workspace::userWorkspace());

        // We just hope for better input next time
        if ( result == 2 )
        {
            result = 0;
        }
        
    }
    
}

