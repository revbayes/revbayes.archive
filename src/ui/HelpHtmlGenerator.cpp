#include "boost/filesystem.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>    
#include <boost/filesystem/fstream.hpp>
#include "boost/algorithm/string_regex.hpp"

#include "pugixml.cpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include "constants.h"

namespace fs = boost::filesystem;
using namespace std;

// read file content

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

    fs::path helpDir(HELP_DIR);

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

    cout << endl << "Generating index.html..." << endl;

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
    
    if(!fs::exists(entry_tpl_file) || !fs::exists(index_tpl_file)){
        std::cout << "Error: One or more of the html template files in help/html directory is missing." 
                << std::endl << "The index html cannot be constructed and program will exit." << std::endl;
        exit(-1);
    }
    
    string entry_tpl = load_file(entry_tpl_file.string());
    string index_tpl = load_file(index_tpl_file.string());

    string entry_result, tmp, tmp1;
    pugi::xml_document xml_doc;
    pugi::xml_parse_result parse_result;
    pugi::xpath_node_set nodeSet;
    

    BOOST_FOREACH(fs::path file, files) {

        // try to load and parse the xml file
        parse_result = xml_doc.load_file((helpDir.string() + "/" + file.string()).c_str(), pugi::parse_default);
        if (parse_result.status != pugi::status_ok) {
            cout << "Warning: failed to parse " << file.string() << ": " << endl << parse_result.description() << endl
                    << "offset: " << parse_result.offset << std::endl;
            continue;
        }

        // parse values from xml and replace the place holders in template
        tmp = xml_doc.child("help_entry").child("name").child_value();
        tmp1 = boost::regex_replace(entry_tpl, boost::regex("#function-name#"), tmp);
                

        // description consists of one or more paragraph that need to be looped
        tmp = "";
        nodeSet = xml_doc.select_nodes("/help_entry/description/p");
        for (pugi::xpath_node_set::const_iterator it = nodeSet.begin(); it != nodeSet.end(); ++it) {
            pugi::xpath_node node = *it;
            tmp += "<p>" + string(node.node().child_value()) + "</p>";
        }
        
        tmp1 = boost::regex_replace(tmp1, boost::regex("#function-description#"), tmp);
        
        tmp1 = boost::regex_replace(tmp1, boost::regex("#function-file#"), file.string());
        
        entry_result += tmp1 + "\n";
    }
    
    // insert entries into main html page
    string index_result(boost::regex_replace(index_tpl, boost::regex("#manual-entries#"), entry_result));

    // write out new file content
    fstream fs;   
    fs.open((helpDir.string() + "/html/index.html").c_str(), fstream::out | fstream::trunc);
    fs << index_result;
    fs.close();
    
    cout << "The index.html file is now updated." << endl;

    return 0;
}








