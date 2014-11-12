
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "ModelVector.h"
#include "Monitor.h"
#include "RevObject.h"
#include "RbException.h"
#include "RlMonitor.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Monitor::Monitor(void) : WorkspaceToCoreWrapperObject<RevBayesCore::Monitor>() {
    
}


Monitor::Monitor(RevBayesCore::Monitor *m) : WorkspaceToCoreWrapperObject<RevBayesCore::Monitor>( m ) {
    
}


Monitor::Monitor(const Monitor &m) : WorkspaceToCoreWrapperObject<RevBayesCore::Monitor>( m ) {
    
}


/** Get Rev type of object */
const std::string& Monitor::getClassType(void) { 
    
    static std::string revType = "Monitor";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Monitor::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::Monitor>::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}

