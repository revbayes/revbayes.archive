#include <stdio.h>
#include <string>
#include <vector>

#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RevClient.h"
#include "RevLanguageMain.h"
#include "Parser.h"
#include "Workspace.h"

#ifdef RB_MPI
#include <mpi.h>
#endif

int main(int argc, char* argv[]) {
    
#   ifdef RB_MPI
    int process_id = 0;
    int num_processes = 0;
    try
    {
        MPI_Init(&argc, &argv);
//        process_id = MPI::COMM_WORLD.Get_rank();
        MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
//        num_processes = MPI::COMM_WORLD.Get_size();
        MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
        
        unsigned int seed = 0;
        
        // sync the random number generators
        if ( process_id == 0 )
        {
            seed = RevBayesCore::GLOBAL_RNG->getSeed();
            
        }
        
        MPI_Bcast(&seed, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
        RevBayesCore::GLOBAL_RNG->setSeed( seed );
        
    }
    catch (char* str)
    {
        return -1;
    }
#endif
    
    /* seek out files from command line */
    std::vector<std::string> sourceFiles;
    int argIndex = 1;
    while (argIndex < argc)
    {
        sourceFiles.push_back(std::string(argv[argIndex++]));
    }
    
    /* initialize environment */
    RevLanguageMain rl = RevLanguageMain();
    rl.startRevLanguageEnvironment(sourceFiles);
    
#   ifdef RB_XCODE

#   ifndef RB_MPI
    int process_id = 0;

#   endif
    /* Declare things we need */
    int result = 0;
    std::string commandLine = "";
    std::string line = "";
        
    for (;;)
    {
        /* Print prompt based on state after previous iteration */
        if ( process_id == 0 )
        {
            if (result == 0 || result == 2)
            {
                std::cout << "> ";
            }
            else
            {
                std::cout << "+ ";
            }

            /* Get the line */
            std::istream& retStream = std::getline(std::cin, line);
            if (!retStream)
            {
#               ifdef RB_MPI
                MPI::Finalize();
#               endif
                exit(0);
            }
            
            if (result == 0 || result == 2)
            {
                commandLine = line;
            }
            else if (result == 1)
            {
                commandLine += ";" + line;
            }
        }
        
        size_t bsz = commandLine.size();
#       ifdef RB_MPI
        MPI_Bcast(&bsz, 1, MPI_INT, 0, MPI_COMM_WORLD);
#       endif
        
        char * buffer = new char[bsz+1];
        buffer[bsz] = 0;
        for (int i = 0; i < bsz; i++)
        {
            buffer[i] = commandLine[i];
        }
#       ifdef RB_MPI
        MPI_Bcast(buffer, (int)bsz, MPI_CHAR, 0, MPI_COMM_WORLD);
#       endif
        
        std::string tmp = std::string( buffer );

        result = RevLanguage::Parser::getParser().processCommand(tmp, &RevLanguage::Workspace::userWorkspace());
    }
    
#   else
    
    RevClient c;
    c.startInterpretor();
    
#   endif

#   ifdef RB_MPI
    MPI_Finalize();
#   endif
    
    return 0;
}


