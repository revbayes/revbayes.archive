#include "RbHelpRenderer.h"

#include "RbException.h"
#include "RbHelpArgument.h"
#include "RlUserInterface.h"
#include "StringUtilities.h"
#include "TerminalFormatter.h"

using namespace RevBayesCore;

HelpRenderer::HelpRenderer(void)
{
    
    sectionBreak = "\n\n";
    lineBreak = "\n";

}


std::string HelpRenderer::renderAliases(const std::vector<std::string> &aliases, size_t w) const
{
    std::string result = "";
    
    // aliases
    if ( aliases.size() > 0 )
    {
        result.append( TerminalFormatter::makeUnderlined("Aliases") );
        result.append( sectionBreak );
        
        for (std::vector<std::string>::const_iterator it = aliases.begin(); it != aliases.end(); ++it)
        {
            result.append( StringUtilities::formatTabWrap(*it, 1, w) );
            result.append( lineBreak );
        }
        result.append( sectionBreak );
    }
    
    return result;
}


std::string HelpRenderer::renderArguments(const std::vector<RbHelpArgument> &arguments, size_t w) const
{
    std::string result = "";
    
    if ( arguments.size() > 0)
    {
        // check if we have multiple arguments
        if ( arguments.size() == 1 )
        {
            result.append( TerminalFormatter::makeUnderlined("Argument") );
        }
        else
        {
            result.append( TerminalFormatter::makeUnderlined("Arguments") );
        }
        result.append( sectionBreak );
        
        const std::vector<RbHelpArgument>& args = arguments;
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
            
            result.append( renderArgument( arg, w, longestArgLabel, spaces ) );
            
            //            result.append( lineBreak );
            
        }
        
        result.append( sectionBreak );
    }
    
    return result;
}


std::string HelpRenderer::renderArgument(const RevBayesCore::RbHelpArgument &arg, size_t w, int longestArgLabel, const std::string &spaces) const
{
    std::string result = "";
    
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
    
    return result;
}


std::string HelpRenderer::renderAuthors(const std::vector<std::string> &authors, size_t w) const
{
    std::string result = "";
    
    if ( authors.size() > 0 )
    {
        
        result.append( TerminalFormatter::makeUnderlined("Author") );
        result.append( sectionBreak );
    
        for (std::vector<std::string>::const_iterator it = authors.begin(); it != authors.end(); ++it)
        {
            result.append( StringUtilities::formatTabWrap(*it, 1, w, false) );
            result.append( lineBreak );
        }
        result.append( sectionBreak );

    }
    
    return result;
}


std::string HelpRenderer::renderConstructors(const std::vector<RbHelpFunction> &ctors, size_t w) const
{
    std::string result = "";
    
    if ( ctors.size() > 0)
    {
        // check if we have multiple arguments
        if ( ctors.size() == 1 )
        {
            result.append( TerminalFormatter::makeUnderlined("Constructor") );
        }
        else
        {
            result.append( TerminalFormatter::makeUnderlined("Constructors") );
        }
        result.append( sectionBreak );
        
        for (std::vector<RbHelpFunction>::const_iterator it = ctors.begin(); it != ctors.end(); ++it)
        {
            const RbHelpFunction &functionHelp = *it;
            
            result.append( StringUtilities::formatTabWrap(functionHelp.getUsage(),1,w,true) );
            result.append( lineBreak );
            result.append( sectionBreak );
            
            // arguments
            result.append( renderArguments( functionHelp.getArguments(), w ) );
            
        }
        
    }

    
    return result;
}


std::string HelpRenderer::renderDescription(const std::vector<std::string> &descriptions, size_t w) const
{
    std::string result = "";
    
    // descriptions
    if ( descriptions.size() > 0 )
    {
        result.append( TerminalFormatter::makeUnderlined("Description") );
        result.append( sectionBreak );
    
        for (std::vector<std::string>::const_iterator it = descriptions.begin(); it != descriptions.end(); ++it)
        {
            result.append( StringUtilities::formatTabWrap(*it, 1, w) );
            result.append( lineBreak );
            result.append( sectionBreak );
        }
    
    }
    
    return result;
}



std::string HelpRenderer::renderDetails(const std::vector<std::string> &details, size_t w) const
{
    std::string result = "";
    
    // details
    if ( details.size() > 0)
    {
        result.append( TerminalFormatter::makeUnderlined("Details") );
        result.append( sectionBreak );
        
        for (std::vector<std::string>::const_iterator it = details.begin(); it != details.end(); ++it)
        {
            result.append( StringUtilities::formatTabWrap(*it, 1, w) );
            result.append( sectionBreak );
        }
        
        result.append( lineBreak );
    }
    
    return result;
}


