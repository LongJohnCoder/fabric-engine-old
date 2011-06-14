/*
 *
 *  Created by Peter Zion on 10-09-27.
 *  Copyright 2010 Fabric 3D Inc. All rights reserved.
 *
 */

#ifndef _FABRIC_MT_TASK_SET_H
#define _FABRIC_MT_TASK_SET_H

#include <Fabric/Base/Config.h>

#include <vector>
#if defined(FABRIC_OS_LINUX)
# include <string>
#endif

namespace Fabric
{
  namespace MT
  {
    class TaskBase;
    
    class TaskGroup
    {
    public:
    
      TaskGroup();
      TaskGroup( TaskGroup const &that );
      ~TaskGroup();
      
      TaskGroup &operator =( TaskGroup const &that );
      
      void clear();
      void add( TaskBase const *task );
      
      void execute( void *userdata ) const;
      
      std::string desc() const;
    
    private:
    
      struct Context
      {
        TaskGroup const *taskGroup;
        void *userdata;
      };
    
      static void ExecuteTask( void *context, size_t index );
      void executeTask( void *userdata, unsigned index ) const;
    
      std::vector< TaskBase const * > m_tasks;
    };
  };
};

#endif //_FABRIC_MT_TASK_SET_H
