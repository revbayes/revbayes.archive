#ifndef RbHelpFunction_H
#define RbHelpFunction_H

#include "RbHelpArgument.h"
#include "RbHelpEntry.h"

#include <string>
#include <vector>

namespace RevBayesCore {
    
    
    /**
     * \brief The help entry for a function call.
     *
     * A help entry for a function call provides all the help information available.
     *
     * \copyright (c) Copyright 2009-2013 (GPL version 3)
     * \author The RevBayes Development Core Team (Johan Dunfalk & Sebastian Hoehna)
     * \since Version 1.0, 2014-09-15
     *
     */
    class RbHelpFunction : public RbHelpEntry {
        
    public:
        
        RbHelpFunction();
        
        // getter
        const std::vector<RbHelpArgument>&          getArguments(void) const;
        const std::string&                          getExample(void) const;
        const std::string&                          getReturnType(void) const;
        const std::string&                          getUsage(void) const;

        // setters
        void                                        setArguments(const std::vector<RbHelpArgument> &a);
        void                                        setExample(const std::string &e);
        void                                        setReturnType(const std::string &r);
        void                                        setUsage(const std::string &u);
        
        
    private:
        
        std::vector<RbHelpArgument>                 arguments;
        std::string                                 author;
        std::vector<std::string>                    details;
        std::string                                 example;
        std::vector<RbHelpReference>                references;
        std::string                                 returnType;
        std::vector<std::string>                    seeAlso;
        std::string                                 usage;
        
        
    };

    
}

#endif