std::string HelpRenderer::renderHelp(const RbHelpFunction &functionHelp, size_t w)
{
    
    std::string result = "";
    
    // we do not print the name at the top
    
    // title
    if ( functionHelp.getTitle().size() > 0 )
    {
        result.append( functionHelp.getTitle() );
        result.append( lineBreak );
        result.append( sectionBreak );
    }
    
    // aliases
    result.append( renderAliases( functionHelp.getAliases(), w ) );
    
    // description
    result.append( renderDescription( functionHelp.getDescription(), w ) );

    // usage
    result.append( renderUsage( functionHelp.getUsage(), w ) );
    
    // arguments
    result.append( renderArguments( functionHelp.getArguments(), w ) );
    
    // return value
    result.append( TerminalFormatter::makeUnderlined("Return type") );
    result.append( sectionBreak );
    result.append( StringUtilities::formatTabWrap(functionHelp.getReturnType(), 1, w) );
    result.append( lineBreak );
    result.append( sectionBreak );
    
    
    // details
    result.append( renderDetails( functionHelp.getDetails(), w ) );
    
    // example
    if ( functionHelp.getExample().size() > 0 )
    {
        result.append( TerminalFormatter::makeUnderlined("Example") );
        result.append( lineBreak );
    
        result.append( StringUtilities::formatTabWrap(functionHelp.getExample(), 1, w, false) );
        result.append( lineBreak );
    }
    
    // author
    result.append( renderAuthors( functionHelp.getAuthor(), w ) );
    
    // references
    result.append( renderReferences( functionHelp.getReferences(), w ) );
    
    // see also
    result.append( renderSeeAlso( functionHelp.getSeeAlso(), w ) );
    
    
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
    result.append( renderAliases( typeHelp.getAliases(), w ) );
    
    // description
    result.append( renderDescription( typeHelp.getDescription(), w ) );
    
    // constructors
    result.append( renderConstructors( typeHelp.getConstructors(), w ) );
        
//    // example
//    result.append( TerminalFormatter::makeUnderlined("Example") );
//    result.append( lineBreak );
//            
//    result.append( StringUtilities::formatTabWrap( typeHelp.getExample(), 1, w, false) );
    //    result.append( lineBreak );
    
    // methods
    result.append( renderMethods( typeHelp.getMethods(), w, typeHelp.getName() ) );
    
    // authors
    result.append( renderAuthors( typeHelp.getAuthor(), w ) );
    
    // references
    result.append( renderReferences( typeHelp.getReferences(), w ) );
    
    // see also
    result.append( renderSeeAlso( typeHelp.getSeeAlso(), w ) );
    
    
    return result;
}


std::string HelpRenderer::renderMethods( const std::vector<RbHelpFunction>& methods, size_t w, const std::string &base_name ) const
{
    std::string result = "";
    
    // methods
    if ( methods.size() > 0)
    {
        // check if we have multiple arguments
        if ( methods.size() == 1 )
        {
            result.append( TerminalFormatter::makeUnderlined("Method") );
        }
        else
        {
            result.append( TerminalFormatter::makeUnderlined("Methods") );
        }
        result.append( sectionBreak );
        
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
        
        std::string methodCall = "For more detailed information on methods type, for example, '?" + base_name + "." + methods[0].getName() + "'";
        result.append( StringUtilities::formatTabWrap( methodCall, 1, w) );
        
        result.append( sectionBreak );
    }
    
    
    return result;
}



std::string HelpRenderer::renderReferences(const std::vector<RbHelpReference> &refs, size_t w) const
{
    std::string result = "";
    
    if ( refs.size() > 0 )
    {
        // check if we have multiple references
        if ( refs.size() == 1 )
        {
            result.append( TerminalFormatter::makeUnderlined("Reference") );
        }
        else
        {
            result.append( TerminalFormatter::makeUnderlined("References") );
        }
        result.append( sectionBreak );
        
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

    return result;
}


std::string HelpRenderer::renderSeeAlso(const std::vector<std::string> &others, size_t w) const
{
    std::string result = "";
    
    if ( others.size() > 0 )
    {
        result.append( TerminalFormatter::makeUnderlined("See also") );
        result.append( sectionBreak );
        
        for (std::vector<std::string>::const_iterator it = others.begin(); it != others.end(); ++it)
        {
            result.append( StringUtilities::formatTabWrap(*it, 1, w) );
            result.append( lineBreak );
        }
        result.append(lineBreak);
        
    }

    return result;
}


std::string HelpRenderer::renderUsage(const std::string &usage, size_t w) const
{
    std::string result = "";
    
    if (usage.size() > 0)
    {
        result.append( TerminalFormatter::makeUnderlined("Usage") );
        result.append( sectionBreak );
        
        result.append( StringUtilities::formatTabWrap( usage, 1, w, true) );
        result.append( lineBreak );
        result.append( sectionBreak );
    }
    
    return result;
}
