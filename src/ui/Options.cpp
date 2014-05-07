/* 
 * File:   Options.cpp
 * Author: johadunf
 * 
 * Created on May 6, 2014, 2:54 PM
 */

#include "Options.h"
#include "libs/filesystem.h"
#include <boost/program_options.hpp>

#include <boost/filesystem.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

using namespace boost;
namespace po = boost::program_options;
namespace fs = boost::filesystem;

Options::Options() {
    
    // set up reasonable default values
    this->message = "";    
    this->includePath = "." + directorySeparator();       
    this->help = false;
    this->interactive = true;
    this->disableReadline = true;    

}

void Options::parseOptions(int argc, const char** argv) {
    
    std::string endl = "\n";
    // set up the command line options to parse    
    po::options_description desc("Available options are");
    desc.add_options()
            ("help,h", "print this help")
            ("interactive,i", "enable interactive prompt\nBy default enabled when no files are given, otherwise disabled.")
            ("disable-readline,d", "some terminals can't handle readline functionality well. Try this option if the output of your terminal is scrambled.")
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
        std::ostringstream _desc;
        _desc << desc;
        this->help = true;
        this->message.append("Usage: [options] file1 file2 ...")
                .append(endl)
                .append(_desc.str())
                .append(endl);
    }

    if (vm.count("include-path")) {
        this->message.append("Include path is: ")
                .append(includePath)
                .append(endl);
    }

    if (vm.count("input-file")) {
        this->interactive = false; // default behavior if source file(s) supplied. Can be overridden by user

        for (unsigned int i = 0; i < sourceFiles.size(); i++) {
            fs::path tmp(fs::path(includePath) / sourceFiles[i]);
            // check that file exists
            if (!fs::exists(tmp)) {
                this->message.append("Warning: file '")
                        .append(tmp.string())
                        .append("' does not exist.")
                        .append(endl);
            } else {
                checkedSourceFiles.push_back(tmp.string());
            }
        }
    }

    if (vm.count("interactive") || interactive) {
        this->interactive = true;
        this->message.append("Interactive mode enabled.").append(endl);
    } else {
        this->interactive = false;
        this->message.append("Interactive prompt is not enabled, program about to exit.")
                .append(endl)
                .append("Use argument --help to see all command line argument options.")
                .append(endl);        
    }

    if (vm.count("disable-readline")) {
        this->disableReadline = true;
    }

}

void Options::setIncludePath(std::string includePath) {

    this->includePath = includePath;
}

std::string Options::getIncludePath() const {

    return includePath;
}

std::vector<std::string> Options::getInputFiles() const {

    return checkedSourceFiles;
}

bool Options::isDisableReadline() const {

    return disableReadline;
}

bool Options::isInteractive() const {

    return interactive;
}

std::string Options::getMessage() const {

    return message;
}

bool Options::isHelp() const {
    return help;
}



