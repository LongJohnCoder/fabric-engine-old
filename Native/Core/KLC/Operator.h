/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_KL_OPERATOR_H
#define _FABRIC_KL_OPERATOR_H

#include <Fabric/Core/GC/Object.h>
#include <Fabric/Core/KLC/GenericFunctionPtr.h>
#include <Fabric/Base/RC/ConstHandle.h>

namespace Fabric
{
  namespace CG
  {
    class Diagnostics;
  };
  
  namespace KLC
  {
    class Executable;
    
    class Operator : public GC::Object
    {
      FABRIC_GC_OBJECT_CLASS_DECL()
      
    public:
    
      static RC::Handle<Operator> Create(
        RC::ConstHandle<Executable> const &executable,
        GenericFunctionPtr functionPtr
        );
    
      GenericFunctionPtr getGenericFunctionPtr() const;
      CG::Diagnostics const &getDiagnostics() const;
        
      virtual void jsonExec(
        std::string const &cmd,
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
      
    protected:
    
      Operator(
        FABRIC_GC_OBJECT_CLASS_PARAM,
        RC::ConstHandle<Executable> const &executable,
        GenericFunctionPtr functionPtr
        );
    
    private:
    
      void jsonExecGetDiagnostics(
        RC::ConstHandle<JSON::Value> const &arg,
        Util::JSONArrayGenerator &resultJAG
        );
    
      RC::ConstHandle<Executable> m_executable;
      GenericFunctionPtr m_functionPtr;
    };
  }
}

#endif //_FABRIC_KL_OPERATOR_H
