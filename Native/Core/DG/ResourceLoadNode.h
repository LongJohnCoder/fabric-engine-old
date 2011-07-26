/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_DG_RESOURCELOADNODE_H
#define _FABRIC_DG_RESOURCELOADNODE_H

#include <Fabric/Core/DG/Node.h>

namespace Fabric
{
  namespace IO
  {
    class Stream;
  };

  namespace DG
  {
    class ResourceLoadNode : public Node
    {

    public:
    
      static RC::Handle<ResourceLoadNode> Create( std::string const &name, RC::Handle<Context> const &context );
      
      static void jsonExecCreate( RC::ConstHandle<JSON::Value> const &arg, RC::Handle<Context> const &context );

    protected:
    
      ResourceLoadNode( std::string const &name, RC::Handle<Context> const &context );

      virtual void evaluateLocal( void *userdata );

      void streamData( std::string const &url, std::string const &mimeType, size_t offset, size_t size, void const *data, void *userData );
      static void StreamData( std::string const &url, std::string const &mimeType, size_t offset, size_t size, void const *data, RC::Handle<RC::Object> const &target, void *userData )
      {
        RC::Handle<ResourceLoadNode>::StaticCast(target)->streamData( url, mimeType, offset, size, data, userData );
      }

      void streamEnd( std::string const &url, std::string const &mimeType, void *userData );
      static void StreamEnd( std::string const &url, std::string const &mimeType, RC::Handle<RC::Object> const &target, void *userData )
      {
        RC::Handle<ResourceLoadNode>::StaticCast(target)->streamEnd( url, mimeType, userData );
      }
      
      void streamFailure( std::string const &url, std::string const &errorDesc, void *userData );
      static void StreamFailure( std::string const &url, std::string const &errorDesc, RC::Handle<RC::Object> const &target, void *userData )
      {
        RC::Handle<ResourceLoadNode>::StaticCast(target)->streamFailure( url, errorDesc, userData );
      }

      void setData( std::string const *mimeType, void *data, size_t dataSize, std::string const *errorDesc, bool notify );

      void evaluateResource();
      static void EvaluateResource( void *userData, size_t index )
      {
        RC::Handle<ResourceLoadNode>::StaticCast(userData)->evaluateResource();
      }

    private:
    
      //TODO: don't have a stream; wrap it... RC::Handle<IO::Stream> m_stream;
      RC::Handle<IO::Stream> m_stream;
      std::string m_streamURL, m_streamMimeType;
      std::vector<uint8_t> m_streamData;
      size_t m_streamGeneration;
    };
  };
};

#endif //_FABRIC_DG_RESOURCELOADNODE_H
