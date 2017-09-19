#include "mongoose.h"
#include "db.h"
#include "config.h"

static void http_get_payload(const char *buff, char *payload);
static int http_get_key(const char *payload, const char *key, char *buff);
static int http_get_key_int(const char *payload, const char *key);
static int http_key_exists(const char *payload, const char *key);
static char *http_url_decode(const char *str);

static const char *s_http_port = HTTP_SERVER_PORT;

static void ev_handler(struct mg_connection *c, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {
    struct http_message *hm = (struct http_message *) p;

    char buff[512];
    char payload[1028];
    char response[256];
    char key_buff[256];
    char interval_buff[256];

    strncpy(payload, hm->message.p, hm->message.len);
	http_get_payload(payload, buff);

	int interval = http_get_key_int(buff, "interval");
	http_get_key(buff, "key", key_buff);

	char *url = http_url_decode(key_buff);
	
	if (db_insert(url, interval) >= 0) {
		sprintf(response, "OK");	
	} else {
		sprintf(response, "ERROR");
	}
	
	free(url);
	mg_send_head(c, 200, strlen(response), "Content-Type: text/plain");
    mg_printf(c, "%s\n", response);
  }
}

static int http_get_key(const char *payload, const char *key, char *ans)
{
	char buffer[256];
	int i, j, k = 0;
	for(i = 0; payload[i]; i++) {
		if (payload[i] == '=') {
			buffer[k++] = '\0';
			if (strcmp(buffer, key) == 0) {
				i++;
				// If null it will just check if the key exist or not
				if (ans) {
					for(j = 0; payload[i] && payload[i] != '&'; j++, i++)
						ans[j] = payload[i];
					ans[j] = '\0';	
				}
				return 1;
			} else {
				for(; payload[i] && payload[i] != '&'; i++);
			}
			k = 0;
		} else{
			buffer[k++] = payload[i];
		}
	}
	return 0;
}



static int http_get_key_int(const char *payload, const char *key)
{
	char buff[256];
	if (http_get_key(payload, key, buff))
		return atoi(buff);
	return 0;
}

static int http_key_exists(const char *payload, const char *key)
{
	return http_get_key(payload, key, NULL);
}

static void http_get_payload(const char *buff, char *payload)
{
	int i, j;
	for (i = 0; buff[i + 4]; i++) {
		if (buff[i] == '\r' &&
			buff[i + 1] == '\n' &&
			buff[i + 2] == '\r' &&
			buff[i + 3] == '\n')
		{
			i = i + 4;
			for (j = 0; buff[i + j]; j++)
				payload[j] = buff[i + j];
			return;
		}
	}
}

static char *http_url_decode(const char *str) {
  int d = 0; /* whether or not the string is decoded */

  char *dStr = malloc(strlen(str) + 1);
  char eStr[] = "00"; /* for a hex code */

  strcpy(dStr, str);

  while(!d) {
    d = 1;
    int i; /* the counter for the string */

    for(i=0;i<strlen(dStr);++i) {

      if(dStr[i] == '%') {
        if(dStr[i+1] == 0)
          return dStr;

        if(isxdigit(dStr[i+1]) && isxdigit(dStr[i+2])) {

          d = 0;

          /* combine the next to numbers into one */
          eStr[0] = dStr[i+1];
          eStr[1] = dStr[i+2];

          /* convert it to decimal */
          long int x = strtol(eStr, NULL, 16);

          /* remove the hex */
          memmove(&dStr[i+1], &dStr[i+3], strlen(&dStr[i+3])+1);

          dStr[i] = x;
        }
      }
    }
  }

  return dStr;
}

void server_start() {
  struct mg_mgr mgr;
  struct mg_connection *c;

  mg_mgr_init(&mgr, NULL);
  c = mg_bind(&mgr, s_http_port, ev_handler);
  mg_set_protocol_http_websocket(c);

  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);
}

