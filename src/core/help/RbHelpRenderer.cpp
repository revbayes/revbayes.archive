#include "RbHelpRenderer.h"

#include "RbException.h"
#include "RbHelpArgument.h"
#include "RlUserInterface.h"
#include "StringUtilities.h"
#include "TerminalFormatter.h"

using namespace RevBayesCore;

HelpRenderer::HelpRenderer(void){
    
    sectionBreak = "\n\n";
    lineBreak = "\n";

}


std::string HelpRenderer::renderHelp(const RbHelpFunction &functionHelp, size_t w)
{
    
    std::string result = "";
    
    // we do not print the name at the top
    
    // title
    result.append( functionHelp.getTitle() );
    result.append( lineBreak );
    result.append( sectionBreak );
    
    // aliases
    if ( functionHelp.getAliases().size() > 0 )
    {
        result.append( TerminalFormatter::makeUnderlined("Aliases") );
        result.append( sectionBreak );
    
        const std::vector<std::string> & aliases = functionHelp.getAliases();
        for (std::vector<std::string>::const_iterator it = aliases.begin(); it != aliases.end(); ++it)
        {
            result.append( StringUtilities::formatTabWrap(*it, 1, w) );
            result.append( lineBreak );
        }
        result.append( sectionBreak );
    }
    
    // description
    result.append( TerminalFormatter::makeUnderlined("Description") );
    result.append( sectionBreak );
    
    const std::vector<std::string> & descriptions = functionHelp.getDescription();
    for (std::vector<std::string>::const_iterator it = descriptions.begin(); it != descriptions.end(); ++it)
    {
        result.append( StringUtilities::formatTabWrap(*it, 1, w) );
        result.append( lineBreak );
        result.append( sectionBreak );
    }
    
    // usage
    if (functionHelp.getUsage().size() > 0)
    {
        result.append( TerminalFormatter::makeUnderlined("Usage") );
        result.append( sectionBreak );
        
        result.append( StringUtilities::formatTabWrap(functionHelp.getUsage(), 1, w, true) );
        result.append( lineBreak );
        result.append( sectionBreak );
    }
    
    // argument
    if (functionHelp.getArguments().size() > 0)
    {
        // check if we have multiple arguments
        if ( functionHelp.getArguments().size() == 1 )
        {
            result.append( TerminalFormatter::makeUnderlined("Argument") );
        }
        else
        {
            result.append( TerminalFormatter::makeUnderlined("Arguments") );
        }
        result.append( sectionBreak );
        
        const std::vector<RbHelpArgument>& args = functionHelp.getArguments();
        int longestArgLabel = 0;
        for (std::vector<RbHelpArgument>::const_iterator it = args.begin(); it != args.end(); ++it)
        {
            if ( longestArgLabel < it->getLabel().size() )
            {
                longestArgLabel = int(it->getLabel().size());
            }
        }
        
        int indentSize = longestArgLabel + 4 + 3;
        std::string spaces = "";
        for (size_t i = 0; i < indentSize; ++i)
        {
            spaces += " ";
        }
        
        for (std::vector<RbHelpArgument>::const_iterator it = args.begin(); it != args.end(); ++it)
        {
            const RbHelpArgument& arg = *it;

            // label
            std::string l = arg.getLabel();
            StringUtilities::fillWithSpaces(l, longestArgLabel+4, false);
            std::string formattedLabel = StringUtilities::formatStringForScreen(l, "", spaces, w);
            formattedLabel = formattedLabel.substr(0, formattedLabel.size()-1);
            formattedLabel.append( " : " );
            
            // argument description
            formattedLabel.append( arg.getDescription() );
            result.append( StringUtilities::formatStringForScreen(formattedLabel, "", spaces, w) );
//            result.append( lineBreak );
            
            // value type
            result.append( spaces );
            result.append( "Type:       " );
            result.append( arg.getValueType() );
            result.append( ", " );
            result.append( arg.getArgumentDagNodeType() );
            result.append( ", " );
            result.append( arg.getArgumentPassingMethod() );
            result.append( lineBreak );
            
            if ( arg.getDefaultValue().size() > 0 )
            {
                result.append( spaces );
                result.append( "Default:    " );
                result.append( arg.getDefaultValue() );
                result.append( lineBreak );
            }
            
            if (arg.getOptions().size() > 0)
            {
                result.append( spaces );
                result.append( "Options:    " );
                const std::vector<std::string> & options = arg.getOptions();
                for (std::vector<std::string>::const_iterator opt = options.begin(); opt != options.end(); ++opt)
                {
                    // append a '|' separator if we had an option element before
                    if ( opt != options.begin() )
                    {
                        result.append("|");
                    }
                    
                    result.append( *opt );
                    
                }
                result.append( lineBreak );
                
            }
            
//            result.append( lineBreak );
            
        }
        
        result.append( sectionBreak );
    }
    
    // return value
    result.append( TerminalFormatter::makeUnderlined("Return type") );
    result.append( sectionBreak );
    result.append( StringUtilities::formatTabWrap(functionHelp.getReturnType(), 1, w) );
    result.append( lineBreak );
    result.append( sectionBreak );
    
    
    // details
    if ( functionHelp.getDetails().size() > 0)
    {
        result.append( TerminalFormatter::makeUnderlined("Details") );
        result.append( sectionBreak );
        
        const std::vector<std::string> & details = functionHelp.getDetails();
        for (std::vector<std::string>::const_iterator it = details.begin(); it != details.end(); ++it)
        {
            result.append( StringUtilities::formatTabWrap(*it, 1, w) );
            result.append( sectionBreak );
        }
        
        result.append( lineBreak );
    }
    
    // example
    result.append( TerminalFormatter::makeUnderlined("Example") );
    result.append( lineBreak );
    
    result.append( StringUtilities::formatTabWrap(functionHelp.getExample(), 1, w, false) );
    result.append( lineBreak );
    
    // reference
    if ( functionHelp.getReferences().size() > 0 )
    {
        // check if we have multiple references
        if ( functionHelp.getReferences().size() == 1 )
        {
            result.append( TerminalFormatter::makeUnderlined("Reference") );
        }
        else
        {
            result.append( TerminalFormatter::makeUnderlined("References") );
        }
        result.append( sectionBreak );
        
        const std::vector<RbHelpReference>& refs = functionHelp.getReferences();
        for (std::vector<RbHelpReference>::const_iterator it = refs.begin(); it != refs.end(); ++it)
        {
            const RbHelpReference &ref = *it;
            
            if ( ref.getCitation() != "" )
            {
                result.append( StringUtilities::formatTabWrap(ref.getCitation(), 1, w) );
                result.append( lineBreak );
            }
            
            if ( ref.getUrl() != "" )
            {
                result.append( StringUtilities::formatTabWrap(ref.getUrl(), 1, w) );
                result.append( lineBreak );
            }
            
            if ( ref.getDoi() != "" )
            {
                result.append( StringUtilities::formatTabWrap(ref.getDoi(), 1, w) );
                result.append( lineBreak );
            }
            result.append( lineBreak );
            
            
        }
        
        result.append( lineBreak );
        
    }
    
    // author
    result.append( TerminalFormatter::makeUnderlined("Author") );
    result.append( sectionBreak );
    
    const std::vector<std::string> & autors = functionHelp.getAuthor();
    for (std::vector<std::string>::const_iterator it = autors.begin(); it != autors.end(); ++it)
    {
        result.append( StringUtilities::formatTabWrap(*it, 1, w, false) );
        result.append( lineBreak );
    }
    result.append( sectionBreak );
    
    // see also
    if ( functionHelp.getSeeAlso().size() > 0 )
    {
        result.append( TerminalFormatter::makeUnderlined("See also") );
        result.append( sectionBreak );
        
        const std::vector<std::string> & others = functionHelp.getSeeAlso();
        for (std::vector<std::string>::const_iterator it = others.begin(); it != others.end(); ++it)
        {
            result.append( StringUtilities::formatTabWrap(*it, 1, w) );
            result.append( lineBreak );
        }
        result.append(lineBreak);
        
    }
    
    
    return result;
    
}

