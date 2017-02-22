#include "ContinuousCharacterData.h"
#include "ContinuousTaxonData.h"
#include "NclReader.h"
#include "NexusWriter.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "TypedDagNode.h"

#include <string>
#include <algorithm>

using namespace RevBayesCore;

/**
 * Default constructor,
 * Does nothing except instanciating the object.
 */
ContinuousCharacterData::ContinuousCharacterData() 
{
    
}



/**
 * Index (const) operator to access a TaxonData object at position i.
 *
 * \param[in]    i    The position of the TaxonData object.
 *
 * \return            The TaxonData object at position i.
 */
ContinuousTaxonData& ContinuousCharacterData::operator[]( const size_t i )
{
    
    return getTaxonData( i );
}



/** 
 * Index (const) operator to access a TaxonData object at position i.
 *
 * \param[in]    i    The position of the TaxonData object.
 *
 * \return            The TaxonData object at position i.
 */
const ContinuousTaxonData& ContinuousCharacterData::operator[]( const size_t i ) const 
{
    
    return getTaxonData( i );
}


/**
 * Add another character data object to this character data object.
 *
 * \param[in]    obsd    The CharacterData object that should be added.
 */
ContinuousCharacterData& ContinuousCharacterData::concatenate(const AbstractCharacterData &obsd, std::string type)
{
    std::cout << "IN ContinuousCharacterData::add" <<std::endl;
    const ContinuousCharacterData* rhs = dynamic_cast<const ContinuousCharacterData* >( &obsd );
    if ( rhs == NULL )
    {
        throw RbException("Adding wrong character data type into ContinuousCharacterData!!!");
    }
    
    
    return concatenate( *rhs, type );
}


/**
 * Add another character data object to this character data object.
 *
 * \param[in]    obsd    The CharacterData object that should be added.
 */
ContinuousCharacterData& ContinuousCharacterData::concatenate(const HomologousCharacterData &obsd, std::string type)
{
    std::cout << "IN ContinuousCharacterData::add" <<std::endl;
    const ContinuousCharacterData* rhs = dynamic_cast<const ContinuousCharacterData* >( &obsd );
    if ( rhs == NULL )
    {
        throw RbException("Adding wrong character data type into ContinuousCharacterData!!!");
    }
    
    
    return concatenate( *rhs, type );
}


/**
 * Add another character data object to this character data object.
 *
 * \param[in]    obsd    The CharacterData object that should be added.
 */
