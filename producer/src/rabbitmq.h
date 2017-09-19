#ifndef RABBITMQ_H
#define RABBITMQ_H

int rabbitmq_connect(const char *hostname, int port);
int rabbitmq_login(const char *username, const char *password);
int rabbitmq_send(const char *queue_name, char *msg);
void rabbitmq_close();

#define BUFFER_SIZE 256

#endif