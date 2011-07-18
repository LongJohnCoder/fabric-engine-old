/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_GLOBAL_VECTOR_H
#define _FABRIC_AST_GLOBAL_VECTOR_H

#include <Fabric/Base/RC/Vector.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace JSON
  {
    class Array;
  };
  
  namespace CG
  {
    class ModuleBuilder;
    class Diagnostics;
  };
  
  namespace AST
  {
    class Global;
    
    class GlobalVector : public RC::Vector< RC::ConstHandle<Global> >
    {
    public:
      
      static RC::Handle<GlobalVector> Create();

      RC::Handle<JSON::Array> toJSON() const;
          
      void llvmCompileToModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const;
    
    protected:
    
      GlobalVector();
    };
  };
};

#endif //_FABRIC_AST_GLOBAL_VECTOR_H
