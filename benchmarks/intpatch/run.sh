#!/bin/bash
# Run script for Homework 2 EECS 583 Winter 2023

# ACTION NEEDED: If the path is different, please update it here.
PATH2LIB=~/583Final/IntPatch/build/src/LLVMHW2.so        # Specify your build directory in the project

# ACTION NEEDED: Choose the correct pass when running.
PASS=-intpatch-detect                  # Choose either -fplicm-correctness ...
# PASS=-fplicm-performance                 # ... or -fplicm-performance

# Delete outputs from previous runs. Update this when you want to retain some files.
rm -f default.profraw *_prof *_fplicm *.bc *.profdata *_output *.ll

# Convert source code to bitcode (IR).
# clang -fsanitize=unsigned-integer-overflow ${1}.c
clang -emit-llvm -fno-strict-overflow  -c ${1}.c -o ${1}.bc

# # Canonicalize natural loops (Ref: llvm.org/doxygen/LoopSimplify_8h_source.html)
opt -passes='loop-simplify' ${1}.bc -o ${1}.ls.bc

# Instrument profiler passes.
opt -passes='pgo-instr-gen,instrprof' ${1}.ls.bc -o ${1}.ls.prof.bc

# Note: We are using the New Pass Manager for these passes! 

# Generate binary executable with profiler embedded
clang -fprofile-instr-generate -fno-strict-overflow ${1}.ls.prof.bc -o ${1}_prof

# When we run the profiler embedded executable, it generates a default.profraw file that contains the profile data.
./${1}_prof > correct_output

# Converting it to LLVM form. This step can also be used to combine multiple profraw files,
# in case you want to include different profile runs together.
llvm-profdata merge -o ${1}.profdata default.profraw

# The "Profile Guided Optimization Use" pass attaches the profile data to the bc file.
opt -passes="pgo-instr-use" -o ${1}.profdata.bc -pgo-test-profile-file=${1}.profdata < ${1}.ls.prof.bc > /dev/null

# We now use the profile augmented bc file as input to your pass.
opt -enable-new-pm=0 -o ${1}.fplicm.bc -load ${PATH2LIB} ${PASS} < ${1}.profdata.bc > /dev/null


clang -emit-llvm -fno-strict-overflow -S ${1}.c -o fun.ll
# Generate binary excutable before FPLICM: Unoptimzied code
clang ${1}.ls.bc -fno-strict-overflow -o ${1}_no_fplicm 
# Generate binary executable after FPLICM: Optimized code
clang ${1}.fplicm.bc -fno-strict-overflow -o ${1}_fplicm

# Generate text IR before pass
llvm-dis ${1}.ls.bc -o ${1}_before.ll
# Generate text IR before pass
llvm-dis ${1}.fplicm.bc -o ${1}_after.ll

# Produce output from binary to check correctness

./${1}_fplicm > fplicm_output

echo -e "\n=== Results ==="

echo -e ">> PASS\n"
# Measure performance
echo -e "1. Unsanitized Code"
./${1}_no_fplicm
echo -e "\n$?"
echo -e "\n\n"
echo -e "2. Sanitized Code"
./${1}_fplicm
echo -e "\n$?"
echo -e "\n\n"

# Cleanup: Remove this if you want to retain the created files.
# rm -f default.profraw *_prof *_fplicm *.bc *.profdata *_output *.ll a.out
