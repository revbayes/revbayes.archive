#include "RbHelpRenderer.h"

#include "RbHelpArgument.h"
#include "RlUserInterface.h"
#include "StringUtilities.h"
#include "TerminalFormatter.h"

using namespace RevBayesCore;

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

std::string HelpRenderer::renderFunctionHelp(const RbHelpFunction &functionHelp)
{
    
    int w = 80;
    std::string result = "";
    
    // name
    result.append(TerminalFormatter::makeBold(functionHelp.getName()));
    result.append(sectionBreak);
    
    // title
    result.append(functionHelp.getTitle());
    result.append(sectionBreak);
    
    // description
    result.append(TerminalFormatter::makeUnderlined("Description"));
    result.append(sectionBreak);
    
    const std::vector<std::string> & descriptions = functionHelp.getDescription();
    for (std::vector<std::string>::const_iterator it = descriptions.begin(); it != descriptions.end(); ++it)
    {
        result.append( StringUtilities::formatTabWrap(*it, 1, w) );
        result.append( sectionBreak );
    }
    
    // usage
    if (functionHelp.getUsage().size() > 0)
    {
        result.append(TerminalFormatter::makeUnderlined("Usage"));
        result.append(sectionBreak);
        
        result.append(StringUtilities::formatTabWrap(functionHelp.getUsage(), 1, w));
        result.append(sectionBreak);
    }
    
    // argument
    if (functionHelp.getArguments().size() > 0)
    {
        result.append(TerminalFormatter::makeUnderlined("Arguments"));
        result.append(sectionBreak);
        
        const std::vector<RbHelpArgument>& args = functionHelp.getArguments();
        for (std::vector<RbHelpArgument>::const_iterator it = args.begin(); it != args.end(); ++it)
        {
            const RbHelpArgument& arg = *it;
            result.append( StringUtilities::formatTabWrap(arg.getLabel(), 1, w) );
            result.append( lineBreak );
            
            result.append( StringUtilities::formatTabWrap(arg.getDescription(), 2, w) );
            result.append( sectionBreak );
            
            result.append( StringUtilities::formatTabWrap("Argument type", 2, w) );
            result.append( lineBreak );
            
            result.append( StringUtilities::formatTabWrap(arg.getArgumentType(), 3, w) );
            result.append (sectionBreak );
            
            result.append( StringUtilities::formatTabWrap("Value type", 2, w) );
            result.append( lineBreak );
            
            result.append( StringUtilities::formatTabWrap(arg.getValueType(), 3, w) );
            result.append( sectionBreak );
            
            if (arg.getOptions().size() > 0)
            {
                result.append( StringUtilities::formatTabWrap("Options", 2, w) );
                result.append(lineBreak);
                
                const std::vector<std::string> & options = arg.getOptions();
                for (std::vector<std::string>::const_iterator opt = options.begin(); opt != options.end(); ++opt)
                {
                    result.append( StringUtilities::formatTabWrap(*opt, 3, w) );
                    result.append(lineBreak);
                }
                result.append(lineBreak);
                
            }
            
            
            if (arg.getDefaultValue().size() > 0)
            {
                result.append( StringUtilities::formatTabWrap("Default value", 2, w) );
                result.append( lineBreak );
                
                result.append( StringUtilities::formatTabWrap(arg.getDefaultValue(), 3, w) );
                result.append( sectionBreak );
            }
            
        }
    }
    
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
        
    }
    
    // example
    result.append( TerminalFormatter::makeUnderlined("Example") );
    result.append( sectionBreak );
    
    result.append( StringUtilities::formatTabWrap(functionHelp.getExample(), 1, w, false) );
    result.append( sectionBreak );
    
    // reference
    if ( functionHelp.getReferences().size() > 0 )
    {
        result.append( TerminalFormatter::makeUnderlined("Reference") );
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