std::string HelpRenderer::renderHelp(const RbHelpEntry &entryHelp, size_t w)
{
    if ( dynamic_cast< const RbHelpFunction *>( &entryHelp ) != NULL)
    {
        return renderHelp( static_cast<const RbHelpFunction &>( entryHelp ), w);
    }
    else if ( dynamic_cast< const RbHelpType *>( &entryHelp ) != NULL)
    {
        return renderHelp( static_cast<const RbHelpType &>( entryHelp ), w);
    }
    
    throw RbException("Unexpected help type.");
}

std::string HelpRenderer::renderHelp(const RbHelpType &typeHelp, size_t w)
{
    std::string result = "";
    
    // we do not print the name at the top
    
    // title
    result.append( typeHelp.getTitle() );
    result.append( lineBreak );
    result.append( sectionBreak );
    
    // aliases
    if ( typeHelp.getAliases().size() > 0 )
    {
        result.append( TerminalFormatter::makeUnderlined("Aliases") );
        result.append( sectionBreak );
        
        const std::vector<std::string> & aliases = typeHelp.getAliases();
        for (std::vector<std::string>::const_iterator it = aliases.begin(); it != aliases.end(); ++it)
        {
            result.append( StringUtilities::formatTabWrap(*it, 1, w) );
            result.append( lineBreak );
        }
        result.append( sectionBreak );
    }
    
    // description
    result.append( TerminalFormatter::makeUnderlined("Description"));
    result.append( sectionBreak );
    
    const std::vector<std::string> & descriptions = typeHelp.getDescription();
    for (std::vector<std::string>::const_iterator it = descriptions.begin(); it != descriptions.end(); ++it)
    {
        result.append( StringUtilities::formatTabWrap(*it, 1, w) );
        result.append( lineBreak );
        result.append( sectionBreak );
    }
    
    // constructors
    if ( typeHelp.getConstructors().size() > 0)
    {
        // check if we have multiple arguments
        if ( typeHelp.getConstructors().size() == 1 )
        {
            result.append( TerminalFormatter::makeUnderlined("Constructor") );
        }
        else
        {
            result.append( TerminalFormatter::makeUnderlined("Constructors") );
        }
        result.append( sectionBreak );
        
        const std::vector<RbHelpFunction>& ctors = typeHelp.getConstructors();
        for (std::vector<RbHelpFunction>::const_iterator it = ctors.begin(); it != ctors.end(); ++it)
        {
            const RbHelpFunction &functionHelp = *it;
            
            result.append( StringUtilities::formatTabWrap(functionHelp.getUsage(),1,w,true) );
            result.append( lineBreak );
            result.append( sectionBreak );
            
            // argument
            if (functionHelp.getArguments().size() > 0)
            {
                // check if we have multiple arguments
                if ( functionHelp.getArguments().size() == 1 )
                {
                    result.append( TerminalFormatter::makeUnderlined("Argument") );
                }
                else
                {
                    result.append( TerminalFormatter::makeUnderlined("Arguments") );
                }
                result.append( sectionBreak );
                
                const std::vector<RbHelpArgument>& args = functionHelp.getArguments();
                int longestArgLabel = 0;
                for (std::vector<RbHelpArgument>::const_iterator it = args.begin(); it != args.end(); ++it)
                {
                    if ( longestArgLabel < it->getLabel().size() )
                    {
                        longestArgLabel = int(it->getLabel().size());
                    }
                }
                
                size_t indentSize = longestArgLabel + 4 + 3;
                std::string spaces = "";
                for (size_t i = 0; i < indentSize; ++i)
                {
                    spaces += " ";
                }
                
                for (std::vector<RbHelpArgument>::const_iterator it = args.begin(); it != args.end(); ++it)
                {
                    const RbHelpArgument& arg = *it;
                    
                    // label
                    std::string l = arg.getLabel();
                    StringUtilities::fillWithSpaces(l, longestArgLabel+4, false);
                    std::string formattedLabel = StringUtilities::formatStringForScreen(l, "", spaces, w);
                    formattedLabel = formattedLabel.substr(0, formattedLabel.size()-1);
                    formattedLabel.append( " : " );
                    
                    // argument description
                    formattedLabel.append( arg.getDescription() );
                    result.append( StringUtilities::formatStringForScreen(formattedLabel, "", spaces, w) );
                    //            result.append( lineBreak );
                    
                    // value type
                    result.append( spaces );
                    result.append( "Type:       " );
                    result.append( arg.getValueType() );
                    result.append( ", " );
                    result.append( arg.getArgumentDagNodeType() );
                    result.append( ", " );
                    result.append( arg.getArgumentPassingMethod() );
                    result.append( lineBreak );
                    
                    if ( arg.getDefaultValue().size() > 0 )
                    {
                        result.append( spaces );
                        result.append( "Default:    " );
                        result.append( arg.getDefaultValue() );
                        result.append( lineBreak );
                    }
                    
                    if (arg.getOptions().size() > 0)
                    {
                        result.append( spaces );
                        result.append( "Options:    " );
                        const std::vector<std::string> & options = arg.getOptions();
                        for (std::vector<std::string>::const_iterator opt = options.begin(); opt != options.end(); ++opt)
                        {
                            // append a '|' separator if we had an option element before
                            if ( opt != options.begin() )
                            {
                                result.append("|");
                            }
                            
                            result.append( *opt );
                            
                        }
                        result.append( lineBreak );
                        
                    }
                    
                    //            result.append( lineBreak );
                    
                }
                
                result.append( sectionBreak );
            }
            
            // return value
            result.append( TerminalFormatter::makeUnderlined("Value") );
            result.append( sectionBreak );
            result.append( StringUtilities::formatTabWrap(functionHelp.getReturnType(), 1, w) );
            result.append( lineBreak );
            result.append( sectionBreak );
            
            
            // details
            if ( functionHelp.getDetails().size() > 0)
            {
                result.append( TerminalFormatter::makeUnderlined("Details") );
                result.append( sectionBreak );
                
                const std::vector<std::string> & details = functionHelp.getDetails();
                for (std::vector<std::string>::const_iterator it = details.begin(); it != details.end(); ++it)
                {
                    result.append( StringUtilities::formatTabWrap(*it, 1, w) );
                    result.append( sectionBreak );
                }
                
                result.append( lineBreak );
            }
            
            // example
            result.append( TerminalFormatter::makeUnderlined("Example") );
            result.append( lineBreak );
            
            result.append( StringUtilities::formatTabWrap(functionHelp.getExample(), 1, w, false) );
            result.append( lineBreak );
            
            // reference
            if ( functionHelp.getReferences().size() > 0 )
            {
                // check if we have multiple references
                if ( functionHelp.getReferences().size() == 1 )
                {
                    result.append( TerminalFormatter::makeUnderlined("Reference") );
                }
                else
                {
                    result.append( TerminalFormatter::makeUnderlined("References") );
                }
                result.append( sectionBreak );
                
                const std::vector<RbHelpReference>& refs = functionHelp.getReferences();
                for (std::vector<RbHelpReference>::const_iterator it = refs.begin(); it != refs.end(); ++it)
                {
                    const RbHelpReference &ref = *it;
                    
                    if ( ref.getCitation() != "" )
                    {
                        result.append( StringUtilities::formatTabWrap(ref.getCitation(), 1, w) );
                        result.append( lineBreak );
                    }
                    
                    if ( ref.getUrl() != "" )
                    {
                        result.append( StringUtilities::formatTabWrap(ref.getUrl(), 1, w) );
                        result.append( lineBreak );
                    }
                    
                    if ( ref.getDoi() != "" )
                    {
                        result.append( StringUtilities::formatTabWrap(ref.getDoi(), 1, w) );
                        result.append( lineBreak );
                    }
                    result.append( lineBreak );
                    
                    
                }
                
                result.append( lineBreak );
                
            }
            
            // author
            result.append( TerminalFormatter::makeUnderlined("Author") );
            result.append( sectionBreak );
            
            const std::vector<std::string> & autors = functionHelp.getAuthor();
            for (std::vector<std::string>::const_iterator it = autors.begin(); it != autors.end(); ++it)
            {
                result.append( StringUtilities::formatTabWrap(*it, 1, w, false) );
                result.append( lineBreak );
            }
            result.append( sectionBreak );
            
            // see also
            if ( functionHelp.getSeeAlso().size() > 0 )
            {
                result.append( TerminalFormatter::makeUnderlined("See also") );
                result.append( sectionBreak );
                
                const std::vector<std::string> & others = functionHelp.getSeeAlso();
                for (std::vector<std::string>::const_iterator it = others.begin(); it != others.end(); ++it)
                {
                    result.append( StringUtilities::formatTabWrap(*it, 1, w) );
                    result.append( lineBreak );
                }
                result.append(lineBreak);
                
            }


        }
        
        result.append( lineBreak );
    }
    
    // methods
    if ( typeHelp.getMethods().size() > 0)
    {
        // check if we have multiple arguments
        if ( typeHelp.getMethods().size() == 1 )
        {
            result.append( TerminalFormatter::makeUnderlined("Method") );
        }
        else
        {
            result.append( TerminalFormatter::makeUnderlined("Methods") );
        }
        result.append( sectionBreak );
        
        const std::vector<RbHelpFunction>& methods = typeHelp.getMethods();
        for (std::vector<RbHelpFunction>::const_iterator it = methods.begin(); it != methods.end(); ++it)
        {
            result.append( StringUtilities::formatTabWrap(it->getUsage(),1,w,true) );
            result.append( lineBreak );
            
            const std::vector<std::string> & d = it->getDescription();
            for (std::vector<std::string>::const_iterator desc_it = d.begin(); desc_it != d.end(); ++desc_it)
            {
                result.append( StringUtilities::formatTabWrap(*desc_it, 2, w) );
                result.append( sectionBreak );
            }
            
        }
        
        result.append( lineBreak );
        
        std::string methodCall = "For more detailed information on methods type, for example, '?" + typeHelp.getName() + "." + methods[0].getName() + "'";
        result.append( StringUtilities::formatTabWrap( methodCall, 1, w) );

        result.append( sectionBreak );
    }
    
    
    return result;

}
