# 583Final
## README

This repository contains a tool for detecting integer-related vulnerabilities in LLVM IR code.

Background
Integer-related vulnerabilities are security issues that occur when an application or program takes input from an untrusted source and does not correctly validate and sanitize it. Attackers can exploit these vulnerabilities to execute arbitrary code, escalate privileges, or access sensitive data. Some examples of integer-related vulnerabilities are buffer overflows, integer overflows, and format string vulnerabilities.

## Tool
The tool in this repository detects integer-related vulnerabilities in LLVM IR code. It analyzes the code and identifies instructions that use integer operations and/or pointers. It then determines whether these instructions can lead to vulnerabilities based on the types of the operands and whether they are derived from user input.

The tool is implemented as a module pass in LLVM. It takes LLVM IR code as input, analyzes the code, and outputs a report of any detected vulnerabilities.

## Usage
To use the tool, follow these steps:

1. Make sure that LLVM is installed on your system. If you don't have LLVM installed, you can refer to the "Getting Started with the LLVM System" page in the LLVM documentation for full installation details. The documentation can be found at https://llvm.org/docs/GettingStarted.html
After installing LLVM, verify that the following versions are correct:

clang --version
llvm-config --version
opt --version

2. Go to the build directory of the project and run cmake, then make:
cd InPatch/build
cmake ..
make

3. To run the tool on a specific test file, navigate to the benchmarks/intpatch directory and run the following command:
sh run.sh test2
This will run the tool on test2.c.


Alternatively, to run the tool on all test files provided and count the lines before and after, run the following command:
sh test_runner.sh

## Limitations
The tool has several limitations:

The tool only works on LLVM IR code. It does not work on source code.
The tool is not comprehensive and may miss some vulnerabilities.
The tool is not optimized for performance and may be slow on large codebases.
Acknowledgments
The tool is based on the paper "Detecting Integer-related Vulnerabilities in LLVM IR Code" by Chen et al. (2019).
