#ifndef PhyloCTMCSiteHomogeneousDollo_H
#define PhyloCTMCSiteHomogeneousDollo_H

#include "PhyloCTMCSiteHomogeneousRestriction.h"

namespace RevBayesCore {

    class PhyloCTMCSiteHomogeneousDollo : public PhyloCTMCSiteHomogeneousConditional<StandardState> {

        public:
        PhyloCTMCSiteHomogeneousDollo(const TypedDagNode< Tree > *t, size_t nChars, bool c, size_t nSites, bool amb, RestrictionAscertainmentBias::Coding cod = RestrictionAscertainmentBias::NOABSENCESITES, bool normalize = true);

        PhyloCTMCSiteHomogeneousDollo(const PhyloCTMCSiteHomogeneousDollo&);
        // public member functions
        PhyloCTMCSiteHomogeneousDollo*                          clone(void) const;

        virtual void                                            redrawValue(void);
        void                                                    setDeathRate(const TypedDagNode< double > *r);

        protected:

            void                                                computeRootLikelihood(size_t root, size_t l, size_t r);
            void                                                computeRootLikelihood(size_t root, size_t l, size_t r, size_t m);
            void                                                computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r);
            void                                                computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r, size_t m);
            void                                                computeTipLikelihood(const TopologyNode &node, size_t nIdx);

            void                                                computeRootCorrection(size_t root, size_t l, size_t r);
            void                                                computeRootCorrection(size_t root, size_t l, size_t r, size_t m);
            void                                                computeInternalNodeCorrection(const TopologyNode &n, size_t nIdx, size_t l, size_t r);
            void                                                computeInternalNodeCorrection(const TopologyNode &n, size_t nIdx, size_t l, size_t r, size_t m);
            void                                                computeTipCorrection(const TopologyNode &node, size_t nIdx);

            double                                              sumRootLikelihood( void );
            void                                                resizeLikelihoodVectors(void);
            void                                                updateTransitionProbabilities(size_t nodeIdx, double brlen);
            std::vector<double>                                 getRootFrequencies(void) const;
            std::vector<double>                                 getStationaryFrequencies( size_t nodeIndex ) const;

            virtual bool                                        isSitePatternCompatible( std::map<size_t, size_t> );

            virtual void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);

            std::vector<double>                                 integrationFactors;
            std::vector< std::vector<size_t> >                  maskNodeObservationCounts;
            std::vector<double>                                 survival;
            size_t                                              activeMassOffset;
            size_t                                              massNodeOffset;

            bool                                                normalize;
            const TypedDagNode< double >*                       death_rate;

        private:
            double                                              getScaledNodeWeights(const TopologyNode &node, size_t pattern, std::vector<double>& weights);
            double                                              computeIntegratedNodeCorrection(std::vector<double> partials, size_t nodeIndex, size_t mask, size_t mixture);
            void                                                scale(size_t i);
            void                                                scale(size_t i, size_t l, size_t r);
            void                                                scale(size_t i, size_t l, size_t r, size_t m);
            virtual void                                        simulate( const TopologyNode &node, std::vector<StandardState> &taxa, size_t rateIndex, std::map<size_t, size_t>& charCounts);
        };

}

#endif
