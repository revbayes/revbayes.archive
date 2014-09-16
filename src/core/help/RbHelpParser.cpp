#include "RbFileManager.h"
#include "RbHelpParser.h"
#include "XmlDocument.h"
#include "XmlParser.h"

using namespace RevBayesCore;

RbHelpFunction RbHelpParser::parseHelpFunction(const std::string &fn)
{
    // create the help function entry that we will fill with some values later on
    RbHelpFunction helpEntry = RbHelpFunction();
    
    // first we need to load the file
    std::ifstream readStream;
    RbFileManager fm = RbFileManager(fn);
    fm.openFile( readStream );
    
    // then we create the xml document
    XmlParser xParser = XmlParser();
    XmlDocument *xDocument = xParser.parse( readStream );
    
    const XmlElement *xElement = xDocument->getFirstXmlElement();
    
    
    delete xDocument;
    
    return helpEntry;
}

