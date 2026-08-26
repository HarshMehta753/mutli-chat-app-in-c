/**********************************************************************************************************************************************
 * @file     define.h
 * @brief    Contains functionality for socket program
 * @author   Harsh Mehta
 * @version  0.0.1
 * @date     2026.08.27
 **********************************************************************************************************************************************/

#ifndef DEFINE_H
#define DEFINE_H

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "server.h"
#include "client.h"

#define SERVER_SOCK_MAX_CLIENTS        (5)             /**<  Max clients can be added at a time. */
#define SERVER_SOCK_BUFF_SIZE          (128)           /**<  Buffer size. */
#define SERVER_SOCK_CLIENT_LIST_SIZE   (15)            /**<  Available clients list size. */
#define SERVER_SOCK_SND_MSG_SIZE       (30)            /**<  Buffer size for sending messages. */
#define SERVER_SOCK_LISTEN_QUEUE_MAX   (2)             /**<  Max queue size. */
#define CLIENT_SOCK_RCV_SND_BUFF_SIZE  (128)           /**<  Buffer size for sendind and receiving messages. */

#define SERVER_SOCK_ID_ERROR_MSG       "Invalid client ID!Check list"                     /**< Client ID error message. */
#define SERVER_SOCK_FORMAT_ERROR_MSG   "Check Message Format--> <client_id>:<meassage>"   /**< Message format error log. */
#define SERVER_SOCK_CLIENT_LIST_MSG    "Available clients:"                               /**< Available client message. */

/**
 * @brief Client Information 
 */
typedef struct
{
    int id;                  /**< Client ID. */
    int socket;              /**< Socket descriptor of client. */
} server_sock_client_struct_t;

/**
 * @brief Server Configuration Data 
 */
typedef struct
{
    int client_count;                                              /**< Counter to count connected clients. */
    int id_count;                                                  /**< Counter assigning its value as ID to every new client. */
    server_sock_client_struct_t clients[SERVER_SOCK_MAX_CLIENTS];  /**< Array of structure to store client details. */
    pthread_mutex_t mutex_lock;                                    /**< Mutex lock for maintaining synchronization. */

} server_sock_config_t;

/**
 * @brief Client configuration Data
 */
typedef struct
{
    int socket;                        /**< Socket descriptor. */
    bool client_disconnected_f;        /**< Flag user for thread termination purpose. */
} client_sock_config_t;

#endif  /* DEFINE_H */

