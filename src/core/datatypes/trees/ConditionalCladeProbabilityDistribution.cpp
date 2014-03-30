
#include <set>
#include "ConditionalCladeProbabilityDistribution.h"
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


