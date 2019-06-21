#include <stdio.h>
#include <string>
#include <vector>

#include <boost/program_options.hpp>
namespace po = boost::program_options;
using po::variables_map;

#include "RbVersion.h"
#include "RbException.h"
#include "RbSettings.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RevClient.h"
#include "RevLanguageMain.h"
#include "RlCommandLineOutputStream.h"
#include "RlUserInterfaceOutputStream.h"
#include "RlUserInterface.h"
#include "Parser.h"
#include "Workspace.h"

#ifdef RB_MPI
#include <mpi.h>
#endif

std::string usage()
{
    return "Usage: rb [OPTIONS]\n       rb [OPTIONS] <file1> [<file2> ...]";
    // Other usages not mentioned
}


std::string short_description()
{
    return "Bayesian phylogenetic inference using probabilistic graphical models and an interpreted language";
}

//
variables_map parse_cmd_line(int argc, char* argv[])
{
    
    using namespace po;

    // Put all options in one group for now.
    options_description general("Options");
    general.add_options()
	("help,h","Show information on flags.")
	("version,v","Show version and exit.")

	// implicit_value(1) means that -V => -V1
//      RevBayes doesn't use a global verbose_logging flag.
//	("verbose,V",value<int>()->implicit_value(1),"Log extra information for debugging.")

	("batch,b","Run in batch mode.")
    ("args",value<std::vector<std::string> >()->multitoken(),"Command line arguments to initialize RevBayes variables.")
	// composing means that --file can occur multiple times
    ("file",value<std::vector<std::string> >()->composing(),"File(s) to source.")
    ("setOption",value<std::vector<std::string> >()->composing(),"Set an option key=value.")
	;

    // Treat all positional options as "file" options.
    positional_options_description p;
    p.add("file", -1);

    // Parse the command line into variables_map 'args'
    variables_map args;

    try {
        store(command_line_parser(argc, argv).options(general).positional(p).run(), args);
    }
    catch(po::error& e)
    {
        std::cout << "ERROR: " << e.what() << std::endl;
        std::cout << std::endl;
        
        int rank = 0;
#ifdef RB_MPI
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif
        if (rank == 0)
        {
            std::cout << usage() << std::endl;
            std::cout << short_description() << std::endl;
            std::cout << std::endl;
            std::cout << general << std::endl;
            std::cout << "See http://revbayes.github.io for more information." << std::endl;
        }
#ifdef RB_MPI
        MPI_Finalize();
#endif
        std::exit(0);
        
    }
    notify(args);

    // Print flags and usage info in this function since we know the flags here.
    if ( args.count("help") > 0 )
    {
	// Do we want to avoid displaying --file here, since its a positional option also?

        int rank = 0;
#ifdef RB_MPI
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif
        if (rank == 0)
        {
            std::cout<< usage() << std::endl;
            std::cout<< short_description() << std::endl;
            std::cout << std::endl;
            std::cout<< general << std::endl;
            std::cout<< "See http://revbayes.github.io for more information." << std::endl;
        }
#ifdef RB_MPI
        MPI_Finalize();
#endif
        std::exit(0);
    }

    return args;
}

int main(int argc, char* argv[]) {

      using namespace po;
#   ifdef RB_MPI
    int process_id = 0;
    int num_processes = 0;
    try
    {
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
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

    /* Parse argv to get the command line arguments */
    variables_map args = parse_cmd_line(argc, argv);

    if ( args.count("version") > 0 )
    {
        std::cout << RbVersion().getVersion() << std::endl;
        exit(0);
    }

    if ( args.count("verbose") > 0 )
    {
        int verbosity = args["verbose"].as<int>();
    }

    if ( args.count("setOption") > 0 )
    {
        std::vector<std::string> options = args["setOption"].as<std::vector<std::string> >();
        for(int i=0;i<options.size();i++)
        {
            std::vector<std::string> tokens;
            StringUtilities::stringSplit(options[i], "=", tokens);
            if (tokens.size() != 2)
            {
                throw RbException("Option '"+options[i]+"' must have the form key=value");
            }
            else
            {
                RbSettings::userSettings().setOption( tokens[0], tokens[1], false );
            }
        }
    }
    
    std::vector<std::string> rb_args;
    if ( args.count("args") > 0 )
    {
        rb_args = args["args"].as<std::vector<std::string> >();
    }
    

    /*default to interactive mode*/
    bool batch_mode = (args.count("batch") > 0);
    // FIXME -- the batch_mode variable appears to have no effect if true.

    /* seek out files from command line */
    std::vector<std::string> source_files;
    if ( args.count("file") > 0 )
    {
        source_files = args["file"].as<std::vector<std::string> >();
    }
    
    /* initialize environment */
    RevLanguageMain rl = RevLanguageMain(batch_mode);

    CommandLineOutputStream *rev_output = new CommandLineOutputStream();
    RevLanguage::UserInterface::userInterface().setOutputStream( rev_output );
    rl.startRevLanguageEnvironment(rb_args, source_files);

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
