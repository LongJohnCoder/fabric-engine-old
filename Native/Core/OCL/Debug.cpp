#include "Debug.h"

#if defined(FABRIC_BUILD_DEBUG)
namespace Fabric
{
  namespace OCL
  {
    bool gDebugEnabled = false;
  };
};
#endif