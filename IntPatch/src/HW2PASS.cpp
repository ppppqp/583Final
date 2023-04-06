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
#include "llvm/Analysis/AliasSetTracker.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/GlobalsModRef.h"
#include "llvm/Analysis/MemoryLocation.h"
#include <map>


#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Dominators.h"
#include <cassert>

using namespace llvm;

/*
0(00): Trusted and non-overflowed
1(01): trusted and may overflow
2(10): untrusted and non-overflow
3(11): untrusted and may overflow
*/


namespace IntPatch {
    struct DetectPass : public ModulePass {
        static char ID;

        DetectPass() : ModulePass(ID) {}

        void getAnalysisUsage(AnalysisUsage &AU) const override {
            AU.addRequired<AAResultsWrapperPass>();
        }

        virtual bool runOnModule(Module &M) override {

            // std::map<Function*, AAResults*> f2Alias;
            std::map < Value * , int > v2Type;
            std::map < Value * , int > tp_v; // for pointer variable
            std::vector < Value * > all_arithmetic_instr;
            std::map < Value * , bool > vulnerables;
            for (Function &f: M) {
                for (BasicBlock &bb: f) {
                    // initialize variables at key points
                    // TODO: manually identify if a variable is read from program input

                }
            }


            bool change = true;
            int iterCount = 1;

            while (change) {
                errs() << "===============iteration: " << iterCount << "====================\n";
                iterCount++;
                change = false;
                for (Function &f: M) {
                    if (f.isDeclaration()) {
                        errs() << f.getName() << " only has declaration. Skip!\n";
                        continue;
                    }

                    AAResults *aaResults = &getAnalysis<AAResultsWrapperPass>(f).getAAResults();
                    AliasSetTracker aliasSetTracker(*aaResults);
                    for (BasicBlock &bb: f) {
                        for (Instruction &inst: bb) {
                            unsigned opcode = inst.getOpcode();
                            // assignment statement?
                            if (opcode == Instruction::Add || opcode == Instruction::Sub || opcode == Instruction::Mul || opcode == Instruction::Shl) {
                                // arithmetic op, add, subtract, multiply, left shift
                                int type = 1;  // T_01
                                for (int i = 0; i < inst.getNumOperands(); i++) {
                                    Value *operand = inst.getOperand(i);
                                    if (v2Type.find(operand) != v2Type.end()) {
                                        type |= v2Type[operand];
                                    }
                                }

                                change |= (v2Type[&inst] != type);
                                // errs() << "at 82:" << change << "\n";
                                v2Type[&inst] = type;
                                // to record the dangerous instruction
                                all_arithmetic_instr.push_back(&inst);

                            } else if (opcode == Instruction::Store) {
                                // " If variable v1 with type τ is stored into a memory pointed by v,
                                // the target memory will be assigned with type τ, and the memory’s type information
                                // will be joined into tp_v."
                                // store op
                                StoreInst *store = cast<StoreInst>(&inst);
                                Value *storePtr = store->getPointerOperand();
                                MemoryLocation storeAddr = MemoryLocation::get(store);
                                aliasSetTracker.add(store);
                                Value *value = store->getValueOperand();
                                if (v2Type.find(value) != v2Type.end()) {
                                    int valueType = v2Type[value];

                                    // change |= (v2Type[storePtr] != valueType);
                                    // errs() << "at 97:" << change << "\n";
                                    // v2Type[storePtr] = valueTyvpe;

                                    tp_v[storePtr] |= valueType; // initial value of tp_v?
                                    v2Type[&inst] = valueType;
                                    if (v2Type[value] == 3) {
                                        // vulnerability
                                        errs() << inst << "\n\n#######!!!!!!!!Uses a 11 type operand at sink!!!!!!!!########\n\n";
                                        vulnerables[&inst] = true;
                                    }
                                }
                            } else if (opcode == Instruction::Load) {
                                // load op
                                LoadInst *load = cast<LoadInst>(&inst);
                                Value *loadPtr = load->getPointerOperand();
                                MemoryLocation loadAddr = MemoryLocation::get(load);
                                int type = tp_v[loadPtr];
                                //TODO: for all v in the alias,  type |= tp_v[loadAddr]
                                aliasSetTracker.add(load);
                                AliasSet &aliasSet = aliasSetTracker.getAliasSetFor(loadAddr);
                                // errs() << "LOAD OP:" << *load << "\n";
                                // errs() << "ALIAS:\n";
                                for (AliasSet::iterator I = aliasSet.begin(), E = aliasSet.end(); I != E; ++I) {
                                    Value *V = I.getPointer();
                                    // errs() << " * ";
                                    V->print(errs(), false);
                                    // errs() << ":";
                                    // errs() << "\n";
                                    if (tp_v.find(V) != tp_v.end()) {
                                        type |= tp_v[V];
                                    }
                                }
                                if (v2Type.find(&inst) != v2Type.end()) {
                                    // errs() << v2Type[&inst] << type << "\n";
                                    change |= v2Type[&inst] != type;
                                }

                                // if(change){
                                //     errs() << "\nloadPtr!!:";
                                //     errs() << inst << " " << v2Type[&inst] << "=> " << type << "\n";
                                // }
                                errs() << inst << ":" << v2Type[&inst] << "\n";
                                // errs() << "at 120:" << change << "\n";
                                v2Type[&inst] = type;
                            } else if (opcode == Instruction::Br || opcode == Instruction::IndirectBr) {
                                v2Type[&inst] = 0;
                            } else if (opcode == Instruction::ICmp || opcode == Instruction::FCmp) {
                                int type = 0;
                                for (int i = 0; i < inst.getNumOperands(); i++) {
                                    Value *operand = inst.getOperand(i);
                                    if (v2Type.find(operand) != v2Type.end()) {
                                        type |= v2Type[operand];
                                    }
                                }
                                type &= 2;
                                if (v2Type.find(&inst) != v2Type.end()) change |= v2Type[&inst] != type;
                                // errs() << "at 135:" << change << "\n";
                                v2Type[&inst] = type;
                            } else if (opcode == Instruction::PHI) {
                                int type = 0;
                                for (int i = 0; i < inst.getNumOperands(); i++) {
                                    Value *operand = inst.getOperand(i);
                                    if (v2Type.find(operand) != v2Type.end()) {
                                        type |= v2Type[operand];
                                    }
                                }
                                if (v2Type.find(&inst) != v2Type.end()) change |= v2Type[&inst] != type;
                                // errs() << "at 146:" << change << "\n";
                                v2Type[&inst] = type;
                            } else if (opcode == Instruction::Call) {
                                CallInst *call = cast<CallInst>(&inst);
                                Function *fun = call->getCalledFunction();
                                if (fun) {
                                    if (fun->getName() == "__isoc99_fscanf") {
                                        Value *v = call->getArgOperand(2);
                                        errs() << "Dangerous Operand:";
                                        v->print(errs(), false);
                                        errs() << "\n";
                                        v2Type[v] = 3;
                                        tp_v[v] = 3;
                                    }
                                } else {
                                    // indirect call
                                }

                            } else {
                                // default: as assignment
                                int type = 0;
                                if (inst.getNumOperands() > 0 && v2Type.find(inst.getOperand(0)) != v2Type.end()) {
                                    type = v2Type[inst.getOperand(0)];
                                }
                                if (v2Type.find(&inst) != v2Type.end()) change |= v2Type[&inst] < type;
                                // errs() << "at 174:" << change << "\n";
                                // errs() << v2Type[&inst] << type << "\n";
                                v2Type[&inst] |= type;
                            }
                        }
                    }
                }
            }
            errs() << "-------------- End of all iterations. --------------\n";
            for (auto i: v2Type) {
                Instruction *inst = dyn_cast<Instruction>(i.first);
                if (inst) {
                    int type = i.second;
                    errs() << "Function:" << inst->getParent()->getParent()->getName();
                    errs() << " BB:";
                    inst->getParent()->printAsOperand(errs(), false);

                    // errs() << " inst:";
                    // inst->print(errs(), false);
                    errs() << " type:" << type << "\n";
                }
            }
            errs() << "-------------- dangerous registers --------------\n";
            for (auto i: v2Type) {
                Instruction *inst = dyn_cast<Instruction>(i.first);
                if (inst) {
                    int type = i.second;
                    if (type != 3) {
                        continue;
                    }
                    errs() << "Function:" << inst->getParent()->getParent()->getName();
                    errs() << " BB:";
                    inst->getParent()->printAsOperand(errs(), false);

                    errs() << " inst:";
                    inst->print(errs(), false);
                    errs() << " type:" << type << "\n";
                    errs() << "related arithmetic operations: \n";
                    for (auto one: all_arithmetic_instr) {
                        Instruction *arith = dyn_cast<Instruction>(one);
                        if (!arith) {
                            continue;
                        }
                        auto num = arith->getNumOperands();
                        for (int i = 0; i < num; ++i) {
                            if (inst == arith->getOperand(i)) {
                                arith->print(errs(), false);
                            }
                        }
                    }
                }
            }
            // detection complete
            // backward slicing, find all type 3 arithmetic ops that are associated with a vulnerable





            // inject check statement
            LLVMContext &context = M.getContext();
            FunctionType *funcType = FunctionType::get(Type::getVoidTy(context), false);
            // Function* newFunc = Function::Create(funcType, GlobalValue::LinkageTypes::ExternalLinkage, "vulnerableDetected", M);
            // BasicBlock* handlerBB = BasicBlock::Create(context, "handlerBB", newFunc);
            // IRBuilder<> checkFuncBuilder(handlerBB);


            // checkFuncBuilder.CreateRetVoid();


            for (auto i : v2Type) {
                Instruction *inst = dyn_cast<Instruction>(i.first);
                int type = i.second;
                if (!inst || type != 3) continue;
                Function *func = inst->getParent()->getParent();

                // switch statement
                switch (inst->getOpcode()) {
                    case Instruction::Add: {
                        BasicBlock *tempBB = BasicBlock::Create(func->getContext(), "tempBB", func);
                        IRBuilder<> tempBuilder(tempBB);
                        Value *tempRet = tempBuilder.getInt32(42);
                        Constant *str = tempBuilder.CreateGlobalStringPtr("Detected!\n");
                        // FunctionCallee putsFunc = M.getOrInsertFunction("puts", tempBuilder.getInt32Ty(), tempBuilder.getInt8PtrTy(), nullptr);
                        // tempBuilder.CreateCall(putsFunc, str);
                        tempBuilder.CreateRet(tempRet);
                        add_patch(inst, tempBB, context);
                        break;
                    }
                    case Instruction::Mul: {
                        BasicBlock *tempBB = BasicBlock::Create(func->getContext(), "tempBB", func);
                        IRBuilder<> tempBuilder(tempBB);
                        Value *tempRet = tempBuilder.getInt32(43);
                        Constant *str = tempBuilder.CreateGlobalStringPtr("Detected!\n");
                        // FunctionCallee putsFunc = M.getOrInsertFunction("puts", tempBuilder.getInt32Ty(), tempBuilder.getInt8PtrTy(), nullptr);
                        // tempBuilder.CreateCall(putsFunc, str);
                        tempBuilder.CreateRet(tempRet);
                        mul_patch(inst, tempBB, context);
                        break;
                    }
                }
            }

            

            return true;
        }
        void add_patch(Instruction* inst, BasicBlock* tempBB, LLVMContext &context) {
            Function *func = inst->getParent()->getParent();
            IRBuilder<> Builder(inst->getParent());
            Builder.SetInsertPoint(inst->getNextNode());
            Value *op0 = inst->getOperand(0);
            Value *op1 = inst->getOperand(1);
            Value *zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);


