/* 
 * File:   HelpConsoleRenderer.h
 * Author: johadunf
 *
 * Created on May 12, 2014, 2:42 PM
 */

#ifndef HELPCONSOLERENDERER_H
#define	HELPCONSOLERENDERER_H

#include "IHelpRenderer.h"
#include <boost/foreach.hpp>
#include "TerminalFormatter.h"

class HelpConsoleRenderer : public IHelpRenderer {
public:

    std::string renderTypeHelp(TypeHelpEntry typeHelp) {
        std::string result = "";

        // name
        result.append(TerminalFormatter::makeBold(typeHelp.GetName())).append(sectionBreak);
        // title
        result.append(typeHelp.GetTitle()).append(sectionBreak);

        // description
        result.append(TerminalFormatter::makeUnderlined("Description")).append(sectionBreak);

        BOOST_FOREACH(std::string desc, typeHelp.GetDescription()) {
            result.append(formatTabWrap(desc, 1, w)).append(sectionBreak);
        }

        // method members
        if (typeHelp.GetMethodMembers().size() > 0) {
            result.append(TerminalFormatter::makeUnderlined("Method members")).append(sectionBreak);

            BOOST_FOREACH(MethodMemberHelpEntry mm, typeHelp.GetMethodMembers()) {
                result.append(formatTabWrap(mm.GetMethodName(), 1, w)).append(lineBreak);
                result.append(formatTabWrap(mm.GetDescription(), 2, w)).append(sectionBreak);
                result.append(formatTabWrap("Usage", 2, w)).append(lineBreak).append(formatTabWrap(mm.GetUsage(), 3, w)).append(sectionBreak);
                result.append(formatTabWrap("Method type", 2, w)).append(lineBreak).append(formatTabWrap(mm.GetMethodType(), 3, w)).append(sectionBreak);

                if (mm.GetArguments().size() > 0) {
                    result.append(formatTabWrap("Arguments", 2, w)).append(sectionBreak);

                    BOOST_FOREACH(ArgumentHelpEntry arg, mm.GetArguments()) {

                        result.append(formatTabWrap(arg.GetLabel(), 3, w)).append(lineBreak);
                        result.append(formatTabWrap(arg.GetDescription(), 4, w)).append(sectionBreak);

                        result.append(formatTabWrap("Argument type", 4, w)).append(lineBreak);
                        result.append(formatTabWrap(arg.GetArgumentType(), 5, w)).append(sectionBreak);

                        result.append(formatTabWrap("Value type", 4, w)).append(lineBreak);
                        result.append(formatTabWrap(arg.GetValueType(), 5, w)).append(sectionBreak);

                        if (arg.GetDefaultValue().size() > 0) {
                            result.append(formatTabWrap("Default value", 4, w)).append(lineBreak);
                            result.append(formatTabWrap(arg.GetDefaultValue(), 5, w)).append(sectionBreak);
                        }
                    }
                }

                result.append(formatTabWrap("Return value", 2, w)).append(lineBreak).append(formatTabWrap(mm.GetReturnValue(), 3, w)).append(sectionBreak);
            }
        }

        // details
        if (typeHelp.GetDetails().size() > 0) {
            result.append(TerminalFormatter::makeUnderlined("Details")).append(sectionBreak);

            BOOST_FOREACH(std::string det, typeHelp.GetDetails()) {
                result.append(formatTabWrap(det, 1, w)).append(sectionBreak);
            }
        }

        // author
        result.append(TerminalFormatter::makeUnderlined("Author")).append(sectionBreak).append(formatTabWrap(typeHelp.GetAuthor(), 1, w, false)).append(sectionBreak);

        // citation        
        if (typeHelp.GetCitation().size() > 0) {
            result.append(TerminalFormatter::makeUnderlined("Citation")).append(sectionBreak);

            BOOST_FOREACH(std::string cit, typeHelp.GetCitation()) {
                result.append(formatTabWrap(cit, 1, w)).append(sectionBreak);
            }
            result.append(lineBreak);
        }
        return result;


    }

