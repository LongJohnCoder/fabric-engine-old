/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include "ExprType.h"
#include "Adapter.h"

#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Core/RT/Impl.h>

namespace Fabric
{
  namespace CG
  {
    ExprType::ExprType()
      : m_adapter( 0 )
      , m_usage( USAGE_UNSPECIFIED )
    {
    }
  
    ExprType::ExprType( RC::ConstHandle<Adapter> const &desc )
      : m_adapter( desc )
      , m_usage( USAGE_UNSPECIFIED )
    {
    }
  
    ExprType::ExprType( Usage usage )
      : m_adapter( 0 )
      , m_usage( usage )
    {
    }
  
    ExprType::ExprType( RC::ConstHandle<Adapter> const &desc, Usage usage )
      : m_adapter( desc )
      , m_usage( usage )
    {
    }
    
    ExprType::ExprType( ExprType const &that )
      : m_adapter( that.m_adapter )
      , m_usage( that.m_usage )
    {
    }
    
    ExprType::~ExprType()
    {
    }
    
    ExprType &ExprType::operator =( ExprType const &that )
    {
      m_adapter = that.m_adapter;
      m_usage = that.m_usage;
      return *this;
    }
          
    void ExprType::set( RC::ConstHandle<Adapter> const &adapter, Usage usage )
    {
      m_adapter = adapter;
      m_usage = usage;
    }

    bool ExprType::isValid() const
    {
      return m_adapter && m_usage != USAGE_UNSPECIFIED;
    }
    
    ExprType::operator bool() const
    {
      return isValid();
    }
    
    bool ExprType::operator !() const
    {
      return !isValid();
    }
    
    RC::ConstHandle<Adapter> ExprType::getAdapter() const
    {
      return m_adapter;
    }
    
    Usage ExprType::getUsage() const
    {
      return m_usage;
    }

    std::string const &ExprType::getUserName() const
    {
      return m_adapter->getUserName();
    }
    
    std::string const &ExprType::getCodeName() const
    {
      return m_adapter->getCodeName();
    }
    
    RC::ConstHandle<RT::Desc> ExprType::getDesc() const
    {
      return m_adapter->getDesc();
    }
    
    RC::ConstHandle<RT::Impl> ExprType::getImpl() const
    {
      return m_adapter->getDesc()->getImpl();
    }
    
    std::string ExprType::desc() const
    {
      if ( m_adapter )
        return _(m_adapter) + ":" + std::string( m_usage==USAGE_LVALUE? "l-value": "r-value" );
      else return "void";
    }
    
    bool ExprType::operator ==( ExprType const &that ) const
    {
      return m_adapter == that.m_adapter
        && m_usage == that.m_usage;
    }
    
    bool ExprType::operator !=( ExprType const &that ) const
    {
      return !(this->operator==( that ));
    }
  };
};
