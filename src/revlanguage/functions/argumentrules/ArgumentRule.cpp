#include "Argument.h"
#include "ArgumentRule.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RlFunction.h"
#include "TypeSpec.h"
#include "Workspace.h"

#include <sstream>

using namespace RevLanguage;


/**
 * Construct rule with single type;
 * use "" for no label.
 */
ArgumentRule::ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, const std::string& argDesc, EvaluationType et, DagNodeType dt ) :
    argTypeSpecs( 1, argTypeSp ),
    defaultVar( NULL ),
    evalType( et ),
    nodeType( dt ),
    label( argName ),
    description( argDesc ),
    hasDefaultVal( false )
{
    
}

/**
 * Construct rule with single type;
 * use "" for no label.
 */
ArgumentRule::ArgumentRule(const std::string& argName, const TypeSpec& argTypeSp, const std::string& argDesc, EvaluationType et, DagNodeType dt, RevObject *defVal) :
    argTypeSpecs( 1, argTypeSp ),
    defaultVar( new RevVariable( defVal ) ),
    evalType( et ),
    nodeType( dt ),
    label( argName ),
    description( argDesc ),
    hasDefaultVal( true )
{
    
}


/**
 * Construct rule with multiple types;
 * use "" for no label.
 */
ArgumentRule::ArgumentRule(const std::string& argName, const std::vector<TypeSpec>& argTypeSp, const std::string& argDesc, EvaluationType et, DagNodeType dt ) :
    argTypeSpecs( argTypeSp ),
    defaultVar( NULL ),
    evalType( et ),
    nodeType( dt ),
    label( argName ),
    description( argDesc ),
    hasDefaultVal( false )
{
    
}


/**
 * Construct rule with multiple types;
 * use "" for no label.
 */
ArgumentRule::ArgumentRule(const std::string& argName, const std::vector<TypeSpec>& argTypeSp, const std::string& argDesc, EvaluationType et, DagNodeType dt, RevObject *defVal) :
    argTypeSpecs( argTypeSp ),
    defaultVar( new RevVariable( defVal ) ),
    evalType( et ),
    nodeType( dt ),
    label( argName ),
    description( argDesc ),
    hasDefaultVal( true )
{
    
}



ArgumentRule* RevLanguage::ArgumentRule::clone( void ) const
{

    return new ArgumentRule( *this );
}


/**
 * Fit a variable into an argument according to the argument rule. If necessary and
 * appropriate, we do type conversion or type promotion.
 *
 *
 *
 * @todo To conform to the old code we change the required type of the incoming
 *       variable wrapper here. We need to change this so that we do not change
 *       the wrapper here, but make sure that if the argument variable is inserted
 *       in a member variable or container element slot, that the slot variable
 *       wrapper, which should be unique (not the same as the incoming variable
 *       wrapper), has the right required type.
 */