    std::string renderFunctionHelp(FunctionHelpEntry functionHelp) {
        std::string result = "";

        // name
        result.append(TerminalFormatter::makeBold(functionHelp.GetName())).append(sectionBreak);
        // title
        result.append(functionHelp.GetTitle()).append(sectionBreak);
        // description
        result.append(TerminalFormatter::makeUnderlined("Description")).append(sectionBreak);

        BOOST_FOREACH(std::string desc, functionHelp.GetDescription()) {
            result.append(formatTabWrap(desc, 1, w)).append(sectionBreak);
        }

        // usage
        if (functionHelp.GetUsage().size() > 0) {
            result.append(TerminalFormatter::makeUnderlined("Usage")).append(sectionBreak).append(formatTabWrap(functionHelp.GetUsage(), 1, w)).append(sectionBreak);
        }

        // argument
        if (functionHelp.GetArguments().size() > 0) {
            result.append(TerminalFormatter::makeUnderlined("Arguments")).append(sectionBreak);

            BOOST_FOREACH(ArgumentHelpEntry arg, functionHelp.GetArguments()) {

                result.append(formatTabWrap(arg.GetLabel(), 1, w)).append(lineBreak);
                result.append(formatTabWrap(arg.GetDescription(), 2, w)).append(sectionBreak);

                result.append(formatTabWrap("Argument type", 2, w)).append(lineBreak);
                result.append(formatTabWrap(arg.GetArgumentType(), 3, w)).append(sectionBreak);

                result.append(formatTabWrap("Value type", 2, w)).append(lineBreak);
                result.append(formatTabWrap(arg.GetValueType(), 3, w)).append(sectionBreak);

                if (arg.GetOptions().size() > 0) {
                    result.append(formatTabWrap("Options", 2, w)).append(lineBreak);

                    BOOST_FOREACH(std::string option, arg.GetOptions()) {
                        result.append(formatTabWrap(option, 3, w)).append(lineBreak);
                    }
                    result.append(lineBreak);
                }
                if (arg.GetDefaultValue().size() > 0) {
                    result.append(formatTabWrap("Default value", 2, w)).append(lineBreak);
                    result.append(formatTabWrap(arg.GetDefaultValue(), 3, w)).append(sectionBreak);
                }
            }
        }

        // details
        if (functionHelp.GetDetails().size() > 0) {
            result.append(TerminalFormatter::makeUnderlined("Details")).append(sectionBreak);

            BOOST_FOREACH(std::string det, functionHelp.GetDetails()) {
                result.append(formatTabWrap(det, 1, w)).append(sectionBreak);
            }
        }

        // example
        result.append(TerminalFormatter::makeUnderlined("Example")).append(sectionBreak).append(formatTabWrap(functionHelp.GetExample(), 1, w, false)).append(sectionBreak);

        // reference
        if (functionHelp.GetReferences().size() > 0) {
            result.append(TerminalFormatter::makeUnderlined("Reference")).append(sectionBreak);

            BOOST_FOREACH(ReferenceHelpEntry ref, functionHelp.GetReferences()) {
                result.append(formatTabWrap(ref.GetCitation(), 1, w)).append(lineBreak);
                result.append(formatTabWrap(ref.GetUrl(), 1, w)).append(lineBreak);
                result.append(formatTabWrap(ref.GetDoi(), 1, w)).append(sectionBreak);
            }
        }

        // author
        result.append(TerminalFormatter::makeUnderlined("Author")).append(sectionBreak).append(formatTabWrap(functionHelp.GetAuthor(), 1, w, false)).append(sectionBreak);

        // see also        
        if (functionHelp.GetSeeAlso().size() > 0) {
            result.append(TerminalFormatter::makeUnderlined("See also")).append(sectionBreak);

            BOOST_FOREACH(std::string see, functionHelp.GetSeeAlso()) {
                result.append(formatTabWrap(see, 1, w)).append(lineBreak);
            }
            result.append(lineBreak);
        }


        return result;

    }

private:

    /**
     * Wraps text so that each line doesn't exceeds column width.
     * Prepends tabs to each line.
     * 
     * @param s             string to format
     * @param tabs          number of tabs to prepend to each line
     * @param width         column width
     * @param removeFormat  strips newline, tab etc if set to true
     * @return              formatted text
     */
    std::string formatTabWrap(std::string s, int tabs, int width, bool removeFormat = true) {

        std::string tabbing = "";
        for (int i = 0; i < tabs * 4; i++) {
            tabbing.append(" ");
        }

        std::string result = tabbing;
        int w = width - tabbing.size();
        int cc = 0; // character count
        char lastChar = '\0';

        for (unsigned int i = 0; i < s.size(); i++) {
            if (result.size() > 0) {
                lastChar = result[result.size() - 1];
            }

            // strip consecutive spaces
            if (!(lastChar == ' ' && s[i] == ' ')) {
                if (!removeFormat) {
                    result += s[i];
                    lastChar = s[i];
                    cc++;
                } else if (!isFormattingChar(s[i])) {
                    result += s[i];
                    lastChar = s[i];
                    cc++;
                }
            }

            if (lastChar == '\n') {
                cc = 0;
                result.append(tabbing);
            }

            if (lastChar == ' ') {
                // we now have a possible point where to wrap the line.
                // peek ahead and see where next possible wrap point is:
                int next = s.substr(i).find_first_of(" ", 1);

                // if next wrap point is beyond the width, then wrap line now
                if (cc + next >= w) {
                    result.append("\n").append(tabbing);
                    // reset char count for next line
                    cc = 0;
                }
            }
        }
        return result;
    }

    /**
     * Indicates if a char is affecting text formatting
     * @param c
     * @return 
     */
    bool isFormattingChar(char c) {
        return c == '\n' || c == '\t' || c == '\r';
    }

};

#endif	/* HELPCONSOLERENDERER_H */

