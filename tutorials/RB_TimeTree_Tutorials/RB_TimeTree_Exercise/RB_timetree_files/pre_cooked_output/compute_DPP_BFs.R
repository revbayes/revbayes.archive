library(gmp)

get_prior_prob <- function(n, k, c){
	s <- abs(Stirling1(n, k))
	x <- c^k
	dv <- 1
	for(i in 1:n){
		dv <- dv * (c + i - 1)
	}
	nv <- log(s) + log(x)
	ln_probability <- nv - log(dv)
	return(exp(ln_probability))
}


dpp_mcmc <- read.table("output/DPP_bears_mcmc.log",header=TRUE)

dpp_ncats <- dpp_mcmc$n_cats
concentration <- dpp_mcmc$concentration[1]

n_gen <- length(dpp_ncats)
burnin <- floor(n_gen*0.1)

num_branches <- 18

dpp_ncats <- dpp_ncats[burnin:n_gen]

num_K <- table(dpp_ncats)

K1_states <- num_K[names(num_K)==1][[1]]

post_K1 <- K1_states / length(dpp_ncats)

post_KN <- 1 - post_K1

prior_K1 <- get_prior_prob(num_branches, 1, concentration)

prior_KN <- 1 - prior_K1

posterior_odds <- post_K1 / post_KN

prior_odds <- prior_K1 / prior_KN

print(paste("Posterior probability of K==1   =  ", post_K1))

print(paste("Prior probability of K==1       =  ", prior_K1))

print(paste("posterior odds                  =  ", posterior_odds))

print(paste("prior odds                      =  ", prior_odds))

bayes_factor <- posterior_odds / prior_odds

print(paste("Bayes factor in favor of K==1   =  ", bayes_factor))
