/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "objparser.h"
#include <sstream>

class Exception
{
public:
  Exception(const char* msg)
    : m_msg(msg)
  {}

  const char* getMsg()
  {
    return m_msg.c_str();
  }

private:
  std::string m_msg;
};

void skipToNextLine( std::istream& stream )
{
  stream.ignore(20000, '\n');
}

bool parsingError( std::istream& stream )
{
  return !stream.good() || stream.fail();
}


bool skipSpaces( std::istream& stream )
{
  char c = 0;
  while( stream.good() )
  {
    char pc = stream.peek();
    if( pc == ' ' || pc == '\t' || pc == '\r' )
      stream.get(c);
    else
      break;
  }
  return c != 0;
}

void skipMandatorySpaces( std::istream& stream )
{
  if( !skipSpaces(stream) )
    throw Exception("space expected");
}

template<int Dim>
void getV( V<Dim>& v, std::istream& stream )
{
  for( int i = 0; i < Dim; ++i )
  {
    skipMandatorySpaces( stream );
    stream >> v.v[i];
    if( parsingError( stream ) )
      throw Exception("float value expected");
  }
}

void ObjParser::ParseV( std::istream& stream )
{
  V3 v;
  getV( v, stream );
  m_points.push_back( v );
  skipToNextLine( stream );// [jeromecg 20110728] Ignore 4D coords for now
}

void ObjParser::ParseN( std::istream& stream )
{
  V3 v;
  getV( v, stream );
  m_normals.push_back( v );
  skipToNextLine( stream );
}

void ObjParser::ParseT( std::istream& stream )
{
  V2 v;
  getV( v, stream );
  m_texCoords.push_back( v );
  skipToNextLine( stream );// [jeromecg 20110728] Ignore 3D coords for now
}

void ObjParser::ParseF( std::istream& stream )
{
  int nbFacePts = 0;
  int initalFacePt = (int)m_triangleIndices.size();
  m_initialPointIndices.resize( m_points.size() );

  while( true )
  {
    bool hadSpaces = skipSpaces( stream );
    if( stream.peek() == '\n' )
    {
      skipToNextLine( stream );
      break;
    }
    else if( !hadSpaces )
      throw Exception("space expected");

    PointIndices indices;

    stream >> indices.m_point;
    if( parsingError( stream ) )
      throw Exception("int value expected");
    --indices.m_point;
    if( indices.m_point < 0 || indices.m_point > (int)m_points.size() )
      throw Exception("out of range point index");

    char c = 0;
    stream.get(c);
    if(c == '/')
    {
      if(stream.peek() != '/')
      {
        stream >> indices.m_texCoord;
        if( parsingError( stream ) )
          throw Exception("int value expected");
        --indices.m_texCoord;
        if( indices.m_texCoord < 0 || indices.m_texCoord > (int)m_texCoords.size() )
          throw Exception("out of range texture index");
      }
      c = 0;
      stream.get(c);
      if(c != '/')
        stream.unget();
      else
      {
        stream >> indices.m_normal;
        if( parsingError( stream ) )
          throw Exception("int value expected");
        --indices.m_normal;
        if( indices.m_normal < 0 || indices.m_normal > (int)m_normals.size() )
          throw Exception("out of range normal index");
      }
    }

    // [jeromecg 20110728] Check if we need to unshare the vertex data
    // Note: this would be more complex (and might have to be done later) if we were taking smoothing groups into account...

    if( !m_initialPointIndices[indices.m_point].isInitialized() )
      m_initialPointIndices[indices.m_point] = indices;
    else if( m_initialPointIndices[indices.m_point] != indices )
    {
      V3 v = m_points[indices.m_point];
      indices.m_point = (int)m_points.size();
      m_points.push_back( v );

      if(indices.m_normal != -1)
      {
        v = m_normals[indices.m_normal];
        indices.m_normal = (int)m_normals.size();
        m_normals.push_back( v );
      }

      if(indices.m_texCoord != -1)
      {
        V2 t = m_texCoords[indices.m_texCoord];
        indices.m_texCoord = (int)m_texCoords.size();
        m_texCoords.push_back( t );
      }
      m_initialPointIndices.push_back( indices );
    }

    nbFacePts++;

    if( nbFacePts <= 3 )
    {
      m_triangleIndices.push_back( indices );
    }
    else
    {
      //Simple triangulation: 0 1 2 / 0 2 3 / 0 3 4...
      PointIndices first = m_triangleIndices[initalFacePt];
      PointIndices second = m_triangleIndices.back();
      m_triangleIndices.push_back( first );
      m_triangleIndices.push_back( second );
      m_triangleIndices.push_back( indices );
    }
  }
  if( nbFacePts < 3 )
    throw Exception("face has less than 3 points");
}

