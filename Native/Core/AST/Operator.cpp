/*
 *  Copyright 2010-2012 Fabric Engine Inc. All rights reserved.
 */

#include "Operator.h"

#include <Fabric/Core/CG/BasicBlockBuilder.h>
#include <Fabric/Core/CG/FunctionBuilder.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/CG/SizeAdapter.h>
#include <Fabric/Core/CG/Manager.h>

#include <llvm/Module.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( Operator );
    
    RC::ConstHandle<Function> Operator::Create(
      CG::Location const &location,
      std::string const &functionName,
      RC::ConstHandle<ParamVector> const &params,
      std::string const *symbolName,
      RC::ConstHandle<CompoundStatement> const &body
      )
    {
      return new Operator( location, functionName, params, symbolName, body );
    }
  
    Operator::Operator( 
      CG::Location const &location,
      std::string const &functionName,
      RC::ConstHandle<ParamVector> const &params,
      std::string const *symbolName,
      RC::ConstHandle<CompoundStatement> const &body
      )
      : Function( location, "", functionName, params, symbolName, body, true )
    {
    }

    void Operator::llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const
    {
      Function::llvmCompileToModule( moduleBuilder, diagnostics, buildFunctionBodies );

      RC::Handle<CG::Context> context = moduleBuilder.getContext();
      RC::Handle<CG::Manager> cgManager = moduleBuilder.getManager();

      size_t numArgs = getParams( cgManager )->size();
      std::vector<llvm::Type const *> argTypes;
      llvm::Type const *int64Type = llvm::Type::getInt64Ty( context->getLLVMContext() );
      llvm::Type const *ptrType = llvm::Type::getInt8PtrTy( context->getLLVMContext() );
      llvm::Type const *ptrArrayType = llvm::ArrayType::get( ptrType, numArgs );
      llvm::Type const *int64ArrayType = llvm::ArrayType::get( int64Type, numArgs );
      argTypes.push_back( int64Type ); // start
      argTypes.push_back( int64Type ); // count
      argTypes.push_back( ptrArrayType ); // bases
      argTypes.push_back( int64ArrayType ); // strides

      llvm::FunctionType const *funcType = llvm::FunctionType::get( llvm::Type::getVoidTy( context->getLLVMContext() ), argTypes, false );

      std::string name = getSymbolName( cgManager ) + ".stub";
      llvm::Function *func = llvm::cast<llvm::Function>( moduleBuilder->getFunction( name ) );

      if ( !func )
      {
        llvm::AttributeWithIndex AWI[1];
        AWI[0] = llvm::AttributeWithIndex::get( ~0u, llvm::Attribute::InlineHint | llvm::Attribute::NoUnwind );
        llvm::AttrListPtr attrListPtr = llvm::AttrListPtr::get( AWI, 1 );

        func = llvm::cast<llvm::Function>( moduleBuilder->getOrInsertFunction( name, funcType, attrListPtr ) );
        //func->setLinkage( llvm::GlobalValue::PrivateLinkage );

        CG::FunctionBuilder functionBuilder( moduleBuilder, funcType, func );
        llvm::Argument *start = functionBuilder[0];
        start->setName( "start" );
        //arg1->addAttr( llvm::Attribute::NoCapture );
        llvm::Argument *count = functionBuilder[1];
        count->setName( "count" );
        //arg2->addAttr( llvm::Attribute::NoCapture );
        llvm::Argument *bases = functionBuilder[2];
        bases->setName( "bases" );
        llvm::Argument *strides = functionBuilder[3];
        strides->setName( "strides" );

        CG::BasicBlockBuilder bbb( functionBuilder );

        llvm::BasicBlock *entryBB = functionBuilder.createBasicBlock( "entry" );
        llvm::BasicBlock *loopCmpBB = functionBuilder.createBasicBlock( "loopCmp" );
        llvm::BasicBlock *loopBodyBB = functionBuilder.createBasicBlock( "loopBody" );
        llvm::BasicBlock *argsLoopCmpBB = functionBuilder.createBasicBlock( "argsLoopCmp" );
        llvm::BasicBlock *argsLoopBodyBB = functionBuilder.createBasicBlock( "argsLoopBody" );
        llvm::BasicBlock *argsDoneBB = functionBuilder.createBasicBlock( "argsDone" );
        llvm::BasicBlock *doneBB = functionBuilder.createBasicBlock( "done" );

        bbb->SetInsertPoint( entryBB );
        RC::ConstHandle<CG::SizeAdapter> sizeAdapter = cgManager->getSizeAdapter();
        llvm::Value *zeroRValue = sizeAdapter->llvmConst( context, 0 );
        llvm::Value *oneRValue = sizeAdapter->llvmConst( context, 1 );
        llvm::Value *numArgsRValue = sizeAdapter->llvmConst( context, numArgs );

        llvm::Value *startRValue = llvm::cast<llvm::Value>( start );
        llvm::Value *countRValue = llvm::cast<llvm::Value>( count );

        llvm::Value *i = sizeAdapter->llvmAlloca( bbb, "i" );
        sizeAdapter->llvmAssign( bbb, i, startRValue );

        bbb->SetInsertPoint( loopCmpBB );
        bbb->CreateCondBr( bbb->CreateICmpULT( sizeAdapter->llvmLValueToRValue( bbb, i ), countRValue ), loopBodyBB, doneBB );

        bbb->SetInsertPoint( loopBodyBB );
        llvm::Value *nexti = bbb->CreateAdd( i, oneRValue, "nexti" );
        sizeAdapter->llvmAssign( bbb, i, sizeAdapter->llvmLValueToRValue( bbb, nexti ) );

        llvm::Value *j = sizeAdapter->llvmAlloca( bbb, "j" );
        sizeAdapter->llvmAssign( bbb, j, zeroRValue );
        bbb->CreateBr( argsLoopCmpBB );

        bbb->SetInsertPoint( argsLoopCmpBB );
        bbb->CreateCondBr( bbb->CreateICmpULT( sizeAdapter->llvmLValueToRValue( bbb, j ), numArgsRValue ), argsLoopBodyBB, argsDoneBB );

        bbb->SetInsertPoint( argsLoopBodyBB );

        // XXX need cleanup
        llvm::Value *nextj = bbb->CreateAdd( j, oneRValue, "nextj" );
        sizeAdapter->llvmAssign( bbb, j, sizeAdapter->llvmLValueToRValue( bbb, nextj ) );
        llvm::Function *realOp = llvm::cast<llvm::Function>( moduleBuilder->getFunction( getSymbolName( cgManager ) ) );
        //bbb->CreateCall( realOp, NULL );

        bbb->CreateBr( argsLoopCmpBB );

        bbb->SetInsertPoint( argsDoneBB );
        bbb->CreateBr( loopCmpBB );

        bbb->SetInsertPoint( doneBB );
        bbb->CreateRetVoid();
      }
    }
  }
}
