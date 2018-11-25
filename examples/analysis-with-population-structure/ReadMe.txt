#!/bin/bash

# This example illustrates how to include variables representing the population structure in the analysis.
# Please refer to chapter "Including prior knowledge in multivariate models" in the manual for background information.

# The dataset contains 10 SNPs, 5 environmental variables and 2 population variables.

# The molecular data is found in "cattle-pop-mark.txt".
# The environmental data is found in
# - "cattle-pop-env-first.csv" with the population variables on the left and the environmental variables on the right;
# - "cattle-pop-env-last.csv" with the environmental variables on the left and the population variables on the right.
# The analysis is configured in
# - "param-pop-first.txt" for the case with the population variables on the left;
# - "param-pop-last.txt" for the case with the population variables on the right.

# Running the analysis
../../binaries/sambada param-pop-first.txt cattle-pop-env-first.csv cattle-pop-mark.txt
# or
../../binaries/sambada param-pop-last.txt cattle-pop-env-last.csv cattle-pop-mark.txt

# Filtering the results
head -1 cattle-pop-mark-first-Out-3.txt > filtered-results.txt
grep "pop1 pop2" cattle-pop-mark-first-Out-3.txt >> filtered-results.txt
# or
head -1 cattle-pop-mark-last-Out-3.txt > filtered-results.txt
grep "pop1 pop2" cattle-pop-mark-last-Out-3.txt >> filtered-results.txt

# Selecting the models in R
# Please see the script "bonferroniModelSelection.R" for details

# Reading results and filtering valid columns
results=read.table("filtered-results.txt", header=T, sep =" ")
results=results[,1:21]
numEnv=5
numMark=30

# Selecting the threshold of p-values
alpha=0.01

# Applying the Bonferroni correction
pVal.threshold=alpha/(numEnv*numMark)

# Computing the p-values
results=cbind(results, pvalG=pchisq(results$GscorePop, 1, lower.tail=F), pvalWald=pchisq(results$WaldScorePop, 1, lower.tail=F))

# Selecting significant models for the G score
results[results$pvalG<pVal.threshold,]

# Selecting significant models for the Wald score
results[results$pvalWald<pVal.threshold,]
