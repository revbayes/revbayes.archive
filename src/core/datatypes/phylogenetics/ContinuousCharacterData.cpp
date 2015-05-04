#include "ContinuousCharacterData.h"
#include "ContinuousTaxonData.h"
#include "RbConstants.h"
#include "RbException.h"

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
const ContinuousTaxonData& ContinuousCharacterData::operator[]( const size_t i ) const 
{
    
    return getTaxonData( i );
}


/**
 * Add another character data object to this character data object.
 *
 * \param[in]    obsd    The CharacterData object that should be added.
 */
ContinuousCharacterData& ContinuousCharacterData::concatenate(const AbstractCharacterData &obsd)
{
    std::cout << "IN ContinuousCharacterData::add" <<std::endl;
    const ContinuousCharacterData* rhs = dynamic_cast<const ContinuousCharacterData* >( &obsd );
    if ( rhs == NULL )
    {
        throw RbException("Adding wrong character data type into ContinuousCharacterData!!!");
    }
    
    
    return concatenate( *rhs );
}


/**
 * Add another character data object to this character data object.
 *
 * \param[in]    obsd    The CharacterData object that should be added.
 */
ContinuousCharacterData& ContinuousCharacterData::concatenate(const ContinuousCharacterData &obsd)
{
    
    // check if both have the same number of taxa
    if ( sequenceNames.size() != obsd.getNumberOfTaxa() )
    {
        throw RbException("Cannot add two character data objects with different number of taxa!");
    }
    
    std::vector<bool> used = std::vector<bool>(obsd.getNumberOfTaxa(),false);
    for (size_t i=0; i<sequenceNames.size(); i++ )
    {
        const std::string &n = sequenceNames[i];
        ContinuousTaxonData& taxon = getTaxonData( n );
        
        size_t idx = obsd.getIndexOfTaxon( n );
        if ( idx != RbConstants::Size_t::inf)
        {
            used[idx] = true;
            taxon.concatenate( obsd.getTaxonData( n ) );

        }
        else
        {
            throw RbException("Cannot add two character data objects because second character data object has no taxon with name '" + n + "n'!");
        }
    }
    
    for (size_t i=0; i<used.size(); i++)
    {
        if ( used[i] == false )
        {
            throw RbException("Cannot add two character data objects because first character data object has no taxon with name '" + obsd.getTaxonNameWithIndex(i) + "n'!");
        }
    }
    
    return *this;
}


/** 
 * Add a sequence (TaxonData) to the character data object.
 *
 * \param[in]    obsd    The TaxonData object that should be added.
 */
void ContinuousCharacterData::addTaxonData(const AbstractTaxonData &obsd) 
{
    
#ifdef ASSERTIONS_ALL
    if ( dynamic_cast<const ContinuousTaxonData* >( &obsd ) == NULL ) 
    {
        throw RbException("Inserting wrong character type into CharacterData!!!");
    }
#endif
    
    // delegate the call to the specialized method
    addTaxonData( static_cast<const ContinuousTaxonData& >( obsd ) );
    
}


/** 
 * Add a sequence (TaxonData) to the character data object.
 *
 * \param[in]    obsd    The TaxonData object that should be added.
 */
void ContinuousCharacterData::addTaxonData(const ContinuousTaxonData &obs) 
{
    
    // add the sequence name to the list
    sequenceNames.push_back( obs.getTaxonName() );
    
    // add the sequence also as a member so that we can access it by name
    taxonMap.insert( std::pair<std::string, ContinuousTaxonData >( obs.getTaxonName(), obs ) );
    
}



/** 
 * Clear the object, that is, remove all TaxonData elements.
 */
void ContinuousCharacterData::clear( void ) 
{
    
    sequenceNames.clear();
    taxonMap.clear();
    
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
    
    for (size_t i = 0; i < getTaxonData( 0 ).size(); ++i)
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
    
    if (i >= getTaxonData( 0 ).size() ) 
    {
        std::stringstream o;
        o << "Only " << getNumberOfCharacters() << " characters in matrix";
        throw RbException( o.str() );
    }
    
    
    deletedCharacters.insert( i );
    
}


/** 
 * Exclude a taxon.
 * We don't actually delete the taxon but instead mark it for exclusion.
 *
 * \param[in]    i    The index of the taxon that will be excluded.
 */
void ContinuousCharacterData::excludeTaxon(size_t i) 
{
    
    if (i >= taxonMap.size()) 
    {
        std::stringstream o;
        o << "Only " << taxonMap.size() << " taxa in matrix";
        throw RbException( o.str() );
    }
    
    deletedTaxa.insert( i );
}


