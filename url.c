#include "url.h"


static void prepchr(char *s, const char c) {
	memmove(s+1, s, strlen(s));
	s[0] = c;
}


static void split_host_port(const char *buf, const size_t bufsz, char *host, uint16_t *port) {
	char *token = NULL;
	char port_raw[6] = { 0 };

	token = strchr(buf, URL_SEPARATOR_PORT);
	if (token) { // host + port
		strncpy(host, buf, token-buf);
		strncpy(
			port_raw,

			token+1,      // port buffer start

			              // port buffer size =
			bufsz         //   total-buffer-size
			- (token-buf) //   - (host-buffer-size)
		);
		*port = (uint16_t)atoi(port_raw);

		return;
	}

	*port = 0;
	strncpy(host, buf, bufsz);
}

void url_parse(URL *it, const char *raw) {
	char copy[300];
	char *cursor = NULL,
	     *token = NULL;

	memset(it, 0, sizeof(*it));
	cursor = strcpy(copy, raw);

	it->scheme = URL_SCHEME_UNKNOWN;

	if ((token=strstr(cursor, URL_SEPARATOR_SCHEME))) {
		it->scheme = url_scheme_parse(
			cursor,       // scheme buffer
			token-cursor  // scheme buffer size
		);
		cursor = token + strlen(URL_SEPARATOR_SCHEME);
	}

	if ((token=strchr(cursor, URL_SEPARATOR_USERINFO))) {
		strncpy(
			it->userinfo,
			cursor,        // userinfo buffer
			token-cursor   // userinfo buffer size
		);
		cursor = token + 1;
	}

	if (it->scheme != URL_SCHEME_FILE) {
		if ((token=strchr(cursor, URL_SEPARATOR_PATH))) {
			split_host_port(
				cursor,       // host[+port] buffer
				token-cursor, // host[+port] buffer size
				it->host,
				&it->port
			);
			cursor = token;
		} else {
			split_host_port(cursor, strlen(cursor), it->host, &it->port);
			cursor = NULL;
		}
	}

	if (cursor) {
		if ((token=strchr(cursor, URL_SEPARATOR_QUERY))) {
			strncpy(
				it->path,
				cursor,        // path buffer
				token-cursor   // path buffer size
			);
			cursor = token + 1;
		} else {

			if ((token=strchr(cursor, URL_SEPARATOR_FRAGMENT))) {
				strncpy(
					it->path,
					cursor,        // path buffer
					token-cursor   // path buffer size
				);
				cursor = token + 1;
				strcpy(it->fragment, cursor);
			} else
				strcpy(it->path, cursor);

			cursor = NULL;
		}
	}

	if (cursor) {
		if ((token=strchr(cursor, URL_SEPARATOR_FRAGMENT))) {
			strncpy(
				it->query,
				cursor,        // fragment buffer
				token-cursor   // fragment buffer size
			);
			cursor = token + 1;
			strcpy(it->fragment, cursor);
		} else {
			strcpy(it->query, cursor);
			cursor = NULL;
		}
	}

	{
		if (it->scheme == URL_SCHEME_FILE) { // add missing /

			if ((strlen(it->path) != 0) &&               // path was parsed
			    (it->path[0] != URL_SEPARATOR_PATH) &&   // missing /
			    (sizeof(it->path) > strlen(it->path)+1)) // have enought space
				prepchr(it->path, URL_SEPARATOR_PATH);

		}

		if (it->scheme == URL_SCHEME_UNKNOWN) { // guess scheme from context

			if ((strlen(it->host) != 0) && // host provided
			    (it->host[0] == '2')) {
				it->scheme = URL_SCHEME_UDP;
			}

			if ((strlen(it->host) == 0) && // no host provided
				  (strlen(it->path) != 0) && // path provided
			    (it->path[0] == '/')) {
				it->scheme = URL_SCHEME_FILE;
			}

		}
	}

	printf("%60s >>> %40s | %5s | %20s | %5d | %20s | %10s | %s\n", raw, url_scheme_string(it->scheme), it->userinfo, it->host, it->port, it->path, it->query, it->fragment);
}

URLScheme url_scheme_parse(char *buf, size_t bufsz) {
	int i = 0;
	char *cursor = buf;
	for (i = 0; i < bufsz; i++) {
		*cursor = tolower(*cursor);
		cursor++;
	}

	if (!strncmp(buf, "udp", bufsz)) {
		return URL_SCHEME_UDP;
	} else if (!strncmp(buf, "rtmp", bufsz)) {
		return URL_SCHEME_RTMP;
	} else if (!strncmp(buf, "http", bufsz)) {
		return URL_SCHEME_HTTP;
	} else if (!strncmp(buf, "https", bufsz)) {
		return URL_SCHEME_HTTPS;
	} else if (!strncmp(buf, "ws", bufsz)) {
		return URL_SCHEME_WS;
	} else if (!strncmp(buf, "wss", bufsz)) {
		return URL_SCHEME_WSS;
	} else if (!strncmp(buf, "rtp", bufsz)) {
		return URL_SCHEME_RTP;
	} else if (!strncmp(buf, "file", bufsz)) {
		return URL_SCHEME_FILE;
	} else if (!strncmp(buf, "ssh", bufsz)) {
		return URL_SCHEME_SSH;
	}

	return URL_SCHEME_UNKNOWN;
}

const char *url_scheme_string(URLScheme it) {
	switch (it) {
	case URL_SCHEME_UDP:     return URL_SCHEME_UDP_STR;
	case URL_SCHEME_RTMP:    return URL_SCHEME_RTMP_STR;
	case URL_SCHEME_HTTP:    return URL_SCHEME_HTTP_STR;
	case URL_SCHEME_HTTPS:   return URL_SCHEME_HTTPS_STR;
	case URL_SCHEME_WS:      return URL_SCHEME_WS_STR;
	case URL_SCHEME_WSS:     return URL_SCHEME_WSS_STR;
	case URL_SCHEME_RTP:     return URL_SCHEME_RTP_STR;
	case URL_SCHEME_FILE:    return URL_SCHEME_FILE_STR;
	case URL_SCHEME_SSH:     return URL_SCHEME_SSH_STR;
	case URL_SCHEME_UNKNOWN: return URL_SCHEME_UNKNOWN_STR;
	}

	return URL_SCHEME_UNKNOWN_STR;
}
