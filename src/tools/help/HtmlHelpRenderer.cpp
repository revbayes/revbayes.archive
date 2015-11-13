#include "HtmlHelpRenderer.h"
#include "RbHelpArgument.h"
#include "RbHelpFunction.h"
#include "RbHelpReference.h"
#include "StringUtilities.h"

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>




std::string HtmlHelpRenderer::divIndent(bool open)
{
    if (open) {
        
        return "<div class=\"indent\">\n";
    }
    return "</div>\n";
}

std::string HtmlHelpRenderer::h2Entry(const std::string &name, const std::string &value)
{
    
    return "<h2>" + name + "</h2>\n<p class=\"indent\">" + value + "</p>\n";
}

std::string HtmlHelpRenderer::pEntries(const std::vector<std::string> &pList, bool indent)
{
    if (pList.size() == 0)
    {
        
        return "";
    }
    
    std::string tmp = "";
    
    BOOST_FOREACH(std::string p, pList)
    {
        if (indent)
        {
            tmp.append("<p class=\"indent\">").append(p).append("</p>\n");
        }
        else
        {
            
            tmp.append("<p>").append(p).append("</p>\n");
        }
    }
    
    return tmp;
}


std::string HtmlHelpRenderer::renderFunctionHelp(const RevBayesCore::RbHelpFunction &functionHelp, bool applyTemplate)
{
    std::string ft = functionTemplate;
    if (!applyTemplate)
    {
        return renderFunctionHelp(functionHelp);
    }
    std::string title = functionHelp.getTitle();
    std::string name = functionHelp.getName();
    
    StringUtilities::replaceSubstring(ft, "#title#", title);
    StringUtilities::replaceSubstring(ft, "#name#", name);
    StringUtilities::replaceSubstring(ft, "#content#", renderFunctionHelp(functionHelp));
    
    return ft;
}

std::string HtmlHelpRenderer::renderFunctionHelp(const RevBayesCore::RbHelpFunction &functionHelp)
{

    std::string result = "";
    
    // name
    //result.append("<span onClick=\"navigateHelp('"+functionHelp.GetName()+"');\">");
    result.append(br);
    result.append( h2Entry("Name", functionHelp.getName()) );
    //result.append("</span>");
    
    // title
    //result.append(functionHelp.GetTitle()).append(sectionBreak);
    
    // description
    result.append( h2Entry("Description", pEntries(functionHelp.getDescription())) );
    
    // usage
    if (functionHelp.getUsage().size() > 0) {
        result.append(h2Entry("Usage", functionHelp.getUsage()));
    }
    
    // argument
    if (functionHelp.getArguments().size() > 0) {
        
        result.append(tag("Arguments", "h2"));
        result.append(divIndent());
        
        BOOST_FOREACH(RevBayesCore::RbHelpArgument arg, functionHelp.getArguments())
        {
            result.append(tag(arg.getLabel(), "b"));
            result.append(divIndent());
            
            result.append(arg.getDescription()).append(br).append(br);
            
            result.append(tag("Argument type: ", "span", "argument")).append(arg.getArgumentPassingMethod()).append(br);
            result.append(tag("Value type: ", "span", "argument")).append(arg.getValueType()).append(br);
            
            if (arg.getOptions().size() > 0)
            {
                
                result.append(br);
                result.append(tag("Options", "b")).append(br);
                result.append(divIndent());
                
                BOOST_FOREACH(std::string option, arg.getOptions())
                {
                    result.append(option).append(br);
                }
                result.append(br);
                result.append(divIndent(false));
            }
            if (arg.getDefaultValue().size() > 0)
            {
                result.append(tag("Default value", "span", "argument")).append(arg.getDefaultValue()).append(br);
            }
            result.append(divIndent(false)).append(br);
        }
        result.append(divIndent(false));
    }
    
    // details
    if (functionHelp.getDetails().size() > 0)
    {
        result.append(h2Entry("Details", pEntries(functionHelp.getDetails())));
    }
    
    // example
    result.append(h2Entry("Example", tag(functionHelp.getExample(), "pre")));
    
    // reference
    if (functionHelp.getReferences().size() > 0)
    {
        
        result.append(tag("Reference", "h2"));
        result.append("<p class=\"indent\">");
        
        BOOST_FOREACH(RevBayesCore::RbHelpReference ref, functionHelp.getReferences())
        {
            result.append(ref.getCitation()).append(br);
            result.append(ref.getUrl()).append(br);
            result.append(ref.getDoi()).append(br);
        }
        result.append("</p>").append(br);
    }
    
    // author
    if (functionHelp.getAuthor().size() == 1)
    {
        result.append(h2Entry("Author", functionHelp.getAuthor()[0]));
    }
    else if (functionHelp.getAuthor().size() > 1)
    {
        result.append(h2Entry("Authors", pEntries(functionHelp.getAuthor())));
    }
    
    
    // see also
    if (functionHelp.getSeeAlso().size() > 0)
    {
        
        result.append(tag("See also", "h2"));
        result.append("<p class=\"indent\">");
        
        BOOST_FOREACH(std::string see, functionHelp.getSeeAlso()) {
            
            //result.append("<a href=\"pages/" + see + ".html\">" + see + "</a>").append(br);
            result.append("<a href=\"\" onClick=\"navigateHelp('" + see + "'); return false;\">" + see + "</a>").append(br);
        }
        result.append("</p>").append(br);
    }
    
    
    return result;
}