ContinuousCharacterData& ContinuousCharacterData::concatenate(const ContinuousCharacterData &obsd, std::string type)
{
    
    // check if both have the same number of taxa
    if ( taxa.size() != obsd.getNumberOfTaxa() && type != "union" && type != "intersection")
    {
        throw RbException("Cannot add two character data objects with different number of taxa!");
    }
    std::vector<string> toDelete;
    std::vector<bool> used = std::vector<bool>(obsd.getNumberOfTaxa(),false);
    for (size_t i=0; i<taxa.size(); i++ )
    {
        const std::string &n = taxa[i].getName();
        ContinuousTaxonData& taxon = getTaxonData( n );
        
        size_t idx = obsd.getIndexOfTaxon( n );
        if ( idx != RbConstants::Size_t::inf)
        {
            used[idx] = true;
            taxon.concatenate( obsd.getTaxonData( n ) );

        }
        else if (type == "intersection")
        {
            toDelete.push_back(n);
        }
        else if (type == "union")
        {
            AbstractTaxonData *taxon_data = obsd.getTaxonData(0).clone();
            taxon_data->setAllCharactersMissing();
            taxon.concatenate( *taxon_data );
            delete taxon_data;
        }
        else
        {
            throw RbException("Cannot add two character data objects because second character data object has no taxon with name '" + n + "n'!");
        }
    }
    for (size_t i=0; i<toDelete.size(); i++)
    {
        deleteTaxon(toDelete[i]);
    }
    
    for (size_t i=0; i<used.size(); i++)
    {
        if ( used[i] == false )
        {
            if(type=="union")
            {
                std::string n = obsd.getTaxonNameWithIndex(i);
                addMissingTaxon( n );

                AbstractTaxonData& taxon = getTaxonData( n );
                const AbstractTaxonData& taxon_data = obsd.getTaxonData(i);

                taxon.concatenate( taxon_data );
            }
            else if(type != "intersection")
            {
                throw RbException("Cannot concatenate two character data objects because first character data object has no taxon with name '" + obsd.getTaxonNameWithIndex(i) + "n'!");
            }
        }
    }
    
    return *this;
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the object. 
 */
ContinuousCharacterData* ContinuousCharacterData::clone( void ) const 
{
    
    return new ContinuousCharacterData(*this);
}


/**
 * Exclude all characters.
 * We don't actually delete the characters but mark them for exclusion.
 */
void ContinuousCharacterData::excludeAllCharacters(void)
{
    
    for (size_t i = 0; i < getTaxonData( 0 ).getNumberOfCharacters(); ++i)
    {
        deletedCharacters.insert( i );
    }
    
}


/** 
 * Exclude a character.
 * We don't actually delete the character but mark it for exclusion.
 *
 * \param[in]    i    The position of the character that will be excluded.
 */
void ContinuousCharacterData::excludeCharacter(size_t i) 
{
    
    if (i >= getTaxonData( 0 ).getNumberOfCharacters() )
    {
        std::stringstream o;
        o << "Only " << getNumberOfCharacters() << " characters in matrix";
        throw RbException( o.str() );
    }
    
    
    deletedCharacters.insert( i );
    
}

void ContinuousCharacterData::executeMethod(const std::string &n, const std::vector<const DagNode *> &args, double &rv) const
{
    
    if ( n == "get" )
    {
        int index_taxon = static_cast<const TypedDagNode<int> *>( args[0] )->getValue()-1;
        int index_site = static_cast<const TypedDagNode<int> *>( args[1] )->getValue()-1;
        rv = getTaxonData(index_taxon)[index_site];
    }
    else
    {
        throw RbException("A continuous character data object does not have a member method called '" + n + "'.");
    }
    
}



/** 
 * Get the cn-th character of the tn-th taxon.
 *
 * \param[in]    tn     The index/position of the taxon.
 * \param[in]    cn     The position of the character.
 *
 * \return              The cn-th character of the tn-th taxon. 
 */
const double& ContinuousCharacterData::getCharacter( size_t tn, size_t cn ) const
{
    
    if ( cn >= getNumberOfCharacters() )
        throw RbException( "Character index out of range" );
    
    return getTaxonData( tn )[cn];
}


/**
 * Get the data type of the character stored in this object.
 *
 * \return      The data type (e.g. DNA, RNA or Standard).
 */
std::string ContinuousCharacterData::getDataType(void) const 
{
    
    std::string dt = "Continuous";
    
    return dt;
}



/** 
 * Get the number of characters in taxon data object. 
 * This i regardless of whether the character are included or excluded.
 * For simplicity we assume that all taxon data objects contain the same number
 * of character and thus we simply return the number from the first taxon data object.
 *
 * \return    The total number of characters
 */
size_t ContinuousCharacterData::getNumberOfCharacters(void) const 
{
    
    if (getNumberOfTaxa() > 0) 
    {
        return getTaxonData(0).getNumberOfCharacters();
    }
    
    return 0;
}


/** 
 * Get the number of characters in taxon data object. 
 * This i regardless of whether the character are included or excluded.
 * For simplicity we assume that all taxon data objects contain the same number
 * of character and thus we simply return the number from the first taxon data object.
 *
 * \return    The total number of characters
 */
size_t ContinuousCharacterData::getNumberOfIncludedCharacters(void) const {
    
    if (getNumberOfTaxa() > 0) 
    {
        return getTaxonData(0).getNumberOfCharacters() - deletedCharacters.size();
    }
    return 0;
}


/**
 * Get the taxon data object with index tn.
 *
 * \return     A const reference to the taxon data object at position tn.
 */
const ContinuousTaxonData& ContinuousCharacterData::getTaxonData( size_t tn ) const 
{
    
    if ( tn >= getNumberOfTaxa() )
    {
        throw RbException( "Taxon index out of range" );
    }
    
    const std::string& name = taxa[tn].getName();
    const std::map<std::string, AbstractTaxonData* >::const_iterator& i = taxonMap.find( name );
    
    if (i != taxonMap.end() ) 
    {
        return static_cast<const ContinuousTaxonData& >(*i->second);
    }
    else 
    {
        throw RbException("Cannot find taxon '" + name + "' in the CharacterData matrix.");
    }
    
}


/** 
 * Get the taxon data object at position tn.
 *
 * \return     A non-const reference to the taxon data object at position tn.
 */
ContinuousTaxonData& ContinuousCharacterData::getTaxonData( size_t tn ) 
{
    
    if ( tn >= getNumberOfTaxa() )
    {
        throw RbException( "Taxon index out of range" );
    }
    
    const std::string& name = taxa[tn].getName();
    const std::map<std::string, AbstractTaxonData* >::iterator& i = taxonMap.find( name );
    
    if (i != taxonMap.end() ) 
    {
        return static_cast< ContinuousTaxonData& >(*i->second);
    }
    else 
    {
        throw RbException("Cannot find taxon '" + name + "' in the CharacterData matrix.");
    }
    
}


/** 
 * Get the taxon data object with name tn.
 *
 * \return     A const reference to the taxon data object with name tn.
 */
const ContinuousTaxonData& ContinuousCharacterData::getTaxonData( const std::string &tn ) const 
{
    
    if ( tn == "" ) 
    {
        throw RbException("Ambiguous taxon name.");
    }
    
    const std::map<std::string, AbstractTaxonData* >::const_iterator& i = taxonMap.find(tn);
    
    if (i != taxonMap.end() ) 
    {
        return static_cast<const ContinuousTaxonData& >(*i->second);
    }
    else 
    {
        throw RbException("Cannot find taxon '" + tn + "' in the CharacterData matrix.");
    }
    
}


/** 
 * Get the taxon data object with name tn.
 *
 * \return     A non-const reference to the taxon data object with name tn.
 */
ContinuousTaxonData& ContinuousCharacterData::getTaxonData( const std::string &tn ) 
{
    
    
    if ( tn == "" ) 
    {
        throw RbException("Ambiguous taxon name.");
    }
    
    const std::map<std::string, AbstractTaxonData* >::iterator& i = taxonMap.find(tn);
    
    if (i != taxonMap.end() ) 
    {
        return static_cast< ContinuousTaxonData& >(*i->second);
    }
    else 
    {
        
        throw RbException("Cannot find taxon '" + tn + "' in the CharacterData matrix.");
    }
    
}


/**
 * Include a character.
 * Since we didn't actually deleted the character but marked it for exclusion
 * we can now simply remove the flag.
 *
 * \param[in]    i    The position of the character that will be included.
 */
void ContinuousCharacterData::includeCharacter(size_t i)
{
    
    if (i >= getTaxonData( 0 ).getNumberOfCharacters() )
    {
        std::stringstream o;
        o << "Only " << getNumberOfCharacters() << " characters in matrix";
        throw RbException( o.str() );
    }
    
    
    deletedCharacters.erase( i );
    
}


void ContinuousCharacterData::initFromFile(const std::string &dir, const std::string &fn)
{
    RbFileManager fm = RbFileManager(dir, fn + ".nex");
    fm.createDirectoryForFile();
    
    // get the global instance of the NCL reader and clear warnings from its warnings buffer
    NclReader reader = NclReader();
    
    std::string myFileType = "nexus";
    std::string dType = "Continuous";
    
    std::string suffix = "|" + dType;
    suffix += "|unknown";
    myFileType += suffix;
        
    std::vector<AbstractCharacterData*> m_i = reader.readMatrices( fm.getFullFileName(), myFileType );
    ContinuousCharacterData *coreM = static_cast<ContinuousCharacterData *>( m_i[0] );

    *this = *coreM;
    
    delete coreM;
    
}


void ContinuousCharacterData::initFromString(const std::string &s)
{
    throw RbException("Cannot initialize a continuous character data matrix from a string.");
}


/**
 * Is the character excluded?
 *
 * \param[in]    i   The position of the character.
 */
bool ContinuousCharacterData::isCharacterExcluded(size_t i) const 
{
    
	std::set<size_t>::const_iterator it = deletedCharacters.find( i );
	if ( it != deletedCharacters.end() )
		return true;
    
    return false;
}


bool ContinuousCharacterData::isCharacterResolved(size_t txIdx, size_t chIdx) const
{

    const ContinuousTaxonData& td = getTaxonData(txIdx);
    return td.isCharacterResolved(chIdx);
}

bool ContinuousCharacterData::isCharacterResolved(const std::string &tn, size_t chIdx) const
{

    const ContinuousTaxonData& td = getTaxonData(tn);
    return td.isCharacterResolved(chIdx);
}


/**
 * Remove all the excluded character.
 *
 */
void ContinuousCharacterData::removeExcludedCharacters( void )
{
    
    for (std::map<std::string, AbstractTaxonData*>::iterator it = taxonMap.begin(); it != taxonMap.end(); ++it)
    {
        it->second->removeCharacters( deletedCharacters );
    }
    
}


/** 
 * Restore a character. We simply do not mark the character as excluded anymore.
 *
 * \param[in]    i    The position of the character to restore.
 */
void ContinuousCharacterData::restoreCharacter(size_t i) 
{
    
    if (i >= getNumberOfCharacters() )
        throw RbException( "Character index out of range" );
    
    deletedCharacters.erase( i );
    
}


void ContinuousCharacterData::writeToFile(const std::string &dir, const std::string &fn) const
{
    RbFileManager fm = RbFileManager(dir, fn + ".nex");
    fm.createDirectoryForFile();
    
    NexusWriter nw( fm.getFullFileName() );
    nw.openStream(false);
    
    nw.writeNexusBlock( *this );
    
    nw.closeStream();
    
}


