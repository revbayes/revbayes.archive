#ifndef WORKSPACEUTILS_H
#define	WORKSPACEUTILS_H

#include "Workspace.h"

#include "RevObject.h"
#include "RevPtr.h"

#include "Variable.h"
#include "RlFunction.h"
#include "ArgumentRules.h"
#include "MethodTable.h"

#include <map>
#include <vector>
#include <algorithm>
#include <stdbool.h>
#include <stdio.h>

#include <boost/foreach.hpp>

typedef std::vector<std::string> StringVector;
typedef std::vector<RevLanguage::Function *> FunctionVector;
typedef std::vector<RevLanguage::Argument> ArgumentVector;
typedef std::multimap<std::string, RevLanguage::Function*> FunctionMap;
typedef std::map<std::string, RevLanguage::RevPtr<RevLanguage::Variable> > VariableTable;
typedef std::map<std::string, RevLanguage::RevObject*> TypeTable;

class WorkspaceUtils {
public:
    
    struct FunctionSignature{
        std::string returnType;
        std::string name;
        StringVector arguments;
    };

    //////////// functions //////////////////////

    StringVector getFunctions(void) {
        FunctionMap functionsMap = RevLanguage::Workspace::userWorkspace().getFunctionTable().getTableCopy(true);

        StringVector functions;
        for (FunctionMap::iterator it = functionsMap.begin(); it != functionsMap.end(); ++it) {
            functions.push_back(it->first);
        }

        return makeUnique(functions);
    }

    StringVector getFunctionParameters(std::string name) {

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

    std::vector<FunctionSignature> getFunctionSignatures(std::string name) {
        std::vector<FunctionSignature> result;
        if (!RevLanguage::Workspace::globalWorkspace().existsFunction(name)) {
            return result;
        }

        FunctionVector v = RevLanguage::Workspace::globalWorkspace().getFunctionTable().findFunctions(name);

        for (FunctionVector::iterator it = v.begin(); it != v.end(); it++) {
            FunctionSignature functionSignature;
            functionSignature.returnType = (*it)->getReturnType().getType();
            functionSignature.name = name;
                    
            const RevLanguage::ArgumentRules& argRules = (*it)->getArgumentRules();
            for (size_t i = 0; i < argRules.size(); i++) {
                functionSignature.arguments.push_back(argRules[i].getArgumentLabel());
            }
            result.push_back(functionSignature);
        }

        return result;
    }

    bool isFunction(std::string name) {
        return RevLanguage::Workspace::globalWorkspace().existsFunction(name);
    }


    //////////// variables  //////////////////////

    StringVector getVariables(bool all = false) {

        StringVector objects;

        VariableTable v = RevLanguage::Workspace::userWorkspace().getVariableTable();

        for (VariableTable::iterator it = v.begin(); it != v.end(); ++it) {
            objects.push_back(it->first);
        }

        if (all) {

            v = RevLanguage::Workspace::globalWorkspace().getVariableTable();

            for (VariableTable::iterator it = v.begin(); it != v.end(); ++it) {
                objects.push_back(it->first);
            }
        }

        return makeUnique(objects);
    }

    StringVector getVariableMembers(std::string name){
        StringVector sv;

        if (!isVariable(name)) {
            return sv;
        }

        RevLanguage::RevPtr<RevLanguage::Variable> variable = RevLanguage::Workspace::userWorkspace().getVariable(name);
                
        RevLanguage::MethodTable &methods = const_cast<RevLanguage::MethodTable&> (variable->getRevObject().getMethods());

        std::multimap<std::string, RevLanguage::Function*> printTable = methods.getTableCopy(false);
        for (std::multimap<std::string, RevLanguage::Function *>::const_iterator i = printTable.begin(); i != printTable.end(); i++) {
            sv.push_back(i->first);
        }

        return sv;
    }
    
    bool isVariable(std::string name){
        return RevLanguage::Workspace::userWorkspace().existsVariable(name);
    }


    //////////// types //////////////////////

    StringVector getMethodParameters(std::string typeName, std::string methodName) {
        StringVector sv;
        // make sure type exists
        if (!isType(typeName)) {
            return sv;
        }

        RevLanguage::RevObject *type = RevLanguage::Workspace::globalWorkspace().makeNewDefaultObject(typeName);
        RevLanguage::MethodTable &methods = const_cast<RevLanguage::MethodTable&> (type->getMethods());

        std::multimap<std::string, RevLanguage::Function*> printTable = methods.getTableCopy(false);
        for (std::multimap<std::string, RevLanguage::Function *>::const_iterator i = printTable.begin(); i != printTable.end(); i++) {
            if (i->first == methodName) {
                const RevLanguage::ArgumentRules& argRules = i->second->getArgumentRules();
                for (size_t i = 0; i < argRules.size(); i++) {
                    sv.push_back(argRules[i].getArgumentLabel());
                }
            }
        }

        return makeUnique(sv);
    }

    StringVector getTypes(bool all = true) {

        StringVector objects;

        TypeTable t = RevLanguage::Workspace::userWorkspace().getTypeTable();

        for (TypeTable::iterator it = t.begin(); it != t.end(); ++it) {
            objects.push_back(it->first);
        }

        if (all) {

            t = RevLanguage::Workspace::globalWorkspace().getTypeTable();

            for (TypeTable::iterator it = t.begin(); it != t.end(); ++it) {
                objects.push_back(it->first);
            }
        }

        return makeUnique(objects);
    }

    StringVector getTypeMembers(std::string name) {
        StringVector sv;

        if (!isType(name)) {
            return sv;
        }

        RevLanguage::RevObject *type = RevLanguage::Workspace::globalWorkspace().makeNewDefaultObject(name);
        RevLanguage::MethodTable &methods = const_cast<RevLanguage::MethodTable&> (type->getMethods());

        std::multimap<std::string, RevLanguage::Function*> printTable = methods.getTableCopy(false);
        for (std::multimap<std::string, RevLanguage::Function *>::const_iterator i = printTable.begin(); i != printTable.end(); i++) {
            sv.push_back(i->first);
        }

        return sv;
    }

    bool isType(std::string name) {
        return RevLanguage::Workspace::globalWorkspace().existsType(name);
    }

    //////////// objects //////////////////////

    // hide the distinction of variables and types

    StringVector getObjects(bool all = true) {
        StringVector t = getTypes(all);
        StringVector v = getVariables(all);
        t.insert(t.end(), v.begin(), v.end());
        return makeUnique(t);
    }
    
    StringVector getObjectMembers(std::string name){
        StringVector sv;
        
        if(isType(name)){ 
            return makeUnique(getTypeMembers(name));
        }
        if(isVariable(name)){ 
            return makeUnique(getVariableMembers(name));
        }
        return sv;
    }
    
    bool isObject(std::string name){        
        return isType(name) || isVariable(name);
    }


private:

    StringVector makeUnique(StringVector v) {
        StringVector result;
        for (StringVector::iterator it = v.begin(); it < std::unique(v.begin(), v.end()); it++) {
            result.push_back(*it);
        }
        return result;
    }
};




#endif	/* WORKSPACEUTILS_H */

