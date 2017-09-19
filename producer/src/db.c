#include <string.h>
#include <stdlib.h>
#include "db.h"

static entry_t *entries = NULL;
static int entry_nb = 0;
static int entry_id = 0;
static int cursor = 0;

/**
 * Simple deletion and create with a key
 * It should be optimize using a hash tree|table to omptimize the key lookup
 */

void db_start()
{
	if (entries == NULL) {
		entries = (entry_t*) malloc(sizeof(entry_t) * DB_MAX_ENTRIES);
		memset(entries, 0, DB_MAX_ENTRIES * sizeof(entry_t));
	}
}

int db_insert(const char *key, int interval)
{
	int i;

	// Check if key exist
	if (db_get_id(key) >= 0)
		return DB_INSERT_ERROR_KEY_EXIST;


	// Look for an empty place
	for (i = 0; i < 2; i++) {
		while (entries[entry_id].key != NULL && entry_id < DB_MAX_ENTRIES) {
			entry_id++;
		}

		if (entry_id < DB_MAX_ENTRIES)
			break;
		if (i == 0)
			entry_id = 0;
	}

	if (entry_id < DB_MAX_ENTRIES) {
		entries[entry_id].key = strdup(key);
		entries[entry_id].interval = interval;
		entries[entry_id].time = 0;
		return entry_id;
	} else {
		return DB_INSERT_ERROR_FULL;
	}
}

int db_get_id(const char *key)
{
	int i;
	for (i = 0; i < DB_MAX_ENTRIES; i++) {
		if (entries[i].key && strcmp(entries[i].key, key) == 0)
			return i;
	}
	return -1;
}

void db_delete(const char *key)
{
	int i;
	int id;

	id = db_get_id(key);
	if (id >= 0 && entries[id].key) {
		free(entries[id].key);
		entries[id].key = NULL;
		entries[id].interval = 0;
		entries[id].time = 0;
	}
}

void db_close()
{
	int i;
	for (i = 0; i < DB_MAX_ENTRIES; i++) {
		if (entries[i].key) {
			free(entries[i].key);
			entries[i].key = NULL;
			entries[i].interval = 0;
			entries[i].time = 0;
		}
	}

	free(entries);
}

int db_current_id()
{
	return cursor;
}

entry_t *db_current_entry()
{
	return &entries[cursor];
}

entry_t *db_get_entry(int id)
{
	return &entries[id];
}

int db_next()
{
	return ++cursor % DB_MAX_ENTRIES;
}

int db_prev()
{
	return --cursor % DB_MAX_ENTRIES;
}

int db_get_max()
{
	return DB_MAX_ENTRIES;
}