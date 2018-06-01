# Read result file for univariate models
## Insert correct name here
res.1=read.table("markers-Out-1.txt", T, " ", stringsAsFactors=F)

# Read the result file for the null models
## Insert correct name here
res.0=read.table("markers-Out-0.txt", T, " ", stringsAsFactors=F)

# Drop the last column in both files since they are null
res.1=res.1[,1:15]
res.0=res.0[,1:5]

# Remove unconverted models (i.e. models with an error)
res.1=res.1[res.1$NumError == 0,]

# Check number of monomorphic markers
table(res.0$NumError)

# Keep the number of polymorphic markers as reference
numMark=table(res.0$NumError)[[1]]

# Number of environmental variables
## Insert correct number here
numEnv=8

# Choice of alpha (could be 0.05)
alpha=0.01

# Score threshold for model significance (same value for G and Wald)
# Models with a score higher than this value are considered as significant.
# When using the option "SIGNIF" or "BEST", Sambada enforce this threshold for both G and Wald scores. One can also use a single test (G or Wald) for assessing significance.
# Note: The number of degree of freedom (df) is 1 since we compare models involving one env var with models without any env var
score.threshold=qchisq(alpha/(numEnv*numMark), 1, lower.tail=F)

# Score threshold for model significance (same value for G and Wald)
# Models with a p-value lower than this value are considered as significant.
# When using the option "SIGNIF" or "BEST", Sambada enforce this threshold for both G and Wald p-values. One can also use a single test (G or Wald) for assessing significance.
pVal.threshold=alpha/(numEnv*numMark)

# Computing p-values:
res.1=cbind(res.1, pvalG=pchisq(res.1$Gscore, 1, lower.tail=F), pvalWald=pchisq(res.1$WaldScore, 1, lower.tail=F))

# Selecting models passing the G test (p-value for G score lower than the threshold)
res.1[res.1$pvalG<pVal.threshold,]
# Check if there are matches here

# If not, check if there are a couple of significant models if setting alpha to 0.05
alpha=0.05
pVal.threshold=alpha/(numEnv*numMark)
res.1[res.1$pvalG<pVal.threshold,]
