#ifndef __URL__
#define __URL__


#include <ctype.h>  // tolower
#include <stdio.h>  // printf
#include <stdlib.h> // atoi
#include <stdint.h> // uint16_t
#include <string.h> // strstr


#define URL_SEPARATOR_SCHEME   "://"
#define URL_SEPARATOR_PORT     ':'
#define URL_SEPARATOR_USERINFO '@'
#define URL_SEPARATOR_PATH     '/'
#define URL_SEPARATOR_QUERY    '?'
#define URL_SEPARATOR_FRAGMENT '#'


// scheme://[userinfo@]host/path[?query][#fragment]
typedef struct url_s URL;
typedef enum url_scheme_enum URLScheme;
typedef enum url_flag_enum URLFlag;


enum url_scheme_enum {
	URL_SCHEME_UDP,
	URL_SCHEME_RTMP,
	URL_SCHEME_HTTP,
	URL_SCHEME_HTTPS,
	URL_SCHEME_WS,
	URL_SCHEME_WSS,
	URL_SCHEME_RTP,
	URL_SCHEME_FILE,
	URL_SCHEME_SSH,

	URL_SCHEME_UNKNOWN,
};

#define URL_SCHEME_UDP_STR      "UDP - User Datagram Protocol"
#define URL_SCHEME_RTMP_STR     "RTMP - Real Time Messaging Protocol"
#define URL_SCHEME_HTTP_STR     "HTTP - HyperText Transfer Protocol"
#define URL_SCHEME_HTTPS_STR    "HTTPS - HyperText Transfer Protocol Secure"
#define URL_SCHEME_WS_STR       "WS - WebSocket"
#define URL_SCHEME_WSS_STR      "WS - WebSocket Secure"
#define URL_SCHEME_RTP_STR      "RTP - WebSocket Secure"
#define URL_SCHEME_FILE_STR     "FILE - File System"
#define URL_SCHEME_SSH_STR      "SSH - Secure Shell"
#define URL_SCHEME_UNKNOWN_STR  "UNKNOWN - unknown url scheme"

enum url_flag_enum {
  URL_FLAG_MULTICAST = 1, // 00000001
};

struct url_s {
	URLScheme scheme;
	char      userinfo[100];
	char      host[100];
	uint16_t  port;
	char      path[255];
	char      query[255];
	char      fragment[100];
	int       flags;
};


void url_parse(URL *it, const char *raw);
void url_sprint_json(URL *it, char *buf, size_t bufsz);

URLScheme url_scheme_parse(char *buf, size_t bufsz);
const char *url_scheme_string(URLScheme it);


#endif // __URL__
