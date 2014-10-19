#ifndef RbHelpParser_H
#define RbHelpParser_H

#include "RbHelpFunction.h"
#include "RbHelpType.h"

#include "pugixml.hpp"

#include <string>
#include <vector>

namespace RevBayesCore {
    
    
    /**
     * \brief The class parsing in out help system from xml.
     *
     * The help parser creates help entries from a xml file and returns the internal
     * help entry data structure.
     *
     * \copyright (c) Copyright 2009-2013 (GPL version 3)
     * \author The RevBayes Development Core Team (Johan Dunfalk & Sebastian Hoehna)
     * \since Version 1.0, 2014-09-15
     *
     */
    class RbHelpParser {
        
    public:
        
        enum HelpEntryType { DISTRIBUTION, FUNCTION, MONITOR, MOVE, TYPE };

        RbHelpType*                         parseHelpDistribution(const std::string &fn);
        RbHelpFunction                      parseHelpFunction(const std::string &fn);
        RbHelpType*                         parseHelpMonitor(const std::string &fn);
        RbHelpType*                         parseHelpMove(const std::string &fn);
        RbHelpType*                         parseHelpType(const std::string &fn);
        HelpEntryType                       testHelpEntry(const std::string &fn);
                
    private:

        RbHelpFunction                      parseInternalHelpFunction(const pugi::xpath_node &node);
        void                                parseInternalHelpType(const pugi::xpath_node &node, RbHelpType *helpEntry);
        
    };
    
    
    
}

#endif

