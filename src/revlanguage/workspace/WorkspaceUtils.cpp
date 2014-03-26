#include "WorkspaceUtils.h"
#include "Workspace.h"
#include "VariableSlot.h"
#include "RevLanguageMain.h"
#include "RlFunction.h"
#include "ArgumentRules.h"

#include <map>
#include <vector>
#include <algorithm>
#include <stdbool.h>
#include <stdio.h>

typedef std::vector<std::string> StringVector;

/* @return vector with all functions the user have access to */
StringVector WorkspaceUtils::getFunctions() {
    typedef std::multimap<std::string, RevLanguage::Function*> FunctionMap;

    FunctionMap functionsMap = RevLanguage::Workspace::userWorkspace().getFunctionTable().getTableCopy(true);

    StringVector functions;
    for (FunctionMap::iterator it = functionsMap.begin(); it != functionsMap.end(); ++it) {
        functions.push_back(it->first);
    }

    return makeUnique(functions);
}

/* @return vector with all objects the user have access to */
// todo: combine global and userspace ?

StringVector WorkspaceUtils::getVariables() {
    typedef std::map<std::string, RevLanguage::VariableSlot* > VariableTable;
    VariableTable v = RevLanguage::Workspace::userWorkspace().getVariableTable();

    StringVector objects;
    for (VariableTable::iterator it = v.begin(); it != v.end(); ++it) {
        objects.push_back(it->first);
    }

    return makeUnique(objects);
}

/* @return vector with all types the user have access to */
StringVector WorkspaceUtils::getTypes() {
    typedef std::map<std::string, RevLanguage::RbLanguageObject*> TypeTable;

    TypeTable t = RevLanguage::Workspace::globalWorkspace().getTypeTable();

    StringVector objects;
    for (TypeTable::iterator it = t.begin(); it != t.end(); ++it) {
        objects.push_back(it->first);
    }

    return makeUnique(objects);
}

/* @return vector with all types + variables the user have access to */
StringVector WorkspaceUtils::getObjects() {
    StringVector t = getTypes();
    StringVector v = getVariables();
    t.insert(t.end(), v.begin(), v.end());
    return t;
}

StringVector WorkspaceUtils::getFunctionParameters(std::string name) {
    typedef std::vector<RevLanguage::Function *> FunctionVector;
    typedef std::vector<RevLanguage::Argument> ArgumentVector;
    StringVector result;
    if (!RevLanguage::Workspace::globalWorkspace().existsFunction(name)) {
        return result;
    }

    FunctionVector v = RevLanguage::Workspace::globalWorkspace().getFunctionTable().findFunctions(name);
    for (FunctionVector::iterator it = v.begin(); it != v.end(); it++) {
        const RevLanguage::ArgumentRules& argRules = (*it)->getArgumentRules();
        for (size_t i = 0; i < argRules.size(); i++) {
            result.push_back(argRules[i].getArgumentLabel());
        }       
    }

    return makeUnique(result);
}

StringVector WorkspaceUtils::getFunctionSignatures(std::string name) {
    typedef std::vector<RevLanguage::Function *> FunctionVector;
    typedef std::vector<RevLanguage::Argument> ArgumentVector;
    StringVector result;
    if (!RevLanguage::Workspace::globalWorkspace().existsFunction(name)) {
        return result;
    }

    FunctionVector v = RevLanguage::Workspace::globalWorkspace().getFunctionTable().findFunctions(name);
    for (FunctionVector::iterator it = v.begin(); it != v.end(); it++) {
        result.push_back((*it)->toString()); 
    }

    return makeUnique(result);
}