            Value *cmp1 = Builder.CreateICmpSGT(op0, zero); // signed > 0
            Value *cmp2 = Builder.CreateICmpSGT(op1, zero);
            Value *cmp3 = Builder.CreateICmpSGT(inst, zero); //


            // the sum of two positive values leads to a negative value
            // cmp1 xor cmp2 == 0, cmp3 = !cmp1

            Value *cmp1Eqcmp2 = Builder.CreateICmpEQ(cmp1, cmp2);
            Value *cmp1Neqcmp3 = Builder.CreateICmpNE(cmp1, cmp3);
            Value *allHold = Builder.CreateAnd(cmp1Eqcmp2, cmp1Neqcmp3);


            // split the BB?
            // branch to the original successor BB if any of the condition is false
            // branch to the new BB (which calls the checkFunc) if all conditions are true


            BasicBlock *originalBB = inst->getParent()->splitBasicBlock(dyn_cast<Instruction>(allHold)->getNextNode());
            errs() << "SPLIT AT:";
            originalBB->printAsOperand(errs(), false);
            errs() << "\n";


            Instruction *oldTerminator = inst->getParent()->getTerminator();
            Builder.SetInsertPoint(oldTerminator);

            BranchInst *branch = Builder.CreateCondBr(allHold, tempBB, originalBB);
            oldTerminator->eraseFromParent();
        }

