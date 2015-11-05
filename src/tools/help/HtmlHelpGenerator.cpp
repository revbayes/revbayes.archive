#include <vector>
#include <iostream>
#include <fstream>
#include "HtmlHelpRenderer.h"

#include "RbFileManager.h"
#include "RbHelpDistribution.h"
#include "RbHelpMonitor.h"
#include "RbHelpMove.h"
#include "RbHelpSystem.h"
#include "RbSettings.h"
#include "StringUtilities.h"
#include "Workspace.h"


std::string load_file(std::string filename) {
    std::string s;
    std::string r("");
    std::ifstream in(filename.c_str());
    while (getline(in, s)) {
        r += s + "\n";
    }
    in.close();
    return r;
}

int main(int argc, const char * argv[])
{
    
    std::cout << std::endl << "This tool will automatically generate the index.html page." << std::endl
    << "If you have made any manual changes to " << std::endl
    << "'help/index.html' these will be overwritten. " << std::endl
    << std::endl;
    
    std::cout << std::endl << "Generating index.html and separate html manual pages..." << std::endl;
    
    // read html templates
    std::string entry_tpl_file = "html-template/entry.tpl.html";
    std::string index_tpl_file =  "html-template/index.tpl.html";
    std::string type_tpl_file =  "html-template/type-page.tpl.html";
    std::string dist_tpl_file =  "html-template/distribution-page.tpl.html";
    std::string move_tpl_file =  "html-template/move-page.tpl.html";
    std::string mntr_tpl_file =  "html-template/monitor-page.tpl.html";
    std::string function_tpl_file = "html-template/function-page.tpl.html";
    
    std::string entry_tpl = load_file(entry_tpl_file);
    std::string index_tpl = load_file(index_tpl_file);
    std::string type_tpl = load_file(type_tpl_file);
    std::string dist_tpl = load_file(dist_tpl_file);
    std::string move_tpl = load_file(move_tpl_file);
    std::string mntr_tpl = load_file(mntr_tpl_file);
    std::string function_tpl = load_file(function_tpl_file);
    
    std::string function_entry_result, type_entry_result, dist_entry_result, mntr_entry_result, move_entry_result, tmp;
    
    // first we need to initialize the workspace
    RevLanguage::Workspace::globalWorkspace().initializeGlobalWorkspace();
    
    
    RevBayesCore::RbHelpSystem& help = RevBayesCore::RbHelpSystem::getHelpSystem();
    HtmlHelpRenderer renderer = HtmlHelpRenderer();
    renderer.setTypeTemplate(type_tpl);
    renderer.setDistributionTemplate(dist_tpl);
    renderer.setMoveTemplate(move_tpl);
    renderer.setMonitorTemplate(mntr_tpl);
    renderer.setFunctionTemplate(function_tpl);
    
    const std::set<std::string> &functionEntryNames = help.getFunctionEntries();
    
    for (std::set<std::string>::const_iterator it=functionEntryNames.begin(); it!=functionEntryNames.end(); ++it)
    {
        std::string n = *it;
        const RevBayesCore::RbHelpFunction& functionEntry = dynamic_cast<const RevBayesCore::RbHelpFunction&>( help.getHelp( n ) );
        
        std::string entry = entry_tpl;
        
        std::string functionName = n;
        if (functionName.size() > 0)
        {
            // add section to index.html
            StringUtilities::replaceSubstring(entry, "#entry-name#", functionName);
            StringUtilities::replaceSubstring(entry, "#entry-type#", "function");
            tmp = "";
            
            for (std::vector<std::string>::const_iterator j=functionEntry.getDescription().begin(); j!=functionEntry.getDescription().end(); ++j)
            {
                std::string desc = *j;
                tmp.append("<p>").append(desc).append("</p>");
            }
            StringUtilities::replaceSubstring(entry, "#entry-description#", tmp);
            StringUtilities::replaceSubstring(entry, "#more-content#", renderer.renderFunctionHelp( functionEntry ));
            
            function_entry_result += entry + "\n";
            
            // create a manual page
            std::fstream fs;
            std::string functionPage = "html/pages/" + functionName + ".html";
            fs.open(functionPage, std::fstream::out | std::fstream::trunc);
            fs << renderer.renderFunctionHelp(functionEntry, true);
            fs.close();
            
        }
       
    }
    
    const std::set<std::string> &typeEntryNames = help.getTypeEntries();
    
    for (std::set<std::string>::const_iterator it=typeEntryNames.begin(); it!=typeEntryNames.end(); ++it)
    {
        std::string n = *it;

        const RevBayesCore::RbHelpType& typeEntry = dynamic_cast<const RevBayesCore::RbHelpType&>( help.getHelp( n ) );
        
        std::string typeName = n;
        if (typeName.size() > 0)
        {
            std::string entry = entry_tpl;
            
            const RevBayesCore::RbHelpDistribution* distEntry = dynamic_cast<const RevBayesCore::RbHelpDistribution*>( &typeEntry );
            const RevBayesCore::RbHelpMonitor*      mntrEntry = dynamic_cast<const RevBayesCore::RbHelpMonitor*>     ( &typeEntry );
            const RevBayesCore::RbHelpMove*         moveEntry = dynamic_cast<const RevBayesCore::RbHelpMove*>        ( &typeEntry );
            if ( distEntry != NULL )
            {
                
                // add section to index.html
                StringUtilities::replaceSubstring(entry, "#entry-name#", typeName);
                StringUtilities::replaceSubstring(entry, "#entry-type#", "distribution");
                tmp = "";
                
                
                for (std::vector<std::string>::const_iterator j=typeEntry.getDescription().begin(); j!=typeEntry.getDescription().end(); ++j)
                {
                    std::string desc = *j;
                    tmp.append("<p>").append(desc).append("</p>");
                }
                StringUtilities::replaceSubstring(entry, "#entry-description#", tmp);
                StringUtilities::replaceSubstring(entry, "#more-content#", renderer.renderTypeHelp( typeEntry ));
                
                dist_entry_result += entry + "\n";
                
                // create a manual page
                std::fstream fs;
                std::string typePage = "html/pages/" + typeName + ".html";
                fs.open(typePage, std::fstream::out | std::fstream::trunc);
                fs << renderer.renderTypeHelp(typeEntry, true);
                fs.close();

                
            }
            else if ( mntrEntry != NULL )
            {
                // add section to index.html
                StringUtilities::replaceSubstring(entry, "#entry-name#", typeName);
                StringUtilities::replaceSubstring(entry, "#entry-type#", "monitor");
                tmp = "";
                
                
                for (std::vector<std::string>::const_iterator j=typeEntry.getDescription().begin(); j!=typeEntry.getDescription().end(); ++j)
                {
                    std::string desc = *j;
                    tmp.append("<p>").append(desc).append("</p>");
                }
                StringUtilities::replaceSubstring(entry, "#entry-description#", tmp);
                StringUtilities::replaceSubstring(entry, "#more-content#", renderer.renderTypeHelp( typeEntry ));
                
                mntr_entry_result += entry + "\n";
                
                // create a manual page
                std::fstream fs;
                std::string typePage = "html/pages/" + typeName + ".html";
                fs.open(typePage, std::fstream::out | std::fstream::trunc);
                fs << renderer.renderTypeHelp(typeEntry, true);
                fs.close();

            }
            else if ( moveEntry != NULL )
            {
                // add section to index.html
                StringUtilities::replaceSubstring(entry, "#entry-name#", typeName);
                StringUtilities::replaceSubstring(entry, "#entry-type#", "move");
                tmp = "";
                
                
                for (std::vector<std::string>::const_iterator j=typeEntry.getDescription().begin(); j!=typeEntry.getDescription().end(); ++j)
                {
                    std::string desc = *j;
                    tmp.append("<p>").append(desc).append("</p>");
                }
                StringUtilities::replaceSubstring(entry, "#entry-description#", tmp);
                StringUtilities::replaceSubstring(entry, "#more-content#", renderer.renderTypeHelp( typeEntry ));
                
                move_entry_result += entry + "\n";
                
                // create a manual page
                std::fstream fs;
                std::string typePage = "html/pages/" + typeName + ".html";
                fs.open(typePage, std::fstream::out | std::fstream::trunc);
                fs << renderer.renderTypeHelp(typeEntry, true);
                fs.close();

            }
            else
            {
                // add section to index.html
                StringUtilities::replaceSubstring(entry, "#entry-name#", typeName);
                StringUtilities::replaceSubstring(entry, "#entry-type#", "type");
                tmp = "";
                
                
                for (std::vector<std::string>::const_iterator j=typeEntry.getDescription().begin(); j!=typeEntry.getDescription().end(); ++j)
                {
                    std::string desc = *j;
                    tmp.append("<p>").append(desc).append("</p>");
                }
                StringUtilities::replaceSubstring(entry, "#entry-description#", tmp);
                StringUtilities::replaceSubstring(entry, "#more-content#", renderer.renderTypeHelp( typeEntry ));
                
                type_entry_result += entry + "\n";
                
                // create a manual page
                std::fstream fs;
                std::string typePage = "html/pages/" + typeName + ".html";
                fs.open(typePage, std::fstream::out | std::fstream::trunc);
                fs << renderer.renderTypeHelp(typeEntry, true);
                fs.close();
                
            }

            
        }
        
    }
    
    // insert entries into main html page
    StringUtilities::replaceSubstring(index_tpl, "#manual-function-entries#", function_entry_result);
    StringUtilities::replaceSubstring(index_tpl, "#manual-type-entries#", type_entry_result);
    StringUtilities::replaceSubstring(index_tpl, "#manual-distribution-entries#", dist_entry_result);
    StringUtilities::replaceSubstring(index_tpl, "#manual-monitor-entries#", mntr_entry_result);
    StringUtilities::replaceSubstring(index_tpl, "#manual-move-entries#", move_entry_result);
    
    // write out new file content
    std::fstream fs;
    fs.open("html/index.html", std::fstream::out | std::fstream::trunc);
    fs << index_tpl;
    fs.close();
    
    std::cout << "\nHtml files are updated.\n" << std::endl;
    
    return 0;
}
