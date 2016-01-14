#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "DelimitedDataReader.h"
#include "HomologousDiscreteCharacterData.h"
#include "Ellipsis.h"
#include "Func_readDataDelimitedFile.h"
#include "ModelVector.h"
#include "NaturalNumbersState.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "RlContinuousCharacterData.h"
#include "RlHomologousDiscreteCharacterData.h"
#include "RlNaturalNumbersState.h"
#include "RlMatrixReal.h"
#include "RlStandardState.h"
#include "RlString.h"
#include "StringUtilities.h"


using namespace RevLanguage;

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_readDataDelimitedFile* Func_readDataDelimitedFile::clone( void ) const
{
    
    return new Func_readDataDelimitedFile( *this );
}


std::string Func_readDataDelimitedFile::bitToState(const std::string &s)
{
    
    std::stringstream ss;
    char* ptr;
    long parsed = strtol(s.c_str(), &ptr, 2);
    
    if (parsed > RbConstants::Integer::max)
    {
        throw RbException("ERROR: readTSVBitsetData token " + s + " too large to store as NaturalNumber");
    }
    
    if (s.find_first_not_of("01") != std::string::npos)
    {
        throw RbException("ERROR: readTSVBitsetData token " + s + " contains non-binary characters");
    }
    
    ss << parsed;
    
    return ss.str();
}


/** Execute function */
RevPtr<RevVariable> Func_readDataDelimitedFile::execute( void )
{
    
    // get the information from the arguments for reading the file
    const std::string& fn  = static_cast<const RlString&>( args[0].getVariable()->getRevObject() ).getValue();
    const std::string& dt  = static_cast<const RlString&>( args[1].getVariable()->getRevObject() ).getValue();
    const std::string& del = static_cast<const RlString&>( args[2].getVariable()->getRevObject() ).getValue();
    
    if (dt == "Continuous")
    {
        
        // get data from file
        RevBayesCore::DelimitedDataReader* tsv_data = new RevBayesCore::DelimitedDataReader(fn, del[0]);
        const std::vector<std::vector<std::string> >&data = tsv_data->getChars();
        
        bool isRealPos = true;
        
        // loop through data and get each Continuous value
        ModelVector<ModelVector<Real> > mReal;
        for (size_t i = 0; i < data.size(); ++i)
        {
            // make row
            ModelVector<Real> mRealRow;
            
            for (size_t j= 0; j < data[i].size(); ++j)
            {
                double v = atof( data[i][j].c_str() );
                if (v < 0)
                    isRealPos = false;
                
                // add value to row
                mRealRow.push_back(atof( data[i][j].c_str() ));
            }
            
            // add row to matrix
            mReal.push_back(mRealRow);
        }
        
        if (isRealPos)
        {
            ModelVector<ModelVector<RealPos> > mRealPos;
            for (size_t i = 0; i < mReal.size(); ++i)
            {
                ModelVector<RealPos> mRealPosRow;
                for (size_t j = 0; j < mReal[i].size(); ++j) {
//                    std::cout << mReal[i][j] << " ";
                    mRealPosRow.push_back( mReal[i][j] );
                }
//                std::cout << "\n";
                mRealPos.push_back(mRealPosRow);
            }
//            std::cout << "\n";
            return new RevVariable( new ModelVector<ModelVector<RealPos> >(mRealPos) );
        }
        else
        {
            return new RevVariable( new ModelVector<ModelVector<Real> >(mReal) );
        }
    }
    else
    {
        
        throw RbException( "Invalid data type. Valid data types are: NaturalNumbers" );
        
    }
}


/** Get argument rules */
const ArgumentRules& Func_readDataDelimitedFile::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
        
        argumentRules.push_back( new ArgumentRule( "file",      RlString::getClassTypeSpec(), "The name of the file to read in.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "type",      RlString::getClassTypeSpec(), "The type of data.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "delimiter", RlString::getClassTypeSpec(), "The delimiter between columns.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString( "\t" ) ) );
        rulesSet = true;
        
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_readDataDelimitedFile::getClassType(void)
{
    
    static std::string revType = "Func_readDataDelimitedFile";
    
    return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_readDataDelimitedFile::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_readDataDelimitedFile::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "readDataDelimitedFile";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_readDataDelimitedFile::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_readDataDelimitedFile::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = NaturalNumbersState::getClassTypeSpec();
    return returnTypeSpec;
}




