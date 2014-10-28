#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <map>
#include <vector>
#include <algorithm>
#include <stdbool.h>

#include "Options.h"
#include "Configuration.h"
#include "RbClient.h"
#include "RevLanguageMain.h"
#include "Parser.h"
#include "Workspace.h"

#include "libs/Filesystem.h"
#include "IHelp.h"
#include "help/HelpParser.h"
#include "help/IHelpRenderer.h"
#include "help/HelpConsoleRenderer.h"

#include "boost/filesystem.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

namespace fs = boost::filesystem;

int main(int argc, const char* argv[]) {
    

    // parse command line arguments
    Options *options = new Options();
    options->parseOptions(argc, argv);
    std::cout << options->getMessage() << std::endl;
    if (options->isHelp()) {
        exit(0);
    }

    // read / create configuration file
    Configuration *configuration = new Configuration();
    configuration->parseConfiguration();
    std::cout << configuration->getMessage() << std::endl;
    
    // set up help
    IHelpRenderer *helpRenderer = new HelpConsoleRenderer();
    HelpParser *help = new HelpParser();
    help->setRenderer(helpRenderer);
    help->setHelpDir(configuration->getHelpDir());    

    /* initialize environment */    
    RevLanguageMain rl(help);

    // pass input files to Rev
    rl.startRevLanguageEnvironment(options->getInputFiles());
    // exit after processing the input files if not interactive
    if (!options->isInteractive()) {
        exit(0);
    }
    
    if (!options->isDisableReadline()) {
        // remove on production
//        TabTest t;
//        t.run();
        
        // enable on production
        RbClient c;
        c.startInterpretor(help, options, configuration);
        
    }


    /* Start the very basic RB interpreter if disable-readline has been set */
    std::cout << "Starting RevBayes with basic interpretor" << std::endl;
    char *default_prompt = (char *) "RevBayes > ";
    char *incomplete_prompt = (char *) "RevBayes + ";
    char *prompt = default_prompt;
    int result = 0;
    std::string commandLine;
    std::string line;

    while (true) {

        std::cout << prompt;
        std::istream& retStream = getline(std::cin, line);

        if (!retStream)
            exit(0);

        if (result == 0 || result == 2) {
            prompt = default_prompt;
            commandLine = line;
        } else if (result == 1) {
            prompt = incomplete_prompt;
            commandLine += line;
        }

        result = RevLanguage::Parser::getParser().processCommand(commandLine, &RevLanguage::Workspace::userWorkspace());
    }

    return 0;

}


