
#include <set>
#include <fstream>
#include "ConditionalCladeProbabilityDistribution.h"
#include "NewickConverter.h"
#include "BranchLengthTree.h"

#include <boost/algorithm/string.hpp>
#include <boost/math/special_functions/binomial.hpp>
#include <boost/math/special_functions/factorials.hpp>


using namespace RevBayesCore;



ConditionalCladeProbabilityDistribution::ConditionalCladeProbabilityDistribution()
{
  //formal constructor must be followed by load state
  ;
}


ConditionalCladeProbabilityDistribution::ConditionalCladeProbabilityDistribution(Tree& tree)
{
  constructorTree = tree.clone();
  construct(*constructorTree);
}


void ConditionalCladeProbabilityDistribution::construct(Tree& tree)
{
  //  t=new boost::timer();

  last_leafset_id = 0;
  numberOfObservedTrees = 0;
  // ? name_separator="+";
  std::vector < std::string > leaves = tree.getTipNames();//del-loc

  int id=0;
  for (std::vector <std::string >::iterator it=leaves.begin();it!=leaves.end();it++ )
    {      
      id++;
      std::string leaf_name=(*it);
      //Taxon tax (leaf_name, "");
      leaf_ids[leaf_name]=id;
      setOfAllLeafIds.insert(id);
      id_leaves[id]=leaf_name;
    }
  alpha=0;
  beta=0;
  numTaxons=taxons.size();
  /*  size_t lword  = BipartitionTools::LWORD;
    nbint = (numTaxons + lword - 1) / lword;*/
  //  size_t nbword = (numTaxons + lword - 1) / lword;
  //  nbint  = nbword * lword / (CHAR_BIT * sizeof(int));
    
    completeBitVector = boost::dynamic_bitset<> (numTaxons + 1) ;//new int[nbint];
    //All leaves are present in completeBitVector:
    for (size_t i = 0; i < numTaxons + 1; i++)
    {
        completeBitVector[i] = 1;
    }

  //maybe should use boost pow
  //number of bipartitions of completeBitVector
  K_Gamma=pow(2.,(int)numTaxons-1)-1;
  //number of unrooted trees on numTaxons leaves
  if (numTaxons==2)
    numberOfPossibleTreeTopologies=1;
  else
    numberOfPossibleTreeTopologies=boost::math::double_factorial<double>(2*numTaxons-5);

  //del-locs
  leaves.clear();
}

const std::string& ConditionalCladeProbabilityDistribution::getAleRepresentation( ) const                                            //Writes the object to a stream.

{
    // create the newick string
    std::stringstream ou;

  
  //must be first!
  ou<< "#constructor_string" << std::endl;
  std::string constructor_string = constructorTree->getNewickRepresentation();
  boost::trim(constructor_string);
  ou<< constructor_string << std::endl;

  ou<< "#observations" <<std::endl;
  ou<< numberOfObservedTrees << std::endl;

  ou<< "#Bip_counts" << std::endl;
  for (std::map <size_t, size_t>::const_iterator it=BipartitionCounts.begin();it!=BipartitionCounts.end();it++)
    ou<<(*it).first<<"\t"<<(*it).second<< std::endl;

    ou << "#Bip_bls" << std::endl;
    for (std::map <size_t, std::vector<double> >::const_iterator it = BipartitionBranchLengths.begin(); it != BipartitionBranchLengths.end(); ++it) {
        ou << it->first;
        for (size_t j = 0; j < it->second.size(); ++j) {
            ou << "\t" << it->second[j];
        }
        ou << std::endl;
    }
    
  ou<< "#Dip_counts" << std::endl;

  size_t index=0;
  for ( std::vector < boost::unordered_map< std::pair<size_t, size_t>, double> >::const_iterator it=tripletFrequencies.begin();it!=tripletFrequencies.end();it++ )
    {
      for (boost::unordered_map< std::pair<size_t, size_t>, double>::const_iterator jt=(*it).begin();jt!=(*it).end();jt++)
	{
	  ou<< index <<"\t";
	  ou<<(*jt).first.first<<"\t";
	  ou<<(*jt).first.second<<"\t";
	  ou<<(*jt).second<<std::endl;
	}
      ++index;
    }
  
  ou<< "#last_leafset_id" <<std::endl;
  ou<< last_leafset_id << std::endl;

  ou<< "#leaf-id" <<std::endl;
  for ( std::map< std::string,int >::const_iterator it=leaf_ids.begin();it!=leaf_ids.end();it++)
    ou<<(*it).first<<"\t"<<(*it).second<< std::endl;

  ou<< "#set-id" <<std::endl;
  for ( std::map < boost::dynamic_bitset<>, size_t>::const_iterator it=set_ids.begin();it!=set_ids.end();it++)
    {
      ou << (*it).second;
      ou << "\t:";
        for (size_t i = 0; i<  numTaxons + 1; ++i) {
            //if (BipartitionTools::testBit((*it).first, i))
               if ( (*it).first[i] ) //if the leaf is present, we print it
                   ou << "\t" << i;
        }
        
   /*   for (set< int>::iterator  jt=(*it).first.begin();jt!=(*it).first.end();jt++)
	ou << "\t" << (*jt);*/
      ou << std::endl;
    }

  ou<< "#END" << std::endl;

  return ou.str();
  
}


