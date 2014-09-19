/* 
 * File:   Options.h
 * Author: johadunf
 *
 * Created on May 6, 2014, 2:54 PM
 */

#ifndef OPTIONS_H
#define	OPTIONS_H

#include "libs/Filesystem.h"

#include <boost/program_options.hpp>
#include <boost/foreach.hpp>

typedef std::vector<std::string> StringVector;

class Options {
public:

    Options() {

        // set up reasonable default values
        this->message = "";
        this->includePath = "." + Filesystem::directorySeparator() + ";../../examples/;../../downloads/";
        this->help = false;
        this->interactive = true;
        this->disableReadline = false;

    }

    void parseOptions(int argc, const char** argv) {

        std::string endl = "\n";
        // set up the command line options to parse    
        boost::program_options::options_description desc("Available options are");
        desc.add_options()
                ("help,h", "print this help")
                ("interactive,i", "enable interactive prompt\nBy default enabled when no files are given, otherwise disabled.")
                ("disable-readline,d", "some terminals can't handle readline functionality well. Try this option if the output of your terminal is scrambled.")
                ("include-path,p", boost::program_options::value<std::string>(&includePath)->default_value(includePath), "Semicolon separated list of paths to scan for input files")
                ("input-file,f", boost::program_options::value< std::vector<std::string> >(&sourceFiles), "input file. ")
                ;

        // multiple files can be appended at end of argument chain
        boost::program_options::positional_options_description p;
        p.add("input-file", -1);

        // parse commandline
        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        boost::program_options::notify(vm);

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

    StringVector getIncludePaths() const {
        StringVector tmp, p;
        boost::split(tmp, includePath, boost::is_any_of(";"));

        BOOST_FOREACH(std::string s, tmp) {
            boost::trim(s);
            p.push_back(s);
        }
        return p;
    }

    std::vector<std::string> getInputFiles() const {

        return checkedSourceFiles;
    }

    bool isDisableReadline() const {

        return disableReadline;
    }

    bool isInteractive() const {

        return interactive;
    }

    std::string getMessage() const {

        return message;
    }

    bool isHelp() const {
        return help;
    }


private:

    std::string message;
    std::string includePath;
    std::string inputFile;

    bool help;
    bool interactive;
    bool disableReadline;

    std::vector<std::string> sourceFiles;
    std::vector<std::string> checkedSourceFiles;



};

#endif	/* OPTIONS_H */

