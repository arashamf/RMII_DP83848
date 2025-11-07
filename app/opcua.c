#include "opcua.h"
#include "open62541.h"
#ifdef DEBUG_MODE 
#include "usart.h"
#endif

//-----------------------------------------------
#ifdef UA_ARCHITECTURE_FREERTOSLWIP
extern uint8_t IP_ADDRESS[];
#endif
char my_ip [16];
//-----------------------------------------------
void opcua_thread(void *pvParameters)
{
        IP_ADDRESS[0] = 192;
        IP_ADDRESS[1] = 168;
        IP_ADDRESS[2] = 111;
        IP_ADDRESS[3] = 197;
        //The default 64KB of memory for sending and receicing buffer caused problems to many users. With the code below, they are reduced to ~16KB
        UA_UInt32 sendBufferSize = 4096;       //64 KB was too much for my platform
        UA_UInt32 recvBufferSize = 4096;       //64 KB was too much for my platform
        UA_UInt16 portNumber = 4840;

        #if defined OPEN62541_FEERTOS_USE_OWN_MEM && DEBUG_MODE
        sprintf (dbg_buf, "opcua_task; free_heap=%u\r\n", xPortGetFreeHeapSize());
        dbg_putStr (dbg_buf);
        #endif

        UA_Server* mUaServer = UA_Server_new();
        if (mUaServer == NULL)
        {
                sprintf (dbg_buf, "UA_server_error; free_heap=%u\r\n", xPortGetFreeHeapSize());
                dbg_putStr (dbg_buf);
        }
        UA_ServerConfig *uaServerConfig = UA_Server_getConfig(mUaServer);
        UA_ServerConfig_setMinimalCustomBuffer(uaServerConfig, portNumber, 0, sendBufferSize, recvBufferSize);

        //VERY IMPORTANT: Set the hostname with your IP before starting the server
       // sprintf (my_ip, "%u.%u.%u.%u", IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
       // UA_ServerConfig_setCustomHostname(uaServerConfig, UA_STRING(my_ip));
       UA_ServerConfig_setCustomHostname(uaServerConfig, UA_STRING("192.168.111.197"));

        //The rest is the same as the example
        UA_Boolean running = true;

        // add a variable node to the adresspace
        UA_VariableAttributes attr = UA_VariableAttributes_default;
        UA_Int32 myInteger = 42;
        UA_Variant_setScalarCopy(&attr.value, &myInteger, &UA_TYPES[UA_TYPES_INT32]);
        attr.description = UA_LOCALIZEDTEXT_ALLOC("en-US","the answer");
        attr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US","the answer");
        UA_NodeId myIntegerNodeId = UA_NODEID_STRING_ALLOC(1, "the.answer");
        UA_QualifiedName myIntegerName = UA_QUALIFIEDNAME_ALLOC(1, "the answer");
        UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
        UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
        UA_Server_addVariableNode(mUaServer, myIntegerNodeId, parentNodeId, parentReferenceNodeId, myIntegerName, UA_NODEID_NULL, attr, NULL, NULL);

        // allocations on the heap need to be freed 
        UA_VariableAttributes_clear(&attr);
        UA_NodeId_clear(&myIntegerNodeId);
        UA_QualifiedName_clear(&myIntegerName);

        UA_StatusCode retval = UA_Server_run(mUaServer, &running);

        #ifdef DEBUG_MODE 
        sprintf (dbg_buf, "UA_Server_status=%lu\r\n", retval);
        dbg_putStr (dbg_buf);
        #endif

        UA_Server_delete(mUaServer);
}