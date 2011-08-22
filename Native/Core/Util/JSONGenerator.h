/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_UTIL_JSON_GENERATOR_H
#define _FABRIC_UTIL_JSON_GENERATOR_H

#include <Fabric/Base/Util/SimpleString.h>
#include <Fabric/Core/Util/Assert.h>

namespace Fabric
{
  namespace Util
  {
    class JSONObjectGenerator;
    class JSONArrayGenerator;
    
    class JSONGenerator
    {
    public:
    
      JSONGenerator( Util::SimpleString *ss )
        : m_ss( ss )
      {
        FABRIC_ASSERT( m_ss );
      }
      
      ~JSONGenerator()
      {
        FABRIC_ASSERT( !m_ss );
      }
      
      void makeBoolean( bool value ) const
      {
        FABRIC_ASSERT( m_ss );
        if ( value )
          m_ss->append( "true", 4 );
        else
          m_ss->append( "false", 5 );
        FABRIC_ASSERT( !(m_ss = 0) );
      }
      
      void makeInteger( int32_t value ) const
      {
        FABRIC_ASSERT( m_ss );
        m_ss->increaseLength( snprintf( m_ss->expand( 11 ), 11, "%d", (int)value ) );
        FABRIC_ASSERT( !(m_ss = 0) );
      }
      
      void makeScalar( float value ) const
      {
        FABRIC_ASSERT( m_ss );
        m_ss->increaseLength( snprintf( m_ss->expand( 32 ), 32, "%f", value ) );
        FABRIC_ASSERT( !(m_ss = 0) );
      }
      
      void makeString( char const *data, size_t length ) const
      {
        FABRIC_ASSERT( m_ss );
        char *tail = m_ss->expand( 2*length+2 );
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
        m_ss->increaseLength( p - tail );
        FABRIC_ASSERT( !(m_ss = 0) );
      }
      
      void makeString( char const *cStr ) const
      {
        makeString( cStr, strlen( cStr ) );
      }
      
      void makeString( std::string const &string ) const
      {
        makeString( string.data(), string.length() );
      }
      
      JSONObjectGenerator makeObject() const;
      
      JSONArrayGenerator makeArray() const;
      
      void appendJSON( Util::SimpleString const &ss ) const
      {  
        FABRIC_ASSERT( m_ss );
        m_ss->append( ss );
        FABRIC_ASSERT( !(m_ss = 0) );
      }
      
    private:
    
      mutable SimpleString *m_ss;
    };
    
    class JSONObjectGenerator
    {
    public:
    
      JSONObjectGenerator( SimpleString *ss )
        : m_ss( ss )
        , m_count( 0 )
      {
        m_ss->append( '{' );
      }
      
      ~JSONObjectGenerator()
      {
        m_ss->append( '}' );
      }
      
      JSONGenerator makeMember( char const *name ) const
      {
        if ( m_count++ == 0 )
          m_ss->append( ',' );
        JSONGenerator( m_ss ).makeString( name );
        m_ss->append( ':' );
        return JSONGenerator( m_ss );
      }
      
    private:
    
      SimpleString *m_ss;
      mutable size_t m_count;
    };
    
    inline JSONObjectGenerator JSONGenerator::makeObject() const
    {
      FABRIC_ASSERT( m_ss );
      JSONObjectGenerator result = JSONObjectGenerator( m_ss );
      FABRIC_ASSERT( !(m_ss = 0) );
      return result;
    }
    
    class JSONArrayGenerator
    {
    public:
    
      JSONArrayGenerator( SimpleString *ss )
        : m_ss( ss )
        , m_count( 0 )
      {
        m_ss->append( '[' );
      }
      
      ~JSONArrayGenerator()
      {
        m_ss->append( ']' );
      }
      
      JSONGenerator makeElement() const
      {
        if ( m_count++ == 0 )
          m_ss->append( ',' );
        return JSONGenerator( m_ss );
      }
      
    private:
    
      SimpleString *m_ss;
      mutable size_t m_count;
    };
    
    inline JSONArrayGenerator JSONGenerator::makeArray() const
    {
      FABRIC_ASSERT( m_ss );
      JSONArrayGenerator result = JSONArrayGenerator( m_ss );
      FABRIC_ASSERT( !(m_ss = 0) );
      return result;
    }
  };
};

#endif //_FABRIC_UTIL_JSON_GENERATOR_H
