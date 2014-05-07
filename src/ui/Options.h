/* 
 * File:   Options.h
 * Author: johadunf
 *
 * Created on May 6, 2014, 2:54 PM
 */

#ifndef OPTIONS_H
#define	OPTIONS_H

#include <boost/filesystem.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

namespace fs = boost::filesystem;

class Options {
public:
    Options();
    void parseOptions(int argc, const char** argv);
    
    void setIncludePath(std::string includePath);
    std::string getIncludePath() const;    
    std::string getMessage() const;
    
    bool isHelp() const;
    bool isDisableReadline() const;
    bool isInteractive() const;    
    std::vector<std::string> getInputFiles() const;
    
    
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

