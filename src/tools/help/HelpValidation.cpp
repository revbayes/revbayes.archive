#include "FunctionTable.h"
#include "RbHelpSystem.h"
#include "Workspace.h"

#include <fstream>
#include <iostream>
#include <string>


int main(int argc, const char * argv[])
{
    
    std::cout << "\n Checking help files ... \n\n";
    
    RevLanguage::Workspace& w = RevLanguage::Workspace::globalWorkspace();
    w.initializeGlobalWorkspace();
    RevLanguage::FunctionTable& ft = w.getFunctionTable();
    
    std::cout << "Found " << ft.size() << " functions in the workspace." << std::endl;
    std::cout << std::endl;
    
    for( std::multimap<std::string, RevLanguage::Function*>::iterator it = ft.begin(); it != ft.end(); ++it)
    {
        std::string fName = it->first;
//        RevLanguage::Function* f = it->second;
        
        if ( fName[0] == '_' || fName[0] == '[')
        {
            continue;
        }
        
        RevBayesCore::RbHelpSystem& hs = RevBayesCore::RbHelpSystem::getHelpSystem();
        bool has = hs.isHelpAvailableForQuery(fName);
        
        if ( has == false )
        {
            std::cout << "Missing help entry for " << fName << "." << std::endl;
        }
        
    }
    
}


