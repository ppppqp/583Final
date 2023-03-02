//===-- Frequent Path Loop Invariant Code Motion Pass --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
//
// EECS583 W23 - This pass can be used as a template for your FPLICM homework
//               assignment.
//               The passes get registered as "fplicm-correctness" and
//               "fplicm-performance".
//
//
////===-------------------------------------------------------------------===//
#include <queue>
#include <set>
#include <vector>

#include "llvm/Analysis/BlockFrequencyInfo.h"
#include "llvm/Analysis/BranchProbabilityInfo.h"
#include "llvm/Analysis/DominanceFrontier.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopIterator.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar/LoopPassManager.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include <map>

using namespace llvm;

/*
0: Trusted and non-overflowed
1: trusted and may overflow
2: untrusted and non-overflow
3: untrusted and may overflow
*/

namespace IntPatch{
  struct DetectPass: public ModulePass {
    static char ID;
    DetectPass(): ModulePass(ID) {}
    virtual bool runOnModule(Module &M) override{
      std::map<Instruction*, int> inst2Type;
      // 
      for(Function &f : M){
        for(BasicBlock &bb : f){
          bb.printAsOperand(errs(), false);
          for(Instruction &inst : bb){
            unsigned opcode = inst.getOpcode();
            // assignment statement?
            if(opcode == Instruction::Add || opcode == Instruction::Sub || opcode == Instruction::Mul 
                    || opcode == Instruction::UDiv || opcode == Instruction::SDiv || opcode == Instruction::URem || opcode == Instruction::Shl || opcode == Instruction::LShr
                    || opcode == Instruction::AShr || opcode == Instruction::And || opcode == Instruction::Or || opcode == Instruction::Xor
                    || opcode == Instruction::ICmp || opcode == Instruction::SRem
            ){
              // arithmetic op

            } else if(opcode == Instruction::Store){
            // store op
            } else if(opcode == Instruction::Load){
            // load op
            }
          } 
        }
      }
      return false;
    }
  };
}

char IntPatch::DetectPass::ID = 0;
static RegisterPass<IntPatch::DetectPass> X(
    "intpatch-detect",
    "Detect Vulnerability", false, false);
