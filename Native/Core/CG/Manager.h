#ifndef _FABRIC_CG_MANAGER_H
#define _FABRIC_CG_MANAGER_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Core/RT/StructMemberInfo.h>

#include <string>
#include <map>
#include <llvm/LLVMContext.h>

namespace llvm
{
  class Module;
  class ExecutionEngine;
};

namespace Fabric
{
  namespace RT
  {
    class Desc;
    class Manager;
  };
  
  namespace CG
  {
    class Adapter;
    class BooleanAdapter;
    class ByteAdapter;
    class ConstStringAdapter;
    class IntegerAdapter;
    class SizeAdapter;
    class ScalarAdapter;
    class StringAdapter;
    class StructAdapter;
    class FixedArrayAdapter;
    class VariableArrayAdapter;
    class OpaqueAdapter;
    class ModuleBuilder;
    
    class Manager : public RC::Object
    {
      typedef std::map< RC::ConstHandle<RT::Desc>, RC::ConstHandle<Adapter> > DescToAdapterMap;
      
    public:
    
      static RC::Handle<Manager> Create( RC::Handle<RT::Manager> const &rtManager );
      
      RC::Handle<RT::Manager> getRTManager() const;
      
      RC::ConstHandle<Adapter> getAdapter( RC::ConstHandle<RT::Desc> const &desc ) const;
      RC::ConstHandle<Adapter> maybeGetAdapter( std::string const &userName ) const;
      RC::ConstHandle<Adapter> getAdapter( std::string const &userName ) const;
      std::string const &getCodeName( std::string const &userName ) const;
      
      RC::ConstHandle<BooleanAdapter> getBooleanAdapter() const;
      RC::ConstHandle<ByteAdapter> getByteAdapter() const;
      RC::ConstHandle<IntegerAdapter> getIntegerAdapter() const;
      RC::ConstHandle<SizeAdapter> getSizeAdapter() const;
      RC::ConstHandle<ScalarAdapter> getScalarAdapter() const;
      RC::ConstHandle<StringAdapter> getStringAdapter() const;
      RC::ConstHandle<OpaqueAdapter> getDataAdapter() const;
      RC::ConstHandle<ConstStringAdapter> getConstStringAdapter( size_t length ) const;
      
      RC::ConstHandle<VariableArrayAdapter> getVariableArrayOf( RC::ConstHandle<Adapter> const &adapter ) const;
      RC::ConstHandle<FixedArrayAdapter> getFixedArrayOf( RC::ConstHandle<Adapter> const &adapter, size_t length ) const;
      
      RC::ConstHandle<StructAdapter> registerStruct( std::string const &name, RT::StructMemberInfoVector const &structMemberInfoVector );
      RC::ConstHandle<Adapter> registerAlias( std::string const &name, RC::ConstHandle<Adapter> const &adapter );
      
      llvm::LLVMContext &getLLVMContext() const;
      
      void llvmPrepareModule( ModuleBuilder &moduleBuilder ) const;
      
      void *llvmResolveExternalFunction( std::string const &functionName ) const;
      void llvmAddGlobalMappingsToExecutionEngine( llvm::ExecutionEngine *executionEngine, llvm::Module &module ) const;

      RC::ConstHandle<RT::Desc> getStrongerTypeOrNone( RC::ConstHandle<RT::Desc> const &lhsDesc, RC::ConstHandle<RT::Desc> const &rhsDesc ) const;
      
    protected:
    
      Manager( RC::Handle<RT::Manager> const &rtManager );
      
    private:
    
      typedef std::map< size_t, RC::ConstHandle<ConstStringAdapter> > ConstStringAdapters;
      
      mutable llvm::LLVMContext m_llvmContext;
    
      RC::Handle<RT::Manager> m_rtManager;
      
      mutable DescToAdapterMap m_descToAdapterMap;
      
      mutable RC::ConstHandle<BooleanAdapter> m_booleanAdapter;
      mutable RC::ConstHandle<ByteAdapter> m_byteAdapter;
      mutable RC::ConstHandle<IntegerAdapter> m_integerAdapter;
      mutable RC::ConstHandle<SizeAdapter> m_sizeAdapter;
      mutable RC::ConstHandle<ScalarAdapter> m_scalarAdapter;
      mutable RC::ConstHandle<StringAdapter> m_stringAdapter;
      mutable RC::ConstHandle<OpaqueAdapter> m_dataAdapter;
      mutable ConstStringAdapters m_constStringAdapters;
    };
  };
};

#endif //_FABRIC_CG_MANAGER_H
