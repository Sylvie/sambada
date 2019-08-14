cmake_minimum_required(VERSION 3.10)
project(sambada)

set(CMAKE_CXX_STANDARD 11)

add_definitions(
        -Dsambada_executable_extension="${CMAKE_EXECUTABLE_SUFFIX}"
        -Dsambada_top_source_dir="${PROJECT_SOURCE_DIR}"
)

if (WIN32)
    add_definitions(
            -Dsambada_folder_separator="\\\\"
    )
else ()
    add_definitions(
            -Dsambada_folder_separator="/"
    )
endif (WIN32)

include_directories(ext/scythestat-1.0.3)
include_directories(ext/scythestat-1.0.3/scythestat)
include_directories(ext/scythestat-1.0.3/scythestat/rng)
include_directories(ext/shapelib-1.3.0)
include_directories(src)
include_directories(src/common)
include_directories(src/histograms)
include_directories(src/modeles)
include_directories(src/variables)
include_directories(test)
include_directories(test/utils)
include_directories(test/integration/utils)
include_directories(test/unit)

add_library(shp
        ext/shapelib-1.3.0/dbfopen.c
        ext/shapelib-1.3.0/safileio.c
        ext/shapelib-1.3.0/shapefil.h
        ext/shapelib-1.3.0/shpopen.c
        ext/shapelib-1.3.0/shptree.c
        )

add_library(intermediate
        src/RegressionLogistique-bienvenue.cpp
        src/RegressionLogistique.hpp
        src/Toolbox.cpp
        src/Toolbox.hpp
        src/Erreur.cpp
        src/Erreur.hpp
        )

set_target_properties(shp
        intermediate
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/src/"
        )

add_library(variables
        src/common/TypesCommuns.hpp
        src/variables/CombinaisonVariables.hpp
        src/variables/CombinaisonVariables.cpp
        src/variables/FamilleVariablesFactory.hpp
        src/variables/FamilleVariablesFactory.cpp
        src/variables/EtiquetteCombinaisonVariables.cpp
        src/variables/EtiquetteCombinaisonVariables.hpp
        )

set_target_properties(variables
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/src/variables/"
        )

add_library(histograms
        src/histograms/Histogram.hpp
        src/histograms/Histogram.cpp
        src/histograms/GroupHistograms.cpp
        src/histograms/GroupHistograms.hpp
        src/histograms/StoreyHistograms.cpp
        src/histograms/StoreyHistograms.hpp
        src/histograms/StoreyHistogramsStreamWriter.cpp
        src/histograms/StoreyHistogramsStreamWriter.hpp
        )

set_target_properties(histograms
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/src/histograms/"
        )

add_library(common
        src/common/TypesCommuns.hpp
        src/common/Matrices.hpp
        src/common/Probability.hpp
        src/common/Probability.cpp
        )

set_target_properties(common
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/src/common/"
        )

add_library(modeles
        src/modeles/Modele.hpp
        src/modeles/Modele.cpp
        src/modeles/scriptorium/Archiviste.hpp
        src/modeles/scriptorium/Archiviste.cpp
        src/modeles/scriptorium/scribe/FlotSortieFactory.hpp
        src/modeles/scriptorium/scribe/FlotSortieFichierFactory.cpp
        src/modeles/scriptorium/scribe/FlotSortieFichierFactory.hpp
        src/modeles/scriptorium/scribe/FlotSortieChaineFactory.cpp
        src/modeles/scriptorium/scribe/FlotSortieChaineFactory.hpp
        src/modeles/scriptorium/scribe/Scribe.hpp
        src/modeles/scriptorium/scribe/Scribe.cpp
        )

set_target_properties(modeles
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/src/modeles/"
        )

add_executable(sambada
        src/Archiviste.cpp
        src/Archiviste.hpp
        src/Chronometre.cpp
        src/Chronometre.hpp
        src/Duree.cpp
        src/Duree.hpp
        src/FluxSortie.cpp
        src/FluxSortie.hpp
        src/Journal.cpp
        src/Journal.hpp
        src/JournalTemporaire.cpp
        src/JournalTemporaire.hpp
        src/mainSambada.cpp
        src/RegressionLogistique-calc.cpp
        src/RegressionLogistique-as.cpp
        src/RegressionLogistique-io.cpp
        src/Supervision.cpp
        src/Supervision.hpp
        src/LecteurCheminAcces.cpp
        src/LecteurCheminAcces.hpp
        src/common/Matrices.hpp
        src/common/TypesCommuns.hpp
        )

