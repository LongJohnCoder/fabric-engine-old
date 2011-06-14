/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_UTIL_SIMPLE_STRING_H
#define _FABRIC_UTIL_SIMPLE_STRING_H

#include <Fabric/Base/Util/Bits.h>

#include <algorithm>
#include <string.h>

namespace Fabric
{
  namespace Util
  {
    class SimpleString
    {
    public:
    
      SimpleString()
        : m_data( 0 )
        , m_length( 0 )
        , m_allocSize( 0 )
      {
      }
      
      SimpleString( char const *data, size_t length )
      {
        init( data, length );
      }
      
      SimpleString( char const *cString )
      {
        init( cString, strlen( cString ) );
      }
      
      SimpleString( std::string const &string )
      {
        init( string.data(), string.length() );
      }
      
      SimpleString( SimpleString const &that )
      {
        init( that.m_data, that.m_length );
      }
      
      ~SimpleString()
      {
        if ( m_data )
          free( m_data );
      }
      
      SimpleString &operator =( SimpleString const &that )
      {
        if ( m_data )
          free( m_data );
        init( that.m_data, that.m_length );
      }
      
      char const *getData() const
      {
        return m_data;
      }
      
      char *getData()
      {
        return m_data;
      }
      
      char const *getCString() const
      {
        if ( m_data )
          m_data[m_length] = '\0';
        return m_data;
      }
      
      operator char const *() const
      {
        return getCString();
      }
      
      char *getTail()
      {
        return &m_data[m_length];
      }
      
      size_t getLength() const
      {
        return m_length;
      }
      
      // Ensure we can hold at least length+1 characters
      char *reserve( size_t length )
      {
        size_t lengthPlusOne = length + 1;
        if ( lengthPlusOne > m_allocSize )
        {
          m_allocSize = AllocSizeForLengthPlusOne( lengthPlusOne );
          if ( m_data )
            m_data = (char *)realloc( m_data, m_allocSize );
          else m_data = (char *)malloc( m_allocSize );
        }
        return m_data;
      }
      
      // Ensure that length can be expanded by at least delta
      char *expand( size_t delta )
      {
        return &reserve( m_length + delta )[m_length];
      }
      
      void increaseLength( size_t delta )
      {
        m_length += delta;
      }
      
      void append( char ch )
      {
        expand(1)[0] = ch;
        increaseLength( 1 );
      }
      
      SimpleString &operator +=( char ch )
      {
        append( ch );
        return *this;
      }
      
      void append( char const *data, size_t length )
      {
        memcpy( expand( length ), data, length );
        increaseLength( length );
      }
      
      void append( char const *cString )
      {
        append( cString, strlen( cString ) );
      }
      
      SimpleString &operator +=( char const *cString )
      {
        append( cString );
        return *this;
      }
      
      void append( std::string const &string )
      {
        append( string.data(), string.length() );
      }
       
      SimpleString &operator +=( std::string const &string )
      {
        append( string );
        return *this;
      }
      
      void appendQuotedJSONString( char const *data, size_t length )
      {
        char *tail = expand( 2*length+2 );
        char *p = tail;
        *p++ = '"';
        char const *dataEnd = data + length;
        for ( ; data != dataEnd; ++data )
        {
          switch ( *data )
          {
            case '"':
              *p++ = '\\';
              *p++ = '"';
              break;
            case '\\':
              *p++ = '\\';
              *p++ = '\\';
              break;
            case '\b':
              *p++ = '\\';
              *p++ = 'b';
              break;
            case '\f':
              *p++ = '\\';
              *p++ = 'f';
              break;
            case '\n':
              *p++ = '\\';
              *p++ = 'n';
              break;
            case '\r':
              *p++ = '\\';
              *p++ = 'r';
              break;
            case '\t':
              *p++ = '\\';
              *p++ = 't';
              break;
            default:
              *p++ = *data;
              break;
          }
        }
        *p++ = '"';
        increaseLength( p - tail );
      }
      
      char *takeCString()
      {
        char *result = m_data;
        if ( result )
          result[m_length] = '\0';
        m_length = 0;
        m_allocSize = 0;
        m_data = 0;
        return result;
      }
    
    protected:
    
      size_t AllocSizeForLengthPlusOne( size_t lengthPlusOne )
      {
        return std::max<size_t>( nextPowerOfTwoMinusOne(lengthPlusOne), 255 );
      }
    
      void init( char const *data, size_t length )
      {
        if ( m_length = length )
        {
          m_allocSize = AllocSizeForLengthPlusOne( length + 1 );
          m_data = (char *)malloc( m_allocSize );
          memcpy( m_data, data, length );
          m_data[length] = '\0';
        }
        else
        {
          m_allocSize = 0;
          m_data = 0;
        }
      }
      
    private:
    
      size_t m_length;
      size_t m_allocSize;
      char *m_data;
    };
  };
};

#endif //_FABRIC_UTIL_SIMPLE_STRING_H
