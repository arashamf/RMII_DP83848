//Includes ------------------------------------------------------------------
#include "net.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lwip.h"
#include "lwip/sockets.h"
#ifdef DEBUG_MODE 
#include "usart.h"
#endif

//Private define-------------------------------------------------------------
#define SOCK_TARGET_PORT    1000
#define MAX_CONN            2
#define TCP_BUFF_SIZE       200

//Private variables ---------------------------------------------------------
char answer_buff[] = "answer";
uint32_t buflen = TCP_BUFF_SIZE;
char in_buffer[TCP_BUFF_SIZE];
TaskHandle_t Thread_Handler=NULL;

//Exported types ------------------------------------------------------------
typedef struct  
{
  struct sockaddr_in remotehost; //сокет входящего соединения
  socklen_t sockaddrsize; 
  int accept_sock; //неотрицательный файловый дескриптор принятого сокета
} struct_client_socket;
struct_client_socket client_socket;

//Private function prototypes -----------------------------------------------


//---------------------------------------------------------------------------
void tcp_task(void *pvParameters)
{
    MX_LWIP_Init();

    int ret;
    int sock; //номер создаваемого сокета
    int accept_sock = -1;
    struct sockaddr_in address; //сокет запущенного сервера
    struct sockaddr_in remotehost;  //сокет входящего соединения

    socklen_t sockaddrsize;
    memset(&address, 0, sizeof(address));
    address.sin_len = sizeof(address); //set up address to connect to
    address.sin_family = AF_INET;
    address.sin_port = htons(SOCK_TARGET_PORT);
    address.sin_addr.s_addr = INADDR_ANY;
    

    if ((sock = lwip_socket(AF_INET,SOCK_STREAM, 0)) >= 0) //socket-create an endpoint for communication
    {
        //bind() присваивает адрес неназванному сокету. Сокеты, созданные с помощью функции socket(), изначально не имеют имен; 
        //они идентифицируются только по их семейству адресов
        if (lwip_bind(sock, (struct sockaddr *)&address, sizeof (address)) ==  0)
        {
            //Функция listen() помечает сокет в режиме подключения, указанный в аргументе socket, как принимающий соединения, и 
            //ограничивает количество незавершенных подключений в очереди прослушивания сокета вторым аргументом
            lwip_listen(sock, MAX_CONN);
            for(;;)
            {
                accept_sock = lwip_accept(sock, (struct sockaddr *)&remotehost, &sockaddrsize); //порлучение сокета клиента при входящем соединении
                if(accept_sock >= 0)
				{
                    client_socket.accept_sock = accept_sock;
					client_socket.remotehost = remotehost;
					client_socket.sockaddrsize = sockaddrsize;
                    ret = lwip_recvfrom( accept_sock,  in_buffer, buflen, 0, (struct sockaddr *)&remotehost, &sockaddrsize);
                    if(ret > 0)
                    {
                        lwip_sendto (accept_sock, answer_buff, strlen((char*)answer_buff), 0, (struct sockaddr *)&remotehost, sockaddrsize);
                        #ifdef DEBUG_MODE 
                        sprintf (dbg_buf, "mysocket=%u accept=%u, in_port=%u, in_addr=%lu, in_msg:\r\n%s", sock, accept_sock, 
                        remotehost.sin_port, remotehost.sin_addr.s_addr, in_buffer);
                        dbg_putStr (dbg_buf);
                        #endif

                        if (in_buffer[ret-1] == '\n')
                        {
                            ret = lwip_close(accept_sock);                           
                            #ifdef DEBUG_MODE 
                            sprintf (dbg_buf, "close_socket, status=%u\r\n", ret);
                            dbg_putStr (dbg_buf);
                            #endif
                        }
                    }
                }
                vTaskDelay(5);
            }
        }
        else
        {   lwip_close(sock);    }
    }
}

//---------------------------------------------------------------------------
uint32_t tcp_thread (int sock)
{
   // int sock = &pvParameters
    uint32_t ret = 0;
    //Функция accept() извлекает первое соединение из очереди ожидающих подключения, создает новый сокет с тем же типом 
    //протокола и семейством адресов, что и у указанного сокета, и присваивает этому сокету новый файловый дескриптор
    client_socket.accept_sock = lwip_accept(sock, (struct sockaddr *)&client_socket.remotehost, &client_socket.sockaddrsize);
    if(client_socket.accept_sock >= 0)
    {
        if ((ret = lwip_recvfrom (client_socket.accept_sock, in_buffer, buflen, 0, (struct sockaddr *)&client_socket.remotehost, &client_socket.sockaddrsize)) > 0)
        {        
            in_buffer[ret] = 0; 
            #ifdef DEBUG_MODE 
            sprintf (dbg_buf, "my_socket=%u, socket=%u accept, in_port=%u, in_addr=%lu, in_msg:\r\n%s\r\n", sock,
            client_socket.accept_sock, client_socket.remotehost.sin_port, client_socket.remotehost.sin_addr.s_addr, in_buffer);
            dbg_putStr (dbg_buf);
            #endif
            lwip_sendto (client_socket.accept_sock, answer_buff, strlen((char*)answer_buff), 0, (struct sockaddr *)&client_socket.remotehost, client_socket.sockaddrsize);
            lwip_close(client_socket.accept_sock);
        }
    }
    return ret;
}