std::string HtmlHelpRenderer::renderTypeHelp(const RevBayesCore::RbHelpType &typeHelp, bool applyTemplate)
{
    
    if (!applyTemplate)
    {
        return renderTypeHelp(typeHelp);
    }
    std::string title = typeHelp.getTitle();
    std::string name = typeHelp.getName();
    
    std::string tt = typeTemplate;
    StringUtilities::replaceSubstring(tt, "#title#", title);
    StringUtilities::replaceSubstring(tt, "#name#", name);
    StringUtilities::replaceSubstring(tt, "#content#", renderTypeHelp(typeHelp));
    
    return tt;
}

std::string HtmlHelpRenderer::renderTypeHelp(const RevBayesCore::RbHelpType &typeHelp)
{
    std::string result = "";
    std::string tmp = "";
    
    // title
    //result.append(typeHelp.GetTitle()).append(sectionBreak);
    
    // name
    //result.append("<span onClick=\"navigateHelp('"+typeHelp.GetName()+"');\">");
    result.append(br).append(h2Entry("Name", typeHelp.getName()));
    //result.append("</span>");
    
    // description
    result.append(h2Entry("Description", pEntries(typeHelp.getDescription())));
    
    // constructors
    if (typeHelp.getConstructors().size() > 0)
    {
        result.append(tag("Constructors", "h2"));
        result.append(divIndent());
        
        BOOST_FOREACH(RevBayesCore::RbHelpFunction mm, typeHelp.getConstructors())
        {
            
            result.append(renderFunctionHelp(mm));
        }
        
        result.append(divIndent(false));
    }
    
    // method members
    if (typeHelp.getMethods().size() > 0)
    {
        result.append(tag("Methods", "h2"));
        result.append(divIndent());

        BOOST_FOREACH(RevBayesCore::RbHelpFunction mm, typeHelp.getMethods())
        {
            
            result.append(renderFunctionHelp(mm));
//            result.append(tag(mm.getName(), "b"));
//            result.append(divIndent());
//            result.append(mm.getDescription()).append(br).append(br);
//            result.append(tag("Usage: ", "span", "argument")).append(mm.GetUsage()).append(br);
//            result.append(tag("Method type :", "span", "argument")).append(mm.GetMethodType()).append(br);
//            
//            if (mm.GetArguments().size() > 0) {
//                result.append(tag("Arguments", "b")).append(br);
//                
//                BOOST_FOREACH(ArgumentHelpEntry arg, mm.GetArguments()) {
//                    result.append(divIndent());
//                    result.append(tag(arg.GetLabel(), "b")).append(br);
//                    
//                    result.append(divIndent());
//                    result.append(arg.GetDescription()).append(br);
//                    
//                    result.append(tag("Argument type: ", "span", "argument")).append(arg.GetArgumentType()).append(br);
//                    
//                    result.append(tag("Value type: ", "span", "argument")).append(arg.GetValueType()).append(br);
//                    
//                    if (arg.GetDefaultValue().size() > 0) {
//                        result.append(tag("Default value: ", "span", "argument")).append(arg.GetDefaultValue()).append(br);
//                    }
//                    
//                    result.append(divIndent(false)).append(divIndent(false)).append(br);
//                }
//            }
//            
//            result.append(tag("Return value: ", "span", "argument")).append(mm.GetReturnValue()).append(br);
//            result.append(divIndent(false)).append(br);
        }
        result.append(divIndent(false));
    }
    
    // details
    if (typeHelp.getDetails().size() > 0)
    {
        result.append(h2Entry("Details", pEntries(typeHelp.getDetails())));
    }
    
    // author
    if (typeHelp.getAuthor().size() == 1)
    {
        result.append(h2Entry("Author", typeHelp.getAuthor()[0]));
    }
    else if (typeHelp.getAuthor().size() > 1)
    {
        result.append(h2Entry("Authors", pEntries(typeHelp.getAuthor())));
    }
    
    // citation
    if (typeHelp.getReferences().size() > 0)
    {
        result.append(tag("Reference", "h2"));
        result.append("<p class=\"indent\">");
        
        BOOST_FOREACH(RevBayesCore::RbHelpReference ref, typeHelp.getReferences())
        {
            result.append(ref.getCitation()).append(br);
            result.append(ref.getUrl()).append(br);
            result.append(ref.getDoi()).append(br);
        }
        result.append("</p>").append(br);
    }
    
    return result;
}


void HtmlHelpRenderer::setDistributionTemplate(const std::string &t)
{
    // replace the internal value by this new value
    distributionTemplate = t;
}


void HtmlHelpRenderer::setMonitorTemplate(const std::string &t)
{
    // replace the internal value by this new value
    monitorTemplate = t;
}


void HtmlHelpRenderer::setMoveTemplate(const std::string &t)
{
    // replace the internal value by this new value
    moveTemplate = t;
}


void HtmlHelpRenderer::setTypeTemplate(const std::string &t)
{
    // replace the internal value by this new value
    typeTemplate = t;
}

void HtmlHelpRenderer::setFunctionTemplate(const std::string &t)
{
    // replace the internal value by this new value
    functionTemplate = t;
}


std::string HtmlHelpRenderer::tag(const std::string &s, const std::string &t, const std::string &clazz)
{
    std::string tmp = "";
    if (clazz != "")
    {
        
        tmp = "class=\"" + clazz + "\"";
    }
    
    return "<" + t + " " + tmp + ">" + s + "</" + t + ">\n";
}