Argument ArgumentRule::fitArgument( Argument& arg, bool once ) const
{

    RevPtr<RevVariable> theVar = arg.getVariable();
    if ( evalType == BY_VALUE || theVar->isWorkspaceVariable() || theVar->getRevObject().isConstant() )
    {
        once = true;
    }
    
    
    for ( std::vector<TypeSpec>::const_iterator it = argTypeSpecs.begin(); it != argTypeSpecs.end(); ++it )
    {
        if ( evalType == BY_VALUE )
        {
            if ( theVar->getRevObject().isType( *it ) )
            {
                RevPtr<RevVariable> valueVar = RevPtr<RevVariable>( new RevVariable(theVar->getRevObject().clone(),arg.getLabel()) );
                return Argument( valueVar, getArgumentLabel(), false );
            }
            else if ( theVar->getRevObject().isConvertibleTo( *it, once ) != -1)
            {
                // Fit by type conversion. For now, we also modify the type of the incoming variable wrapper.
                RevObject* convertedObject = theVar->getRevObject().convertTo( *it );

                RevPtr<RevVariable> valueVar = RevPtr<RevVariable>( new RevVariable(convertedObject,arg.getLabel()) );
                return Argument( valueVar, getArgumentLabel(), false );
                
            }
        } // if (by-value)
        else
        {
            if ( theVar->getRevObject().isType( *it ) )
            {
                // For now, change the required type of the incoming variable wrapper
                theVar->setRequiredTypeSpec( *it );
            
                if ( !isEllipsis() )
                {
                    return Argument( theVar, getArgumentLabel(), evalType == BY_CONSTANT_REFERENCE );
                }
                else
                {
                    return Argument( theVar, arg.getLabel(), true );
                }
            
            }
            else if ( theVar->getRevObject().isConvertibleTo( *it, once ) != -1  && (*it).isDerivedOf( theVar->getRequiredTypeSpec() ))
            {
                // Fit by type conversion. For now, we also modify the type of the incoming variable wrapper.
                RevObject* convertedObject = theVar->getRevObject().convertTo( *it );
                theVar->replaceRevObject( convertedObject );
                theVar->setRequiredTypeSpec( *it );
                if ( !isEllipsis() )
                {
                    return Argument( theVar, getArgumentLabel(), false );
                }
                else
                {
                    return Argument( theVar, arg.getLabel(), false );
                }
            }
            else
            {
                // Fit by type conversion function
            
                const TypeSpec& typeFrom = theVar->getRevObject().getTypeSpec();
                const TypeSpec& typeTo   = *it;
            
                // create the function name
                std::string functionName = "_" + typeFrom.getType() + "2" + typeTo.getType();
                
                // Package arguments
                std::vector<Argument> args;
                Argument theArg = Argument( theVar, "arg" );
                args.push_back( theVar );
                
                Environment& env = Workspace::globalWorkspace();
            
                try
                {
                    Function* func = env.getFunction(functionName, args, once).clone();

                    // Allow the function to process the arguments
                    func->processArguments( args, once );
            
                    // Set the execution environment of the function
                    func->setExecutionEnviroment( &env );
                
                    // Evaluate the function
                    RevPtr<RevVariable> conversionVar = func->execute();
                
                    // free the memory
                    delete func;
                
                    conversionVar->setHiddenVariableState( true );
                    conversionVar->setRequiredTypeSpec( *it );
                
                    return Argument( conversionVar, getArgumentLabel(), evalType == BY_CONSTANT_REFERENCE );
                
                }
                catch (RbException e)
                {
                // we do nothing here
                }
                
            } // else (type conversion function)

            
        } // else (not by-value)

    }
        
    throw RbException( "Argument type mismatch fitting a " + theVar->getRevObject().getType() + " argument to formal " +
                        getArgumentTypeSpec()[0].getType() + " " + getArgumentLabel() );
}


ArgumentRule::DagNodeType ArgumentRule::getArgumentDagNodeType( void ) const
{
    // return the internal value
    return nodeType;
}


const std::string& ArgumentRule::getArgumentLabel( void ) const
{
    return label;
}


const std::vector<TypeSpec>& ArgumentRule::getArgumentTypeSpec(void) const
{
    return argTypeSpecs;
}



const RevVariable& ArgumentRule::getDefaultVariable( void ) const
{
    
    if ( defaultVar == NULL ) 
    {
        throw RbException("Cannot get default variable \"" + label + "\"");
    }
    
    return *defaultVar;
}


const std::string& ArgumentRule::getArgumentDescription( void ) const
{
    return description;
}


ArgumentRule::EvaluationType ArgumentRule::getEvaluationType( void ) const
{
    
    return evalType;
}


bool ArgumentRule::hasDefault(void) const
{
    
    return hasDefaultVal;
}


/**
 * Test if argument is valid. The boolean flag 'once' is used to signal whether the argument matching
 * is done in a static or a dynamic context. If the rule is constant, then the argument matching
 * is done in a static context (evaluate-once context) regardless of the setting of the once flag.
 * If the argument is constant, we try type promotion if permitted by the variable required type.
 *
 * @todo See the TODOs for fitArgument(...)
 */
