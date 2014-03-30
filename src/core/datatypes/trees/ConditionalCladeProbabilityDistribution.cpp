
#include <set>
#include "ConditionalCladeProbabilityDistribution.h"


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

//
//void ConditionalCladeProbabilityDistribution::construct(Tree& tree)
//{
//  //  t=new boost::timer();
//
//  last_leafset_id = 0;
//  numberOfObservedTrees = 0;
//  // ? name_separator="+";
//  vector <string > leaves = tree.getTipNames();//del-loc
//
//  int id=0;
//  for (vector <string >::iterator it=leaves.begin();it!=leaves.end();it++ )
//    {      
//
//      id++;
//      string leaf_name=(*it);
//      leaf_ids[leaf_name]=id;
//      Gamma_s.insert(id);
//      id_leaves[id]=leaf_name;
//    }
//  alpha=0;
//  beta=0;
//  Gamma_size=Gamma_s.size();
//  /*  size_t lword  = BipartitionTools::LWORD;
//    nbint = (Gamma_size + lword - 1) / lword;*/
//  //  size_t nbword = (Gamma_size + lword - 1) / lword;
//  //  nbint  = nbword * lword / (CHAR_BIT * sizeof(int));
//    
//    Gamma = boost::dynamic_bitset<> (Gamma_size + 1) ;//new int[nbint];
//    //All leaves are present in Gamma:
//    for (auto i = 0; i < Gamma_size + 1; i++)
//    {
//        Gamma[i] = 1;
//    }
//
//  //maybe should use boost pow
//  //number of bipartitions of Gamma
//  K_Gamma=pow(2.,(int)Gamma_size-1)-1;
//  //number of unrooted trees on Gamma_size leaves
//  if (Gamma_size==2)
//    N_Gamma=1;
//  else
//    N_Gamma=boost::math::double_factorial<scalar_type>(2*Gamma_size-5);
//
//  //del-locs
//  leaves.clear();
//  delete tree;
//}
//
//
//



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


