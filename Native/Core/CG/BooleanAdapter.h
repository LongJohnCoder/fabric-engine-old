/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_CG_BOOLEAN_ADAPTER_H
#define _FABRIC_CG_BOOLEAN_ADAPTER_H

#include <Fabric/Core/CG/SimpleAdapter.h>

namespace llvm
{
  class Constant;
}

namespace Fabric
{
  namespace RT
  {
    class BooleanDesc;
  };
  
  namespace CG
  {
    class Manager;
    
    class BooleanAdapter : public SimpleAdapter
    {
      friend class Manager;
      
    public:
      
      virtual std::string toString( void const *data ) const;
    
      llvm::Constant *llvmConst( bool value ) const;

    protected:

      BooleanAdapter( RC::ConstHandle<Manager> const &manager, RC::ConstHandle<RT::BooleanDesc> const &booleanDesc );
     
      virtual void llvmPrepareModule( ModuleBuilder &moduleBuilder, bool buildFunctions ) const;
      
    private:
    
      RC::ConstHandle<RT::BooleanDesc> m_booleanDesc;
    };
  };
};

#endif //_FABRIC_CG_BOOLEAN_ADAPTER_H
