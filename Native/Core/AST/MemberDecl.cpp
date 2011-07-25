/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "MemberDecl.h"
#include <Fabric/Core/CG/Adapter.h>
#include <Fabric/Core/CG/Error.h>
#include <Fabric/Core/CG/Location.h>
#include <Fabric/Core/CG/ModuleBuilder.h>
#include <Fabric/Core/RT/Manager.h>
#include <Fabric/Core/Util/SimpleString.h>

namespace Fabric
{
  namespace AST
  {
    FABRIC_AST_NODE_IMPL( MemberDecl );
    
    MemberDecl::MemberDecl( CG::Location const &location, std::string const &name, std::string const &type )
      : Node( location )
      , m_name( name )
      , m_type( type )
    {
    }
    
    void MemberDecl::appendJSONMembers( Util::SimpleString &ss ) const
    {
      Node::appendJSONMembers(ss);
      ss.append( ",\"name\":" );
      ss.appendJSONString( m_name );
      ss.append( ",\"type\":" );
      ss.appendJSONString( m_type );
    }
    
    void MemberDecl::llvmPrepareModule( CG::ModuleBuilder &moduleBuilder, CG::Diagnostics &diagnostics ) const
    {
      RC::ConstHandle<CG::Adapter> adapter = moduleBuilder.getAdapter( m_type, getLocation() );
      adapter->llvmPrepareModule( moduleBuilder, true );
    }

    void MemberDecl::buildStructMemberInfo( RC::ConstHandle<RT::Manager> const &rtManager, RT::StructMemberInfo &structMemberInfo ) const
    {
      structMemberInfo.name = m_name;
      structMemberInfo.desc = rtManager->maybeGetDesc( m_type );
      if ( !structMemberInfo.desc )
        throw CG::Error( getLocation(), "member " + _(m_name) + ": type " + _(m_type) + " not registered" );
    }
  };
};
