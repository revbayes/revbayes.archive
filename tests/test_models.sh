# example using normal distributed data
rb RevBayes_scripts/normal.Rev

# example MCMC on unrooted tree and GTR subsitution model
rb RevBayes_scripts/GTR.Rev

# likelihood of a GTR+Gamma substitution model
rb RevBayes_scripts/GTR_Gamma_Likelihood.Rev

# a partitioned analysis
rb RevBayes_scripts/partition.Rev

# independent gamma rate (IGR) relaxed clock model
rb RevBayes_scripts/IGR.Rev

# DPP clock model
rb RevBayes_scripts/DPP_clock.Rev

# random local clock model
rb RevBayes_scripts/RLC.Rev


rm *.log
rm *.out
rm *.trees
rm history.txt

rb RevBayes_scripts/test_biogeography_DEC.Rev
