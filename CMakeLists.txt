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
        )

target_link_libraries(sambada
        PRIVATE
        shp
        intermediate
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

add_executable(recodePlink
        src/Erreur.hpp
        src/recodePlink.cpp
        src/RegressionLogistique.hpp
        src/Toolbox.hpp
        )

target_link_libraries(recodePlink
        PRIVATE
        intermediate
        )

add_executable(recodePlinkLFMM
        src/Erreur.hpp
        src/recodeLFMM.cpp
        src/RegressionLogistique.hpp
        src/Toolbox.hpp
        )

target_link_libraries(recodePlinkLFMM
        PRIVATE
        intermediate
        )

set_target_properties(sambada
        supervision
        recodePlink
        recodePlinkLFMM
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

target_link_libraries(SambadaIntegrationTests -lstdc++fs)

file(MAKE_DIRECTORY
        "${CMAKE_BINARY_DIR}/test/integration/sambada/basicPopulationStructureIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/basicPopulationStructureIntTests/bivariateModels/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/basicPopulationStructureIntTests/trivariateModels/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/basicPopulationStructureIntTests/trivariateModelsReal/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/basicPopulationStructureIntTests/quadrivariateModels/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/basicSpatialAutocorrelationIntTests/"
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
        "${CMAKE_BINARY_DIR}/test/integration/supervision/mergeResultFilesWithGScoreThresholdIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/supervision/splitMarkerFileIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/supervision/splitMarkerFileWithoutExtensionIntTests/"
        )

add_executable(SambadaUnitTests
        test/unit/sambada/CheminAccesTests.cpp
        test/unit/sambada/LecteurCheminAccesTests.cpp
        src/LecteurCheminAcces.hpp
        src/LecteurCheminAcces.cpp
        ${TestUtils}
        )

add_executable(SupervisionIntegrationTests
        test/integration/supervision/mergeResultFilesIntTests/mergeResultFilesIntTests.cpp
        test/integration/supervision/mergeResultFilesWithoutExtensionsIntTests/mergeResultFilesWithoutExtensionsIntTests.cpp
        test/integration/supervision/mergeResultFilesWithScoreThresholdIntTests/mergeResultFilesWithGScoreThresholdIntTests.cpp
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
