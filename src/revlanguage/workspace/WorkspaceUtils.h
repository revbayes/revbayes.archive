#ifndef WORKSPACEUTILS_H
#define	WORKSPACEUTILS_H

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <stdbool.h>

typedef std::vector<std::string> StringVector;

class WorkspaceUtils {
public:
    StringVector getFunctions(void);
    StringVector getFunctionParameters(std::string name);
    StringVector getFunctionSignatures(std::string name);    
    bool isFunction(std::string name);

    StringVector getVariables(void);
    StringVector getVariableMembers(std::string name);
    bool isVariable(std::string name);

    StringVector getTypes(void);
    StringVector getTypeMembers(std::string name);
    bool isType(std::string name);

    // to hide the distinction of variables and types
    StringVector getObjects(void);
    StringVector getObjectMembers(std::string name);
    bool isObject(std::string name);


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

