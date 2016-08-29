#ifndef ConstantRateBirthDeathProcessTopology_H
#define ConstantRateBirthDeathProcessTopology_H

#include "BirthDeathProcess.h"

#include <vector>

namespace RevBayesCore {
	
	class Clade;
	
	class ConstantRateBirthDeathProcessTopology : public BirthDeathProcess {
		
	public:
		ConstantRateBirthDeathProcessTopology(const TypedDagNode<double> *ra, const TypedDagNode<double> *s, const TypedDagNode<double> *e,
									  const TypedDagNode<double> *r, const TypedDagNode<double> *mp, const std::string& ss, const std::vector<Clade> &ic, const std::string &cdt,
									  const std::vector<Taxon> &tn);
		
		// public member functions
		ConstantRateBirthDeathProcessTopology*                      clone(void) const;                                                                                  //!< Create an independent clone
		double computeLnProbability( void );
		
	protected:
		// Parameter management functions
		void                                                swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
		
	private:
		
		
		typedef struct TREE_RANKING {
			double rank;
			int leaf;
		} TypeTreeRanking;
		

		
		// helper functions
		double                                              lnSpeciationRate(double t) const;
		double                                              rateIntegral(double t_low, double t_high) const;
		double                                              pSurvival(double start, double end) const;
		double                                              simulateDivergenceTime(double origin, double present, double rho) const;                                                  //!< Simulate n speciation events.
        TypeTreeRanking logNumberRankingsRec(TopologyNode& n ) ;
		
		double logNumberRankings(Tree *tree);
		
		double logProbTreeShape( );
		
		double logProbSubTreeShape(TopologyNode& n);

		
		
		
		// members
		const TypedDagNode<double>*                         speciation;
		const TypedDagNode<double>*                         extinction;
		
	};
	
}

#endif
