/* vim:set ft=c ts=2 sw=2 sts=2 et cindent: */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdint.h>
#include <amqp_tcp_socket.h>
#include <amqp.h>
#include <amqp_framing.h>
#include <unistd.h>
#include "rabbitmq.h"

// Properties
static amqp_connection_state_t conn;
static amqp_socket_t *socket = NULL;

int rabbitmq_connect(const char *hostname, int port)
{
  int status;

  conn = amqp_new_connection();
  socket = amqp_tcp_socket_new(conn);

  if (!socket) {
    return -1;
  }

  status = amqp_socket_open(socket, hostname, port);
  if (status) {
    return -2;
  }

  return 0;
}

int rabbitmq_login(const char *username, const char *password)
{
  amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, username, password);
  amqp_channel_open(conn, 1);
  amqp_get_rpc_reply(conn);
  return 0;
}

int rabbitmq_send(const char *queue_name, char *msg)
{
  char message[BUFFER_SIZE];
  amqp_bytes_t message_bytes;
  strncpy(message, msg, BUFFER_SIZE);
  int i;
  message_bytes.len = strlen(message);
  message_bytes.bytes = message;
  amqp_basic_publish(conn,
                  1,
                  amqp_cstring_bytes(""),
                  amqp_cstring_bytes(queue_name),
                  0,
                  0,
                  NULL,
                  message_bytes);
  return 0;
}

void rabbitmq_close()
{
  amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
  amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
  amqp_destroy_connection(conn); 
}
