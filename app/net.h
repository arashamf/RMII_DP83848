#ifndef __NET_H__
#define __NET_H__

#ifdef __cplusplus
extern "C" {
#endif

//includes -------------------------------------------------------------------
#include "main.h"
#include "lwip/sockets.h"

//----------------------------------------------------------------------------
void tcp_task(void *pvParameters);
//uint32_t tcp_thread (int  );

#ifdef __cplusplus
}
#endif

#endif /* __NET_H__ */