        void mul_patch(Instruction* inst, BasicBlock* tempBB, LLVMContext &context) {
            Function *func = inst->getParent()->getParent();
            IRBuilder<> Builder(inst->getParent());
            Builder.SetInsertPoint(inst->getNextNode());
            Value *op0 = inst->getOperand(0);
            Value *op1 = inst->getOperand(1);
            Value *zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);


            Value *cmp1 = Builder.CreateICmpSGT(op0, zero); // signed > 0
            Value *cmp2 = Builder.CreateICmpSGT(op1, zero);
            Value *cmp3 = Builder.CreateICmpSGT(inst, zero);
            Value *cmp4 = Builder.CreateICmpSLT(inst, zero);


            // the product of values with different signs leads to a negative value
            // cmp1 == cmp2 (same sign), cmp3 < 0, then goto tempBB. ---> condition 1
            // cmp1 != cmp2 (diff sign), cmp3 > 0, then goto tempBB. ---> condition 2

            Value *cmp1Eqcmp2 = Builder.CreateICmpEQ(cmp1, cmp2);
            Value *cmp1Neqcmp2 = Builder.CreateICmpNE(cmp1, cmp2);
            Value *condition1 = Builder.CreateAnd(cmp1Eqcmp2, cmp4);
            Value *condition2 = Builder.CreateAnd(cmp1Neqcmp2, cmp3);
            Value *allHold = Builder.CreateOr(condition1, condition2);


            // split the BB?
            // branch to the original successor BB if any of the condition is false
            // branch to the new BB (which calls the checkFunc) if all conditions are true


            BasicBlock *originalBB = inst->getParent()->splitBasicBlock(dyn_cast<Instruction>(allHold)->getNextNode());
            errs() << "SPLIT AT:";
            originalBB->printAsOperand(errs(), false);
            errs() << "\n";


            Instruction *oldTerminator = inst->getParent()->getTerminator();
            Builder.SetInsertPoint(oldTerminator);

            BranchInst *branch = Builder.CreateCondBr(allHold, tempBB, originalBB);
            oldTerminator->eraseFromParent();
        }
    };
}

char IntPatch::DetectPass::ID = 0;
static RegisterPass <IntPatch::DetectPass> X(
        "intpatch-detect",
        "Detect Vulnerability", false, false);

