#ifndef RbHelpRenderer_H
#define RbHelpRenderer_H

#include "RbHelpFunction.h"
#include "RbHelpType.h"

namespace RevBayesCore {
    
    /**
     * @brief Class rendering the help to the console output.
     *
     * The Help renderer converts the help entry into a nice format that can be printed directly to the console output.
     *
     * \copyright (c) Copyright 2009-2014 (GPL version 3)
     * \author The RevBayes Development Core Team (Johan Dunfalk & Sebastian Hoehna)
     * \since Version 1.0, 2014-09-15
     */
    class HelpRenderer {
        
    public:
        
        HelpRenderer(void);
        std::string                 renderFunctionHelp(const RbHelpFunction &functionHelp, size_t w);
//        std::string                 renderTypeHelp(const RbHelpType &typeHelp, size_t w);
        
        
        
    private:
        
        
        std::string           sectionBreak;
        std::string           lineBreak;
        
    };
    
}

#endif