double ArgumentRule::isArgumentValid( Argument &arg, bool once) const
{
    
    RevPtr<RevVariable> theVar = arg.getVariable();
    if ( theVar == NULL )
    {
        return -1;
    }
    
    if ( evalType == BY_VALUE || theVar->isWorkspaceVariable() || ( theVar->getRevObject().isModelObject() && theVar->getRevObject().getDagNode()->getDagNodeType() == RevBayesCore::DagNode::CONSTANT) )
    {
        once = true;
    }
    
    if ( nodeType == STOCHASTIC && theVar->getRevObject().getDagNode()->getDagNodeType() != RevBayesCore::DagNode::STOCHASTIC )
    {
        return -1;
    }
    else if ( nodeType == DETERMINISTIC && theVar->getRevObject().getDagNode()->getDagNodeType() != RevBayesCore::DagNode::DETERMINISTIC )
    {
        return -1;
    }

    for ( std::vector<TypeSpec>::const_iterator it = argTypeSpecs.begin(); it != argTypeSpecs.end(); ++it )
    {
        if ( theVar->getRevObject().isType( *it ) )
        {
            return 0.0;
        }
        else if ( theVar->getRevObject().isConvertibleTo( *it, once ) != -1 && (*it).isDerivedOf( theVar->getRequiredTypeSpec() ) )
        {
            return theVar->getRevObject().isConvertibleTo( *it, once );
        }
        else if ( theVar->getRevObject().isConvertibleTo( *it, once ) != -1 && evalType == BY_VALUE )
        {
            return theVar->getRevObject().isConvertibleTo( *it, once );
        }

//        else if ( once == true &&
////                 !var->isAssignable() &&
//                  theVar->getRevObject().isConvertibleTo( *it, true ) != -1 &&
//                  (*it).isDerivedOf( theVar->getRequiredTypeSpec() )
//                )
//        {
//            return theVar->getRevObject().isConvertibleTo( *it, true );
//        }
        else if ( nodeType != STOCHASTIC )
        {
            
            const TypeSpec& typeFrom = theVar->getRevObject().getTypeSpec();
            const TypeSpec& typeTo   = *it;
            
            // create the function name
            std::string functionName = "_" + typeFrom.getType() + "2" + typeTo.getType();
            
            // Package arguments
            std::vector<Argument> args;
            Argument theArg = Argument( theVar, "arg" );
            args.push_back( theVar );
            
            Environment& env = Workspace::globalWorkspace();
            try
            {
                // we just want to check if the function exists and can be found
                env.getFunction(functionName, args, once);
                return 0.1;
            }
            catch (RbException e)
            {
                // we do nothing here
            }

        }
        
    }
    
    return -1;
}



bool RevLanguage::ArgumentRule::isEllipsis( void ) const
{
    
    return false;
}
 


/**
 * Print value for user (in descriptions of functions, for instance). We apparently do
 * not use the isConst flag to denote whether an argument is supposed to be passed as
 * a constant currently, so the printing of this modifier is suspended for now.
 *
 */
void ArgumentRule::printValue(std::ostream &o) const
{

    for ( std::vector<TypeSpec>::const_iterator it = argTypeSpecs.begin(); it != argTypeSpecs.end(); ++it )
    {
        if ( it != argTypeSpecs.begin() )
        {
            o << "|";
        }
        
        o << (*it).getType();
    }
    
    // create the default DAG type of the passed-in argument
    std::string dagtype = "";
    // get the type if the variable wasn't NULL
    if ( nodeType == ArgumentRule::DETERMINISTIC )
    {
        dagtype = "<deterministic>";
    }
    else if ( nodeType == ArgumentRule::STOCHASTIC )
    {
        dagtype = "<stochastic>";
    }
    else if ( nodeType == ArgumentRule::CONSTANT )
    {
        dagtype = "<constant>";
    }
    else if ( nodeType == ArgumentRule::DYNAMIC )
    {
        dagtype = "<dynamic>";
    }
    else if ( nodeType == ArgumentRule::ANY )
    {
        dagtype = "<any>";
    }
    else
    {
        dagtype = "<?>";
    }
    o << dagtype;
    
    o << " " << label;
}

