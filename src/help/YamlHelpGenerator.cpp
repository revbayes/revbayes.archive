#include <vector>
#include <iostream>
#include <fstream>
#include "YAMLHelpRenderer.h"
#include "RbHelpDistribution.h"
#include "RbHelpMonitor.h"
#include "RbHelpMove.h"
#include "RbHelpSystem.h"
#include "Workspace.h"

int main(int argc, const char * argv[])
{
    std::string prefix = "";

    if( argc > 1 )
    {
        prefix = std::string(argv[1]);
    }

    std::cout << std::endl << "Generating help files..." << std::endl;
    
    std::string function_entry_result, type_entry_result, dist_entry_result, mntr_entry_result, move_entry_result, tmp;
    
    // first we need to initialize the workspace
    RevLanguage::Workspace::globalWorkspace().initializeGlobalWorkspace();
    
    RevBayesCore::RbHelpSystem& help = RevBayesCore::RbHelpSystem::getHelpSystem();
    YAML::HelpRenderer renderer = YAML::HelpRenderer();
    

    std::string fnFile = prefix + "functions.yml";

    std::fstream fnfs;
    fnfs.open(fnFile, std::fstream::out | std::fstream::trunc );

    const std::set<std::string> &functionEntryNames = help.getFunctionEntries();
    
    for (std::set<std::string>::const_iterator it=functionEntryNames.begin(); it!=functionEntryNames.end(); ++it)
    {
        std::string name = *it;
        
        const RevBayesCore::RbHelpFunction& entry = static_cast<const RevBayesCore::RbHelpFunction&>( help.getHelp( name ) );
        
        if (name.size() > 0 && entry.getUsage() != "c_name()")
        {
            std::cout << "Adding function with name:\t" << name << std::endl;

            fnfs << renderer.renderHelp(entry);
        }
        
    }
    
    fnfs.close();

    std::string dnFile = prefix + "distributions.yml";
    std::string mnFile = prefix + "monitors.yml";
    std::string mvFile = prefix + "moves.yml";
    std::string tpFile = prefix + "types.yml";

    std::fstream dnfs, mnfs, mvfs, tpfs;

    dnfs.open(dnFile, std::fstream::out | std::fstream::trunc );
    mnfs.open(mnFile, std::fstream::out | std::fstream::trunc );
    mvfs.open(mvFile, std::fstream::out | std::fstream::trunc );
    tpfs.open(tpFile, std::fstream::out | std::fstream::trunc );

    const std::set<std::string> &typeEntryNames = help.getTypeEntries();
    
    for (std::set<std::string>::const_iterator it=typeEntryNames.begin(); it!=typeEntryNames.end(); ++it)
    {
        
        std::string name = *it;

        if (name.size() > 0 && name != "c_name" && functionEntryNames.find(name) == functionEntryNames.end() )
        {
            std::cerr << "Adding type with name:\t" << name << std::endl;
            
            const RevBayesCore::RbHelpType& typeEntry = static_cast<const RevBayesCore::RbHelpType&>( help.getHelp( name ) );
            
            const RevBayesCore::RbHelpDistribution* dnEntry = dynamic_cast<const RevBayesCore::RbHelpDistribution*>( &typeEntry );
            const RevBayesCore::RbHelpMonitor*      mnEntry = dynamic_cast<const RevBayesCore::RbHelpMonitor*>     ( &typeEntry );
            const RevBayesCore::RbHelpMove*         mvEntry = dynamic_cast<const RevBayesCore::RbHelpMove*>        ( &typeEntry );

            if ( dnEntry != NULL )
            {
                dnfs << renderer.renderHelp(typeEntry);
            }
            else if ( mnEntry != NULL )
            {
                mnfs << renderer.renderHelp(typeEntry);
            }
            else if ( mvEntry != NULL )
            {
                mvfs << renderer.renderHelp(typeEntry);
            }
            else
            {
                tpfs << renderer.renderHelp(typeEntry);
            }
            
            
        }
        
    }
    
    dnfs.close();
    mnfs.close();
    mvfs.close();
    tpfs.close();

    std::cout << "\nHelp files are updated." << std::endl;

    return 0;
}
