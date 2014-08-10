#ifndef CONDITIONALCLADEPROBABILITYDISTRIBUTION_H
#define	CONDITIONALCLADEPROBABILITYDISTRIBUTION_H

#include <map>
#include <vector>
#include <string>


#include <boost/dynamic_bitset.hpp>
#include <boost/unordered_map.hpp>

//#include "Taxon.h"
#include "Tree.h"


namespace RevBayesCore {
    
    /**
     * Object describing a conditional clade probability distribution.
     *
     * A conditional clade is a clade given its parent clade.
     * The conditional clade probability distribution describes the entire set of 
     * conditional clades encountered in a tree distribution.
     * To do so it contains a vector of taxons, found in all trees,
     * a vector of bitvectors giving presence/absence of taxons in clades.
     * In addition we store a vector of clade probabilities as well as a vector
     * of conditional clade probabilities.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Bastien Boussau, from code by Gergely Szollosi)
     * @since 2014-02-26, version 1.0
     */
    class ConditionalCladeProbabilityDistribution  {
        
    public:
        ~ConditionalCladeProbabilityDistribution()
           {
            }

        ConditionalCladeProbabilityDistribution();                                                               //Does nothing. Formal constructor must be followed by load_state.
        ConditionalCladeProbabilityDistribution(Tree& tree);                                               //Constructs a basic instance by calling construct.
        
        double                      getProbability(Tree& tree) const;                                                  //Computes the probability of a string tree. Calls recompose on the tree, and then uses the map returned by recompose to compute the probability of the whole tree.
        std::pair<Tree ,double>     getMapTree() const;                                    //Returns the maximum a posteriori tree that can be amalgamated from the ConditionalCladeProbabilityDistribution object. Uses a double-recursive traversal of all bipartitions.
        std::string                 getAleRepresentation( ) const ;                                           //Writes the object to a stream.

    private:    
        
        // members
        double alpha;
        double beta;

        Tree*                                           constructorTree;
        size_t                                    numberOfObservedTrees;                               //Number of trees observed to build the object.
        std::vector <std::string>                                taxons;                               //The string could be replaced by a Taxon.
        std::vector < boost::dynamic_bitset<> >            biPartitions;
        std::map < Tree*, size_t >                          tree_counts;
        std::set<int>                                   setOfAllLeafIds;                        //Set containing all leaf ids, i.e. ~Clade of all leaves in the tree.
        boost::dynamic_bitset<>                       completeBitVector;                      //bit vector with all bits to 1 (all species present)
        size_t                                                numTaxons;
        size_t                                                  K_Gamma;              //Total number of bipartitions.
        long double                      numberOfPossibleTreeTopologies;              //number of unrooted trees on Gamma_size leaves
        std::string                                      name_separator;                                                        //Character used between leaf names when writing the content of a leaf set.
        // std::map <std::string,std::string> tree_bipstrings;                                //Map between tree string and string containing all bipartitions in the tree.
        // std::map <std::string,std::string> bipstring_trees;                                //Dual from above. Map between string containing all bipartitions in the tree and tree string.
        std::map <size_t, size_t>                             set_sizes;                                                 //del-loc. Map between a bipartition id and the sizes of the corresponding leaf set.
        std::map <size_t, std::vector < size_t > >    size_ordered_bips;                        //Map between bipartition size, and the ids of all bipartitions of this size.

        size_t                                          last_leafset_id;                                                          //Total number of sets of leaves (=bipartitions) observed in the posterior.
        std::map< std::string,int >                            leaf_ids;                                                //Map between taxon and leaf id. Leaf ids go from 1 to Gamma_size. The string could be replaced by a Taxon.
        std::map< int, std::string >                          id_leaves;                                               //Map between leaf id and taxon. Dual from above. The string could be replaced by a Taxon.
        std::map <size_t, size_t>                     BipartitionCounts;                                        //For each bipartition, gives the number of times it was observed.
        std::map < size_t, std::vector<double> >    BipartitionBranchLengths;                                           //vector of the branch lengths associated to the bipartitions.
        
        //VECTORIZED BELOW  std::map <long int, std::map< std::set<long int>,scalar_type> > Dip_counts;        // Contains the frequency of triplets: mother clade and its two daughter clades. Map between the bipartition id of the mother clade and another map containing a set of bipartition ids (couldn't it be just a pair, in the case of bifurcating trees?) and the frequency of the associated triplet of bipartitions. 

        std::vector < boost::unordered_map< std::pair<size_t, size_t>, double> > tripletFrequencies;  //Contains the frequency of triplets: mother clade and its two daughter clades. Maps the bipartition id of the mother clade (position in the vector) to all its resolutions into pairs of clades. Each pair contains two bipartition ids and is mapped to the frequency of the associated triplet of bipartitions. 
        // std::map <std::set <int>,long int>  set_ids;                                       // Map between a set of leaf ids and the corresponding bipartition index.
        //    std::map< long int, std::set <int> > id_sets;                                      // Dual from above. Map between a bipartition index and the corresponding leaf ids.

        std::map < boost::dynamic_bitset<>, size_t>             set_ids;                                   // Map between a bit vector of leaf ids and the corresponding bipartition index.
        std::map< size_t, boost::dynamic_bitset<> >             id_sets;                                      // Dual from above. Vector linking a bipartition index and the corresponding leaf id bit vector.

        
        //Functions
        void construct(Tree& tree);                                          //Constructs a basic instance.