target_link_libraries(sambada
        PRIVATE
        shp
        intermediate
        variables
        histograms
        common
        modeles
        )

add_executable(supervision
        src/Archiviste.cpp
        src/Archiviste.hpp
        src/mainSupervision.cpp
        src/RegressionLogistique.hpp
        src/Supervision.cpp
        src/Supervision.hpp
        src/Toolbox.cpp
        src/Toolbox.hpp
        )

target_link_libraries(supervision
        PRIVATE
        intermediate
        )

add_executable(recode-plink
        src/Erreur.hpp
        src/recodePlink.cpp
        src/RegressionLogistique.hpp
        src/Toolbox.hpp
        )

target_link_libraries(recode-plink
        PRIVATE
        intermediate
        )

add_executable(recode-plink-lfmm
        src/Erreur.hpp
        src/recodeLFMM.cpp
        src/RegressionLogistique.hpp
        src/Toolbox.hpp
        )

target_link_libraries(recode-plink-lfmm
        PRIVATE
        intermediate
        )

set_target_properties(sambada
        supervision
        recode-plink
        recode-plink-lfmm
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/binaries/"
        )

# Tests

set(CatchSources
        test/utils/catch.hpp
        test/utils/main-catch.cpp
        )

set(TestUtils
        ${CatchSources}
        test/utils/SambadaInputData.hpp
        test/utils/SambadaInputData.cpp
        test/utils/SambadaRegressionResults.hpp
        test/utils/SambadaRegressionResults.cpp
        test/utils/SambadaSpatialAutocorrelationResults.hpp
        test/utils/SambadaSpatialAutocorrelationResults.cpp
        test/utils/SambadaStoreyHistogram.hpp
        test/utils/SambadaStoreyHistogram.cpp
        test/utils/SambadaStoreyHistogramMapper.hpp
        test/utils/SambadaStoreyHistogramMapper.cpp
        )

set(IntegrationTestUtils
        test/integration/utils/SambadaIntegrationTestUtils.hpp
        test/integration/utils/SambadaIntegrationTestUtils.cpp
        )

if (WIN32)
    set(IntegrationTestUtils
            ${IntegrationTestUtils}
            test/integration/utils/SambadaIntegrationTestUtilsWindows.cpp
            )
else ()
    set(IntegrationTestUtils
            ${IntegrationTestUtils}
            test/integration/utils/SambadaIntegrationTestUtilsUnix.cpp
            )
endif (WIN32)

