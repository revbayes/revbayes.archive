/* 
 * File:   HelpHtmlRenderer.h
 * Author: johadunf
 *
 * Created on May 12, 2014, 3:34 PM
 */

#ifndef HELPHTMLRENDERER_H
#define	HELPHTMLRENDERER_H

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>    
#include "boost/algorithm/string_regex.hpp"
#include <vector>

const std::string br = "<br />\n";

class HelpHtmlRenderer : public IHelpRenderer {
public:

    std::string renderTypeHelp(TypeHelpEntry typeHelp) {
        std::string result = "";
        std::string tmp = "";

        // title
        //result.append(typeHelp.GetTitle()).append(sectionBreak);

        // name
        result.append(br).append(h2Entry("Name", typeHelp.GetName()));

        // description
        result.append(h2Entry("Description", pEntries(typeHelp.GetDescription())));


        // method members
        if (typeHelp.GetMethodMembers().size() > 0) {
            result.append(tag("Member methods", "h2"));
            result.append(divIndent());
            BOOST_FOREACH(MethodMemberHelpEntry mm, typeHelp.GetMethodMembers()) {
                result.append(tag(mm.GetMethodName(), "b"));
                result.append(divIndent());
                result.append(mm.GetDescription()).append(br).append(br);
                result.append(tag("Usage: ", "span", "argument")).append(mm.GetUsage()).append(br);
                result.append(tag("Method type :", "span", "argument")).append(mm.GetMethodType()).append(br);

                if (mm.GetArguments().size() > 0) {
                    result.append(tag("Arguments", "b")).append(br);

                    BOOST_FOREACH(ArgumentHelpEntry arg, mm.GetArguments()) {
                        result.append(divIndent());
                        result.append(tag(arg.GetLabel(), "b")).append(br);

                        result.append(divIndent());
                        result.append(arg.GetDescription()).append(br);

                        result.append(tag("Argument type: ", "span", "argument")).append(arg.GetArgumentType()).append(br);

                        result.append(tag("Value type: ", "span", "argument")).append(arg.GetValueType()).append(br);

                        if (arg.GetDefaultValue().size() > 0) {
                            result.append(tag("Default value: ", "span", "argument")).append(arg.GetDefaultValue()).append(br);
                        }

                        result.append(divIndent(false)).append(divIndent(false)).append(br);
                    }
                }

                result.append(tag("Return value: ", "span", "argument")).append(mm.GetReturnValue()).append(br);
                result.append(divIndent(false)).append(br);
            }
            result.append(divIndent(false));
        }

        // details
        if (typeHelp.GetDetails().size() > 0) {
            result.append(h2Entry("Details", pEntries(typeHelp.GetDetails())));
        }

        // author
        result.append(h2Entry("Author", typeHelp.GetAuthor()));

        // citation        
        if (typeHelp.GetCitation().size() > 0) {
            result.append(h2Entry("Citation", pEntries(typeHelp.GetCitation())));
        }
        return result;
    }

    std::string renderFunctionHelp(FunctionHelpEntry functionHelp) {
        std::string result = "";

        // name
        result.append(h2Entry("Name", functionHelp.GetName()));

        // title
        //result.append(functionHelp.GetTitle()).append(sectionBreak);

        // description
        result.append(h2Entry("Description", pEntries(functionHelp.GetDescription())));

        // usage
        if (functionHelp.GetUsage().size() > 0) {
            result.append(h2Entry("Usage", functionHelp.GetUsage()));
        }

        // argument
        if (functionHelp.GetArguments().size() > 0) {
            result.append(tag("Arguments", "h2"));
            result.append(divIndent());

            BOOST_FOREACH(ArgumentHelpEntry arg, functionHelp.GetArguments()) {
                result.append(tag(arg.GetLabel(), "b"));
                result.append(divIndent());

                result.append(arg.GetDescription()).append(br).append(br);

                result.append(tag("Argument type: ", "span", "argument")).append(arg.GetArgumentType()).append(br);
                result.append(tag("Value type: ", "span", "argument")).append(arg.GetValueType()).append(br);

                if (arg.GetOptions().size() > 0) {
                    result.append(br);
                    result.append(tag("Options", "b")).append(br);
                    result.append(divIndent());

                    BOOST_FOREACH(std::string option, arg.GetOptions()) {
                        result.append(option).append(br);
                    }
                    result.append(br);
                    result.append(divIndent(false));
                }
                if (arg.GetDefaultValue().size() > 0) {
                    result.append(tag("Default value", "span", "argument")).append(arg.GetDefaultValue()).append(br);
                }
                result.append(divIndent(false)).append(br);
            }
            result.append(divIndent(false));
        }

        // details
        if (functionHelp.GetDetails().size() > 0) {
            result.append(h2Entry("Details", pEntries(functionHelp.GetDetails())));
        }

        // example
        result.append(h2Entry("Example", tag(functionHelp.GetExample(), "pre")));

        // reference
        if (functionHelp.GetReferences().size() > 0) {

            result.append(tag("Reference", "h2"));
                    result.append("<p class=\"indent\">");
                    BOOST_FOREACH(ReferenceHelpEntry ref, functionHelp.GetReferences()) {
                result.append(ref.GetCitation()).append(br);
                        result.append(ref.GetUrl()).append(br);
                        result.append(ref.GetDoi()).append(br);
            }
            result.append("</p>").append(br);
        }

        // author
        result.append(h2Entry("Author", functionHelp.GetAuthor())).append(br);

                // see also        
        if (functionHelp.GetSeeAlso().size() > 0) {

            result.append(tag("See also", "h2"));
                    result.append("<p class=\"indent\">");

                    BOOST_FOREACH(std::string see, functionHelp.GetSeeAlso()) {

                result.append("<a href=\"pages/" + see + ".html\">" + see + "</a>").append(br);
            }
            result.append("</p>").append(br);
        }


        return result;
    }

    void setPageTemplate(std::string pageTemplate) {

        this->pageTemplate = pageTemplate;
    }

    void setTypeEntryTemplate(std::string typeEntryTemplate) {

        this->typeEntryTemplate = typeEntryTemplate;
    }

    void setFunctionEntryTemplate(std::string functionEntryTemplate) {

        this->functionEntryTemplate = functionEntryTemplate;
    }

private:
    std::string pageTemplate;
            std::string typeEntryTemplate;
            std::string functionEntryTemplate;

            std::string h2Entry(std::string name, std::string value) {

        return "<h2>" + name + "</h2>\n<p class=\"indent\">" + value + "</p>\n";
    }

    std::string pEntries(std::vector<std::string> pList, bool indent = true) {
        if (pList.size() == 0) {

            return "";
        }

        std::string tmp = "";

                BOOST_FOREACH(std::string p, pList) {
            if (indent) {
                tmp.append("<p class=\"indent\">").append(p).append("</p>\n");
            } else {

                tmp.append("<p>").append(p).append("</p>\n");
            }
        }
        return tmp;
    }

    std::string divIndent(bool open = true) {
        if (open) {

            return "<div class=\"indent\">\n";
        }
        return "</div>\n";
    }

    std::string tag(std::string s, std::string t, std::string clazz = "") {
        std::string tmp = "";
        if(clazz != ""){
            tmp = "class=\"" + clazz + "\"";
        }
      
        return "<" + t + " " + tmp + ">" + s + "</" + t + ">\n";
    }

};

#endif	/* HELPHTMLRENDERER_H */

