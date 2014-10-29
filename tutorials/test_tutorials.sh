# basics commands in RevBayes
cd RB_Basics_Tutorial/RevBayes_Scripts
#rb basics.Rev
#rb airline_fatalities_part1.Rev
#rb airline_fatalities_part2.Rev
#rb coalmine_accidents.Rev

#rm airline_fatalities.log
#rm -r output

cd ../..


# substitution models (JC, HKY, GTR, GTR+Gamma, GTR+Gamma+I) on a non-clock tree
cd RB_CTMC_Tutorial
#rb RevBayes_Scripts/full_analysis.Rev

#rm -r output

cd ..


# MCMC move and proposal distribution tutorial
#cd RB_MCMC_Tutorial
#rb RevBayes_Scripts/full_analysis.Rev

#rm -r output

#cd ..


# marginal likelihood estimation on substitution models
cd RB_BayesFactor_Tutorial
#rb RevBayes_Scripts/full_analysis.Rev

#rm -r output

cd ..


# dating tutorial (time tree estimation using different clock models)
cd RB_TimeTree_Tutorial
rb RevBayes_Scripts/full_analysis.Rev

rm -r output

cd ..


# continuous trait evolution tutorials
cd RB_PhyloComparative_Tutorial
#rb RevBayes_Scripts/full_analysis.Rev

#rm -r output

cd ..






