cmake_minimum_required(VERSION 3.10)




set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Wmissing-prototypes -Wredundant-decls -Wshadow -Wvla -Wpointer-arith -O3 -fomit-frame-pointer")
set(NISTFLAGS "-Wno-unused-result -O3 -fomit-frame-pointer")

set(SOURCES
  sign.c
  packing.c
  polyvec.c
  poly.c
  ntt.c
  reduce.c
  rounding.c
)

set(HEADERS
  config.h
  params.h
  api.h
  sign.h
  packing.h
  polyvec.h
  poly.h
  ntt.h
  reduce.h
  rounding.h
  symmetric.h
  randombytes.h
)

set(KECCAK_SOURCES
  ${SOURCES}
  fips202.c
  symmetric-shake.c
)

set(KECCAK_HEADERS
  ${HEADERS}
  fips202.h
)

# 生成共享库
add_library(pqcrystals_fips202_ref SHARED fips202.c fips202.h)

add_library(pqcrystals_dilithium2_ref SHARED
  ${SOURCES}
  symmetric-shake.c
)
target_compile_definitions(pqcrystals_dilithium2_ref PRIVATE DILITHIUM_MODE=2)

add_library(pqcrystals_dilithium3_ref SHARED
  ${SOURCES}
  symmetric-shake.c
)
target_compile_definitions(pqcrystals_dilithium3_ref PRIVATE DILITHIUM_MODE=3)

add_library(pqcrystals_dilithium5_ref SHARED
  ${SOURCES}
  symmetric-shake.c
)
target_compile_definitions(pqcrystals_dilithium5_ref PRIVATE DILITHIUM_MODE=5)

# 生成测试程序
add_executable(test_dilithium2 test/test_dilithium.c randombytes.c ${KECCAK_SOURCES})
target_compile_definitions(test_dilithium2 PRIVATE DILITHIUM_MODE=2)

add_executable(test_dilithium3 test/test_dilithium.c randombytes.c ${KECCAK_SOURCES})
target_compile_definitions(test_dilithium3 PRIVATE DILITHIUM_MODE=3)

add_executable(test_dilithium5 test/test_dilithium.c randombytes.c ${KECCAK_SOURCES})
target_compile_definitions(test_dilithium5 PRIVATE DILITHIUM_MODE=5)

add_executable(test_vectors2 test/test_vectors.c ${KECCAK_SOURCES})
target_compile_definitions(test_vectors2 PRIVATE DILITHIUM_MODE=2)

add_executable(test_vectors3 test/test_vectors.c ${KECCAK_SOURCES})
target_compile_definitions(test_vectors3 PRIVATE DILITHIUM_MODE=3)

add_executable(test_vectors5 test/test_vectors.c ${KECCAK_SOURCES})
target_compile_definitions(test_vectors5 PRIVATE DILITHIUM_MODE=5)

add_executable(test_speed2 test/test_speed.c test/speed_print.c test/cpucycles.c randombytes.c ${KECCAK_SOURCES})
target_compile_definitions(test_speed2 PRIVATE DILITHIUM_MODE=2)

add_executable(test_speed3 test/test_speed.c test/speed_print.c test/cpucycles.c randombytes.c ${KECCAK_SOURCES})
target_compile_definitions(test_speed3 PRIVATE DILITHIUM_MODE=3)

add_executable(test_speed5 test/test_speed.c test/speed_print.c test/cpucycles.c randombytes.c ${KECCAK_SOURCES})
target_compile_definitions(test_speed5 PRIVATE DILITHIUM_MODE=5)

add_executable(test_mul test/test_mul.c randombytes.c ${KECCAK_SOURCES})
target_compile_definitions(test_mul PRIVATE UDBENCH)

# 生成NIST KAT程序
add_executable(PQCgenKAT_sign2 nistkat/PQCgenKAT_sign.c nistkat/rng.c ${KECCAK_SOURCES})
target_compile_definitions(PQCgenKAT_sign2 PRIVATE DILITHIUM_MODE=2)
target_link_libraries(PQCgenKAT_sign2 crypto)

add_executable(PQCgenKAT_sign3 nistkat/PQCgenKAT_sign.c nistkat/rng.c ${KECCAK_SOURCES})
target_compile_definitions(PQCgenKAT_sign3 PRIVATE DILITHIUM_MODE=3)
target_link_libraries(PQCgenKAT_sign3 crypto)

add_executable(PQCgenKAT_sign5 nistkat/PQCgenKAT_sign.c nistkat/rng.c ${KECCAK_SOURCES})
target_compile_definitions(PQCgenKAT_sign5 PRIVATE DILITHIUM_MODE=5)
target_link_libraries(PQCgenKAT_sign5 crypto)

# 清理目标
add_custom_target(clean_build
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/test
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/nistkat
)
