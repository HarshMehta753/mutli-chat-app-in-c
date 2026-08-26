/**********************************************************************************************************************************************
 * @file     client_sock.c
 * @brief    Client program for chat system which receives as well send msgs from/to server 
 * @author   Harsh Mehta
 * @version  0.0.1
 * @date     2026.08.27
 **********************************************************************************************************************************************/

#include "define.h" 

client_sock_config_t client_sock_config = {0}; /**< Client socket configuration structure. */

/**
 * @brief Receives the data from the server.
 * @param arg Additional argument given upon thread creation.
 * @return None
 */
static void *client_sock_receiver(void *arg)
{
    char buffer[CLIENT_SOCK_RCV_SND_BUFF_SIZE] = {0};
    
    /* receives the data from server continously*/
    while (client_sock_config.client_disconnected_f == false)
    {
        memset ( buffer, 0 , strlen(buffer) );
        if ( recv (client_sock_config.socket, buffer, CLIENT_SOCK_RCV_SND_BUFF_SIZE - 1, 0) <= 0)
        {
            printf ("Server Down!\n");
            client_sock_config.client_disconnected_f = true;
            break;
        }
        buffer[strlen(buffer)] = '\0';
        printf("\n%s\n", buffer);
    }
    pthread_exit (NULL);
}

/**
 * @brief Sends the data to the server.
 * @param arg Additional argument given upon thread creation.
 * @return None
 */
static void *client_sock_sender (void *arg)
{
    char buffer[CLIENT_SOCK_RCV_SND_BUFF_SIZE] = {0};
    
    /* gets user input and sends to the required client*/
    while (client_sock_config.client_disconnected_f == false)
    {        
        memset (buffer, 0, strlen(buffer));
        fgets(buffer, CLIENT_SOCK_RCV_SND_BUFF_SIZE, stdin);
        buffer[strlen(buffer)-1] = '\0'; 

        /*thread termination on exit*/
        if (strcmp (buffer, "exit") == 0)
        {
            client_sock_config.client_disconnected_f = true;
            break;
        }
        
        /* sends data to the client through its id*/
        if ( send (client_sock_config.socket, buffer, strlen(buffer), 0) == -1 )
        {
            perror ("Send Error");
        }
    }
    pthread_exit (NULL);
}

int client_process_function(int argc, char *argv[])
{
    struct sockaddr_in client;
    int my_id; 
    pthread_t send_thread, recv_thread;
    client_sock_config.client_disconnected_f = false;
    
    /* Error check */
    if ( argc != 3)
    {
        printf ("Input @cmdline : ./client port_number server_ip_address\n");
        return -1;
    }
    
    /* Socket creation */
    client_sock_config.socket = socket(AF_INET, SOCK_STREAM, 0);
    if ( client_sock_config.socket == -1 )
    {
        printf ("Socket creation failed!\n");
        return -1;
    }
    
    /* Adding port and address info */
    client.sin_family = AF_INET;
    client.sin_port = htons(atoi(argv[1]));
    inet_aton (argv[2], &client.sin_addr); 
 
    /* Building connection to the server */
    if (connect (client_sock_config.socket, (struct sockaddr *)&client, sizeof(client)) == -1)
    {
        perror ("Connect Error");
        return -1;
    }
    
    /* check for data reception from server */
    if ( recv (client_sock_config.socket, &my_id, sizeof(my_id), 0) <= 0 )
    {
        printf ("Disconnected from server!\n");
        return -1;
    }

    printf (">>>>>>>>>>>>>> Connected to the server! <<<<<<<<<<<<<<\n");
    printf ("My ID: %d\n", my_id);

    printf ("\nEnter:\nlist --> to see the available clients\nexit --> to exit from program\n<client_id>:<Msg> --> to send msgs to particular client\n\n");
    
    /* Thread Creation*/
    pthread_create(&recv_thread, NULL, client_sock_receiver, NULL);
    pthread_create(&send_thread, NULL, client_sock_sender, NULL);

    while (client_sock_config.client_disconnected_f == false)
    {
        sleep (1);
    }

    close (client_sock_config.socket);
    return 0;
}

