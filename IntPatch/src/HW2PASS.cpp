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
0(00): Trusted and non-overflowed
1(01): trusted and may overflow
2(10): untrusted and non-overflow
3(11): untrusted and may overflow
*/


namespace IntPatch{
  struct DetectPass: public ModulePass {
    static char ID;
    DetectPass(): ModulePass(ID) {}
    virtual bool runOnModule(Module &M) override{
      std::map<Value*, int> v2Type;
      std::map<Value*, int> tp_v; // for pointer variable
      for(Function &f : M){
        for(BasicBlock &bb: f){
          // initialize variables at key points
          // TODO: manually identify if a variable is read from program input

        }
      }
      bool change = true;
      while(change){
        change = false;
        for(Function &f : M){
          for(BasicBlock &bb : f){
            bb.printAsOperand(errs(), false);
            for(Instruction &inst : bb){
              unsigned opcode = inst.getOpcode();
              // assignment statement?
              if(opcode == Instruction::Add || opcode == Instruction::Sub || opcode == Instruction::Mul){
                // arithmetic op, TODO: should also contain leftshift
                int type = 1;
                for(int i = 0; i < inst.getNumOperands(); i++){
                  Value* operand = inst.getOperand(i);
                  if(v2Type.find(operand) != v2Type.end()){
                    type |= v2Type[operand];
                  }
                }
                change |= (v2Type[&inst] == type);
                v2Type[&inst] = type;
              } else if(opcode == Instruction::Store){
              // " If variable v1 with type τ is stored into a memory pointed by v,
              // the target memory will be assigned with type τ, and the memory’s type information 
              // will be joined into tp_v."
              // store op
                Value* storeAddr = inst.getPointerOperand();
                Value* value = inst.getValueOperand();
                if(v2Type.find(value) != v2Type.end()){
                  int valueType = v2Type[value];
                  change |= (v2Type[storeAddr] == valueType);
                  v2Type[storeAddr] = valueType;

                  tp_v[storeAddr] |= valueType; // initial value of tp_v?
                  //FIXME: What about v2Type[&inst] ?
                }
              } else if(opcode == Instruction::Load){
              // load op
                Value* loadAddr = inst.getPointerOperand();
                Value* value = inst.getValueOperand();
                int type = tp_v[loadAddr];
                //TODO: for all v in the alias,  type |= tp_v[loadAddr]
                if(v2Type.find(&inst) != v2Type.end()) change |= v2Type[&inst] == type;
                v2Type[&inst] = type;

              } else if(opcode == Instruction::Branch){
                v2Type[&inst] = 0;
              } else if(opcode == Instruction::ICmp || opcode == Instruction::FCmp){
                int type = 0;
                for(int i = 0; i < inst->getNumOperands(); i++){
                  Value* operand = inst->getOperand(i);
                  if(v2Type.find(operand) != v2Type.end()){
                    type |= v2Type[operand];
                  }
                }
                type &= 2;
                if(v2Type.find(&inst) != v2Type.end()) change |= v2Type[&inst] == type;
                v2Type[&inst] = type;
              } else if(opcode == Instruction::PHI){
                int type = 0;
                for(int i = 0; i < inst->getNumOperands(); i++){
                  Value* operand = inst->getOperand(i);
                  if(v2Type.find(operand) != v2Type.end()){
                    type |= v2Type[operand];
                  }
                }
                if(v2Type.find(&inst) != v2Type.end()) change |= v2Type[&inst] == type;
                v2Type[&inst] = type;
              } else{
                // default: as assignment
                int type = 0;
                if(v2Type.find(inst.getOperand(0)) != v2Type.end()){
                  type = v2Type[inst.getOperand(0)];
                }
                if(v2Type.find(&inst) != v2Type.end()) change |= v2Type[&inst] == type;
                v2Type[&inst] = type;
              }
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
