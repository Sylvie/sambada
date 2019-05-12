
test_integration_SambadaIntegrationTests_SOURCES += \
		test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/GWRCartesianBisquareIntTests.cpp \
		test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/GWRCartesianDistanceIntTests.cpp \
		test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/GWRSphericalBisquareIntTests.cpp \
		test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/GWRSphericalDistanceIntTests.cpp

#		test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/GWRCartesianGaussianIntTests.cpp \
#		test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/GWRCartesianNearestIntTests.cpp \
#		test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/GWRSphericalDistanceIntTests.cpp \
#		test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/GWRSphericalGaussianIntTests.cpp \
#		test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/GWRSphericalNearestIntTests.cpp


EXTRA_DIST += \
		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/cattle-pop-env.csv \
		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/cattle-pop-mark.txt \
		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/param-cartesian-bisquare.txt \
		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/param-cartesian-distance.txt \
		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/param-spherical-bisquare.txt \
		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/param-spherical-distance.txt \
		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-dim-0.txt \
  		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-cartesian-bisquare-dim-1.txt \
   		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-cartesian-bisquare-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-cartesian-distance-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-cartesian-distance-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-spherical-bisquare-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-spherical-bisquare-dim-2.txt \
		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-spherical-distance-dim-1.txt \
		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-spherical-distance-dim-2.txt


#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/random-sample.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/param-cartesian-nearest.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/param-spherical-distance.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/param-spherical-nearest.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/param-random-sample-cartesian-gaussian.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/param-random-sample-spherical-gaussian.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-cartesian-gaussian-env.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-cartesian-gaussian-env-pval.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-cartesian-gaussian-mark.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-cartesian-gaussian-mark-pval.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-cartesian-nearest-env.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-cartesian-nearest-env-pval.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-cartesian-nearest-mark.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-cartesian-nearest-mark-pval.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-spherical-distance-env.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-spherical-distance-env-pval.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-spherical-distance-mark.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-spherical-distance-mark-pval.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-spherical-gaussian-env.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-spherical-gaussian-env-pval.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-spherical-gaussian-mark.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-spherical-gaussian-mark-pval.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-spherical-nearest-env.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-spherical-nearest-env-pval.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-spherical-nearest-mark.txt \
#		$(top_srcdir)/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/expected-results-spherical-nearest-mark-pval.txt