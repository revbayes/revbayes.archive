/* 
 * File:   HelpParser.h
 * Author: johadunf
 *
 * Created on May 7, 2014, 3:06 PM
 */

#ifndef HELPPARSER_H
#define	HELPPARSER_H

#include <string>
#include "pugixml.hpp"

class HelpParser : public IHelp {
public:

    std::string getHelp(std::string query, int columnWidth) {

    }

    bool isHelpAvailableForQuery(std::string query) {

    }

private:
    pugi::xml_document doc;

    pugi::xml_parse_result loadHelp(std::string qs) {
        // the help file should be all lowercase if to be found
        std::string command = qs;
        std::transform(command.begin(), command.end(), command.begin(), ::tolower);
        std::string helpfile = this->helpDir + command + ".xml";

        // try to load the corresponding xml file
        pugi::xml_parse_result result = doc.load_file(helpfile.c_str(), pugi::parse_default);
        if (result.status != pugi::status_ok) {
            RBOUT(result.description());
        }
        return result;
    }

};

#endif	/* HELPPARSER_H */
