#ifndef RbHelpRenderer_H
#define RbHelpRenderer_H

#include "RbHelpFunction.h"

namespace RevBayesCore {
    
    /**
     * @brief Class rendering the help to the console output.
     *
     * The UserFunction class holds information about a user function and is used both to execute
     * the function and make deterministic variables using the function.
     *
     * \copyright (c) Copyright 2009-2014 (GPL version 3)
     * \author The RevBayes Development Core Team (Sebastian Hoehna)
     * \since Version 1.0, 2014-09-15
     */
    class HelpRenderer {
        
    public:
        
        //        void renderTypeHelp(TypeHelpEntry typeHelp);
        std::string                 renderFunctionHelp(const RbHelpFunction &functionHelp);
        
        
    private:
        
        
        const std::string           sectionBreak = "\n\n";
        const std::string           lineBreak = "\n";
        
    };
    
}

#endif

