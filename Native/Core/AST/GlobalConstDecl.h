/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_GLOBAL_CONST_DECL_H
#define _FABRIC_AST_GLOBAL_CONST_DECL_H

#include <Fabric/Core/AST/Global.h>

namespace Fabric
{
  namespace AST
  {
    class ConstDecl;
    
    class GlobalConstDecl : public Global
    {
    public:

      static RC::Handle<GlobalConstDecl> Create(
        CG::Location const &location,
        RC::ConstHandle<ConstDecl> const &constDecl
        )
      {
        return new GlobalConstDecl( location, constDecl );
      }
    
      virtual std::string localDesc() const;
      virtual std::string deepDesc( std::string const &indent ) const;
      
      virtual void llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics, bool buildFunctionBodies ) const;
      
    protected:
    
      GlobalConstDecl(
        CG::Location const &location,
        RC::ConstHandle<ConstDecl> const &constDecl
        );
    
    private:
    
      RC::ConstHandle<ConstDecl> m_constDecl;
    };
  };
};

#endif //_FABRIC_AST_GLOBAL_CONST_DECL_H