/** 
 * Exclude a taxon.
 * We don't actually delete the taxon but instead mark it for exclusion.
 *
 * \param[in]    s    The name of the taxon that will be excluded.
 */
void ContinuousCharacterData::excludeTaxon(const std::string& s)
{
    
    for (size_t i = 0; i < getNumberOfTaxa(); i++) 
    {
        if (s == sequenceNames[i] ) 
        {
            deletedTaxa.insert( i );
            break;
        }
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
std::string ContinuousCharacterData::getDatatype(void) const 
{
    
    std::string dt = "Continuous";
    
    return dt;
}


/**
 * Get the file name from whcih the character data object was read in.
 *
 * \return    The original file name.
 */
const std::string& ContinuousCharacterData::getFileName(void) const 
{
    
    return fileName;
}

/**
 * Get the file path from whcih the character data object was read in.
 *
 * \return    The original file path.
 */
const std::string& ContinuousCharacterData::getFilePath(void) const
{
    
    return filePath;
}


/**
 * Get the index of the taxon with name 'n'.
 *
 * \par[in]     The name.
 * \return      The index.
 *
 */
size_t ContinuousCharacterData::getIndexOfTaxon(const std::string &n) const
{
    long pos = std::find(sequenceNames.begin(), sequenceNames.end(), n) - sequenceNames.begin();
    
    if ( pos == sequenceNames.size() )
    {
        return RbConstants::Size_t::inf;
    }
    
    return size_t( pos );
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
 * Get the number of taxa currently stored in this object.
 *
 * \return       The number of taxa.
 */
size_t ContinuousCharacterData::getNumberOfTaxa(void) const 
{
    
    return sequenceNames.size();
}

/**
 * Get the number of included taxa currently stored in this object.
 *
 * \return       The number of included taxa.
 */
size_t ContinuousCharacterData::getNumberOfIncludedTaxa(void) const
{
    if (getNumberOfTaxa() > 0)
    {
        return getNumberOfTaxa() - deletedTaxa.size();
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
        throw RbException( "Taxon index out of range" );
    
    const std::string& name = sequenceNames[tn];
    const std::map<std::string, ContinuousTaxonData >::const_iterator& i = taxonMap.find( name ); 
    
    if (i != taxonMap.end() ) 
    {
        return i->second;
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
    
    const std::string& name = sequenceNames[tn];
    const std::map<std::string, ContinuousTaxonData >::iterator& i = taxonMap.find( name ); 
    
    if (i != taxonMap.end() ) 
    {
        return i->second;
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
    
    const std::map<std::string, ContinuousTaxonData >::const_iterator& i = taxonMap.find(tn); 
    
    if (i != taxonMap.end() ) 
    {
        return i->second;
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
    
    const std::map<std::string, ContinuousTaxonData >::iterator& i = taxonMap.find(tn); 
    
    if (i != taxonMap.end() ) 
    {
        return i->second;
    }
    else 
    {
        
        throw RbException("Cannot find taxon '" + tn + "' in the CharacterData matrix.");
    }
    
}


/**
 * Get the names of all taxa.
 *
 * \return     A vector of all taxon names.
 */
const std::vector<std::string>& ContinuousCharacterData::getTaxonNames( void ) const 
{
    
    return sequenceNames;
}



/** 
 * Get the taxon name with index idx.
 *
 * \param[in]    idx    The position of the taxon.
 *
 * \return              The name of the taxon.
 */
const std::string& ContinuousCharacterData::getTaxonNameWithIndex( size_t idx ) const 
{
    
    return sequenceNames[idx];
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
    
    if (i >= getTaxonData( 0 ).size() )
    {
        std::stringstream o;
        o << "Only " << getNumberOfCharacters() << " characters in matrix";
        throw RbException( o.str() );
    }
    
    
    deletedCharacters.erase( i );
    
}



/** 
 * Get the index of the taxon with name s.
 *
 * \param[in]    s    The name of the taxon.
 *
 * \return            The index of the taxon.
 */
size_t ContinuousCharacterData::indexOfTaxonWithName( const std::string& s ) const
{
    
    // search through all names
    for (size_t i=0; i<sequenceNames.size(); i++) 
    {
        if (s == sequenceNames[i] ) 
        {
            return i;
        }
    }
    
    return -1;
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


bool ContinuousCharacterData::isCharacterResolved(size_t txIdx, size_t chIdx) const {

    ContinuousTaxonData td = getTaxonData(txIdx);
    return td.isCharacterResolved(chIdx);
}

bool ContinuousCharacterData::isCharacterResolved(const std::string &tn, size_t chIdx) const {

    ContinuousTaxonData td = getTaxonData(tn);
    return td.isCharacterResolved(chIdx);
}

/**
 * Is the homology established, i.e., is the character data object aligned?
 *
 * \return     True/False whether the homology was established.
 */
bool ContinuousCharacterData::isHomologyEstablished(void) const 
{
    
    return homologyEstablished;
}


/**
 * Is the taxon excluded.
 *
 * \param[in]    idx    The position of the taxon in question.
 */
bool ContinuousCharacterData::isTaxonExcluded(size_t i) const 
{
    
	std::set<size_t>::const_iterator it = deletedTaxa.find( i );
	if ( it != deletedTaxa.end() )
		return true;
    
    return false;
}


/** 
 * Is the taxon excluded?
 *
 * \param[in]    s    The name of the taxon in question.
 */
bool ContinuousCharacterData::isTaxonExcluded(const std::string& s) const
{
    
    size_t i = indexOfTaxonWithName(s);
	std::set<size_t>::const_iterator it = deletedTaxa.find( i );
	if ( it != deletedTaxa.end() )
		return true;
    
    return false;
}


/**
 * Remove all the excluded character.
 *
 */
void ContinuousCharacterData::removeExludedCharacters( void )
{
    
    for (std::map<std::string, ContinuousTaxonData>::iterator it = taxonMap.begin(); it != taxonMap.end(); ++it)
    {
        it->second.removeCharacters( deletedCharacters );
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


/** 
 * Restore a taxon. We simply do not mark the taxon as excluded anymore
 *
 *
 * \param[in]    i    The position of the taxon in question.
 */
void ContinuousCharacterData::restoreTaxon(size_t i) 
{
    
    if ( i >= getNumberOfTaxa() )
        return;
    
    deletedTaxa.erase( i );
    
}


/** 
 * Restore a taxon. We simply do not mark the taxon as excluded anymore.
 *
 * \param[in]    s    The name of the taxon in question.
 */
void ContinuousCharacterData::restoreTaxon(const std::string& s)
{
    
    size_t i = indexOfTaxonWithName( s );
    
    deletedTaxa.erase( i );
    
}


/**
 * Set the original file name for this character data object.
 *
 * \param[in]    fn    The new file name.
 */
void ContinuousCharacterData::setFileName(const std::string& fn) 
{
    
    fileName = fn;
    
}

/**
 * Set the original file path for this character data object.
 *
 * \param[in]    fn    The new file path.
 */
void ContinuousCharacterData::setFilePath(const std::string& fn)
{
    
    filePath = fn;
    
}

/**
 * Set whether the homology has been established.
 *
 * \param[in]    tf    Whether the homology has been established.
 */
void ContinuousCharacterData::setHomologyEstablished(bool tf) 
{
    
    homologyEstablished = tf;
    
}


/**
 * Change the name of a taxon
 *
 * \param[in] currentName    self explanatory.
 * \param[in] newName        self explanatory.
 */
void ContinuousCharacterData::setTaxonName(const std::string& currentName, const std::string& newName)
{
    ContinuousTaxonData t = getTaxonData( currentName );
    t.setTaxonName(newName);
    
    size_t numTax = sequenceNames.size();
    for (size_t i = 0; i < numTax ; ++i)
    {
        if ( sequenceNames[i] == currentName) {
            sequenceNames[i] = newName;
            break;
        }
    }
    taxonMap.erase( currentName );
    taxonMap.insert( std::pair<std::string, ContinuousTaxonData >( newName, t ) );
    
}





/**
 * Print the content of the data matrix.
 */
void ContinuousCharacterData::show(std::ostream &out)
{
    
    size_t nt = this->getNumberOfTaxa();
    for (size_t i=0; i<nt; i++)
    {
        
        const ContinuousTaxonData& taxonData = this->getTaxonData(i);
        std::string taxonName = this->getTaxonNameWithIndex(i);
        size_t nc = taxonData.getNumberOfCharacters();
        std::cout << "   " << taxonName << std::endl;
        std::cout << "   ";
        for (size_t j=0; j<nc; j++)
        {
            
            
            std::cout << taxonData[j] << " ";
            if ( (j+1) % 100 == 0 && (j+1) != nc )
            {
                std::cout << std::endl << "   ";
            }
            
        }
        
        std::cout << std::endl;
    }
    
}



