#ifndef DB_H
#define DB_H

#include <time.h>

typedef struct {
	char *key;
	int interval;
	time_t time;
} entry_t;

#define DB_MAX_ENTRIES 1000
#define DB_INSERT_ERROR_FULL -1
#define DB_INSERT_ERROR_KEY_EXIST -2


int db_get_max();
int db_prev();
int db_next();
entry_t *db_current_entry();
entry_t *db_get_entry(int id);
int db_current_id();
void db_start();
int db_insert(const char *key, int interval);
int db_get_id(const char *key);
void db_delete(const char *key);
void db_close();

#endif