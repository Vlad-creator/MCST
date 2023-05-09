#pragma once

#include <map>
#include <string>
#include <iostream>

#include "llvm/ADT/APInt.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

#include "types.hpp"



struct CodeGen final
{
	llvm::LLVMContext context_;
	llvm::IRBuilder<>* builder_;
	llvm::Module* module_;
	std::map<std::string, llvm::Value*> values;

	llvm::Function* Function_ = nullptr;

	CodeGen(std::string name): builder_(new llvm::IRBuilder(context_)),
	                           module_(new llvm::Module(name, context_)) {};

	void SaveModule(std::string ModuleName);

	void StartFunction(std::string Name);

	void EndCurrentFunction() 
	{ 
		builder_->CreateRetVoid(); 
	}

	llvm::Value* AddOperation(ast::op_type op, llvm::Value* LeftV, llvm::Value* RightV);

	llvm::Value* AddUnOperation(ast::unop_type ty, llvm::Value* LeftV, llvm::Value* RightV);

	llvm::Value* AddDeclRead(std::string VarName)
	{
		auto* Var = values[VarName];
		auto* type = Var->getType()->getPointerElementType();
		return builder_->CreateLoad(type, Var, VarName.c_str());
	}

	llvm::Value* AddDeclWrite(std::string VarName, llvm::Value* rhs)
	{
		auto* Var = values[VarName];
		return builder_->CreateStore(rhs, Var);
	}

	void AddDecl(std::string VarName)
	{
		auto &&BB = Function_->getEntryBlock();
    	auto Ty = llvm::Type::getInt32Ty(context_);
    	llvm::IRBuilder<> TmpB(&BB, BB.begin());
    	auto *Alloca = TmpB.CreateAlloca(Ty, 0, VarName.c_str());
    	values[VarName] = Alloca;
	}

	llvm::BasicBlock* StartIf(llvm::Value* cond)
	{
		llvm::BasicBlock* ThenScope = llvm::BasicBlock::Create(context_, "then");
		llvm::BasicBlock* MergeScope = llvm::BasicBlock::Create(context_, "endif");

		builder_->CreateCondBr(cond, ThenScope, MergeScope);
		Function_->getBasicBlockList().push_back(ThenScope);
		builder_->SetInsertPoint(ThenScope);
		return MergeScope;
	}

	void EndIf(llvm::BasicBlock* MergeScope)
	{
		Function_->getBasicBlockList().push_back(MergeScope);
		builder_->CreateBr(MergeScope);
		builder_->SetInsertPoint(MergeScope);
	}

	using WhileType = std::pair<llvm::BasicBlock*, llvm::BasicBlock*>;

	WhileType StartWhile(llvm::Value* cond)
	{
		llvm::BasicBlock* WhileScope = llvm::BasicBlock::Create(context_, "while");
		llvm::BasicBlock* BodyScope = llvm::BasicBlock::Create(context_, "body");

		builder_->CreateCondBr(cond, BodyScope, WhileScope);
		Function_->getBasicBlockList().push_back(BodyScope);
		builder_->SetInsertPoint(BodyScope);
		return std::make_pair(BodyScope, WhileScope);
	}

	void EndWhile(llvm::Value* cond, WhileType WhileTy)
	{
		Function_->getBasicBlockList().push_back(WhileTy.second);
		builder_->CreateCondBr(cond, WhileTy.first, WhileTy.second);
		builder_->SetInsertPoint(WhileTy.second);
	}

	llvm::Type* getInt32Ty() 
	{ 
		return llvm::Type::getInt32Ty(context_); 
	}

    llvm::Type* getVoidTy() 
    { 
    	return llvm::Type::getVoidTy(context_); 
    }

    void createFnDecl(llvm::FunctionType *FT, std::string Name) 
    {
    	llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, module_);
    }

    ~CodeGen() 
    {
    	delete builder_;
    	delete module_;
    }

};

CodeGen *createCodeGen(std::string Name);

extern std::unique_ptr<CodeGen> GlobalGen;