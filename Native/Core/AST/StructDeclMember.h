/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_AST_STRUCT_DECL_MEMBER_H
#define _FABRIC_AST_STRUCT_DECL_MEMBER_H

#include <Fabric/Core/AST/Node.h>
#include <Fabric/Core/RT/StructMemberInfo.h>

namespace Fabric
{
  namespace RT
  {
    class Manager;
  };
  
  namespace CG
  {
    class Adapter;
  };
  
  namespace AST
  {
    class StructDeclMember: public Node
    {
      FABRIC_AST_NODE_DECL( StructDeclMember );

    public:
    
      static RC::Handle<StructDeclMember> Create( CG::Location const &location, std::string const &name, std::string const &typeName )
      {
        return new StructDeclMember( location, name, typeName );
      }

      RC::Handle<JSON::Object> toJSON() const;
      
      RT::StructMemberInfo getStructMemberInfo( RC::ConstHandle<RT::Manager> const &rtManager ) const;

    protected:
    
      StructDeclMember( CG::Location const &location, std::string const &name, std::string const &typeName );
      
    private:
    
      std::string m_name;
      std::string m_typeName;
    };
  };
};

#endif //_FABRIC_AST_STRUCT_DECL_MEMBER_H
