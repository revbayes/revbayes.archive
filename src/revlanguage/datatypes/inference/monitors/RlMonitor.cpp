
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "Monitor.h"
#include "RbLanguageObject.h"
#include "RbException.h"
#include "RlMonitor.h"
#include "TypeSpec.h"
#include "Vector.h"


using namespace RevLanguage;

Monitor::Monitor(void) : RlControlVariableWrapper<RevBayesCore::Monitor>() {
    
}


Monitor::Monitor(RevBayesCore::Monitor *m) : RlControlVariableWrapper<RevBayesCore::Monitor>( m ) {
    
}


Monitor::Monitor(const Monitor &m) : RlControlVariableWrapper<RevBayesCore::Monitor>( m ) {
    
}


/** Get class name of object */
const std::string& Monitor::getClassName(void) { 
    
    static std::string rbClassName = "Monitor";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Monitor::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RlControlVariableWrapper<RevBayesCore::Monitor>::getClassTypeSpec() ) );
    
	return rbClass; 
}

