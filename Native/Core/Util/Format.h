/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_UTIL_FORMAT_H
#define _FABRIC_UTIL_FORMAT_H

#include <Fabric/Base/Config.h>
#include <string>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

namespace Fabric
{
  namespace Util
  {
    std::string formatHex( size_t count, void const *data );
  };
  
  inline std::string _( void const *ptr )
  {
    char buffer[19];
    snprintf( buffer, 19, "%p", ptr );
    return buffer;
  }
  
  std::string _( char const *data, size_t length, size_t maxLength = SIZE_MAX, char quote = '\'' );
  
  inline std::string _( char const *cStr, size_t maxLength = SIZE_MAX )
  {
    return _( cStr, strlen(cStr), maxLength, '\'' );
  }
  
  inline std::string _( std::string const &string, size_t maxLength = SIZE_MAX )
  {
    return _( string.data(), string.length(), maxLength, '\'' );
  }
  
  inline std::string _( bool value )
  {
    return value? "true": "false";
  }
  
  inline std::string _( int32_t integer )
  {
    char buffer[32];
    snprintf( buffer, 32, "%d", (int)integer );
    return std::string( buffer );
  }
  
  inline std::string _( size_t size )
  {
    char buffer[32];
    snprintf( buffer, 32, "%lu", (unsigned long)size );
    return std::string( buffer );
  }
  
  std::string _( float value );
  std::string _( double value );
};

#endif //_FABRIC_UTIL_FORMAT_H
