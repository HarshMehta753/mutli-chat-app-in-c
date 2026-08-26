/**********************************************************************************************************************************************
 * @file     server.c
 * @brief    Server program for chat system which manages communication between different clients. 
 * @author   Harsh Mehta
 * @version  0.0.1
 * @date     2026.08.27
 **********************************************************************************************************************************************/

#include "define.h"

server_sock_config_t server_sock_config = {0}; /**< Server socket configuration structure. */

/**
 * @brief Sends the connected clients list to client.
 * @param socket Socket descriptor of destination client.
 * @return None
 */
static void server_sock_send_list (int socket)
{
    char avlbl_clients[SERVER_SOCK_BUFF_SIZE] = SERVER_SOCK_CLIENT_LIST_MSG;
    char *ptr = avlbl_clients + strlen(avlbl_clients);

    pthread_mutex_lock(&server_sock_config.mutex_lock);

    for(int i = 0; i < server_sock_config.client_count; i++)
    {
        ptr += sprintf( ptr, " %d", server_sock_config.clients[i].id);
    }

    pthread_mutex_unlock(&server_sock_config.mutex_lock);
    send(socket, avlbl_clients, strlen(avlbl_clients), 0);
}

/**
 * @brief Removes the disconnected client from the array of structure.
 * @param socket Socket descriptor of destination client.
 * @return None.
 */
static void server_sock_remove_client (int socket)
{
    bool is_it_found_f = false;

    pthread_mutex_lock (&server_sock_config.mutex_lock);

    for ( int i = 0; i < server_sock_config.client_count; i++)
    { 
        /* finding matching descriptor */
        if (server_sock_config.clients[i].socket == socket)
        {
            is_it_found_f = true;
        }
        /* Shifting the index data*/
        if ( (is_it_found_f == true) && (i < (server_sock_config.client_count-1)))
        {
            server_sock_config.clients[i] = server_sock_config.clients[i + 1];
        }
    }
    if (is_it_found_f == true )
    {
        server_sock_config.client_count--;
    }
    pthread_mutex_unlock (&server_sock_config.mutex_lock);
}

/**
 * @brief Handler that is called upon thread creation,manages communucation between clients
 * @param arg Argument provided on thread creation.
 * @return None 
 */
void* server_sock_client_handler (void *arg)
{
    int n = 0;
        
    char buff[SERVER_SOCK_BUFF_SIZE];
    
    /* error messages*/
    char *err1 = SERVER_SOCK_ID_ERROR_MSG;
    char *err2 = SERVER_SOCK_FORMAT_ERROR_MSG;
    
    if (arg == NULL)
    {
        printf ("%s() Argument not provided!\n", __func__);
        pthread_exit(NULL);        
    }
    
    server_sock_client_struct_t data = *(server_sock_client_struct_t *)arg;
         
    while (1)
    {
        char msg[SERVER_SOCK_SND_MSG_SIZE] = {0};
        int send_id = 0, send_sock = -1;

        n = recv (data.socket, &buff, sizeof(buff), 0);
        /* error check */
        if (n <= 0)
        {
            printf("%s() Client %d Disconnected!\n", __func__, data.id);
            server_sock_remove_client (data.socket);
            close (data.socket);
            break;
        }
       
        buff[n] = '\0';
        
        /* comparing received string */
        if (strcmp (buff, "list") == 0)
        {
            server_sock_send_list (data.socket);
            continue;
        }
        
        /* parsing msg*/
        if ( sscanf(buff, "%d:%[^\n]", &send_id, msg) == 2)
        {
            pthread_mutex_lock (&server_sock_config.mutex_lock);
            for (int i = 0; i < server_sock_config.client_count; i++)
            {
                if (server_sock_config.clients[i].id == send_id)
                {
                    send_sock = server_sock_config.clients[i].socket;
                    break;
                }
            }
            pthread_mutex_unlock (&server_sock_config.mutex_lock);

            if (send_sock != -1)
            {
                char out[SERVER_SOCK_BUFF_SIZE];
                sprintf(out, "From Client %d : %s", data.id, msg);
                
                if ( send(send_sock, out, strlen(out), 0) > 0)
                {
                    printf ("Sent msg from client %d to client %d\n", data.id, send_id);
                }
            }
            else
            {
                send (data.socket, err1, strlen(err1), 0);
            }
        }
        else
        {
            send (data.socket, err2, strlen(err2), 0);
        }
    }
    pthread_exit(NULL);
}

int server_process_function (int argc, char *argv[])
{
    int server_fd = 0 ;
    struct sockaddr_in server;
        
    server_sock_config.id_count = 1;
    
    /* Error check */
    if (argc != 2)
    {
        printf("%s() Input @cmdline format: ./server port_number\n ", __func__);
        return -1;
    }
    
    /* mutex initialization*/
    pthread_mutex_init (&server_sock_config.mutex_lock, NULL);
    
    server_fd = socket (AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1)
    {
        printf("Socket creation failed!\n");
        return -1;
    }
    else
    {
        printf("Socket Creation Successfull!\n");
    }
    
    /* storing port and address info */
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));;
    server.sin_addr.s_addr = INADDR_ANY;

    /* bind check */
    if ( bind (server_fd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror ("bind error");
        return -1;
    }
    
    /* listen check */
    if ( listen (server_fd, SERVER_SOCK_LISTEN_QUEUE_MAX) == -1)
    {
        perror ("listen error");
        return -1;
    }

    while (1)
    {
        int id = 0, client_fd = 0;
        pthread_t client_thread;
       
        client_fd = accept ( server_fd, NULL, NULL);
        if (client_fd <= 0)
        {
            printf ("%s() Failed to accept or no client requested yet for connection!\n", __func__);
            continue;
        }

        pthread_mutex_lock (&server_sock_config.mutex_lock);
        
        id = server_sock_config.id_count++;

        server_sock_config.clients [server_sock_config.client_count].id = id;
        server_sock_config.clients [server_sock_config.client_count].socket = client_fd;

        server_sock_config.client_count++;
        
    	send (client_fd, &id, sizeof(id), 0);
        printf ("Client %d Connected!\n", id);

        pthread_mutex_unlock (&server_sock_config.mutex_lock);

        pthread_create(&client_thread, NULL, server_sock_client_handler, &server_sock_config.clients [server_sock_config.client_count-1]);
        pthread_detach(client_thread);
    }

    close(server_fd);
    return 0;
}

