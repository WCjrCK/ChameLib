cmake_minimum_required(VERSION 3.0)
project(ChameLib)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_BUILD_TYPE "Release")

include_directories(include)

find_package(OpenSSL REQUIRED)

file(GLOB LIBRARY_SOURCES
    src/AE/*.cpp
    src/base/*.cpp
    src/exception/*.cpp
    src/curve/*.cpp
    src/utils/*.cpp
    src/SE/*.cpp
    src/signature/*.cpp
    src/ABE/*.cpp
    src/scheme/CH/*.cpp
    src/scheme/IBCH/*.cpp
    src/scheme/PBCH/*.cpp
)
add_library(ChameLib STATIC ${LIBRARY_SOURCES})
target_link_libraries(ChameLib PUBLIC gmp pbc OpenSSL::SSL)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -mavx2 -O0")
add_compile_options(-Wunknown-pragmas)

FIND_PACKAGE(OpenMP REQUIRED)
if (OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif ()

if (ENABLE_OMP)
    message("OpenMP enabled")
    add_definitions("-DParallel")
endif (ENABLE_OMP)

if (LOCAL_DEBUG)
    message("LOCAL_DEBUG enabled")
    add_definitions("-DDEBUG")
endif (LOCAL_DEBUG)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS}")

SET(BUILD_TESTS ON)
if(BUILD_TESTS)
    find_package(GTest REQUIRED)

    enable_testing()

    macro(add_test_executable test_name test_source)
        add_executable(${test_name} ${test_source})
        target_include_directories(${test_name} PRIVATE test/common)
        target_link_libraries(${test_name} PRIVATE ChameLib GTest::GTest pthread)
        add_test(NAME ${test_name} COMMAND ${test_name})
    endmacro()

    add_test_executable(RSA_test test/AE_test/RSA_test.cpp)
    # add_test_executable(PKE_CPA_AMV_2017_test test/AE_test/PKE_CPA_AMV_2017_test.cpp)
    # add_test_executable(PKE_CCA_AMV_2017_test test/AE_test/PKE_CCA_AMV_2017_test.cpp)
    add_test_executable(AES_test test/SE_test/AES_test.cpp)
    add_test_executable(BLS_test test/signature_test/BLS_test.cpp)
    add_test_executable(CP_ABE_test test/ABE_test/CP_ABE_test.cpp)
    add_test_executable(MA_ABE_test test/ABE_test/MA_ABE_test.cpp)
    add_test_executable(ABET_test test/ABE_test/ABET_test.cpp)
    add_test_executable(RABE_XNM_test test/ABE_test/RABE_XNM_test.cpp)
    add_test_executable(RABE_TMM_test test/ABE_test/RABE_TMM_test.cpp)

    # add_test_executable(CH_AMV_2017_test test/scheme_test/CH_test/CH_AMV_2017_test.cpp)
    add_test_executable(CH_CDK_2017_test test/scheme_test/CH_test/CH_CDK_2017_test.cpp)
    add_test_executable(CH_ET_BC_CDK_2017_test test/scheme_test/CH_test/CH_ET_BC_CDK_2017_test.cpp)
    add_test_executable(CH_ET_KOG_CDK_2017_test test/scheme_test/CH_test/CH_ET_KOG_CDK_2017_test.cpp)
    add_test_executable(CH_FS_ECC_CCT_2024_test test/scheme_test/CH_test/CH_FS_ECC_CCT_2024_test.cpp)
    add_test_executable(CH_KEF_CZK_2004_test test/scheme_test/CH_test/CH_KEF_CZK_2004_test.cpp)
    add_test_executable(CH_KEF_DL_CZT_2011_test test/scheme_test/CH_test/CH_KEF_DL_CZT_2011_test.cpp)
    add_test_executable(CH_KEF_DLP_LLA_2012_test test/scheme_test/CH_test/CH_KEF_DLP_LLA_2012_test.cpp)
    add_test_executable(CH_KEF_MH_RSA_F_AM_2004_test test/scheme_test/CH_test/CH_KEF_MH_RSA_F_AM_2004_test.cpp)
    add_test_executable(CH_KEF_MH_RSANN_F_AM_2004_test test/scheme_test/CH_test/CH_KEF_MH_RSANN_F_AM_2004_test.cpp)
    add_test_executable(CH_KEF_MH_SDH_DL_AM_2004_test test/scheme_test/CH_test/CH_KEF_MH_SDH_DL_AM_2004_test.cpp)
    add_test_executable(CH_KEF_NoMH_AM_2004_test test/scheme_test/CH_test/CH_KEF_NoMH_AM_2004_test.cpp)
    add_test_executable(CHET_RSA_CDK_2017_test test/scheme_test/CH_test/CHET_RSA_CDK_2017_test.cpp)
    add_test_executable(CR_CH_DSS_2020_test test/scheme_test/CH_test/CR_CH_DSS_2020_test.cpp)
    add_test_executable(FCR_CH_PreQA_DKS_2020_test test/scheme_test/CH_test/FCR_CH_PreQA_DKS_2020_test.cpp)
    add_test_executable(MCH_CDK_2017_test test/scheme_test/CH_test/MCH_CDK_2017_test.cpp)

    add_test_executable(IB_CH_KEF_CZS_2014_test test/scheme_test/IBCH_test/IB_CH_KEF_CZS_2014_test.cpp)
    add_test_executable(IB_CH_MD_LSX_2022_test test/scheme_test/IBCH_test/IB_CH_MD_LSX_2022_test.cpp)
    add_test_executable(IB_CH_ZSS_S1_2003_test test/scheme_test/IBCH_test/IB_CH_ZSS_S1_2003_test.cpp)
    add_test_executable(IB_CH_ZSS_S2_2003_test test/scheme_test/IBCH_test/IB_CH_ZSS_S2_2003_test.cpp)
    add_test_executable(ID_B_CollRes_XSL_2021_test test/scheme_test/IBCH_test/ID_B_CollRes_XSL_2021_test.cpp)

    add_test_executable(DPCH_MXN_2022_test test/scheme_test/PBCH_test/DPCH_MXN_2022_test.cpp)
    add_test_executable(MAPCH_ZLW_2021_test test/scheme_test/PBCH_test/MAPCH_ZLW_2021_test.cpp)
    add_test_executable(PCH_DSS_2019_test test/scheme_test/PBCH_test/PCH_DSS_2019_test.cpp)
    add_test_executable(PCHBA_TLL_2020_test test/scheme_test/PBCH_test/PCHBA_TLL_2020_test.cpp)
    add_test_executable(RPCH_TMM_2022_test test/scheme_test/PBCH_test/RPCH_TMM_2022_test.cpp)
    add_test_executable(RPCH_XNM_2021_test test/scheme_test/PBCH_test/RPCH_XNM_2021_test.cpp)
    
    add_test_executable(base_test test/scheme_test/base_test.cpp)
endif()

install(TARGETS ChameLib ARCHIVE DESTINATION lib)
install(DIRECTORY include/ DESTINATION include/ChameLib)
