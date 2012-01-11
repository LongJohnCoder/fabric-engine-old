/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/Core/MR/ArrayProducer.h>
#include <Fabric/Core/RT/Desc.h>
#include <Fabric/Core/MT/Impl.h>
#include <Fabric/Core/MT/LogCollector.h>
#include <Fabric/Core/MT/Util.h>
#include <Fabric/Core/Util/Log.h>
#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Core/Util/JSONGenerator.h>

namespace Fabric
{
  namespace MR
  {
    ArrayProducer::ArrayProducer()
    {
    }
    
    ArrayProducer::ComputeState::ComputeState( RC::ConstHandle<ArrayProducer> const &arrayProducer )
      : m_arrayProducer( arrayProducer )
      , m_count( arrayProducer->getCount() )
    {
    }
      
    void ArrayProducer::ComputeState::produce( void *data ) const
    {
      produce( 0, m_count, data );
    }
      
    void ArrayProducer::ComputeState::produceJSON( Util::JSONGenerator &jg ) const
    {
      produceJSON( 0, m_count, jg );
    }
      
    void ArrayProducer::ComputeState::produceJSON( size_t index, Util::JSONGenerator &jg ) const
    {
      RC::ConstHandle<RT::Desc> elementDesc = m_arrayProducer->getElementDesc();
      
      size_t allocSize = elementDesc->getAllocSize();
      void *valueData = alloca( allocSize );
      memset( valueData, 0, allocSize );
      produce( index, valueData );
      elementDesc->generateJSON( valueData, jg );
      elementDesc->disposeData( valueData );
    }
    
    
    class ArrayProducer::Execution
    {
    public:
    
      Execution(
        RC::ConstHandle<ArrayProducer::ComputeState> const &computeState,
        size_t allocSize,
        void *datas,
        size_t index,
        size_t count
        )
        : m_computeState( computeState )
        , m_allocSize( allocSize )
        , m_datas( datas )
        , m_index( index )
        , m_count( count )
        , m_logCollector( MT::tlsLogCollector.get() )
        , m_numJobs( std::min( m_count, 4 * MT::getNumCores() ) )
        , m_indicesPerJob( (m_count + m_numJobs - 1) / m_numJobs )
      {
      }
      
      void run()
      {
        MT::executeParallel(
          m_logCollector,
          m_numJobs,
          &Callback,
          this,
          false
          );
      }
      
    protected:
      
      void callback( size_t jobIndex )
      {
        size_t index = jobIndex * m_indicesPerJob;
        size_t endIndex = std::min( index + m_indicesPerJob, m_count );
        
        uint8_t *data = reinterpret_cast<uint8_t *>( m_datas ) + index * m_allocSize;
        while ( index < endIndex )
        {
          m_computeState->produce( m_index + index, data );
          ++index;
          data += m_allocSize;
        }
      }
      
      static void Callback( void *userdata, size_t jobIndex )
      {
        static_cast<Execution *>( userdata )->callback( jobIndex );
      }
      
    private:
    
      RC::ConstHandle<ArrayProducer::ComputeState> m_computeState;
      size_t m_allocSize;
      void *m_datas;
      size_t m_index;
      size_t m_count;

      RC::Handle<MT::LogCollector> m_logCollector;
      size_t m_numJobs;
      size_t m_indicesPerJob;
    };

    void ArrayProducer::ComputeState::produce( size_t index, size_t count, void *datas ) const
    {
      Execution(
        this,
        m_arrayProducer->getElementDesc()->getAllocSize(),
        datas,
        index,
        count
        ).run();
    }

    void ArrayProducer::ComputeState::produceJSON( size_t index, size_t count, Util::JSONGenerator &jg ) const
    {
      RC::ConstHandle<RT::Desc> elementDesc = m_arrayProducer->getElementDesc();
      size_t allocSize = elementDesc->getAllocSize();
      size_t totalAllocSize = allocSize * count;
      void *datas = alloca( totalAllocSize );
      memset( datas, 0, totalAllocSize );
      produce( index, count, datas );
      {
        Util::JSONArrayGenerator jag = jg.makeArray();
        uint8_t *data = reinterpret_cast<uint8_t *>( datas );
        for ( size_t i=0; i<count; ++i )
        {
          Util::JSONGenerator elementJG = jag.makeElement();
          elementDesc->generateJSON( data, elementJG );
          data += allocSize;
        }
      }
      elementDesc->disposeDatas( datas, count, allocSize );
    }
    
    struct ProduceJSONAsyncCallbackData_0
    {
      RC::Handle<ArrayProducer::ComputeState> computeState;
      Util::JSONObjectGenerator *jsonObjectGenerator;
    };

