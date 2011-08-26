/*
 *
 *  Created by Peter Zion on 10-12-02.
 *  Copyright 2010 Fabric Technologies Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_AST_AND_OP_H
#define _FABRIC_AST_AND_OP_H

#include <Fabric/Core/AST/Expr.h>

namespace Fabric
{
  namespace CG
  {
    class ModuleBuilder;
  };
  
  namespace AST
  {
    class AndOp : public Expr
    {
      FABRIC_AST_NODE_DECL( AndOp );
      
    public:
        
      static RC::ConstHandle<AndOp> Create( CG::Location const &location, RC::ConstHandle<Expr> const &left, RC::ConstHandle<Expr> const &right );
      
      virtual void llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctions ) const;
      
      virtual RC::ConstHandle<CG::Adapter> getType( CG::BasicBlockBuilder const &basicBlockBuilder ) const;
      virtual CG::ExprValue buildExprValue( CG::BasicBlockBuilder &basicBlockBuilder, CG::Usage usage, std::string const &lValueErrorDesc ) const;
      
    protected:
    
      AndOp( CG::Location const &location, RC::ConstHandle<Expr> const &left, RC::ConstHandle<Expr> const &right );
      
      virtual void appendJSONMembers( Util::JSONObjectGenerator const &jsonObjectGenerator ) const;

    private:
    
      RC::ConstHandle<Expr> m_left;
      RC::ConstHandle<Expr> m_right;
    };
  };
};

#endif //_FABRIC_AST_AND_OP_H