add_executable(SambadaIntegrationTests
        test/integration/sambada/basicPopulationStructureIntTests/populationStructureWithSavetypeBestIntTests.cpp
        test/integration/sambada/basicPopulationStructureIntTests/trivariateModels/populationStructureAsNormalVariablesIntTests.cpp
        test/integration/sambada/basicPopulationStructureIntTests/trivariateModels/populationStructureAsFirstVariablesIntTests.cpp
        test/integration/sambada/basicPopulationStructureIntTests/trivariateModels/populationStructureAsLastVariablesIntTests.cpp
        test/integration/sambada/basicPopulationStructureIntTests/trivariateModelsReal/populationStructureAsNormalVariablesIntTests.cpp
        test/integration/sambada/basicPopulationStructureIntTests/trivariateModelsReal/populationStructureAsFirstVariablesIntTests.cpp
        test/integration/sambada/basicPopulationStructureIntTests/trivariateModelsReal/populationStructureAsLastVariablesIntTests.cpp
        test/integration/sambada/basicPopulationStructureIntTests/bivariateModels/populationStructureAsFirstVariablesBivariateModelsIntTests.cpp
        test/integration/sambada/basicPopulationStructureIntTests/bivariateModels/populationStructureAsLastVariablesBivariateModelsIntTests.cpp
        test/integration/sambada/basicPopulationStructureIntTests/quadrivariateModels/populationStructureAsFirstVariablesQuadrivariateModelsIntTests.cpp
        test/integration/sambada/basicPopulationStructureIntTests/quadrivariateModels/populationStructureAsLastVariablesQuadrivariateModelsIntTests.cpp
        test/integration/sambada/basicSpatialAutocorrelationIntTests/basicSpatialAutocorrelationIntTests.cpp
        test/integration/sambada/basicSpatialAutocorrelationIntTests/basicSpatialAutocorrelationMissingValuesCartesianBisquareIntTests.cpp
        test/integration/sambada/basicSpatialAutocorrelationIntTests/basicSpatialAutocorrelationMissingValuesCartesianNearestIntTests.cpp
        test/integration/sambada/flotFactoriesIntTests/flotSortieFichierFactoryIntTests.cpp
        test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/GWRCartesianBisquareIntTests.cpp
        test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/GWRCartesianDistanceIntTests.cpp
        test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/GWRCartesianGaussianIntTests.cpp
        test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/GWRCartesianNearestIntTests.cpp
        test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/GWRMissingValuesCartesianGaussianIntTests.cpp
        test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/GWRMissingValuesCartesianNearestIntTests.cpp
        test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/GWRSphericalBisquareIntTests.cpp
        test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/GWRSphericalDistanceIntTests.cpp
        test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/GWRSphericalGaussianIntTests.cpp
        test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/GWRSphericalNearestIntTests.cpp
        test/integration/sambada/modelSelectionSavetypeBestIntTests/modelSelectionSavetypeAllIntTests.cpp
        test/integration/sambada/modelSelectionSavetypeBestIntTests/modelSelectionSavetypeBestIntTests.cpp
        test/integration/sambada/noExtensionsToFilenamesIntTests/noExtensionsToFilenamesIntTests.cpp
        test/integration/sambada/noExtensionsToFilenamesIntTests/noExtensionsToFilenamesUniqueFileIntTests.cpp
        test/integration/sambada/noNewLinesEndOfFilesIntTests/noNewLinesEndOfFilesIntTests.cpp
        test/integration/sambada/regressionResultsIntTests/regressionResultsDim1IntTests.cpp
        test/integration/sambada/regressionResultsIntTests/regressionResultsDim4IntTests.cpp
        test/integration/sambada/resultFilesLocationIntTests/resultsInDifferentFolderAsDataFilesIntTest.cpp
        test/integration/sambada/resultFilesLocationIntTests/resultsInDifferentFolderAsUniqueDataFileIntTest.cpp
        test/integration/sambada/resultFilesLocationIntTests/resultsInSameFolderAsDataFilesIntTest.cpp
        test/integration/sambada/resultFilesLocationIntTests/resultsInSameFolderAsUniqueDataFileIntTest.cpp
        test/integration/sambada/resultFilesLocationIntTests/resultsInSameFolderAsDataFilesWithoutExtensionsIntTest.cpp
        test/integration/sambada/resultFilesLocationIntTests/resultsInSameFolderAsUniqueDataFileWithoutExtensionIntTest.cpp
        test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationCartesianBisquareIntTests.cpp
        test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationCartesianDistanceIntTests.cpp
        test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationCartesianGaussianIntTests.cpp
        test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationCartesianNearestIntTests.cpp
        test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationSphericalBisquareIntTests.cpp
        test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationSphericalDistanceIntTests.cpp
        test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationSphericalGaussianIntTests.cpp
        test/integration/sambada/spatialAutocorrelationResultsIntTests/spatialAutocorrelationSphericalNearestIntTests.cpp
        test/integration/sambada/splitInputFilesIntTests/splitInputFilesWithFilenamesExtensionsIntTests.cpp
        test/integration/sambada/splitInputFilesIntTests/splitInputFilesWithoutFilenamesExtensionsIntTests.cpp
        test/integration/sambada/storeyHistogramsIntTests/univariateModels/storeyHistogramsDim1IntTests.cpp
        test/integration/sambada/storeyHistogramsIntTests/bivariateModels/storeyHistogramsWithPopStructAsFirstVarDim2IntTests.cpp
        test/integration/sambada/storeyHistogramsIntTests/bivariateModels/storeyHistogramsWithPopuStructAsLastVarDim2IntTests.cpp
        test/integration/sambada/storeyHistogramsIntTests/trivariateModels/storeyHistogramsDim3IntTests.cpp
        test/integration/sambada/storeyHistogramsIntTests/trivariateModels/storeyHistogramsWithPopStructAsFirstVarsDim3IntTests.cpp
        test/integration/sambada/storeyHistogramsIntTests/trivariateModels/storeyHistogramsWithPopStructAsLastVarsDim3IntTests.cpp
        test/integration/sambada/storeyHistogramsIntTests/trivariateModelsReal/storeyHistogramsSavetypeRealDim3IntTests.cpp
        test/integration/sambada/storeyHistogramsIntTests/quadrivariateModels/storeyHistogramsWithPopStructAsFirstVarsDim4IntTests.cpp
        test/integration/sambada/storeyHistogramsIntTests/quadrivariateModels/storeyHistogramsWithPopStructAsLastVarsDim4IntTests.cpp
        test/integration/sambada/storeyHistogramsScoreThresholdIntTests/univariateModels/storeyHistogramsWithScoreThresholdDim1IntTests.cpp
        test/integration/sambada/storeyHistogramsScoreThresholdIntTests/bivariateModels/storeyHistogramsScoreThresholdPopStructAsFirstVarDim2IntTests.cpp
        test/integration/sambada/storeyHistogramsScoreThresholdIntTests/bivariateModels/storeyHistogramsScoreThresholdPopStructAsLastVarDim2IntTests.cpp
        test/integration/sambada/storeyHistogramsScoreThresholdIntTests/bivariateModels/storeyHistogramsPopStructAsFirstVarDim2IntTests.cpp
        test/integration/sambada/storeyHistogramsScoreThresholdIntTests/bivariateModels/storeyHistogramsPopStructAsLastVarDim2IntTests.cpp
        test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/storeyHistogramsScoreThresholdDim3IntTests.cpp
        test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/storeyHistogramsScoreThresholdPopStructAsFirstVarsDim3IntTests.cpp
        test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/storeyHistogramsScoreThresholdPopStructAsLastVarsDim3IntTests.cpp
        test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/storeyHistogramsScoreThresholdDim3IntTests.cpp
        test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/storeyHistogramsScoreThresholdPopStructAsFirstVarsDim3IntTests.cpp
        test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/storeyHistogramsScoreThresholdPopStructAsLastVarsDim3IntTests.cpp
        test/integration/sambada/storeyHistogramsScoreThresholdIntTests/quadrivariateModels/storeyHistScoreThresholdPopStructAsFirstVarsDim4IntTests.cpp
        test/integration/sambada/storeyHistogramsScoreThresholdIntTests/quadrivariateModels/storeyHistScoreThresholdPopStructAsLastVarsDim4IntTests.cpp
        test/integration/sambada/storeyHistogramsIntTests/storeyHistogramsWithSavetypeBestIntTests.cpp
        test/integration/sambada/wrongNumberOfIndividualsIntTests/wrongNumberOfIndividualsInEnvironmentalDataIntTests.cpp
        test/integration/sambada/wrongNumberOfIndividualsIntTests/wrongNumberOfIndividualsInMolecularDataIntTests.cpp
        ${IntegrationTestUtils}
        ${TestUtils}
        )

