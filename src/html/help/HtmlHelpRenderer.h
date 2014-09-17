#ifndef HtmlHelpRenderer_H
#define HtmlHelpRenderer_H

#include "RbHelpFunction.h"
#include "RbHelpType.h"

#include <string>

/**
 * @brief Class rendering the help to files in HTML format.
 *
 * The HTML-Help renderer converts the help entry into HTML files.
 *
 * \copyright (c) Copyright 2009-2014 (GPL version 3)
 * \author The RevBayes Development Core Team (Johan Dunfalk & Sebastian Hoehna)
 * \since Version 1.0, 2014-09-15
 */
class HtmlHelpRenderer {
        
public:
        
    std::string                 renderTypeHelp(const RevBayesCore::RbHelpType &typeHelp);
    std::string                 renderTypeHelp(const RevBayesCore::RbHelpType &typeHelp, bool applyTemplate);
    std::string                 renderFunctionHelp(const RevBayesCore::RbHelpFunction &functionHelp);
    std::string                 renderFunctionHelp(const RevBayesCore::RbHelpFunction &functionHelp, bool applyTemplate);
    
    void                        setTypeTemplate(const std::string &t);
    void                        setFunctionTemplate(const std::string &t);

        
private:
    
    // helper methods
    std::string                 divIndent(bool open = true);
    std::string                 h2Entry(const std::string &name, const std::string &value);
    std::string                 pEntries(const std::vector<std::string> &pList, bool indent = true);
    std::string                 tag(const std::string &s, const std::string &t, const std::string &clazz = "");

    
    std::string                 typeTemplate;
    std::string                 functionTemplate;
    
    const std::string           br = "<br />\n";
};
    

#endif

