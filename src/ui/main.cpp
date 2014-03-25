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

#include "RevLanguageMain.h"
#include "RbOptions.h"
#include "Parser.h"
#include "Workspace.h"
#include "workspace/FunctionTable.h"
#include "functions/RlFunction.h"
#include "WorkspaceUtils.h"
#include "CommandLineUtils.h"
extern "C" {
#include "linenoise.h"
}
#include "lineedit/EditorMachine.h"
#include "lineedit/EditorState.h"
#include "lineedit/lineeditUtils.h"

#include <boost/filesystem.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

#include <boost/program_options.hpp>
using namespace boost;
namespace po = boost::program_options;
namespace fs = boost::filesystem;

int main(int argc, const char* argv[]) {

    fs::path slash("/");
    std::string systemSlash = slash.make_preferred().native(); // convert to current os directory separator
    std::string includePath("." + systemSlash);

    int interactive = true;
    std::vector<std::string> sourceFiles;
    std::vector<std::string> checkedSourceFiles;

    // set up the command line options to parse    
    po::options_description desc("Available options are");
    desc.add_options()
            ("help", "print this help")
            ("interactive,i", "enable interactive prompt\nBy default enabled when no files are given, otherwise disabled.")
            ("include-path,p", po::value<std::string>(&includePath)->default_value(includePath), "include path to prepend to file names.")
            ("input-file,f", po::value< std::vector<std::string> >(&sourceFiles), "input file. ")
            ;

    // multiple files can be appended at end of argument chain
    po::positional_options_description p;
    p.add("input-file", -1);

    // parse commandline
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << "Usage: [options] file1 file2 ...\n\n";
        std::cout << desc << std::endl;
        return(0);
    }

    if (vm.count("include-path")) {
        std::cout << "Include path is: " << includePath << "\n";
    }

    if (vm.count("input-file")) {
        interactive = false; // default behaviour if source file(s) supplied. Can be overridden by user
        // fix base path
        for (unsigned int i = 0; i < sourceFiles.size(); i++) {
            
            fs::path tmp(fs::path(includePath) / sourceFiles[i]);
            
            // check that file exists
            if (!fs::exists(tmp)) {
                std::cout << "Warning: file '" + tmp.string() + "' does not exist." << std::endl;
            } else {
                checkedSourceFiles.push_back(tmp.string());
            }
        }
    }
    
    /* initialize environment */
    RevLanguageMain rl;
    rl.startRevLanguageEnvironment(checkedSourceFiles);
    
    
    if (vm.count("interactive") || interactive) {
        std::cout << "Interactive mode enabled." << "\n";
    } else {
        std::cout << "Interactive prompt is not enabled, program about to exit. \nUse argument --help to see all command line argument options." << std::endl;
        exit(0);
    }
    
    

    /* Declare things we need */
    char *default_prompt = (char *) "RevBayes > ";
    char *incomplete_prompt = (char *) "RevBayes + ";
    char *prompt = default_prompt;
    int result = 0;
    std::string commandLine;
    std::string line;

    for (;;) {

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


