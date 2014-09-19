#ifndef RbHelpParser_H
#define RbHelpParser_H

#include "RbHelpFunction.h"

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
        
        RbHelpFunction                      parseHelpFunction(const std::string &fn);
                
    private:
        
    };
    
    
    
}

#endif

