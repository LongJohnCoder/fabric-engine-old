#include "BooleanImpl.h"

#include <Fabric/Base/JSON/Boolean.h>
#include <Fabric/Core/Util/Encoder.h>
#include <Fabric/Core/Util/Decoder.h>
#include <Fabric/Core/Util/SimpleString.h>
#include <Fabric/Core/Util/Format.h>

namespace Fabric
{
  

  namespace RT
  {
    BooleanImpl::BooleanImpl( std::string const &codeName )
      : SimpleImpl( codeName, DT_BOOLEAN, sizeof(bool) )
    {
    }

    RC::Handle<JSON::Value> BooleanImpl::getJSONValue( void const *src ) const
    {
      return JSON::Boolean::Create( getValue(src) );
    }
    
    void BooleanImpl::setData( void const *src, void *dst ) const
    {
      setValue( getValue( src ), dst );
    }
    
    void const *BooleanImpl::getDefaultData() const
    {
      static bool const defaultData = 0;
      return &defaultData;
    }

    void BooleanImpl::setDataFromJSONValue( RC::ConstHandle<JSON::Value> const &jsonValue, void *dst ) const
    {

      if ( !jsonValue->isBoolean() )
        throw Exception("value is not boolean");
      RC::ConstHandle<JSON::Boolean> jsonBoolean = RC::ConstHandle<JSON::Boolean>::StaticCast( jsonValue );
      setValue( jsonBoolean->value(), dst );
    }

    Util::Encoder &BooleanImpl::encode( Util::Encoder &encoder, void const *src ) const
    {
      return encoder.put( getValue( src ) );
    }
    
    Util::Decoder &BooleanImpl::decode( Util::Decoder &decoder, void *dst ) const
    {
      bool boolData;
      decoder.get( boolData );
      setValue( boolData, dst );
      return decoder;
    }  
    
    std::string BooleanImpl::descData( void const *data ) const
    {
      return _( getValue(data) );
    }
  };
};
