/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#include <Fabric/Core/AST/BinOpImpl.h>
#include <Fabric/Core/AST/Param.h>
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/OverloadNames.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( BinOpImpl );
    
    RC::ConstHandle<BinOpImpl> BinOpImpl::Create(
      CG::Location const &location,
      std::string const &returnType,
      CG::BinOpType binOpType,
      RC::ConstHandle<AST::Param> const &lhs,
      RC::ConstHandle<AST::Param> const &rhs,
      RC::ConstHandle<CompoundStatement> const &body
      )
    {
      return new BinOpImpl( location, returnType, binOpType, lhs, rhs, body );
    }
        
    BinOpImpl::BinOpImpl(
      CG::Location const &location,
      std::string const &returnType,
      CG::BinOpType binOpType,
      RC::ConstHandle<AST::Param> const &lhs,
      RC::ConstHandle<AST::Param> const &rhs,
      RC::ConstHandle<CompoundStatement> const &body
      )
      : FunctionBase(
        location,
        returnType,
        body
        )
      , m_binOpType( binOpType )
      , m_params( ParamVector::Create( lhs, rhs ) )
    {
    }
    
    RC::Handle<JSON::Object> BinOpImpl::toJSON() const
    {
      RC::Handle<JSON::Object> result = FunctionBase::toJSON();
      result->set( "op", JSON::String::Create( CG::binOpUserName( m_binOpType ) ) );
      return result;
    }
    
    std::string BinOpImpl::getEntryName( RC::Handle<CG::Manager> const &cgManager ) const
    {
      RC::ConstHandle<ParamVector> params = getParams( cgManager );
      return CG::binOpOverloadName( m_binOpType, params->get(0)->getAdapter( cgManager ), params->get(1)->getAdapter( cgManager ) );
    }

    RC::ConstHandle<ParamVector> BinOpImpl::getParams( RC::Handle<CG::Manager> const &cgManager ) const
    {
      return m_params;
    }
  };
};