        void readFromFile(std::string fname);                                                          //Reads the object from a stream.

        void construct(std::vector<Tree>& trees,bool count_topologies=false);     //Given a vector of trees, fills an approx_posterior object by recursively calling decompose, and then doing some more counts.
        //  scalar_type nbipp(std::string tree) const;                                              //Computes the proportion of bipartitions already in the approx_posterior object that are present in the tree
        double binomial(int n,int m) const;                                                //Computes the binomial coefficient.
        double trinomial(int n1,int n2,int n3) const;                                      //Computes the multinomial coefficient for 3 elements.

        Tree* mapBacktrack(size_t g_id, std::map<size_t, double > * qmpp) const;//Recursive function that, given a bipartition id and a map associating bipartition ids to their maximum a posteriori value, builds the maximum a posteriori tree, complete with (average) branch lengths.
        Tree* random_tree() const;                                                        //Function that returns a random tree with unit branch lengths. Calls random_split.
        std::vector< Tree* >  all_trees() const {return all_trees(completeBitVector);};                //del-loc. Builds all rooted trees that can be built based on the complete set of leaves.

        //algorithmic
        void addTreeToDistribution(std::string G_string,std::set<int> * bip_ids=NULL );                //Parses a tree in string format and updates the ConditionalCladeProbabilityDistribution object accordingly (notably updates the Bip_bls, Bip_counts, Dip_counts, and set_ids + id_sets through set2id)
        std::map < boost::dynamic_bitset<> , double> recompose(Tree& tree) const;              //For a given input tree, returns a map between all sets of leaves contained in the tree and their corresponding conditional clade probability.
        void register_leafset(std::string);

        //numeric
        double Bi(int n2) const;                                                            //Returns the total number of binary tree topologies possible given a fixed bipartition between n2 leaves on one side and numTaxons-n2 leaves on the other side.
        double Tri(int n2,int n3) const;                                                    //Returns the total number of binary tree topologies possible given a fixed trifurcation between n2 leaves in one clade, n3 in another, and numTaxons-n2-n3 leaves in the last one.
        
        //from: http://rosettacode.org/wiki/Power_set#Recursive_version (accessed 12/13/11)
        //"Given a set S, the power set (or powerset) of S, written P(S), or 2S, is the set of all subsets of S."
        template<typename Set> std::set<Set> powerset(const Set& s, size_t n);
        //from: http://rosettacode.org/wiki/Power_set#Recursive_version (accessed 12/13/11)
        template<typename Set> std::set<Set> powerset(const Set& s);

        double p_dip(long int g_id,long int gp_id,long int gpp_id) const;                   //Probability of a trifurcation given by the ids of the clades.
        //scalar_type p_dip(std::set<int> gamma,std::set<int> gammap,std::set<int> gammapp); //Probability of a trifurcation given by the leaf sets of the clades.
        double p_dip(boost::dynamic_bitset<> gamma, boost::dynamic_bitset<> gammap, boost::dynamic_bitset<> gammapp) const; //Probability of a trifurcation given by the leaf sets of the clades.
    
        double p_bip(long int g_id) const;                                                  //Probability of a bipartition given by its id. Uses the correction term alpha.
        //  scalar_type p_bip(std::set<int> gamma);                                            //Probability of a bipartition given by its leaf set.
        double p_bip(boost::dynamic_bitset<> gamma) const;                                            //Probability of a bipartition given by its leaf set.


        //nuisance
        size_t set2id( boost::dynamic_bitset<>  leaf_set) ;                                           //If the set of leaves exists, returns the set id, otherwise creates a new set id for this set and returns it.
        std::string set2name( boost::dynamic_bitset<> leafSet) const;                                      //Prints the leaf names of leaves contained in a leaf set
        Tree* random_split( boost::dynamic_bitset<> leafSet) const;                                     //Recursive function that returns a random subtree given a leaf set as input and given the approx_posterior object. Can return clades never observed in the posterior sample.
        std::vector< Tree* >  all_trees( boost::dynamic_bitset<> leafSet) const;                        //del-loc. Builds all rooted trees that can be built with leaf set gamma.
        double count_trees() const;                                                         //Counts trees that can be amalgamated with the approx_posterior object with the complete leaf set, without actually building these trees.
        double count_trees(long int g_id) const;                                            //Counts trees that can be amalgamated with the leaf set with id g_id, without actually building these trees.

        double count_all_trees( boost::dynamic_bitset<> leafSet) const;                                 //Counts all trees that can be built with the leaf set gamma, without actually building these trees.
	void setAlpha ( double a ) ;                                                 //Set the value for the alpha parameter used for normalizing counts
	void setBeta ( double b );                                                  //Set the value for the beta parameter used for normalizing counts
	std::vector < std::string > getLeafNames() const;										 //get a vector containing all leaf names in the ale.
	void computeOrderedVectorOfClades (std::vector <size_t>&  ids, std::vector <size_t>& id_sizes); //fills the ids and id_sizes maps, ids of clades ordered by their size.


        
        
        
    
    };
          // Global functions using the class
    std::ostream&                       operator<<(std::ostream& o, const ConditionalCladeProbabilityDistribution& x);                                         //!< Overloaded output operator
  
}


#endif	/* CONDITIONALCLADEPROBABILITYDISTRIBUTION_H */