void ConditionalCladeProbabilityDistribution::readFromFile(std::string fname)
{
  std::string tree_string;
  std::ifstream file_stream;
  file_stream.open(fname.c_str());
  std::string reading="#nothing";
  if (file_stream.is_open())  //  ########## read state ############
  {
      while (! file_stream.eof())
      {
          std::string line;
          getline (file_stream,line);
          
          if (boost::find_first(line, "#"))
          {
              boost::trim(line);
              reading=line;
          }
          else if (reading=="#constructor_string")
          {
              //cout << reading << std::endl;
              boost::trim(line);
              NewickConverter c;
              BranchLengthTree *blTree = c.convertFromNewick(line);
              constructorTree = new BranchLengthTree( *blTree );
              construct(*constructorTree);
              reading="#nothing";
          }
          else if (reading=="#observations")
          {
              boost::trim(line);
              numberOfObservedTrees=atof(line.c_str());
          }
          else if (reading=="#Bip_counts")
          {
              //cout << reading << std::endl;
              std::vector<std::string> tokens;
              boost::trim(line);
              boost::split(tokens,line,boost::is_any_of("\t "),boost::token_compress_on);
              BipartitionCounts[atol(tokens[0].c_str())]=atof(tokens[1].c_str());
          }
          else if (reading=="#Bip_bls")
          {
              //cout << reading << std::endl;
              std::vector<std::string> tokens;
              boost::trim(line);
              boost::split(tokens,line,boost::is_any_of("\t "),boost::token_compress_on);
              std::vector< double > branchLengths;
              for (size_t i = 1; i < tokens.size(); ++i) {
                  branchLengths.push_back( atof(tokens[i].c_str()) );
              }
              BipartitionBranchLengths[atol(tokens[0].c_str())] = branchLengths;
          }
          else if (reading=="#Dip_counts")
          {
              //cout << reading << std::endl;
              std::vector<std::string> tokens;
              boost::trim(line);
              boost::split(tokens,line,boost::is_any_of("\t "),boost::token_compress_on);
              std::pair<long int, long int> parts;
              /*PREVECTORIZATION CODE
               set<long int> parts;
               parts.insert(atoi(tokens[1].c_str()));
               parts.insert(atoi(tokens[2].c_str()));
               tripletFrequencies[atol(tokens[0].c_str())][parts]=atof(tokens[3].c_str());
               */
              
              parts.first = atoi(tokens[1].c_str());
              parts.second = atoi(tokens[2].c_str());
              if ( atol(tokens[0].c_str()) >= (long int) ( tripletFrequencies.size() ) )
              {
                  boost::unordered_map< std::pair<size_t, size_t>,double> temp ;
                  while (atol(tokens[0].c_str()) >  (int) tripletFrequencies.size() ) {
                      tripletFrequencies.push_back(temp);
                  }
                  
                  temp[parts]=atof(tokens[3].c_str());
                  tripletFrequencies.push_back(temp);
              }
              else
              {
                  tripletFrequencies[atol(tokens[0].c_str())][parts]=atof(tokens[3].c_str());
              }
          }
          else if (reading=="#last_leafset_id")
          {
              //cout << reading << std::endl;
              boost::trim(line);
              last_leafset_id=atol(line.c_str());
          }
          else if (reading=="#leaf-id")
          {
              //cout << reading << std::endl;
              std::vector<std::string> tokens;
              boost::trim(line);
              boost::split(tokens,line,boost::is_any_of("\t "),boost::token_compress_on);
              int id=atoi(tokens[1].c_str());
              std::string leaf_name=tokens[0];
              leaf_ids[leaf_name]=id;
              id_leaves[id]=leaf_name;
          }
          else if (reading=="#set-id")
          {
              //cout << reading << std::endl;
              std::vector<std::string> fields;
              boost::trim(line);
              boost::split(fields,line,boost::is_any_of(":"),boost::token_compress_on);
              boost::trim(fields[0]);
              long int set_id=atol(fields[0].c_str());
              std::vector<std::string> tokens;
              boost::trim(fields[1]);
              boost::split(tokens,fields[1],boost::is_any_of("\t "),boost::token_compress_on);
              boost::dynamic_bitset<> temp( numTaxons + 1 );
              
              for (std::vector<std::string>::iterator it=tokens.begin();it!=tokens.end();it++) { //Setting the proper bits to 1
                  temp[static_cast<int>(atoi((*it).c_str()))] = 1;
              }

             // std::cout <<"setid : "<< set_id << " READING: " << temp << std::endl;
              set_ids[temp]=set_id;
              id_sets[set_id]=temp;
          }
      }
  }
    //Attempt adding something for the root bipartition:
    boost::dynamic_bitset<> temp (numTaxons +1);
    for (size_t i = 1 ; i<numTaxons +1; ++i) {
        temp[i] = 1;
    }
    id_sets[-1] = temp;
    set_ids[temp] = -1;

  for ( std::map< size_t,  boost::dynamic_bitset<> >::const_iterator  it = id_sets.begin(); it != id_sets.end(); it++ )
    {
        size_t size = 0;
        for (size_t i=0; i< numTaxons + 1; ++i) {
         //   if (BipartitionTools::testBit( (*it).second, static_cast<int>(i) ) ) {
                if ( (*it).second[i] ) {
                size+=1;
            }
        }
        set_sizes[ (*it).first ] = size;
        size_ordered_bips[size].push_back( (*it).first );
        // std::cout << size << " AND "<< (*it).first <<std::endl;
        /*  set_sizes[(*it).first]=(*it).second.size();
      size_ordered_bips[(*it).second.size()].push_back((*it).first);*/
    }
/*    for ( auto it = size_ordered_bips.begin(); it != size_ordered_bips.end(); it++ )
    {
        VectorTools::print ( (*it).second );
    }*/
    
}





