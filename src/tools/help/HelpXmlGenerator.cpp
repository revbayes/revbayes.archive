#include "HelpXmlGenerator.h"

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "FunctionTable.h"
#include "OptionRule.h"
#include "RevNullObject.h"
#include "RevPtr.h"
#include "RlFunction.h"
#include "Variable.h"
#include "Workspace.h"

#include <fstream>
#include <iostream>
#include <string>

void writeXmlTemplate(std::string funcName, RevLanguage::Function *func)
{
    std::string filename = funcName + ".xml";
    
    std::fstream outStream;
    outStream.open( filename.c_str(), std::fstream::out);
    
    outStream << "<?xml version=\"1.0\"?>" << std::endl;
    outStream << "<?xml-stylesheet href=\"lib/help.xsl\" type=\"text/xsl\"?>" << std::endl;
    outStream << "" << std::endl;
    outStream << "<revbayes-help" << std::endl;
    outStream << "\txmlns=\"http://www.revbayes.org/schema/help-entry\"" << std::endl;
    outStream << "\txmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" << std::endl;
    outStream << "\txsi:schemaLocation=\"http://www.revbayes.org/schema/help-entry lib/rb-help-entry.xsd\"" << std::endl;
    outStream << ">" << std::endl;
    outStream << "<function-help-entry>" << std::endl;
    outStream << "" << std::endl;
    outStream << "    <name>" << funcName << "</name>" << std::endl;
    outStream << "" << std::endl;
    outStream << "    <alias>anotherNameForTheSameFunction</alias>" << std::endl;
    outStream << "    <alias>yetAnotherNameForTheSameFunction</alias>" << std::endl;
    outStream << "" << std::endl;
    outStream << "    <title>Some title you want to give this function</title>" << std::endl;
    outStream << "" << std::endl;
    outStream << "    <description>" << std::endl;
    outStream << "        <p>" << std::endl;
    outStream << "        This is a 'brief' description of the amazing function you have implemented." << std::endl;
    outStream << "        </p>"<< std::endl;
    outStream << "    </description>" << std::endl;
    outStream << "" << std::endl;
    outStream << "    <usage>" << std::endl;
    outStream << "        <![CDATA[" << std::endl;
    outStream << "         " << funcName << "(" << std::endl;
    
    const RevLanguage::ArgumentRules& argRules = func->getArgumentRules();
    size_t index = 0;
    for (RevLanguage::ArgumentRules::const_iterator it = argRules.begin(); it != argRules.end(); ++it, ++index)
    {
        outStream << "               " << (*it)->getArgumentTypeSpec()[0].getType() << "        " << (*it)->getArgumentLabel();
        if ( (*it)->hasDefault() )
        {
            outStream << " = ";
            const RevLanguage::Variable& var = (*it)->getDefaultVariable();
            RevLanguage::RevObject& revObj = var.getRevObject();
            if ( revObj != RevLanguage::RevNullObject::getInstance() )
            {
                RevBayesCore::DagNode *dag = revObj.getDagNode();
                dag->printValue(outStream, "");
            }
            else
            {
                outStream << "NULL";
            }
        }
        if ( index < argRules.size()-1 )
            outStream << ",";
        
        outStream << std::endl;
    }
    
    outStream << "               )" << std::endl;
    outStream << "         ]]> " << std::endl;
    outStream << "    </usage>" << std::endl;
    outStream << "" << std::endl;
    
    
    for (RevLanguage::ArgumentRules::const_iterator it = argRules.begin(); it != argRules.end(); ++it, ++index)
    {
        outStream << "    <argument>" << std::endl;
        outStream << "        <label>" << (*it)->getArgumentLabel() << "</label>" << std::endl;
        
        std::string argPassing = "";
        if ( (*it)->getEvaluationType() == RevLanguage::ArgumentRule::BY_CONSTANT_REFERENCE )
        {
            argPassing = "by-const-reference";
        }
        else if ( (*it)->getEvaluationType() == RevLanguage::ArgumentRule::BY_REFERENCE )
        {
            argPassing = "by-reference";
        }
        else if ( (*it)->getEvaluationType() == RevLanguage::ArgumentRule::BY_VALUE )
        {
            argPassing = "by-value";
        }
        else
        {
            argPassing = "unknown";
        }
        outStream << "        <argumentPassingMethod>" << argPassing << "</argumentPassingMethod>" << std::endl;

        std::string argNodeType = "";
        if ( (*it)->getArgumentDagNodeType() == RevLanguage::ArgumentRule::ANY )
        {
            argNodeType = "any-DAG-node";
        }
        else if ( (*it)->getArgumentDagNodeType() == RevLanguage::ArgumentRule::CONSTANT )
        {
            argNodeType = "const-DAG-node";
        }
        else if ( (*it)->getArgumentDagNodeType() == RevLanguage::ArgumentRule::DETERMINISTIC )
        {
            argNodeType = "deterministic-DAG-node";
        }
        else if ( (*it)->getArgumentDagNodeType() == RevLanguage::ArgumentRule::STOCHASTIC )
        {
            argNodeType = "stochastic-DAG-node";
        }
        else if ( (*it)->getArgumentDagNodeType() == RevLanguage::ArgumentRule::DYNAMIC )
        {
            argNodeType = "dynamic-DAG-node";
        }
        else
        {
            argNodeType = "unknown";
        }
        outStream << "        <argumentNodeType>" << argNodeType << "</argumentNodeType>" << std::endl;
        outStream << "        <description>A description of the argument.</description>" << std::endl;
        outStream << "        <valueType>" << (*it)->getArgumentTypeSpec()[0].getType() << "</valueType>" << std::endl;
        
        RevLanguage::OptionRule* oRule = dynamic_cast<RevLanguage::OptionRule*>( *it );
        if ( oRule != NULL )
        {
            const std::vector<std::string> &opts = oRule->getOptions();
            for (std::vector<std::string>::const_iterator o = opts.begin(); o != opts.end(); ++o)
            {
                outStream << "        <option>\"" << *o << "\"</option>" << std::endl;
            }
        }
        
        if ( (*it)->hasDefault() )
        {
            outStream << "        <defaultValue>";
            const RevLanguage::Variable& var = (*it)->getDefaultVariable();
            RevLanguage::RevObject& revObj = var.getRevObject();
            if ( revObj != RevLanguage::RevNullObject::getInstance() )
            {
                RevBayesCore::DagNode *dag = revObj.getDagNode();
                dag->printValue(outStream, "");
            }
            else
            {
                outStream << "NULL";
            }
            outStream << "</defaultValue>" << std::endl;
        }
        
        outStream << "    </argument>" << std::endl;
    }
    
    outStream << "" << std::endl;
    outStream << "    <returnValue>" << func->getReturnType().getType() << "</returnValue>" << std::endl;
    outStream << "" << std::endl;
    outStream << "    <details>" << std::endl;
    outStream << "        <!-- Since this section can be lengthy, the text is sectioned with <p> tags -->" << std::endl;
    outStream << "        <p>" << std::endl;
    outStream << "        The implementation of this fancy function ... " << std::endl;
    outStream << "        </p>" << std::endl;
    outStream << "    </details>" << std::endl;
    outStream << "" << std::endl;
    outStream << "    <example>" << std::endl;
    outStream << "        <![CDATA[" << std::endl;
    outStream << "         # an executable example of the function described here" << std::endl;
    outStream << "         trueTree <- readTrees(\"data/primates.tree\")[1]" << std::endl;
    outStream << "         ..." << std::endl;
    outStream << "         ]]>" << std::endl;
    outStream << "" << std::endl;
    outStream << "    </example>" << std::endl;
    outStream << "" << std::endl;
    outStream << "    <reference>" << std::endl;
    outStream << "        <citation>" << std::endl;
    outStream << "            First, Second, and Third Authors. Year. Title of paper. Journal, Issue, Pages." << std::endl;
    outStream << "        </citation>" << std::endl;
    outStream << "        <url>An optional URL to the paper</url>" << std::endl;
    outStream << "        <doi>An optional DOI to the paper</doi>" << std::endl;
    outStream << "    </reference>" << std::endl;
    outStream << "" << std::endl;
    outStream << "    <author>Your name here</author>" << std::endl;
    outStream << "" << std::endl;
    outStream << "    <seeAlso>" << std::endl;
    outStream << "        <!-- The xslt page will try to auto generate links to the referred help entries, therefore only exact names of help items should be listed here (case sensitive) -->" << std::endl;
    outStream << "        <entry>similar function</entry>" << std::endl;
    outStream << "    </seeAlso>" << std::endl;
    outStream << "" << std::endl;
    outStream << "" << std::endl;
    outStream << "    </function-help-entry>" << std::endl;
    outStream << "</revbayes-help>" << std::endl;
    outStream << "" << std::endl;
                          
                          
    outStream.close();

}


int main(int argc, const char * argv[]) {
    
    std::cout << "\nGenerating XML help templates ... \n\n";
    
    RevLanguage::Workspace& w = RevLanguage::Workspace::globalWorkspace();
    w.initializeGlobalWorkspace();
    RevLanguage::FunctionTable& ft = w.getFunctionTable();
    
    std::cout << "Found " << ft.size() << " functions in the workspace." << std::endl;
    
    for( std::multimap<std::string, RevLanguage::Function*>::iterator it = ft.begin(); it != ft.end(); ++it)
    {
        std::string fName = it->first;
        RevLanguage::Function* f = it->second;
        
        std::cout << "Creating new XML template for function " << fName << std::endl;
     
        writeXmlTemplate(fName, f);
    }
    
}


