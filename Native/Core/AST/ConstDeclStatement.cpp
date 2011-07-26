/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "ConstDeclStatement.h"
#include "ConstDecl.h"
#include <Fabric/Core/CG/BasicBlockBuilder.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/Manager.h>
#include <Fabric/Base/JSON/String.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( ConstDeclStatement );
    
    RC::ConstHandle<ConstDeclStatement> ConstDeclStatement::Create(
      CG::Location const &location,
      RC::ConstHandle<ConstDecl> const &constDecl
      )
    {
      return new ConstDeclStatement( location, constDecl );
    }

    ConstDeclStatement::ConstDeclStatement(
      CG::Location const &location,
      RC::ConstHandle<ConstDecl> const &constDecl
      )
      : Statement( location )
      , m_constDecl( constDecl )
    {
    }
    
    RC::Handle<JSON::Object> ConstDeclStatement::toJSONImpl() const
    {
      RC::Handle<JSON::Object> result = Statement::toJSONImpl();
      result->set( "constDecl", m_constDecl->toJSONImpl() );
      return result;
    }
    
    void ConstDeclStatement::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      m_constDecl->llvmPrepareModule( moduleBuilder, diagnostics );
    }
    
    void ConstDeclStatement::llvmCompileToBuilder( CG::BasicBlockBuilder &basicBlockBuilder, CG::Diagnostics &diagnostics ) const
    {
      try
      {
        m_constDecl->llvmCompileToScope( basicBlockBuilder.getScope(), basicBlockBuilder.getManager() );
      }
      catch ( CG::Error e )
      {
        addError( diagnostics, e );
      }
    }
  };
};