std::string ConditionalCladeProbabilityDistribution::set2name(boost::dynamic_bitset<> leaf_set) const
{
    std::string name="";
    for (size_t i = 0; i< numTaxons + 1; ++i) {
       // if ( BipartitionTools::testBit(leaf_set, static_cast<int>(i)) ) {
      if ( leaf_set[i] )  {
	//stringstream tmp;
	//tmp<<i;
	name += id_leaves.at(i) + name_separator;
		//XX
	    }
    }
    //std::cout << name.substr(0,name.size()-1) <<std::endl;
    //return name.substr(0,name.size()-1);
    return name.substr(0,name.size());
}

size_t ConditionalCladeProbabilityDistribution::set2id(boost::dynamic_bitset<> leaf_set)
{
    size_t id=set_ids[leaf_set];
    if (!id)
    {
        last_leafset_id++;
        set_ids[leaf_set]=last_leafset_id;
        // TMP for debug
        //Dip_levels[leaf_set.size()].push_back(last_leafset_id);
        //id2name[last_leafset_id]=set2name(leaf_set);
        //VEC
        boost::unordered_map< std::pair<size_t, size_t>, double > tmp ;
        tripletFrequencies.push_back(tmp);
        //VEC
        id_sets[last_leafset_id]=leaf_set;
        BipartitionBranchLengths[last_leafset_id]= std::vector<double> (0.0, 0); //default value
       // std::cout << "notid: "<< last_leafset_id <<std::endl;
        return last_leafset_id;
    }
    else
    {
       // std::cout << "id: "<< id <<std::endl;
        return id;
    }
}





















 // Global functions using the class
std::ostream& RevBayesCore::operator<<(std::ostream& o, const ConditionalCladeProbabilityDistribution& x) {
    o << x.getAleRepresentation();
    
    return o;
}


//Utilitary functions


//from: http://rosettacode.org/wiki/Power_set#Recursive_version (accessed 12/13/11)
//"Given a set S, the power set (or powerset) of S, written P(S), or 2S, is the set of all subsets of S."
template<typename Set> std::set<Set> ConditionalCladeProbabilityDistribution::powerset(const Set& s, size_t n)
{
  typedef typename Set::const_iterator SetCIt;
  typedef typename std::set<Set>::const_iterator PowerSetCIt;
  std::set<Set> res;
  if(n > 0) {
    std::set<Set> ps = powerset(s, n-1);
    for(PowerSetCIt ss = ps.begin(); ss != ps.end(); ss++)
      for(SetCIt el = s.begin(); el != s.end(); el++) {
	Set subset(*ss);
	subset.insert(*el);
	res.insert(subset);
      }
    res.insert(ps.begin(), ps.end());
  } else
    res.insert(Set());
  return res;
}
//from: http://rosettacode.org/wiki/Power_set#Recursive_version (accessed 12/13/11)
template<typename Set> std::set<Set> ConditionalCladeProbabilityDistribution::powerset(const Set& s)
{
  return powerset(s, s.size());
}