set_target_properties(SambadaIntegrationTests
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test/integration/sambada/"
        )

add_dependencies(SambadaIntegrationTests sambada)

target_link_libraries(SambadaIntegrationTests -lstdc++fs
        modeles)

file(MAKE_DIRECTORY
        "${CMAKE_BINARY_DIR}/test/integration/sambada/basicPopulationStructureIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/basicPopulationStructureIntTests/bivariateModels/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/basicPopulationStructureIntTests/trivariateModels/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/basicPopulationStructureIntTests/trivariateModelsReal/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/basicPopulationStructureIntTests/quadrivariateModels/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/basicSpatialAutocorrelationIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/flotFactoriesIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/geographicallyWeightedRegressionResultsIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/modelSelectionSavetypeBestIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/noExtensionsToFilenamesIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/noNewLinesEndOfFilesIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/regressionResultsIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/resultFilesLocationIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/spatialAutocorrelationResultsIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/splitInputFilesIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/storeyHistogramsIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/storeyHistogramsIntTests/univariateModels/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/storeyHistogramsIntTests/bivariateModels/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/storeyHistogramsIntTests/trivariateModels/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/storeyHistogramsIntTests/trivariateModelsReal/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/storeyHistogramsIntTests/quadrivariateModels/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/univariateModels/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/bivariateModels/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModels/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/trivariateModelsEnd/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/storeyHistogramsScoreThresholdIntTests/quadrivariateModels/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/wrongNumberOfIndividualsIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/supervision/mergeResultFilesIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/supervision/mergeResultFilesWithoutExtensionsIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/supervision/mergeResultFilesWithCustomDelimitorIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/supervision/mergeResultFilesWithGScoreSortIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/supervision/splitMarkerFileIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/supervision/splitMarkerFileWithoutExtensionIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/recodePlink/recodeForSambadaIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/recodePlink/recodeForLFMMIntTests/"
        )

