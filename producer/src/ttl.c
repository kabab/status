#include <unistd.h>
#include "db.h"
#include "config.h"
#include "rabbitmq.h"
#include <stdio.h>

static void ttl_setup()
{
	rabbitmq_connect(RABBITMQ_HOST, RABBITMQ_PORT);
	rabbitmq_login(RABBITMQ_USERNAME, RABBITMQ_PASSWORD);
	db_start();
}

void ttl_start()
{
	ttl_setup();
	db_insert("http://twitter.com", 10);
}

void ttl_loop()
{
	int i;
	time_t current_time;
	printf("%d\n", db_get_max());
	while(1) {
		usleep(100000);
		current_time = time(NULL);

		for (i = 0; i < db_get_max(); i++) {
			entry_t *entry = db_get_entry(i);
			if (!entry->key) {
				break;
			} else {
				if (entry->time < current_time) {
					printf("%s\n", entry->key);
					entry->time = current_time + entry->interval;
					rabbitmq_send(RABBITMQ_QUEUE, entry->key);
				}
			}
		}
	}
}

void ttl_stop()
{
	db_close();
}