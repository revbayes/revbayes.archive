#include "HtmlHelpGenerator.h"

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include "boost/algorithm/string_regex.hpp"

#include "pugixml.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include "Configuration.h"
#include "HtmlHelpRenderer.h"

#include "RbHelpSystem.h"


std::string load_file(std::string filename) {
    std::string s;
    std::string r("");
    std::ifstream in(filename.c_str());
    while (getline(in, s)) {
        r += s + "\n";
    }
    in.close();
    return r;
}

int main(int argc, const char * argv[]) {
    
    Configuration configuration;
    configuration.parseConfiguration();
    std::cout << configuration.getMessage() << std::endl;
    
    
    fs::path helpDir(configuration.getHelpDir());
    
    std::cout << std::endl << "This tool will automatically generate the index.html page in:" << std::endl
    << helpDir.string() << " - directory" << std::endl
    << "All '*.xml' files here will be parsed and the function names added " << std::endl
    << "in alphabetical order to index.html. If you have made any manual changes to " << std::endl
    << "'help/index.html' these will be overwritten if you choose to continue. " << std::endl
    << std::endl
    << "Enter 'y' if you wish to continue," << std::endl
    << "any other key will cancel: ";
    
    if (getchar() != 'y') {
        exit(0);
    }
    
    std::cout << std::endl << "Generating index.html and separate html manual pages..." << std::endl;
    
    if (!fs::exists(helpDir)) {
        std::cout << "Error: Cannot find help directory: " << helpDir.string() << std::endl;
        exit(-1);
    }
    
    // gather all xml files in help dir, filtered by '.ext'
    std::string ext = ".xml";
    std::vector<fs::path> files;
    
    for (fs::directory_iterator it(helpDir); it != fs::directory_iterator(); ++it) {
        if (it->path().extension().string() == ext) {
            files.push_back(it->path().filename());
        }
    }
    
    std::cout << "Parsing files:" << std::endl;
    
    BOOST_FOREACH(fs::path file, files) {
        std::cout << "\t" + file.string() << std::endl;
    }
    
    // read html templates
    fs::path entry_tpl_file = helpDir / "lib/html-template/entry.tpl.html";
    fs::path index_tpl_file = helpDir / "lib/html-template/index.tpl.html";
    fs::path type_tpl_file = helpDir / "lib/html-template/type-page.tpl.html";
    fs::path function_tpl_file = helpDir / "lib/html-template/function-page.tpl.html";
    
    if (!fs::exists(entry_tpl_file) || !fs::exists(index_tpl_file) || !fs::exists(type_tpl_file) || !fs::exists(function_tpl_file)) {
        std::cout << "Error: One or more of the html template files in help/html directory is missing."
        << std::endl << "The html cannot be constructed and program will exit." << std::endl;
        exit(-1);
    }
    
    std::string entry_tpl = load_file(entry_tpl_file.string());
    std::string index_tpl = load_file(index_tpl_file.string());
    std::string type_tpl = load_file(type_tpl_file.string());
    std::string function_tpl = load_file(function_tpl_file.string());
    
    std::string function_entry_result, type_entry_result, tmp, tmp1;
    
    RevBayesCore::RbHelpSystem help = RevBayesCore::RbHelpSystem::getHelpSystem();
    HtmlHelpRenderer renderer = HtmlHelpRenderer();
    renderer.setTypeTemplate(type_tpl);
    renderer.setFunctionTemplate(function_tpl);
    
    const std::set<std::string> &functionEntryNames = help.getFunctionEntries();
    
    BOOST_FOREACH(std::string n, functionEntryNames) {
        
        RevBayesCore::RbHelpFunction functionEntry = help.getHelp( n );
        
        std::string functionName = n;
        if (functionName.size() > 0) {
            // add section to index.html
            tmp1 = boost::regex_replace(entry_tpl, boost::regex("#entry-name#"), functionName);
            tmp1 = boost::regex_replace(tmp1, boost::regex("#entry-type#"), "function");
            tmp = "";
            
            BOOST_FOREACH(std::string desc, functionEntry.getDescription()) {
                tmp.append("<p>").append(desc).append("</p>");
            }
            tmp1 = boost::regex_replace(tmp1, boost::regex("#entry-description#"), tmp);
            tmp1 = boost::regex_replace(tmp1, boost::regex("#more-content#"), renderer.renderFunctionHelp( functionEntry ));
            
            function_entry_result += tmp1 + "\n";
            
            // create a manual page
            std::fstream fs;
            std::string functionPage = "html/pages/" + functionName + "-function.html";
            fs.open(fs::path(helpDir / functionPage).string().c_str(), std::fstream::out | std::fstream::trunc);
            fs << renderer.renderFunctionHelp(functionEntry, true);
            fs.close();
            
        }
        
//        std::string typeName = help->getRbHelpEntry().GetTypeHelpEntry().GetName();
//        if (typeName.size() > 0) {
//            tmp1 = boost::regex_replace(entry_tpl, boost::regex("#entry-name#"), typeName);
//            tmp1 = boost::regex_replace(tmp1, boost::regex("#entry-type#"), "type");
//            tmp = "";
//            
//            BOOST_FOREACH(std::string desc, help->getRbHelpEntry().GetTypeHelpEntry().GetDescription()) {
//                tmp.append("<p>").append(desc).append("</p>");
//            }
//            tmp1 = boost::regex_replace(tmp1, boost::regex("#entry-description#"), tmp);
//            tmp1 = boost::regex_replace(tmp1, boost::regex("#more-content#"), renderer->renderTypeHelp(help->getRbHelpEntry().GetTypeHelpEntry()));
//            
//            type_entry_result += tmp1 + "\n";
//            
//            // create a manual page
//            fstream fs;
//            std::string typePage = "html/pages/" + typeName + "-type.html";
//            fs.open(fs::path(helpDir / typePage).string().c_str(), fstream::out | fstream::trunc);
//            fs << renderer->renderTypeHelp(help->getRbHelpEntry().GetTypeHelpEntry(), true);
//            fs.close();
//        }
        
    }
    
    // insert entries into main html page
    std::string index_result = boost::regex_replace(index_tpl, boost::regex("#manual-function-entries#"), function_entry_result);
    index_result = boost::regex_replace(index_result, boost::regex("#manual-type-entries#"), type_entry_result);
    
    // write out new file content
    std::fstream fs;
    fs.open(fs::path(helpDir / "html/index.html").string().c_str(), std::fstream::out | std::fstream::trunc);
    fs << index_result;
    fs.close();
    
    std::cout << "\nHtml files are updated.\n" << std::endl;
    
    return 0;
}