    void ArrayProducer::ComputeState::produceJSONAsync(
      Util::JSONObjectGenerator &jsonObjectGenerator,
      void (*finishedCallback)( void * ),
      void *finishedUserdata
      )
    {
      ProduceJSONAsyncCallbackData_0 *produceJSONAsyncCallbackData = new ProduceJSONAsyncCallbackData_0;
      produceJSONAsyncCallbackData->computeState = this;
      produceJSONAsyncCallbackData->jsonObjectGenerator = &jsonObjectGenerator;
      
      MT::ThreadPool::Instance()->executeParallelAsync(
        MT::tlsLogCollector.get(),
        1,
        &ArrayProducer::ComputeState::ProduceJSONAsyncCallback_0,
        produceJSONAsyncCallbackData,
        MT::ThreadPool::Idle,
        finishedCallback,
        finishedUserdata
        );
    }
    
    void ArrayProducer::ComputeState::ProduceJSONAsyncCallback_0(
      void *userdata,
      size_t index
      )
    {
      ProduceJSONAsyncCallbackData_0 *produceJSONAsyncCallbackData = static_cast<ProduceJSONAsyncCallbackData_0 *>( userdata );
      {
        Util::JSONGenerator jg = produceJSONAsyncCallbackData->jsonObjectGenerator->makeMember( "result" );
        produceJSONAsyncCallbackData->computeState->produceJSON( jg );
      }
      delete produceJSONAsyncCallbackData;
    }
    
    struct ProduceJSONAsyncCallbackData_1
    {
      RC::Handle<ArrayProducer::ComputeState> computeState;
      size_t index;
      Util::JSONObjectGenerator *jsonObjectGenerator;
    };

    void ArrayProducer::ComputeState::produceJSONAsync(
      size_t index,
      Util::JSONObjectGenerator &jsonObjectGenerator,
      void (*finishedCallback)( void * ),
      void *finishedUserdata
      )
    {
      ProduceJSONAsyncCallbackData_1 *produceJSONAsyncCallbackData = new ProduceJSONAsyncCallbackData_1;
      produceJSONAsyncCallbackData->computeState = this;
      produceJSONAsyncCallbackData->index = index;
      produceJSONAsyncCallbackData->jsonObjectGenerator = &jsonObjectGenerator;
      
      MT::ThreadPool::Instance()->executeParallelAsync(
        MT::tlsLogCollector.get(),
        1,
        &ArrayProducer::ComputeState::ProduceJSONAsyncCallback_1,
        produceJSONAsyncCallbackData,
        MT::ThreadPool::Idle,
        finishedCallback,
        finishedUserdata
        );
    }
    
    void ArrayProducer::ComputeState::ProduceJSONAsyncCallback_1(
      void *userdata,
      size_t index
      )
    {
      ProduceJSONAsyncCallbackData_1 *produceJSONAsyncCallbackData = static_cast<ProduceJSONAsyncCallbackData_1 *>( userdata );
      {
        Util::JSONGenerator jg = produceJSONAsyncCallbackData->jsonObjectGenerator->makeMember( "result" );
        produceJSONAsyncCallbackData->computeState->produceJSON( produceJSONAsyncCallbackData->index, jg );
      }
      delete produceJSONAsyncCallbackData;
    }
    
    struct ProduceJSONAsyncCallbackData_2
    {
      RC::Handle<ArrayProducer::ComputeState> computeState;
      size_t index;
      size_t count;
      Util::JSONObjectGenerator *jsonObjectGenerator;
    };

    void ArrayProducer::ComputeState::produceJSONAsync(
      size_t index,
      size_t count,
      Util::JSONObjectGenerator &jsonObjectGenerator,
      void (*finishedCallback)( void * ),
      void *finishedUserdata
      )
    {
      ProduceJSONAsyncCallbackData_2 *produceJSONAsyncCallbackData = new ProduceJSONAsyncCallbackData_2;
      produceJSONAsyncCallbackData->computeState = this;
      produceJSONAsyncCallbackData->index = index;
      produceJSONAsyncCallbackData->count = count;
      produceJSONAsyncCallbackData->jsonObjectGenerator = &jsonObjectGenerator;
      
      MT::ThreadPool::Instance()->executeParallelAsync(
        MT::tlsLogCollector.get(),
        1,
        &ArrayProducer::ComputeState::ProduceJSONAsyncCallback_2,
        produceJSONAsyncCallbackData,
        MT::ThreadPool::Idle,
        finishedCallback,
        finishedUserdata
        );
    }
    
    void ArrayProducer::ComputeState::ProduceJSONAsyncCallback_2(
      void *userdata,
      size_t index
      )
    {
      ProduceJSONAsyncCallbackData_2 *produceJSONAsyncCallbackData = static_cast<ProduceJSONAsyncCallbackData_2 *>( userdata );
      {
        Util::JSONGenerator jg = produceJSONAsyncCallbackData->jsonObjectGenerator->makeMember( "result" );
        produceJSONAsyncCallbackData->computeState->produceJSON( produceJSONAsyncCallbackData->index, produceJSONAsyncCallbackData->count, jg );
      }
      delete produceJSONAsyncCallbackData;
    }
  }
}
