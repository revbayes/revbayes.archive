#ifndef RbHelpFunction_H
#define RbHelpFunction_H

#include "RbHelpArgument.h"
#include "RbHelpEntry.h"
#include "RbHelpReference.h"

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
        const std::vector<std::string>&             getAliases(void) const;
        const std::vector<RbHelpArgument>&          getArguments(void) const;
        const std::string&                          getAuthor(void) const;
        const std::vector<std::string>&             getDetails(void) const;
        const std::string&                          getExample(void) const;
        const std::vector<RbHelpReference>&         getReferences(void) const;
        const std::string&                          getReturnValue(void) const;
        const std::vector<std::string>&             getSeeAlso(void) const;
        const std::string&                          getUsage(void) const;

        // setters
        void                                        setAliases(const std::vector<std::string> &a);
        void                                        setArguments(const std::vector<RbHelpArgument> &a);
        void                                        setAuthor(const std::string &a);
        void                                        setDetails(const std::vector<std::string> &d);
        void                                        setExample(const std::string &e);
        void                                        setSeeAlso(const std::vector<std::string> &s);
        void                                        setReferences(const std::vector<RbHelpReference> &r);
        void                                        setReturnValue(const std::string &r);
        void                                        setUsage(const std::string &u);
        
        
    private:
        
        std::vector<std::string>                    aliases;
        std::vector<RbHelpArgument>                 arguments;
        std::string                                 author;
        std::vector<std::string>                    details;
        std::string                                 example;
        std::vector<RbHelpReference>                references;
        std::string                                 returnValue;
        std::vector<std::string>                    seeAlso;
        std::string                                 usage;
        
        
    };

    
}

#endif

