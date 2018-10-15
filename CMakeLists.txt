cmake_minimum_required(VERSION 3.10)
project(sambada)

set(CMAKE_CXX_STANDARD 11)

add_definitions(
        -Dsambada_executable_extension="${CMAKE_EXECUTABLE_SUFFIX}"
        -Dsambada_top_source_dir="${PROJECT_SOURCE_DIR}"
)

include_directories(ext/scythestat-1.0.3)
include_directories(ext/scythestat-1.0.3/scythestat)
include_directories(ext/scythestat-1.0.3/scythestat/rng)
include_directories(ext/shapelib-1.3.0)
include_directories(src)
include_directories(test)
include_directories(test/utils)
include_directories(test/integration/utils)

add_library(shp
        ext/shapelib-1.3.0/dbfopen.c
        ext/shapelib-1.3.0/safileio.c
        ext/shapelib-1.3.0/shapefil.h
        ext/shapelib-1.3.0/shpopen.c
        ext/shapelib-1.3.0/shptree.c
        )

add_library(intermediate
        src/RegressionLogistique-bienvenue.cpp
        src/RegressionLogistique.h
        src/Toolbox.cpp
        src/Toolbox.h
        src/Erreur.cpp
        src/Erreur.h
        )

add_executable(sambada
        src/Archiviste.cpp
        src/Archiviste.h
        src/Chronometre.cpp
        src/Chronometre.h
        src/Duree.cpp
        src/Duree.h
        src/FluxSortie.cpp
        src/FluxSortie.h
        src/Journal.cpp
        src/Journal.h
        src/JournalTemporaire.cpp
        src/JournalTemporaire.h
        src/mainSambada.cpp
        src/RegressionLogistique-calc.cpp
        src/RegressionLogistique-io.cpp
        src/Supervision.cpp
        src/Supervision.h
        )

target_link_libraries(sambada
        PRIVATE
        shp
        intermediate
        )

add_executable(supervision
        src/Archiviste.cpp
        src/Archiviste.h
        src/mainSupervision.cpp
        src/RegressionLogistique.h
        src/Supervision.cpp
        src/Supervision.h
        src/Toolbox.cpp
        src/Toolbox.h
        )

target_link_libraries(supervision
        PRIVATE
        intermediate
        )

add_executable(recodePlink
        src/Erreur.h
        src/recodePlink.cpp
        src/RegressionLogistique.h
        src/Toolbox.h
        )

target_link_libraries(recodePlink
        PRIVATE
        intermediate
        )

add_executable(recodePlinkLFMM
        src/Erreur.h
        src/recodeLFMM.cpp
        src/RegressionLogistique.h
        src/Toolbox.h
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
        test/utils/SambadaRegressionResults.h
        test/utils/SambadaRegressionResults.cpp
        )

set(IntegrationTestUtils
        test/integration/utils/SambadaIntegrationTestUtils.h
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
        test/integration/sambada/noExtensionsToFilenamesIntTests/noExtensionsToFilenamesIntTests.cpp
        test/integration/sambada/noNewLinesEndOfFilesIntTests/noNewLinesEndOfFilesIntTests.cpp
        test/integration/sambada/regressionResultsIntTests/regressionResultsDim1IntTests.cpp
        test/integration/sambada/regressionResultsIntTests/regressionResultsDim4IntTests.cpp
        test/integration/sambada/resultFilesLocationIntTests/resultsInDifferentFolderAsDataFilesIntTest.cpp
        test/integration/sambada/resultFilesLocationIntTests/resultsInDifferentFolderAsUniqueDataFileIntTest.cpp
        test/integration/sambada/resultFilesLocationIntTests/resultsInSameFolderAsDataFilesIntTest.cpp
        test/integration/sambada/resultFilesLocationIntTests/resultsInSameFolderAsUniqueDataFileIntTest.cpp
        test/integration/sambada/splitInputFilesIntTests/splitInputFilesWithFilenamesExtensionsIntTests.cpp
        test/integration/sambada/splitInputFilesIntTests/splitInputFilesWithoutFilenamesExtensionsIntTests.cpp
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
        "${CMAKE_BINARY_DIR}/test/integration/sambada/noExtensionsToFilenamesIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/noNewLinesEndOfFilesIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/regressionResultsIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/resultFilesLocationIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/splitInputFilesIntTests/"
        "${CMAKE_BINARY_DIR}/test/integration/sambada/wrongNumberOfIndividualsIntTests/"
        )