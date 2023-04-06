# 583Final
## README

This repository contains a tool for detecting integer-related vulnerabilities in LLVM IR code.

Background
Integer-related vulnerabilities are security issues that occur when an application or program takes input from an untrusted source and does not correctly validate and sanitize it. Attackers can exploit these vulnerabilities to execute arbitrary code, escalate privileges, or access sensitive data. Some examples of integer-related vulnerabilities are buffer overflows, integer overflows, and format string vulnerabilities.

## Tool
The tool in this repository detects integer-related vulnerabilities in LLVM IR code. It analyzes the code and identifies instructions that use integer operations and/or pointers. It then determines whether these instructions can lead to vulnerabilities based on the types of the operands and whether they are derived from user input.

The tool is implemented as a module pass in LLVM. It takes LLVM IR code as input, analyzes the code, and outputs a report of any detected vulnerabilities.

## Usage
To use the tool, you must have LLVM installed on your system. Once you have LLVM installed, follow these steps:

Build the tool by running make in the root directory of the repository.
Run the tool on your LLVM IR code using the following command:
TODO

## Limitations
The tool has several limitations:

The tool only works on LLVM IR code. It does not work on source code.
The tool is not comprehensive and may miss some vulnerabilities.
The tool is not optimized for performance and may be slow on large codebases.
Acknowledgments
The tool is based on the paper "Detecting Integer-related Vulnerabilities in LLVM IR Code" by Chen et al. (2019).
