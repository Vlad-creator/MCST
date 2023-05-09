#include "../include/CodeGen.hpp"
#include "../include/node.hpp"
#include <iostream>
#include <stdexcept>
#include <string>


void CodeGen::SaveModule(std::string ModuleName)
{
	std::error_code EC;
    llvm::raw_fd_ostream Outfile{ModuleName, EC};

    module_->print(Outfile, nullptr);
    Outfile.close();

    if (Outfile.has_error()) 
    {
        std::cerr << Outfile.error().message() << std::endl;
        throw std::runtime_error("Error printing to file: ");
    }
}

void CodeGen::StartFunction(std::string Name)
{
	llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getVoidTy(context_), false);
    Function_ = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, Name, module_);
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(context_, "entry", Function_);
    builder_->SetInsertPoint(BB);
}

llvm::Value* CodeGen::AddOperation(ast::op_type op, llvm::Value* LeftV, llvm::Value* RightV)
{
	switch (op) 
	{
    case ast::op_type::PLUS:
        return builder_->CreateAdd(LeftV, RightV);
    case ast::op_type::MINUS:
        return builder_->CreateSub(LeftV, RightV);
    case ast::op_type::GR:
        return builder_->CreateICmpSGT(LeftV, RightV);
    case ast::op_type::LS:
        return builder_->CreateICmpSLT(LeftV, RightV);
    case ast::op_type::GREQ:
        return builder_->CreateICmpSGE(LeftV, RightV);
    case ast::op_type::LSEQ:
        return builder_->CreateICmpSLE(LeftV, RightV);
    case ast::op_type::EQUAL:
        return builder_->CreateICmpEQ(LeftV, RightV);
    case ast::op_type::NEQUAL:
        return builder_->CreateICmpNE(LeftV, RightV);
    case ast::op_type::LAND:
        return builder_->CreateAnd(LeftV, RightV);
    case ast::op_type::LOR:
    	return builder_->CreateOr(LeftV, RightV);
    case ast::op_type::DIV:
    	return builder_->CreateSDiv(LeftV, RightV);
    case ast::op_type::MUL:
    	return builder_->CreateMul(LeftV, RightV);
    case ast::op_type::MOD:
    	return builder_->CreateSRem(LeftV, RightV);
    default:
    	break;
    }
    return nullptr;
}

llvm::Value* CodeGen::CodeGen::AddUnOperation(ast::unop_type UnOp, llvm::Value* LeftV, llvm::Value* RightV)
{
	LeftV = llvm::ConstantInt::get(getInt32Ty(), 0);
	switch (UnOp)
	{
	case ast::unop_type::UnPLUS:
    	return builder_->CreateAdd(LeftV, RightV);
    case ast::unop_type::UnMINUS:
    	return builder_->CreateSub(LeftV, RightV);
    case ast::unop_type::OUTPUT: 
    {
    	auto *CalleeF = module_->getFunction("__pcl_print");
	    assert(CalleeF && "Driver shall create decl for __pcl_print");
	    assert(RightV && "Print required non-null right arg");
	    llvm::Value *ArgsV[] = {RightV};
	    return builder_->CreateCall(CalleeF, ArgsV);
    }
	case ast::unop_type::INPUT: 
	{
	    auto *CalleeF = module_->getFunction("__pcl_scan");
	    assert(CalleeF && "Driver shall create decl for __pcl_scan");
	    return builder_->CreateCall(CalleeF);
    }
    case ast::unop_type::NOT:
    	return builder_->CreateNot(RightV);

	}
	return nullptr;
};

CodeGen* createCodeGen(std::string Name) 
{
  CodeGen *Gen = new CodeGen(Name);

  // prototype for print and scan functions
  llvm::Type *Tys[] = {Gen->getInt32Ty()};
  llvm::FunctionType *FTPrint =
      llvm::FunctionType::get(Gen->getVoidTy(), Tys, /* va args? */ false);
  llvm::FunctionType *FTScan =
      llvm::FunctionType::get(Gen->getInt32Ty(), /* va args? */ false);

  // creating decls for modules
  Gen->createFnDecl(FTPrint, "__pcl_print");
  Gen->createFnDecl(FTScan, "__pcl_scan");
  return Gen;
}