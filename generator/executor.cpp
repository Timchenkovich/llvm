extern "C" {
 #include "../sim/sim.h"
}
#include <fstream>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

using namespace llvm;

int main() {
  LLVMContext context;
  std::unique_ptr<Module> module = std::make_unique<Module>("generated", context);
  IRBuilder<> builder(context);
  Type* voidType = Type::getVoidTy(context);
  
  Type* intType = Type::getInt32Ty(context);
  ArrayRef<Type*> simPutPixelArgs = {intType, intType, intType};
  FunctionType* simPutPixelType = FunctionType::get(voidType, simPutPixelArgs, false);
  FunctionCallee simPutPixelFunc = module->getOrInsertFunction("simPutPixel", simPutPixelType);
  
  FunctionType* simFlushType = FunctionType::get(voidType, false);
  FunctionCallee simFlushFunc = module->getOrInsertFunction("simFlush", simFlushType);
  
  FunctionType* simRandType = FunctionType::get(intType, false);
  FunctionCallee simRandFunc = module->getOrInsertFunction("simRand", simRandType);
  
  std::string fill_backgroundFuncName = "fill_background";
  ArrayRef<Type*> fill_backgroundArgs = {Type::getInt32Ty(context)};
  FunctionType* fill_backgroundFuncType = FunctionType::get(voidType, fill_backgroundArgs, false);
  Function::LinkageTypes fill_backgroundLinkage = Function::InternalLinkage;
  Function* fill_backgroundFunc = Function::Create(fill_backgroundFuncType, fill_backgroundLinkage, fill_backgroundFuncName, module.get());
  
  std::string adjust_directionsFuncName = "adjust_directions";
  ArrayRef<Type*> adjust_directionsArgs = {Type::getInt32Ty(context), Type::getInt32Ty(context), Type::getInt64Ty(context)->getPointerTo(), Type::getInt64Ty(context)->getPointerTo(), Type::getInt32Ty(context), Type::getInt32Ty(context)};
  FunctionType* adjust_directionsFuncType = FunctionType::get(voidType, adjust_directionsArgs, false);
  Function::LinkageTypes adjust_directionsLinkage = Function::InternalLinkage;
  Function* adjust_directionsFunc = Function::Create(adjust_directionsFuncType, adjust_directionsLinkage, adjust_directionsFuncName, module.get());
  
  std::string adjust_positionsFuncName = "adjust_positions";
  ArrayRef<Type*> adjust_positionsArgs = {Type::getInt64Ty(context)->getPointerTo(), Type::getInt64Ty(context)->getPointerTo(), Type::getInt64Ty(context)->getPointerTo(), Type::getInt64Ty(context)->getPointerTo(), Type::getInt32Ty(context), Type::getInt32Ty(context)};
  FunctionType* adjust_positionsFuncType = FunctionType::get(voidType, adjust_positionsArgs, false);
  Function::LinkageTypes adjust_positionsLinkage = Function::InternalLinkage;
  Function* adjust_positionsFunc = Function::Create(adjust_positionsFuncType, adjust_positionsLinkage, adjust_positionsFuncName, module.get());
  
  std::string change_colorsFuncName = "change_colors";
  ArrayRef<Type*> change_colorsArgs = {Type::getInt64Ty(context)->getPointerTo(), Type::getInt64Ty(context)->getPointerTo(), Type::getInt64Ty(context)->getPointerTo()};
  FunctionType* change_colorsFuncType = FunctionType::get(voidType, change_colorsArgs, false);
  Function::LinkageTypes change_colorsLinkage = Function::InternalLinkage;
  Function* change_colorsFunc = Function::Create(change_colorsFuncType, change_colorsLinkage, change_colorsFuncName, module.get());
  
  std::string reflect_dotsFuncName = "reflect_dots";
  ArrayRef<Type*> reflect_dotsArgs = {Type::getInt32Ty(context), Type::getInt32Ty(context), Type::getInt32Ty(context), Type::getInt32Ty(context), Type::getInt32Ty(context)};
  FunctionType* reflect_dotsFuncType = FunctionType::get(voidType, reflect_dotsArgs, false);
  Function::LinkageTypes reflect_dotsLinkage = Function::InternalLinkage;
  Function* reflect_dotsFunc = Function::Create(reflect_dotsFuncType, reflect_dotsLinkage, reflect_dotsFuncName, module.get());
  
  std::string draw_half_circleFuncName = "draw_half_circle";
  ArrayRef<Type*> draw_half_circleArgs = {Type::getInt32Ty(context), Type::getInt32Ty(context), Type::getInt32Ty(context), Type::getInt32Ty(context), Type::getInt32Ty(context)};
  FunctionType* draw_half_circleFuncType = FunctionType::get(voidType, draw_half_circleArgs, false);
  Function::LinkageTypes draw_half_circleLinkage = Function::InternalLinkage;
  Function* draw_half_circleFunc = Function::Create(draw_half_circleFuncType, draw_half_circleLinkage, draw_half_circleFuncName, module.get());
  
  std::string draw_circleFuncName = "draw_circle";
  ArrayRef<Type*> draw_circleArgs = {Type::getInt32Ty(context), Type::getInt32Ty(context), Type::getInt32Ty(context), Type::getInt32Ty(context)};
  FunctionType* draw_circleFuncType = FunctionType::get(voidType, draw_circleArgs, false);
  Function::LinkageTypes draw_circleLinkage = Function::InternalLinkage;
  Function* draw_circleFunc = Function::Create(draw_circleFuncType, draw_circleLinkage, draw_circleFuncName, module.get());
  
  std::string appFuncName = "app";
  ArrayRef<Type*> appArgs = {};
  FunctionType* appFuncType = FunctionType::get(voidType, appArgs, false);
  Function::LinkageTypes appLinkage = Function::ExternalLinkage;
  Function* appFunc = Function::Create(appFuncType, appLinkage, appFuncName, module.get());
  
  // defining fill_background function
  {
    BasicBlock* bb1 = BasicBlock::Create(context, "", fill_backgroundFunc);
    BasicBlock* bb2 = BasicBlock::Create(context, "", fill_backgroundFunc);
    BasicBlock* bb4 = BasicBlock::Create(context, "", fill_backgroundFunc);
    BasicBlock* bb5 = BasicBlock::Create(context, "", fill_backgroundFunc);
    BasicBlock* bb8 = BasicBlock::Create(context, "", fill_backgroundFunc);
    
    Value* val0 = fill_backgroundFunc->args().begin() + 0;
    builder.SetInsertPoint(bb1);
    builder.CreateBr(bb2);
    builder.SetInsertPoint(bb2);
    PHINode* val3 = builder.CreatePHI(Type::getInt32Ty(context), 2);
    builder.CreateBr(bb8);
    builder.SetInsertPoint(bb4);
    builder.CreateRetVoid();
    builder.SetInsertPoint(bb5);
    Value* val6 = builder.CreateAdd(val3, builder.getInt32(1), "", true, true);
    Value* val7 = builder.CreateICmp(CmpInst::Predicate::ICMP_EQ, val6, builder.getInt32(512));
    builder.CreateCondBr(val7, bb4, bb2);
    builder.SetInsertPoint(bb8);
    PHINode* val9 = builder.CreatePHI(Type::getInt32Ty(context), 2);
    {
      CallInst* call = builder.CreateCall(simPutPixelFunc, {val3, val9, val0});
      call->setTailCall(true);
    }
    Value* val10 = builder.CreateAdd(val9, builder.getInt32(1), "", true, true);
    Value* val11 = builder.CreateICmp(CmpInst::Predicate::ICMP_EQ, val10, builder.getInt32(256));
    builder.CreateCondBr(val11, bb5, bb8);
    
    val3->addIncoming(builder.getInt32(0), bb1);
    val3->addIncoming(val6, bb5);
    val9->addIncoming(builder.getInt32(0), bb2);
    val9->addIncoming(val10, bb8);
  }
  
  // defining adjust_directions function
  {
    BasicBlock* bb6 = BasicBlock::Create(context, "", adjust_directionsFunc);
    BasicBlock* bb13 = BasicBlock::Create(context, "", adjust_directionsFunc);
    BasicBlock* bb16 = BasicBlock::Create(context, "", adjust_directionsFunc);
    BasicBlock* bb21 = BasicBlock::Create(context, "", adjust_directionsFunc);
    BasicBlock* bb24 = BasicBlock::Create(context, "", adjust_directionsFunc);
    
    Value* val0 = adjust_directionsFunc->args().begin() + 0;
    Value* val1 = adjust_directionsFunc->args().begin() + 1;
    Value* val2 = adjust_directionsFunc->args().begin() + 2;
    Value* val3 = adjust_directionsFunc->args().begin() + 3;
    Value* val4 = adjust_directionsFunc->args().begin() + 4;
    Value* val5 = adjust_directionsFunc->args().begin() + 5;
    builder.SetInsertPoint(bb6);
    Value* val7 = builder.CreateShl(val4, builder.getInt32(1), "", false, true);
    Value* val8 = builder.CreateAdd(val7, val5, "");
    Value* val9 = builder.CreateAdd(val8, val0, "");
    Value* val10 = builder.CreateICmp(CmpInst::Predicate::ICMP_SGT, val9, builder.getInt32(511));
    Value* val11 = builder.CreateICmp(CmpInst::Predicate::ICMP_SGT, val5, val0);
    Value* val12 = builder.CreateOr(val11, val10, "");
    builder.CreateCondBr(val12, bb13, bb16);
    builder.SetInsertPoint(bb13);
    Value* val14 = builder.CreateLoad(Type::getInt32Ty(context), val2);
    Value* val15 = builder.CreateSub(builder.getInt32(0), val14, "", false, true);
    builder.CreateStore(val15, val2);
    builder.CreateBr(bb16);
    builder.SetInsertPoint(bb16);
    Value* val17 = builder.CreateICmp(CmpInst::Predicate::ICMP_SGT, val5, val1);
    Value* val18 = builder.CreateAdd(val8, val1, "");
    Value* val19 = builder.CreateICmp(CmpInst::Predicate::ICMP_SGT, val18, builder.getInt32(255));
    Value* val20 = builder.CreateOr(val17, val19, "");
    builder.CreateCondBr(val20, bb21, bb24);
    builder.SetInsertPoint(bb21);
    Value* val22 = builder.CreateLoad(Type::getInt32Ty(context), val3);
    Value* val23 = builder.CreateSub(builder.getInt32(0), val22, "", false, true);
    builder.CreateStore(val23, val3);
    builder.CreateBr(bb24);
    builder.SetInsertPoint(bb24);
    builder.CreateRetVoid();
    
  }
  
  // defining adjust_positions function
  {
    BasicBlock* bb6 = BasicBlock::Create(context, "", adjust_positionsFunc);
    BasicBlock* bb15 = BasicBlock::Create(context, "", adjust_positionsFunc);
    BasicBlock* bb18 = BasicBlock::Create(context, "", adjust_positionsFunc);
    BasicBlock* bb23 = BasicBlock::Create(context, "", adjust_positionsFunc);
    BasicBlock* bb26 = BasicBlock::Create(context, "", adjust_positionsFunc);
    
    Value* val0 = adjust_positionsFunc->args().begin() + 0;
    Value* val1 = adjust_positionsFunc->args().begin() + 1;
    Value* val2 = adjust_positionsFunc->args().begin() + 2;
    Value* val3 = adjust_positionsFunc->args().begin() + 3;
    Value* val4 = adjust_positionsFunc->args().begin() + 4;
    Value* val5 = adjust_positionsFunc->args().begin() + 5;
    builder.SetInsertPoint(bb6);
    Value* val7 = builder.CreateLoad(Type::getInt32Ty(context), val0);
    Value* val8 = builder.CreateLoad(Type::getInt32Ty(context), val1);
    Value* val9 = builder.CreateShl(val4, builder.getInt32(1), "", false, true);
    Value* val10 = builder.CreateAdd(val9, val5, "");
    Value* val11 = builder.CreateAdd(val7, val10, "");
    Value* val12 = builder.CreateICmp(CmpInst::Predicate::ICMP_SGT, val11, builder.getInt32(511));
    Value* val13 = builder.CreateICmp(CmpInst::Predicate::ICMP_SLT, val7, val5);
    Value* val14 = builder.CreateOr(val13, val12, "");
    builder.CreateCondBr(val14, bb15, bb18);
    builder.SetInsertPoint(bb15);
    Value* val16 = builder.CreateLoad(Type::getInt32Ty(context), val2);
    Value* val17 = builder.CreateSub(builder.getInt32(0), val16, "", false, true);
    builder.CreateStore(val17, val2);
    builder.CreateBr(bb18);
    builder.SetInsertPoint(bb18);
    Value* val19 = builder.CreateICmp(CmpInst::Predicate::ICMP_SLT, val8, val5);
    Value* val20 = builder.CreateAdd(val8, val10, "");
    Value* val21 = builder.CreateICmp(CmpInst::Predicate::ICMP_SGT, val20, builder.getInt32(255));
    Value* val22 = builder.CreateOr(val19, val21, "");
    builder.CreateCondBr(val22, bb23, bb26);
    builder.SetInsertPoint(bb23);
    Value* val24 = builder.CreateLoad(Type::getInt32Ty(context), val3);
    Value* val25 = builder.CreateSub(builder.getInt32(0), val24, "", false, true);
    builder.CreateStore(val25, val3);
    builder.CreateBr(bb26);
    builder.SetInsertPoint(bb26);
    Value* val27 = builder.CreateLoad(Type::getInt32Ty(context), val2);
    Value* val28 = builder.CreateMul(val27, val5, "", false, true);
    Value* val29 = builder.CreateLoad(Type::getInt32Ty(context), val0);
    Value* val30 = builder.CreateAdd(val29, val28, "", false, true);
    builder.CreateStore(val30, val0);
    Value* val31 = builder.CreateLoad(Type::getInt32Ty(context), val3);
    Value* val32 = builder.CreateMul(val31, val5, "", false, true);
    Value* val33 = builder.CreateLoad(Type::getInt32Ty(context), val1);
    Value* val34 = builder.CreateAdd(val33, val32, "", false, true);
    builder.CreateStore(val34, val1);
    builder.CreateRetVoid();
    
  }
  
  // defining change_colors function
  {
    BasicBlock* bb3 = BasicBlock::Create(context, "", change_colorsFunc);
    BasicBlock* bb5 = BasicBlock::Create(context, "", change_colorsFunc);
    BasicBlock* bb8 = BasicBlock::Create(context, "", change_colorsFunc);
    
    Value* val0 = change_colorsFunc->args().begin() + 0;
    Value* val1 = change_colorsFunc->args().begin() + 1;
    Value* val2 = change_colorsFunc->args().begin() + 2;
    builder.SetInsertPoint(bb3);
    Value* val4 = builder.CreateLoad(Type::getInt32Ty(context), val0);
    SwitchInst* sw0 = builder.CreateSwitch(val4, bb8, 2);
    builder.SetInsertPoint(bb5);
    Value* val6 = builder.CreateLoad(Type::getInt32Ty(context), val1);
    Value* val7 = builder.CreateSub(builder.getInt32(0), val6, "", false, true);
    builder.CreateStore(val7, val1);
    builder.CreateBr(bb8);
    builder.SetInsertPoint(bb8);
    CallInst* val9 = builder.CreateCall(simRandFunc, {});
    val9->setTailCall(true);
    builder.CreateStore(val9, val2);
    Value* val10 = builder.CreateLoad(Type::getInt32Ty(context), val1);
    Value* val11 = builder.CreateLoad(Type::getInt32Ty(context), val0);
    Value* val12 = builder.CreateAdd(val11, val10, "", false, true);
    builder.CreateStore(val12, val0);
    builder.CreateRetVoid();
    
    sw0->addCase(builder.getInt32(-1), bb5);
    sw0->addCase(builder.getInt32(0), bb5);
  }
  
  // defining reflect_dots function
  {
    BasicBlock* bb5 = BasicBlock::Create(context, "", reflect_dotsFunc);
    
    Value* val0 = reflect_dotsFunc->args().begin() + 0;
    Value* val1 = reflect_dotsFunc->args().begin() + 1;
    Value* val2 = reflect_dotsFunc->args().begin() + 2;
    Value* val3 = reflect_dotsFunc->args().begin() + 3;
    Value* val4 = reflect_dotsFunc->args().begin() + 4;
    builder.SetInsertPoint(bb5);
    Value* val6 = builder.CreateAdd(val2, val0, "", false, true);
    Value* val7 = builder.CreateAdd(val3, val1, "", false, true);
    {
      CallInst* call = builder.CreateCall(simPutPixelFunc, {val6, val7, val4});
      call->setTailCall(true);
    }
    Value* val8 = builder.CreateSub(val0, val2, "", false, true);
    {
      CallInst* call = builder.CreateCall(simPutPixelFunc, {val8, val7, val4});
      call->setTailCall(true);
    }
    Value* val9 = builder.CreateAdd(val3, val0, "", false, true);
    Value* val10 = builder.CreateAdd(val2, val1, "", false, true);
    {
      CallInst* call = builder.CreateCall(simPutPixelFunc, {val9, val10, val4});
      call->setTailCall(true);
    }
    Value* val11 = builder.CreateSub(val0, val3, "", false, true);
    {
      CallInst* call = builder.CreateCall(simPutPixelFunc, {val11, val10, val4});
      call->setTailCall(true);
    }
    builder.CreateRetVoid();
    
  }
  
  // defining draw_half_circle function
  {
    BasicBlock* bb5 = BasicBlock::Create(context, "", draw_half_circleFunc);
    BasicBlock* bb11 = BasicBlock::Create(context, "", draw_half_circleFunc);
    BasicBlock* bb14 = BasicBlock::Create(context, "", draw_half_circleFunc);
    BasicBlock* bb19 = BasicBlock::Create(context, "", draw_half_circleFunc);
    BasicBlock* bb25 = BasicBlock::Create(context, "", draw_half_circleFunc);
    BasicBlock* bb29 = BasicBlock::Create(context, "", draw_half_circleFunc);
    BasicBlock* bb42 = BasicBlock::Create(context, "", draw_half_circleFunc);
    
    Value* val0 = draw_half_circleFunc->args().begin() + 0;
    Value* val1 = draw_half_circleFunc->args().begin() + 1;
    Value* val2 = draw_half_circleFunc->args().begin() + 2;
    Value* val3 = draw_half_circleFunc->args().begin() + 3;
    Value* val4 = draw_half_circleFunc->args().begin() + 4;
    builder.SetInsertPoint(bb5);
    Value* val6 = builder.CreateMul(val3, val2, "", false, true);
    Value* val7 = builder.CreateAdd(val6, val1, "", false, true);
    {
      CallInst* call = builder.CreateCall(simPutPixelFunc, {val0, val7, val4});
      call->setTailCall(true);
    }
    {
      CallInst* call = builder.CreateCall(simPutPixelFunc, {val0, val7, val4});
      call->setTailCall(true);
    }
    Value* val8 = builder.CreateAdd(val6, val0, "", false, true);
    {
      CallInst* call = builder.CreateCall(simPutPixelFunc, {val8, val1, val4});
      call->setTailCall(true);
    }
    Value* val9 = builder.CreateSub(val0, val6, "", false, true);
    {
      CallInst* call = builder.CreateCall(simPutPixelFunc, {val9, val1, val4});
      call->setTailCall(true);
    }
    Value* val10 = builder.CreateICmp(CmpInst::Predicate::ICMP_SLT, val2, builder.getInt32(0));
    builder.CreateCondBr(val10, bb42, bb11);
    builder.SetInsertPoint(bb11);
    Value* val12 = builder.CreateShl(val2, builder.getInt32(1), "", true, true);
    Value* val13 = builder.CreateSub(builder.getInt32(3), val12, "", false, true);
    builder.CreateBr(bb14);
    builder.SetInsertPoint(bb14);
    PHINode* val15 = builder.CreatePHI(Type::getInt32Ty(context), 2);
    PHINode* val16 = builder.CreatePHI(Type::getInt32Ty(context), 2);
    PHINode* val17 = builder.CreatePHI(Type::getInt32Ty(context), 2);
    Value* val18 = builder.CreateICmp(CmpInst::Predicate::ICMP_SGT, val15, builder.getInt32(0));
    builder.CreateCondBr(val18, bb19, bb25);
    builder.SetInsertPoint(bb19);
    Value* val20 = builder.CreateAdd(val16, builder.getInt32(-1), "", false, true);
    Value* val21 = builder.CreateSub(val17, val20, "", false, true);
    Value* val22 = builder.CreateShl(val21, builder.getInt32(2), "", false, true);
    Value* val23 = builder.CreateAdd(val15, builder.getInt32(10), "", true, false);
    Value* val24 = builder.CreateAdd(val23, val22, "");
    builder.CreateBr(bb29);
    builder.SetInsertPoint(bb25);
    Value* val26 = builder.CreateShl(val17, builder.getInt32(2), "", false, true);
    Value* val27 = builder.CreateAdd(val26, builder.getInt32(6), "", true, false);
    Value* val28 = builder.CreateAdd(val27, val15, "");
    builder.CreateBr(bb29);
    builder.SetInsertPoint(bb29);
    PHINode* val30 = builder.CreatePHI(Type::getInt32Ty(context), 2);
    PHINode* val31 = builder.CreatePHI(Type::getInt32Ty(context), 2);
    Value* val32 = builder.CreateAdd(val17, builder.getInt32(1), "", true, true);
    Value* val33 = builder.CreateMul(val32, val3, "", false, true);
    Value* val34 = builder.CreateMul(val30, val3, "", false, true);
    Value* val35 = builder.CreateAdd(val33, val0, "", false, true);
    Value* val36 = builder.CreateAdd(val34, val1, "", false, true);
    {
      CallInst* call = builder.CreateCall(simPutPixelFunc, {val35, val36, val4});
      call->setTailCall(true);
    }
    Value* val37 = builder.CreateSub(val0, val33, "", false, true);
    {
      CallInst* call = builder.CreateCall(simPutPixelFunc, {val37, val36, val4});
      call->setTailCall(true);
    }
    Value* val38 = builder.CreateAdd(val34, val0, "", false, true);
    Value* val39 = builder.CreateAdd(val33, val1, "", false, true);
    {
      CallInst* call = builder.CreateCall(simPutPixelFunc, {val38, val39, val4});
      call->setTailCall(true);
    }
    Value* val40 = builder.CreateSub(val0, val34, "", false, true);
    {
      CallInst* call = builder.CreateCall(simPutPixelFunc, {val40, val39, val4});
      call->setTailCall(true);
    }
    Value* val41 = builder.CreateICmp(CmpInst::Predicate::ICMP_SGT, val30, val17);
    builder.CreateCondBr(val41, bb14, bb42);
    builder.SetInsertPoint(bb42);
    builder.CreateRetVoid();
    
    val15->addIncoming(val31, bb29);
    val15->addIncoming(val13, bb11);
    val16->addIncoming(val30, bb29);
    val16->addIncoming(val2, bb11);
    val17->addIncoming(val32, bb29);
    val17->addIncoming(builder.getInt32(0), bb11);
    val30->addIncoming(val20, bb19);
    val30->addIncoming(val16, bb25);
    val31->addIncoming(val24, bb19);
    val31->addIncoming(val28, bb25);
  }
  
  // defining draw_circle function
  {
    BasicBlock* bb4 = BasicBlock::Create(context, "", draw_circleFunc);
    
    Value* val0 = draw_circleFunc->args().begin() + 0;
    Value* val1 = draw_circleFunc->args().begin() + 1;
    Value* val2 = draw_circleFunc->args().begin() + 2;
    Value* val3 = draw_circleFunc->args().begin() + 3;
    builder.SetInsertPoint(bb4);
    {
      CallInst* call = builder.CreateCall(draw_half_circleFunc, {val0, val1, val2, builder.getInt32(1), val3});
      call->setTailCall(true);
    }
    {
      CallInst* call = builder.CreateCall(draw_half_circleFunc, {val0, val1, val2, builder.getInt32(-1), val3});
      call->setTailCall(true);
    }
    builder.CreateRetVoid();
    
  }
  
  // defining app function
  {
    BasicBlock* bb0 = BasicBlock::Create(context, "", appFunc);
    BasicBlock* bb14 = BasicBlock::Create(context, "", appFunc);
    BasicBlock* bb22 = BasicBlock::Create(context, "", appFunc);
    BasicBlock* bb24 = BasicBlock::Create(context, "", appFunc);
    BasicBlock* bb27 = BasicBlock::Create(context, "", appFunc);
    BasicBlock* bb31 = BasicBlock::Create(context, "", appFunc);
    BasicBlock* bb54 = BasicBlock::Create(context, "", appFunc);
    BasicBlock* bb56 = BasicBlock::Create(context, "", appFunc);
    
    builder.SetInsertPoint(bb0);
    CallInst* val1 = builder.CreateCall(simRandFunc, {});
    val1->setTailCall(true);
    Value* val2 = builder.CreateSRem(val1, builder.getInt32(449), "");
    CallInst* val3 = builder.CreateCall(simRandFunc, {});
    val3->setTailCall(true);
    Value* val4 = builder.CreateSRem(val3, builder.getInt32(193), "");
    CallInst* val5 = builder.CreateCall(simRandFunc, {});
    val5->setTailCall(true);
    Value* val6 = builder.CreateAnd(val5, builder.getInt32(1), "");
    Value* val7 = builder.CreateICmp(CmpInst::Predicate::ICMP_EQ, val6, builder.getInt32(0));
    Value* val8 = builder.CreateSelect(val7, builder.getInt32(-1), builder.getInt32(1));
    CallInst* val9 = builder.CreateCall(simRandFunc, {});
    val9->setTailCall(true);
    Value* val10 = builder.CreateAnd(val9, builder.getInt32(1), "");
    Value* val11 = builder.CreateICmp(CmpInst::Predicate::ICMP_EQ, val10, builder.getInt32(0));
    Value* val12 = builder.CreateSelect(val11, builder.getInt32(-1), builder.getInt32(1));
    CallInst* val13 = builder.CreateCall(simRandFunc, {});
    val13->setTailCall(true);
    builder.CreateBr(bb14);
    builder.SetInsertPoint(bb14);
    PHINode* val15 = builder.CreatePHI(Type::getInt32Ty(context), 2);
    PHINode* val16 = builder.CreatePHI(Type::getInt32Ty(context), 2);
    PHINode* val17 = builder.CreatePHI(Type::getInt32Ty(context), 2);
    PHINode* val18 = builder.CreatePHI(Type::getInt32Ty(context), 2);
    PHINode* val19 = builder.CreatePHI(Type::getInt32Ty(context), 2);
    PHINode* val20 = builder.CreatePHI(Type::getInt32Ty(context), 2);
    PHINode* val21 = builder.CreatePHI(Type::getInt32Ty(context), 2);
    builder.CreateBr(bb22);
    builder.SetInsertPoint(bb22);
    PHINode* val23 = builder.CreatePHI(Type::getInt32Ty(context), 2);
    builder.CreateBr(bb27);
    builder.SetInsertPoint(bb24);
    Value* val25 = builder.CreateAdd(val23, builder.getInt32(1), "", true, true);
    Value* val26 = builder.CreateICmp(CmpInst::Predicate::ICMP_EQ, val25, builder.getInt32(512));
    builder.CreateCondBr(val26, bb31, bb22);
    builder.SetInsertPoint(bb27);
    PHINode* val28 = builder.CreatePHI(Type::getInt32Ty(context), 2);
    {
      CallInst* call = builder.CreateCall(simPutPixelFunc, {val23, val28, val20});
      call->setTailCall(true);
    }
    Value* val29 = builder.CreateAdd(val28, builder.getInt32(1), "", true, true);
    Value* val30 = builder.CreateICmp(CmpInst::Predicate::ICMP_EQ, val29, builder.getInt32(256));
    builder.CreateCondBr(val30, bb24, bb27);
    builder.SetInsertPoint(bb31);
    Value* val32 = builder.CreateAdd(val15, builder.getInt32(30), "", false, true);
    Value* val33 = builder.CreateAdd(val16, builder.getInt32(30), "", false, true);
    {
      CallInst* call = builder.CreateCall(draw_half_circleFunc, {val32, val33, builder.getInt32(30), builder.getInt32(1), val19});
      call->setTailCall(true);
    }
    {
      CallInst* call = builder.CreateCall(draw_half_circleFunc, {val32, val33, builder.getInt32(30), builder.getInt32(-1), val19});
      call->setTailCall(true);
    }
    Value* val34 = builder.CreateAdd(val15, builder.getInt32(20), "", false, true);
    Value* val35 = builder.CreateAdd(val16, builder.getInt32(20), "", false, true);
    {
      CallInst* call = builder.CreateCall(draw_half_circleFunc, {val34, val35, builder.getInt32(6), builder.getInt32(1), val19});
      call->setTailCall(true);
    }
    {
      CallInst* call = builder.CreateCall(draw_half_circleFunc, {val34, val35, builder.getInt32(6), builder.getInt32(-1), val19});
      call->setTailCall(true);
    }
    Value* val36 = builder.CreateAdd(val15, builder.getInt32(40), "", false, true);
    {
      CallInst* call = builder.CreateCall(draw_half_circleFunc, {val36, val35, builder.getInt32(6), builder.getInt32(1), val19});
      call->setTailCall(true);
    }
    {
      CallInst* call = builder.CreateCall(draw_half_circleFunc, {val36, val35, builder.getInt32(6), builder.getInt32(-1), val19});
      call->setTailCall(true);
    }
    Value* val37 = builder.CreateAdd(val16, builder.getInt32(33), "", false, true);
    {
      CallInst* call = builder.CreateCall(draw_half_circleFunc, {val32, val37, builder.getInt32(15), builder.getInt32(1), val19});
      call->setTailCall(true);
    }
    Value* val38 = builder.CreateAdd(val15, builder.getInt32(63), "");
    Value* val39 = builder.CreateICmp(CmpInst::Predicate::ICMP_SGT, val38, builder.getInt32(511));
    Value* val40 = builder.CreateICmp(CmpInst::Predicate::ICMP_SLT, val15, builder.getInt32(3));
    Value* val41 = builder.CreateOr(val40, val39, "");
    Value* val42 = builder.CreateSub(builder.getInt32(0), val17, "", false, true);
    Value* val43 = builder.CreateSelect(val41, val42, val17);
    Value* val44 = builder.CreateICmp(CmpInst::Predicate::ICMP_SLT, val16, builder.getInt32(3));
    Value* val45 = builder.CreateAdd(val16, builder.getInt32(63), "");
    Value* val46 = builder.CreateICmp(CmpInst::Predicate::ICMP_SGT, val45, builder.getInt32(255));
    Value* val47 = builder.CreateOr(val44, val46, "");
    Value* val48 = builder.CreateSub(builder.getInt32(0), val18, "", false, true);
    Value* val49 = builder.CreateSelect(val47, val48, val18);
    Value* val50 = builder.CreateMul(val43, builder.getInt32(3), "", false, true);
    Value* val51 = builder.CreateAdd(val50, val15, "", false, true);
    Value* val52 = builder.CreateMul(val49, builder.getInt32(3), "", false, true);
    Value* val53 = builder.CreateAdd(val52, val16, "", false, true);
    SwitchInst* sw0 = builder.CreateSwitch(val20, bb56, 2);
    builder.SetInsertPoint(bb54);
    Value* val55 = builder.CreateSub(builder.getInt32(0), val21, "", false, true);
    builder.CreateBr(bb56);
    builder.SetInsertPoint(bb56);
    PHINode* val57 = builder.CreatePHI(Type::getInt32Ty(context), 2);
    CallInst* val58 = builder.CreateCall(simRandFunc, {});
    val58->setTailCall(true);
    Value* val59 = builder.CreateAdd(val57, val20, "", false, true);
    {
      CallInst* call = builder.CreateCall(simFlushFunc, {});
      call->setTailCall(true);
    }
    builder.CreateBr(bb14);
    
    val15->addIncoming(val2, bb0);
    val15->addIncoming(val51, bb56);
    val16->addIncoming(val4, bb0);
    val16->addIncoming(val53, bb56);
    val17->addIncoming(val8, bb0);
    val17->addIncoming(val43, bb56);
    val18->addIncoming(val12, bb0);
    val18->addIncoming(val49, bb56);
    val19->addIncoming(val13, bb0);
    val19->addIncoming(val58, bb56);
    val20->addIncoming(builder.getInt32(0), bb0);
    val20->addIncoming(val59, bb56);
    val21->addIncoming(builder.getInt32(-16843009), bb0);
    val21->addIncoming(val57, bb56);
    val23->addIncoming(builder.getInt32(0), bb14);
    val23->addIncoming(val25, bb24);
    val28->addIncoming(builder.getInt32(0), bb22);
    val28->addIncoming(val29, bb27);
    val57->addIncoming(val21, bb31);
    val57->addIncoming(val55, bb54);
    sw0->addCase(builder.getInt32(-1), bb54);
    sw0->addCase(builder.getInt32(0), bb54);
  }
  
  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();
  
  std::error_code ec;
  raw_fd_stream debugOut("generated.ll", ec);
  module->print(debugOut, nullptr);
  debugOut.close();
  ExecutionEngine* ee = EngineBuilder(std::move(module)).create();
  ee->InstallLazyFunctionCreator([=](const std::string& fn) -> void* {
   if (fn == "simPutPixel") {
     return reinterpret_cast<void*>(simPutPixel);
   }
   if (fn == "simFlush") {
     return reinterpret_cast<void*>(simFlush);
   }
   if (fn == "simRand") {
     return reinterpret_cast<void*>(simRand);
   }
   return nullptr;
  });
  ee->finalizeObject();
  
  simInit();
  GenericValue res = ee->runFunction(appFunc, {});
  simExit();
  return 0;
}
