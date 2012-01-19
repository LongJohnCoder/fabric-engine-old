/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include "FileHandleManager.h"
#include <Fabric/Core/Util/Assert.h>
#include <Fabric/Core/IO/Helpers.h>
#include <Fabric/Core/IO/Dir.h>
#include <Fabric/Core/Util/Base64.h>
#include <Fabric/Core/Util/Random.h>
#include <string.h>
#include <fstream>

namespace Fabric
{
  namespace IO
  {
    char const* fileHandleScheme = "fabricio";
    char const* fileHandleSchemeAndSeparator = "fabricio://";

    RC::Handle<FileHandleManager> FileHandleManager::Create()
    {
      return new FileHandleManager();
    }

    FileHandleManager::FileHandleManager()
      : m_encodedHandleLength(0)
    {
    }

    char const *FileHandleManager::getUrlScheme() const
    {
      return fileHandleScheme;
    }

    std::string FileHandleManager::createHandle( std::string const &path, bool folder, bool readOnly )
    {
      Data data;
      data.m_path = path;
      data.m_isFolder = folder;
      data.m_readOnly = readOnly;

      validateEntry( data.m_path );//Check basic syntax, but don't check if exists

      pair< PathToHandleMap::const_iterator, PathToHandleMap::const_iterator > existingRange = m_pathToHandle.equal_range( data.m_path );
      while( existingRange.first != existingRange.second )
      {
        const Data& existingData = existingRange.first->second->second;
        if( existingData.m_isFolder == data.m_isFolder && existingData.m_readOnly == data.m_readOnly )
          return existingRange.first->second->first;
        ++existingRange.first;
      }

      std::string handle;
      while( true )
      {
        static const size_t folderIDByteCount = 24;
        uint8_t folderIDBytes[folderIDByteCount];
        Util::generateSecureRandomBytes( folderIDByteCount, folderIDBytes );
        std::string handle = std::string( fileHandleSchemeAndSeparator ) + Util::encodeBase64( folderIDBytes, folderIDByteCount );

        if( m_encodedHandleLength == 0 )
          m_encodedHandleLength = handle.size();
        else
        {
          FABRIC_ASSERT( m_encodedHandleLength == handle.size() );
        }

        if( m_handleToData.find( handle ) == m_handleToData.end() )
          break;

        //If we are here, either we are really not lucky, or the random algo is really bad!
      }

      std::pair<bool, HandleToDataMap::iterator> handleToDataIter = m_handleToData.insert( std::make_pair( handle, data ) );
      m_pathToHandle.insert( std::make_pair( data.m_path, handleToDataIter) );
    }

    std::string FileHandleManager::createRelativeHandle( std::string const &handlePlusRelativePath, bool folder )
    {
      std::string relativePathPostfix;
      FileHandleManager::Data const &data = validateHandleAndGetData( handlePlusRelativePath, relativePathPostfix );
      if( relativePathPostfix.empty() )
      {
        if( folder != data.m_isFolder )
          throw Exception( "Invalid: trying to change a Folder handle to a File handle or vice-versa" );
        return handlePlusRelativePath;
      }
      FABRIC_ASSERT( data.m_isFolder );//checked by validateHandleAndGetData
      return createHandle( data.m_path + relativePathPostfix, folder, data.m_readOnly );
    }

    bool FileHandleManager::isValidHandle( std::string const &handle ) const
    {
      if( handle.length() < m_encodedHandleLength )
        return false;

      HandleToDataMap::const_iterator iter = m_handleToData.find( std::string( handle, 0, m_encodedHandleLength ) );
      if( iter == m_handleToData.end() )
        return false;

      return handle.length() == m_encodedHandleLength || ( iter->second.m_isFolder && handle[m_encodedHandleLength] == '/' );
    }

    bool FileHandleManager::hasRelativePath( std::string const &handle ) const
    {
      return handle.length() > m_encodedHandleLength;
    }

    FileHandleManager::Data const &FileHandleManager::validateHandleAndGetData( std::string const &handle, std::string& relativePathPostfix ) const
    {
      if( handle.length() < m_encodedHandleLength )
        throw Exception( "Invalid FileHandle" );

      HandleToDataMap::const_iterator iter = m_handleToData.find( handle, m_encodedHandleLength );
      if( iter == m_handleToData.end() )
        throw Exception( "Invalid FileHandle" );

      if( handle.length() > m_encodedHandleLength )
      {
        if( !iter->second.m_isFolder )
          throw Exception( "File handles cannot have relative paths" );
        relativePathPostfix.assign( handle.data() + m_encodedHandleLength, handle.length() - m_encodedHandleLength );

        if( relativePathPostfix[0] != '/' )
          throw Exception( "FileHandle relative paths must start with '/'" );

        //For security reasons, make sure there's no ".." or such in the relative path:
        validateEntry( relativePathPostfix );
      }
      return iter->second;
    }

    std::string FileHandleManager::getPath( std::string const &handle ) const
    {
      std::string relativePathPostfix;
      FileHandleManager::Data const &data = validateHandleAndGetData( handle, relativePathPostfix );
      return relativePathPostfix.empty() ? data.m_path : data.m_path + relativePathPostfix;
    }

    bool FileHandleManager::isReadOnly( std::string const &handle ) const
    {
      std::string relativePathPostfix;
      FileHandleManager::Data const &data = validateHandleAndGetData( handle, relativePathPostfix );
      return data.m_readOnly;
    }

    bool FileHandleManager::isFolder( std::string const &handle ) const
    {
      std::string relativePathPostfix;
      FileHandleManager::Data const &data = validateHandleAndGetData( handle, relativePathPostfix );
      if( relativePathPostfix.empty() )
        return data.m_isFolder;
      FABRIC_ASSERT( data.m_isFolder );//checked by validateHandleAndGetData
      std::string fullPath = data.m_path + relativePathPostfix;
      return DirExists( fullPath );
    }

    bool FileHandleManager::itemExists( std::string const &handle ) const
    {
      std::string relativePathPostfix;
      FileHandleManager::Data const &data = validateHandleAndGetData( handle, relativePathPostfix );
      if( relativePathPostfix.empty() )
      {
        if( data.m_isFolder )
          return DirExists( data.m_path );
        else
          return FileExists( data.m_path );
      }
      FABRIC_ASSERT( data.m_isFolder );//checked by validateHandleAndGetData
      std::string fullPath = data.m_path + relativePathPostfix;
      return DirExists( fullPath ) || FileExists( fullPath );
    }

    bool FileHandleManager::ensureExists( std::string const &handle ) const
    {
      if( !itemExists(handle) )
      {
        if( data.m_readOnly )
          throw Exception( "Error: cannot create file or folders flagged as read-only" );

        std::string relativePathPostfix;
        FileHandleManager::Data const &data = validateHandleAndGetData( handle, relativePathPostfix );
        if( !relativePathPostfix.empty() )
          throw Exception( "Error: ambiguous: handle's relative path could be a file or a folder; create a relative handle instead" );

        if( data.m_isFolder )
          CreateDir( data.m_path );
        else
        {
          std::string dir, file;
          SplitPath( data.m_path, dir, file );
          if( !DirExists( dir ) )
            CreateDir( dir );
          if( !FileExists( data.m_path )
          {
            std::ofstream file( data.m_path.c_str() );
            if( !file.is_open() )
              throw Exception( "Unable to create file from handle " + std::string(handle) );
          }
        }
      }
    }
  };
};
