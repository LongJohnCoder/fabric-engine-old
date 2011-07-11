/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/RT/ConstStringDesc.h>
#include <Fabric/Core/RT/ConstStringImpl.h>
#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>

namespace Fabric
{
  namespace RT
  {
    ConstStringDesc::ConstStringDesc( std::string const &name, RC::ConstHandle<ConstStringImpl> const &constStringImpl )
      : Desc( name, constStringImpl )
      , m_constStringImpl( constStringImpl )
    {
    }
    
    size_t ConstStringDesc::getLength() const
    {
      return m_constStringImpl->getLength();
    }
    
    RC::Handle<JSON::Object> ConstStringDesc::jsonDesc() const
    {
      RC::Handle<JSON::Object> result = Desc::jsonDesc();
      result->set( "internalType", JSON::String::Create("ConstString") );
      result->set( "length", JSON::Integer::Create(getLength()) );
      return result;
    }
  };
};
