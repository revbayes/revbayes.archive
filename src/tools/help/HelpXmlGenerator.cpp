#include "HelpXmlGenerator.h"

#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstructorFunction.h"
#include "FunctionTable.h"
#include "OptionRule.h"
#include "RevNullObject.h"
#include "RevPtr.h"
#include "RlDistribution.h"
#include "RlFunction.h"
#include "RlMonitor.h"
#include "RlMove.h"
#include "RevVariable.h"
#include "Workspace.h"

#include <fstream>
#include <iostream>
#include <string>


void writeHeaderXmlTemplate(std::fstream &outStream)
{
    
    outStream << "<?xml version=\"1.0\"?>" << std::endl;
    outStream << "<?xml-stylesheet href=\"lib/help.xsl\" type=\"text/xsl\"?>" << std::endl;
    outStream << "" << std::endl;
    outStream << "<revbayes-help" << std::endl;
    outStream << "\txmlns=\"http://www.revbayes.org/schema/help-entry\"" << std::endl;
    outStream << "\txmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" << std::endl;
    outStream << "\txsi:schemaLocation=\"http://www.revbayes.org/schema/help-entry lib/rb-help-entry.xsd\"" << std::endl;
    outStream << ">" << std::endl;
    
}


void writeFunctionBodyXmlTemplate(std::fstream &outStream, size_t indentLevel, std::string funcName, RevLanguage::Function *func, bool constructor)
{
    
    std::string tabbing = "";
    for (int i = 0; i < indentLevel * 4; i++)
    {
        tabbing.append(" ");
    }
    
    outStream << tabbing << "" << std::endl;
    
    if ( constructor == false )
    {
        outStream << tabbing << "    <name>" << funcName << "</name>" << std::endl;
        outStream << "" << std::endl;
        outStream << tabbing << "    <alias>anotherNameForTheSameFunction</alias>" << std::endl;
        outStream << tabbing << "    <alias>yetAnotherNameForTheSameFunction</alias>" << std::endl;
        outStream << "" << std::endl;
        outStream << tabbing << "    <title>Some title you want to give this function</title>" << std::endl;
        outStream << tabbing << "" << std::endl;
        outStream << tabbing << "    <description>" << std::endl;
        outStream << tabbing << "        <p>" << std::endl;
        outStream << tabbing << "        This is a 'brief' description of the amazing function you have implemented." << std::endl;
        outStream << tabbing << "        </p>"<< std::endl;
        outStream << tabbing << "    </description>" << std::endl;
    }

    outStream << "" << std::endl;
    outStream << tabbing << "    <usage>" << std::endl;
    outStream << tabbing << "        <![CDATA[" << std::endl;
    outStream << tabbing << "         " << funcName << "( ";
    
    const RevLanguage::ArgumentRules& argRules = func->getArgumentRules();
    size_t index = 0;
    for (RevLanguage::ArgumentRules::const_iterator it = argRules.begin(); it != argRules.end(); ++it, ++index)
    {
        outStream << (*it)->getArgumentTypeSpec()[0].getType() << " " << (*it)->getArgumentLabel();
        if ( (*it)->hasDefault() )
        {
            outStream << " = ";
            const RevLanguage::RevVariable& var = (*it)->getDefaultVariable();
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
            outStream << ", ";
        
    }
    outStream << " )" << std::endl;
    
    outStream << tabbing << "         ]]> " << std::endl;
    outStream << tabbing << "    </usage>" << std::endl;
    outStream << tabbing << "" << std::endl;
    
    
    for (RevLanguage::ArgumentRules::const_iterator it = argRules.begin(); it != argRules.end(); ++it, ++index)
    {
        outStream << tabbing << "    <argument>" << std::endl;
        outStream << tabbing << "        <label>" << (*it)->getArgumentLabel() << "</label>" << std::endl;
        
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
        outStream << tabbing << "        <argumentPassingMethod>" << argPassing << "</argumentPassingMethod>" << std::endl;
        
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
        outStream << tabbing << "        <argumentNodeType>" << argNodeType << "</argumentNodeType>" << std::endl;
        outStream << tabbing << "        <description>A description of the argument.</description>" << std::endl;
        outStream << tabbing << "        <valueType>" << (*it)->getArgumentTypeSpec()[0].getType() << "</valueType>" << std::endl;
        
        RevLanguage::OptionRule* oRule = dynamic_cast<RevLanguage::OptionRule*>( *it );
        if ( oRule != NULL )
        {
            const std::vector<std::string> &opts = oRule->getOptions();
            for (std::vector<std::string>::const_iterator o = opts.begin(); o != opts.end(); ++o)
            {
                outStream << tabbing << "        <option>\"" << *o << "\"</option>" << std::endl;
            }
        }
        
        if ( (*it)->hasDefault() )
        {
            outStream << tabbing << "        <defaultValue>";
            const RevLanguage::RevVariable& var = (*it)->getDefaultVariable();
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
        
        outStream << tabbing << "    </argument>" << std::endl;
    }
    
    outStream << "" << std::endl;
    outStream << tabbing << "    <returnValue>" << func->getReturnType() << "</returnValue>" << std::endl;
    outStream << "" << std::endl;
    outStream << tabbing << "    <details>" << std::endl;
    outStream << tabbing << "        <!-- Since this section can be lengthy, the text is sectioned with <p> tags -->" << std::endl;
    outStream << tabbing << "        <p>" << std::endl;
    outStream << tabbing << "        The implementation of this fancy function ... " << std::endl;
    outStream << tabbing << "        </p>" << std::endl;
    outStream << tabbing << "    </details>" << std::endl;
    outStream << "" << std::endl;
    outStream << tabbing << "    <example>" << std::endl;
    outStream << tabbing << "        <![CDATA[" << std::endl;
    outStream << tabbing << "         # an executable example of the function described here" << std::endl;
    outStream << tabbing << "         trueTree <- readTrees(\"data/primates.tree\")[1]" << std::endl;
    outStream << tabbing << "         ..." << std::endl;
    outStream << tabbing << "         ]]>" << std::endl;
    outStream << "" << std::endl;
    outStream << tabbing << "    </example>" << std::endl;
    outStream << "" << std::endl;
    outStream << tabbing << "    <reference>" << std::endl;
    outStream << tabbing << "        <citation>" << std::endl;
    outStream << tabbing << "            First, Second, and Third Authors. Year. Title of paper. Journal, Issue, Pages." << std::endl;
    outStream << tabbing << "        </citation>" << std::endl;
    outStream << tabbing << "        <url>An optional URL to the paper</url>" << std::endl;
    outStream << tabbing << "        <doi>An optional DOI to the paper</doi>" << std::endl;
    outStream << tabbing << "    </reference>" << std::endl;
    outStream << "" << std::endl;
    outStream << tabbing << "    <author>Your name here</author>" << std::endl;
    outStream << "" << std::endl;
    outStream << tabbing << "    <seeAlso>" << std::endl;
    outStream << tabbing << "        <!-- The xslt page will try to auto generate links to the referred help entries, therefore only exact names of help items should be listed here (case sensitive) -->" << std::endl;
    outStream << tabbing << "        <entry>similar function</entry>" << std::endl;
    outStream << tabbing << "    </seeAlso>" << std::endl;
    outStream << "" << std::endl;
    outStream << "" << std::endl;
    
}


void writeTypeBodyXmlTemplate(std::fstream &outStream, size_t indentLevel, std::string typeName, RevLanguage::Function *ctor,  RevLanguage::RevObject *obj)
{
    
    std::string tabbing = "";
    for (int i = 0; i < indentLevel * 4; i++)
    {
        tabbing.append(" ");
    }
    
    outStream << "" << std::endl;
    outStream << tabbing << "    <name>" << typeName << "</name>" << std::endl;
    outStream << "" << std::endl;
    outStream << tabbing << "    <alias>anotherNameForTheSameFunction</alias>" << std::endl;
    outStream << tabbing << "    <alias>yetAnotherNameForTheSameFunction</alias>" << std::endl;
    outStream << "" << std::endl;
    outStream << tabbing << "    <title>Some title you want to give this function</title>" << std::endl;
    outStream << "" << std::endl;
    outStream << tabbing << "    <description>" << std::endl;
    outStream << tabbing << "        <p>" << std::endl;
    outStream << tabbing << "        This is a 'brief' description of the amazing function you have implemented." << std::endl;
    outStream << tabbing << "        </p>"<< std::endl;
    outStream << tabbing << "    </description>" << std::endl;
    outStream << tabbing << "" << std::endl;
    
    outStream << tabbing << "    <constructor-help-entry>" << std::endl;
    writeFunctionBodyXmlTemplate(outStream, indentLevel+1, ctor->getName(), ctor, true);
    outStream << tabbing << "    </constructor-help-entry>" << std::endl;
    outStream << tabbing << "" << std::endl;
    outStream << tabbing << "" << std::endl;
    
    const RevLanguage::MethodTable& methods = obj->getMethods();
    for (std::multimap<std::string, Function*>::const_iterator it = methods.begin(); it!=methods.end(); ++it)
    {
        
        RevLanguage::Function *func = it->second;
        
        if ( func->getName() != "methods" )
        {
            outStream << tabbing << "    <method-help-entry>" << std::endl;
            writeFunctionBodyXmlTemplate(outStream, indentLevel+1, func->getName(), func, false);
            outStream << tabbing << "    </method-help-entry>" << std::endl;
            outStream << tabbing << "" << std::endl;
            outStream << tabbing << "" << std::endl;
        }
        
    }
    
//    outStream << "    <reference>" << std::endl;
//    outStream << "        <citation>" << std::endl;
//    outStream << "            First, Second, and Third Authors. Year. Title of paper. Journal, Issue, Pages." << std::endl;
//    outStream << "        </citation>" << std::endl;
//    outStream << "        <url>An optional URL to the paper</url>" << std::endl;
//    outStream << "        <doi>An optional DOI to the paper</doi>" << std::endl;
//    outStream << "    </reference>" << std::endl;
//    outStream << "" << std::endl;
//    outStream << "    <author>Your name here</author>" << std::endl;
//    outStream << "" << std::endl;
//    outStream << "    <seeAlso>" << std::endl;
//    outStream << "        <!-- The xslt page will try to auto generate links to the referred help entries, therefore only exact names of help items should be listed here (case sensitive) -->" << std::endl;
//    outStream << "        <entry>similar function</entry>" << std::endl;
//    outStream << "    </seeAlso>" << std::endl;
//    outStream << "" << std::endl;

    
    outStream << "" << std::endl;
    
}


void writeTypeXmlTemplate(std::fstream &outStream, std::string typeName, RevLanguage::Function *ctor, RevLanguage::RevObject *obj)
{
    
    outStream << "    <type-help-entry>" << std::endl;
    
    writeTypeBodyXmlTemplate(outStream, 1, typeName, ctor, obj);
    
    outStream << "    </type-help-entry>" << std::endl;
    outStream << "" << std::endl;
    outStream << "</revbayes-help>" << std::endl;
    outStream << "" << std::endl;

}


void writeDistributionXmlTemplate(std::fstream &outStream, std::string typeName, RevLanguage::Function *ctor, RevLanguage::Distribution *d)
{
    
    outStream << "    <distribution-help-entry>" << std::endl;
    
    writeTypeBodyXmlTemplate(outStream, 1, typeName, ctor, d);
    
    outStream << "    </distribution-help-entry>" << std::endl;
    outStream << "" << std::endl;
    outStream << "</revbayes-help>" << std::endl;
    outStream << "" << std::endl;
    
}


void writeMoveXmlTemplate(std::fstream &outStream, std::string typeName, RevLanguage::Function *ctor, RevLanguage::Move *mv)
{
    
    outStream << "    <move-help-entry>" << std::endl;
    
    writeTypeBodyXmlTemplate(outStream, 1, typeName, ctor, mv);
    
    outStream << "    </move-help-entry>" << std::endl;
    outStream << "" << std::endl;
    outStream << "</revbayes-help>" << std::endl;
    outStream << "" << std::endl;
    
}


void writeMonitorXmlTemplate(std::fstream &outStream, std::string typeName, RevLanguage::Function *ctor, RevLanguage::Monitor *mntr)
{
    
    outStream << "    <monitor-help-entry>" << std::endl;
    
    writeTypeBodyXmlTemplate(outStream, 1, typeName, ctor, mntr);
    
    outStream << "    </monitor-help-entry>" << std::endl;
    outStream << "" << std::endl;
    outStream << "</revbayes-help>" << std::endl;
    outStream << "" << std::endl;
    
}


void writeStandardFunctionXmlTemplate(std::fstream &outStream, std::string funcName, RevLanguage::Function *func)
{
    
    outStream << "    <function-help-entry>" << std::endl;
    
    writeFunctionBodyXmlTemplate(outStream, 1, funcName, func, false);
    
    outStream << "    </function-help-entry>" << std::endl;
    outStream << "" << std::endl;
    outStream << "</revbayes-help>" << std::endl;
    outStream << "" << std::endl;
    
}




void writeXmlTemplate(std::string fn, std::string funcName, RevLanguage::Function *func)
{
    std::string filename = fn + ".xml";
    
    std::fstream outStream;
    outStream.open( filename.c_str(), std::fstream::out);
    
    writeHeaderXmlTemplate( outStream );
    
    RevLanguage::ConstructorFunction *ctorFunc = dynamic_cast< RevLanguage::ConstructorFunction* >( func );
    if ( ctorFunc != NULL )
    {
        RevLanguage::RevObject *obj = ctorFunc->getRevObject();
        
        RevLanguage::Distribution * d = dynamic_cast< RevLanguage::Distribution* >( obj );
        RevLanguage::Move * mv = dynamic_cast< RevLanguage::Move* >( obj );
        RevLanguage::Monitor * mntr = dynamic_cast< RevLanguage::Monitor* >( obj );
        if ( d != NULL )
        {
            writeDistributionXmlTemplate( outStream, funcName, func, d );
        }
        else if ( mv != NULL )
        {
            writeMoveXmlTemplate( outStream, funcName, func, mv );
        }
        else if ( mntr != NULL )
        {
            writeMonitorXmlTemplate( outStream, funcName, func, mntr );
        }
        else
        {
            writeTypeXmlTemplate( outStream, funcName, func, obj );
        }
        
    }
    else
    {
        writeStandardFunctionXmlTemplate( outStream, funcName, func );
    }
    
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
     
        writeXmlTemplate("help/"+fName, fName, f);
    }
    
}


