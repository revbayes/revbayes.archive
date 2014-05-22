#include "boost/filesystem.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include <boost/filesystem/fstream.hpp>

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
#include "libs/Filesystem.h"
#include "IHelp.h"
#include "HelpParser.h"
#include "RbHelpEntry.h"
#include "HelpHtmlRenderer.h"


namespace fs = boost::filesystem;
using namespace std;


string load_file(string filename) {
    string s;
    string r("");
    ifstream in(filename.c_str());
    while (getline(in, s)) {
        r += s + "\n";
    }
    in.close();
    return r;
}

// compare file names

struct compare_path {

    bool operator()(fs::path p1, fs::path p2) {
        return tolower(p1.string().c_str()[0]) < tolower(p2.string().c_str()[0]);
    }
};

int main(int argc, const char * argv[]) {

    // read / create settings file
    Configuration configuration;
    configuration.parseConfiguration();
    std::cout << configuration.getMessage() << std::endl;


    fs::path helpDir(configuration.getHelpDir());

    cout << endl << "This tool will automatically generate the index.html page in:" << endl
            << helpDir.string() << " - directory" << endl
            << "All '*.xml' files here will be parsed and the function names added " << endl
            << "in alphabetical order to index.html. If you have made any manual changes to " << endl
            << "'help/index.html' these will be overwritten if you choose to continue. " << endl
            << endl
            << "Enter 'y' if you wish to continue," << endl
            << "any other key will cancel: ";

    if (getchar() != 'y') {
        exit(0);
    }

    cout << endl << "Generating index.html and separate html manual pages..." << endl;

    if (!fs::exists(helpDir)) {
        cout << "Error: Cannot find help directory: " << helpDir.string() << endl;
        exit(-1);
    }

    // gather all xml files in help dir, filtered by '.ext'
    string ext = ".xml";
    vector<fs::path> files;

    for (fs::directory_iterator it(helpDir); it != fs::directory_iterator(); ++it) {
        if (it->path().extension().string() == ext) {
            files.push_back(it->path().filename());
        }
    }

    sort(files.begin(), files.end(), compare_path());

    cout << "Parsing files:" << endl;

    BOOST_FOREACH(fs::path file, files) {
        cout << "\t" + file.string() << endl;
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

    string entry_tpl = load_file(entry_tpl_file.string());
    string index_tpl = load_file(index_tpl_file.string());
    string type_tpl = load_file(type_tpl_file.string());
    string function_tpl = load_file(function_tpl_file.string());

    string function_entry_result, type_entry_result, tmp, tmp1;

    HelpParser *help = new HelpParser();
    HelpHtmlRenderer *renderer = new HelpHtmlRenderer();
    renderer->setTypeTemplate(type_tpl);
    renderer->setFunctionTemplate(function_tpl);

    BOOST_FOREACH(fs::path file, files) {

        if (!help->setHelp(fs::path(helpDir / file).string())) {
            cout << "Warning: failed to parse " << file.string() << ": " << endl << help->getMessage();
            continue;
        }
        std::string functionName = help->getRbHelpEntry().GetFunctionHelpEntry().GetName();
        if (functionName.size() > 0) {
            // add section to index.html
            tmp1 = boost::regex_replace(entry_tpl, boost::regex("#entry-name#"), functionName);
            tmp1 = boost::regex_replace(tmp1, boost::regex("#entry-type#"), "function");
            tmp = "";

            BOOST_FOREACH(std::string desc, help->getRbHelpEntry().GetFunctionHelpEntry().GetDescription()) {
                tmp.append("<p>").append(desc).append("</p>");
            }
            tmp1 = boost::regex_replace(tmp1, boost::regex("#entry-description#"), tmp);
            tmp1 = boost::regex_replace(tmp1, boost::regex("#more-content#"), renderer->renderFunctionHelp(help->getRbHelpEntry().GetFunctionHelpEntry()));

            function_entry_result += tmp1 + "\n";

            // create a manual page
            fstream fs;
            std::string functionPage = "html/pages/" + functionName + ".html";
            fs.open(fs::path(helpDir / functionPage).string().c_str(), fstream::out | fstream::trunc);
            fs << renderer->renderFunctionHelp(help->getRbHelpEntry().GetFunctionHelpEntry(), true);
            fs.close();

        }

        std::string typeName = help->getRbHelpEntry().GetTypeHelpEntry().GetName();
        if (typeName.size() > 0) {
            tmp1 = boost::regex_replace(entry_tpl, boost::regex("#entry-name#"), typeName);
            tmp1 = boost::regex_replace(tmp1, boost::regex("#entry-type#"), "type");
            tmp = "";

            BOOST_FOREACH(std::string desc, help->getRbHelpEntry().GetTypeHelpEntry().GetDescription()) {
                tmp.append("<p>").append(desc).append("</p>");
            }
            tmp1 = boost::regex_replace(tmp1, boost::regex("#entry-description#"), tmp);
            tmp1 = boost::regex_replace(tmp1, boost::regex("#more-content#"), renderer->renderTypeHelp(help->getRbHelpEntry().GetTypeHelpEntry()));

            type_entry_result += tmp1 + "\n";
            
            // create a manual page
            fstream fs;
            std::string typePage = "html/pages/" + typeName + ".html";
            fs.open(fs::path(helpDir / typePage).string().c_str(), fstream::out | fstream::trunc);
            fs << renderer->renderTypeHelp(help->getRbHelpEntry().GetTypeHelpEntry(), true);
            fs.close();
        }

    }

    // insert entries into main html page
    string index_result = boost::regex_replace(index_tpl, boost::regex("#manual-function-entries#"), function_entry_result);
    index_result = boost::regex_replace(index_result, boost::regex("#manual-type-entries#"), type_entry_result);

    // write out new file content
    fstream fs;
    fs.open(fs::path(helpDir / "html/index.html").string().c_str(), fstream::out | fstream::trunc);
    fs << index_result;
    fs.close();

    cout << "The index.html file is now updated." << endl;

    return 0;
}








