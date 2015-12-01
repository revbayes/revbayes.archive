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
        std::string                 renderHelp(const RbHelpEntry &entryHelp, size_t w);
        std::string                 renderHelp(const RbHelpFunction &functionHelp, size_t w);
        std::string                 renderHelp(const RbHelpType &typeHelp, size_t w);
        
        
        
    private:
        
        std::string                 renderAliases(const std::vector<std::string> &a, size_t w) const;
        std::string                 renderArguments(const std::vector<RbHelpArgument> &a, size_t w) const;
        std::string                 renderArgument(const RbHelpArgument &a, size_t w, int l, const std::string &s) const;
        std::string                 renderAuthors(const std::vector<std::string> &a, size_t w) const;
        std::string                 renderConstructors(const std::vector<RbHelpFunction>& c, size_t w) const;
        std::string                 renderDescription(const std::vector<std::string> &d, size_t w) const;
        std::string                 renderDetails(const std::vector<std::string> &d, size_t w) const;
        std::string                 renderMethods(const std::vector<RbHelpFunction> &m, size_t w, const std::string &n) const;
        std::string                 renderReferences(const std::vector<RbHelpReference> &r, size_t w) const;
        std::string                 renderSeeAlso(const std::vector<std::string> &s, size_t w) const;
        std::string                 renderUsage(const std::string &u, size_t w) const;
        
        std::string           sectionBreak;
        std::string           lineBreak;
        
    };
    
}

#endif

