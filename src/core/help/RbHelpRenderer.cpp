#include "RbHelpRenderer.h"

#include "RbHelpArgument.h"
#include "RlUserInterface.h"
#include "StringUtilities.h"
#include "TerminalFormatter.h"

using namespace RevBayesCore;

HelpRenderer::HelpRenderer(void){
    
    sectionBreak = "\n\n";
    lineBreak = "\n";

}

//void HelpRenderer::renderTypeHelp(TypeHelpEntry typeHelp)
//{
//    std::string result = "";
//
//    // name
//    result.append(TerminalFormatter::makeBold(typeHelp.GetName())).append(sectionBreak);
//    // title
//    result.append(typeHelp.GetTitle()).append(sectionBreak);
//
//    // description
//    result.append(TerminalFormatter::makeUnderlined("Description")).append(sectionBreak);
//
//    BOOST_FOREACH(std::string desc, typeHelp.GetDescription()) {
//        result.append(formatTabWrap(desc, 1, w)).append(sectionBreak);
//    }
//
//    // method members
//    if (typeHelp.GetMethodMembers().size() > 0) {
//        result.append(TerminalFormatter::makeUnderlined("Method members")).append(sectionBreak);
//
//        BOOST_FOREACH(MethodMemberHelpEntry mm, typeHelp.GetMethodMembers()) {
//            result.append(formatTabWrap(mm.GetMethodName(), 1, w)).append(lineBreak);
//            result.append(formatTabWrap(mm.GetDescription(), 2, w)).append(sectionBreak);
//            result.append(formatTabWrap("Usage", 2, w)).append(lineBreak).append(formatTabWrap(mm.GetUsage(), 3, w)).append(sectionBreak);
//            result.append(formatTabWrap("Method type", 2, w)).append(lineBreak).append(formatTabWrap(mm.GetMethodType(), 3, w)).append(sectionBreak);
//
//            if (mm.GetArguments().size() > 0) {
//                result.append(formatTabWrap("Arguments", 2, w)).append(sectionBreak);
//
//                BOOST_FOREACH(ArgumentHelpEntry arg, mm.GetArguments()) {
//
//                    result.append(formatTabWrap(arg.GetLabel(), 3, w)).append(lineBreak);
//                    result.append(formatTabWrap(arg.GetDescription(), 4, w)).append(sectionBreak);
//
//                    result.append(formatTabWrap("Argument type", 4, w)).append(lineBreak);
//                    result.append(formatTabWrap(arg.GetArgumentType(), 5, w)).append(sectionBreak);
//
//                    result.append(formatTabWrap("Value type", 4, w)).append(lineBreak);
//                    result.append(formatTabWrap(arg.GetValueType(), 5, w)).append(sectionBreak);
//
//                    if (arg.GetDefaultValue().size() > 0) {
//                        result.append(formatTabWrap("Default value", 4, w)).append(lineBreak);
//                        result.append(formatTabWrap(arg.GetDefaultValue(), 5, w)).append(sectionBreak);
//                    }
//                }
//            }
//
//            result.append(formatTabWrap("Return value", 2, w)).append(lineBreak).append(formatTabWrap(mm.GetReturnValue(), 3, w)).append(sectionBreak);
//        }
//    }
//
//    // details
//    if (typeHelp.GetDetails().size() > 0) {
//        result.append(TerminalFormatter::makeUnderlined("Details")).append(sectionBreak);
//
//        BOOST_FOREACH(std::string det, typeHelp.GetDetails()) {
//            result.append(formatTabWrap(det, 1, w)).append(sectionBreak);
//        }
//    }
//
//    // author
//    result.append(TerminalFormatter::makeUnderlined("Author")).append(sectionBreak).append(formatTabWrap(typeHelp.GetAuthor(), 1, w, false)).append(sectionBreak);
//
//    // citation
//    if (typeHelp.GetCitation().size() > 0) {
//        result.append(TerminalFormatter::makeUnderlined("Citation")).append(sectionBreak);
//
//        BOOST_FOREACH(std::string cit, typeHelp.GetCitation()) {
//            result.append(formatTabWrap(cit, 1, w)).append(sectionBreak);
//        }
//        result.append(lineBreak);
//    }
//    return result;
//
//
//}

std::string HelpRenderer::renderFunctionHelp(const RbHelpFunction &functionHelp, size_t w)
{
    
    std::string result = "";
    
    // we do not print the name at the top
    
    // title
    result.append(functionHelp.getTitle());
    result.append( lineBreak );
    result.append( sectionBreak );
    
    // description
    result.append(TerminalFormatter::makeUnderlined("Description"));
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
        result.append( lineBreak );
        
        result.append( StringUtilities::formatTabWrap(functionHelp.getUsage(), 1, w, false) );
        result.append( lineBreak );
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
        size_t longestArgLabel = 0;
        for (std::vector<RbHelpArgument>::const_iterator it = args.begin(); it != args.end(); ++it)
        {
            if ( longestArgLabel < it->getLabel().size() )
            {
                longestArgLabel = it->getLabel().size();
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
    result.append( StringUtilities::formatTabWrap(functionHelp.getReturnValue(), 1, w) );
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
            
            result.append( StringUtilities::formatTabWrap(ref.getCitation(), 1, w) );
            result.append( lineBreak );
            
            result.append( StringUtilities::formatTabWrap(ref.getUrl(), 1, w) );
            result.append( lineBreak );
            
            result.append( StringUtilities::formatTabWrap(ref.getDoi(), 1, w) );
            result.append( sectionBreak );
        }
        
    }
    
    // author
    result.append( TerminalFormatter::makeUnderlined("Author") );
    result.append( sectionBreak );
    
    result.append( StringUtilities::formatTabWrap(functionHelp.getAuthor(), 1, w, false) );
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