void ObjParser::ComputeMissingNormals()
{
  //Quick check if any normal is missing
  size_t i;
  for( i = 0; i < m_triangleIndices.size(); ++i )
  {
    if( m_triangleIndices[i].m_normal == -1 )
      break;
  }
  if( i == m_triangleIndices.size() )
    return;

  // [jeromecg 20110728] Make it simple for now: if any normal is missing, recompute them all.
  // [jeromecg 20110728] TODO: take smoothing groups into account

  // Compute triangle normals
  size_t nbTriangles = m_triangleIndices.size() / 3;
  std::vector< V3 > triangleNormals;
  triangleNormals.resize( nbTriangles );

  for( i = 0; i < nbTriangles; ++i )
  {
    V3 v1 = m_points[ m_triangleIndices[ i*3+1 ].m_point ] - m_points[ m_triangleIndices[ i*3 ].m_point ];
    V3 v2 = m_points[ m_triangleIndices[ i*3+2 ].m_point ] - m_points[ m_triangleIndices[ i*3 ].m_point ];
    triangleNormals[i] = v1.Cross( v2 );
    triangleNormals[i].Normalize();
  }

  // Compute point normals by simply averaging adjacent triangle normals
  m_normals.resize( m_points.size(), V3( 0, 0, 0 ) );

  for( i = 0; i < m_triangleIndices.size(); ++i )
  {
    int ptIndex = m_triangleIndices[i].m_point;
    m_normals[ptIndex] = m_normals[ptIndex] + triangleNormals[i/3];
  }

  for( i = 0; i < m_normals.size(); ++i )
  {
    m_normals[i].Normalize();
    m_initialPointIndices[i].m_normal = i;
  }
}

ObjParser::ObjParser( std::istream& stream )
{
  // [jeromecg 20110728] Right now this parser is very simple as it agglomerates all points and faces in 1 big object,
  // ignoring materials, groups, smoothing groups, etc.
  int lineCount = 0;

  try
  {
    while( stream.good() )
    {
      char c = 0;
      stream.get(c);
      switch( c )
      {
      case '\n':
      case '\r':
      case ' ':
      case '\t':
        break;
      case 'v':
        {
          char nextC;
          stream.get(nextC);
          switch( nextC )
          {
          case 'p':
            skipToNextLine( stream );//patch: ignore
            break;
          case 'n':
            ParseN( stream );
            break;
          case 't':
            ParseT( stream );
            break;
          default:
            stream.unget();
            ParseV( stream );
          }
          break;
        }
      case 'f':
        ParseF( stream );
        break;
      default:
         skipToNextLine( stream );
      }
      ++lineCount;
    }
    ComputeMissingNormals();
  }
  catch ( Exception e )
  {
    std::stringstream sstr;
    sstr << "Error while parsing .obj file at line " << lineCount << ": " << e.getMsg();
    m_errors.push_back( sstr.str() );
  }
  catch ( ... )
  {
    std::stringstream sstr;
    sstr << "Unexpected error while parsing .obj file at line " << lineCount;
    m_errors.push_back( sstr.str() );
  }
}

void ObjParser::GetTriangleIndices(int triIndex, int& i1, int& i2, int& i3)const
{
  i1 = m_triangleIndices[triIndex*3].m_point;
  i2 = m_triangleIndices[triIndex*3+1].m_point;
  i3 = m_triangleIndices[triIndex*3+2].m_point;
}

V3 ObjParser::GetPoint(int ptIndex)const
{
  return m_points[ptIndex];
}

V3 ObjParser::GetNormal(int ptIndex)const
{
  int index = m_initialPointIndices[ptIndex].m_normal;
  if( index == -1 )
    return V3( 0, 1, 0 );
  else
    return m_normals[ index ];
}

V2 ObjParser::GetTextureCoord(int ptIndex)const
{
  int index = m_initialPointIndices[ptIndex].m_texCoord;
  if( index == -1 )
    return V2( 0, 0 );
  else
    return m_texCoords[ index ];
}