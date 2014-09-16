#ifndef RbHelpFunction_H
#define RbHelpFunction_H

#include "RbHelpArgument.h"
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
     * \author The RevBayes Development Core Team (Sebastian Hoehna)
     * \since Version 1.0, 2014-09-15
     *
     */
    class RbHelpFunction {
    public:
        
        // getter
        const std::vector<RbHelpArgument>&          getArguments(void) const;
        const std::string&                          getAuthor(void) const;
        const std::vector<std::string>&             getDescription(void) const;
        const std::vector<std::string>&             getDetails(void) const;
        const std::string&                          getExample(void) const;
        const std::string&                          getName(void) const;
        const std::vector<RbHelpReference>&         getReferences(void) const;
        const std::string&                          getReturnValue(void) const;
        const std::vector<std::string>&             getSeeAlso(void) const;
        const std::string&                          getTitle(void) const;
        const std::string&                          getUsage(void) const;

        // setters
        void                                        setArguments(const std::vector<RbHelpArgument> &a);
        void                                        setAuthor(const std::string &a);
        void                                        setDescription(const std::vector<std::string> &d);
        void                                        setDetails(const std::vector<std::string> &d);
        void                                        setExample(const std::string &e);
        void                                        setName(const std::string &n);
        void                                        setSeeAlso(const std::vector<std::string> &s);
        void                                        setReferences(const std::vector<RbHelpReference> &r);
        void                                        setReturnValue(const std::string &r);
        void                                        setTitle(const std::string &t);
        void                                        setUsage(const std::string &u);
        
        
    private:
        
        std::vector<RbHelpArgument>                 arguments;
        std::string                                 author;
        std::vector<std::string>                    description;
        std::vector<std::string>                    details;
        std::string                                 example;
        std::string                                 name;
        std::vector<RbHelpReference>                references;
        std::string                                 returnValue;
        std::vector<std::string>                    seeAlso;
        std::string                                 title;
        std::string                                 usage;
        
        
    };

    
}

#endif

