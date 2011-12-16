/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/KLC/Operator.h>
#include <Fabric/Core/KLC/Executable.h>

namespace Fabric
{
  namespace KLC
  {
    FABRIC_GC_OBJECT_CLASS_IMPL( Operator, GC::Object )
      
    RC::Handle<Operator> Operator::Create(
      RC::ConstHandle<Executable> const &executable,
      GenericFunctionPtr functionPtr
      )
    {
      return new Operator(
        FABRIC_GC_OBJECT_MY_CLASS,
        executable,
        functionPtr
        );
    }

    Operator::Operator(
      FABRIC_GC_OBJECT_CLASS_PARAM,
      RC::ConstHandle<Executable> const &executable,
      GenericFunctionPtr functionPtr
      )
      : GC::Object( FABRIC_GC_OBJECT_CLASS_ARG )
      , m_executable( executable )
      , m_functionPtr( functionPtr )
    {
    }

    GenericFunctionPtr Operator::getGenericFunctionPtr() const
    {
      return m_functionPtr;
    }
    
    CG::Diagnostics const &Operator::getDiagnostics() const
    {
      return m_executable->getDiagnostics();
    }
        
    void Operator::jsonExec(
      std::string const &cmd,
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      if ( cmd == "getDiagnostics" )
        jsonExecGetDiagnostics( arg, resultJAG );
      else GC::Object::jsonExec( cmd, arg, resultJAG );
    }
    
    void Operator::jsonExecGetDiagnostics(
      RC::ConstHandle<JSON::Value> const &arg,
      Util::JSONArrayGenerator &resultJAG
      )
    {
      Util::JSONGenerator jg = resultJAG.makeElement();
      getDiagnostics().generateJSON( jg );
    }
  }
}