add_executable(SambadaUnitTests
        test/unit/sambada/CheminAccesTests.cpp
        test/unit/sambada/LecteurCheminAccesTests.cpp
        src/LecteurCheminAcces.hpp
        src/LecteurCheminAcces.cpp
        ${TestUtils}
        test/unit/sambada/common/ProbabilityTests.cpp
        test/unit/sambada/modeles/ModeleTests.cpp
        test/unit/sambada/modeles/scriptorium/FlotSortieChaineFactoryTests.cpp
        test/unit/sambada/variables/FamilleVariablesFactoryTests.cpp
        test/unit/sambada/histograms/HistogramTests.cpp
        test/unit/sambada/histograms/GroupHistogramsTests.cpp
        test/unit/sambada/histograms/StoreyHistogramsTests.cpp
        test/unit/sambada/histograms/StoreyHistogramsStreamWriterTests.cpp
        test/unit/sambada/histograms/StoreyHistogramsStreamWriterTests.cpp
        test/unit/sambada/modeles/scriptorium/ScribeTests.cpp
        )

target_link_libraries(SambadaUnitTests
        PRIVATE
        common
        variables
        histograms
        modeles
        )

add_executable(SupervisionIntegrationTests
        test/integration/supervision/mergeResultFilesIntTests/mergeResultFilesIntTests.cpp
        test/integration/supervision/mergeResultFilesWithoutExtensionsIntTests/mergeResultFilesWithoutExtensionsIntTests.cpp
        test/integration/supervision/mergeResultFilesWithCustomDelimitorIntTests/mergeResultFilesWithCustomDelimitorIntTests.cpp
        test/integration/supervision/mergeResultFilesWithGScoreSortIntTests/mergeResultFilesWithGScoreSortIntTests.cpp
        test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/mergeResultFilesWithGScoreThresholdIntTests.cpp
        test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/mergeResultFilesWithWaldScoreThresholdIntTests.cpp
        test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/mergeResultFilesWithBothScoreThresholdIntTests.cpp
        test/integration/supervision/splitMarkerFileIntTests/splitMarkerFileIntTests.cpp
        test/integration/supervision/splitMarkerFileWithoutExtensionIntTests/splitMarkerFileWithoutExtensionIntTests.cpp
        ${IntegrationTestUtils}
        ${TestUtils}
        )

set_target_properties(SupervisionIntegrationTests
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test/integration/supervision/"
        )

add_dependencies(SupervisionIntegrationTests sambada)

target_link_libraries(SupervisionIntegrationTests -lstdc++fs)

add_executable(RecodePlinkIntegrationTests
        test/integration/recodePlink/recodeForSambadaIntTests/recodeAllSamplesIntTests.cpp
        test/integration/recodePlink/recodeForSambadaIntTests/recodeSubsetSamplesIntTests.cpp
        test/integration/recodePlink/recodeForLFMMIntTests/recodeAllSamplesLFMMIntTests.cpp
        test/integration/recodePlink/recodeForLFMMIntTests/recodeSubsetSamplesLFMMIntTests.cpp
        ${IntegrationTestUtils}
        ${TestUtils}
        )

set_target_properties(RecodePlinkIntegrationTests
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test/integration/recodePlink/"
        )

add_dependencies(RecodePlinkIntegrationTests recode-plink recode-plink-lfmm)

target_link_libraries(RecodePlinkIntegrationTests -lstdc++fs)
