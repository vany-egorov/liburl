#include <time.h>
#include <stdio.h>

#include "url.h"


#define COLOR_BRIGHT_BLACK    "\x1B[1;30m"
#define COLOR_BRIGHT_RED      "\x1B[1;31m"
#define COLOR_BRIGHT_GREEN    "\x1B[1;32m"
#define COLOR_BRIGHT_YELLOW   "\x1B[1;33m"
#define COLOR_BRIGHT_BLUE     "\x1B[1;34m"
#define COLOR_BRIGHT_MAGENTA  "\x1B[1;35m"
#define COLOR_BRIGHT_CYAN     "\x1B[1;36m"
#define COLOR_BRIGHT_WHITE    "\x1B[1;37m"

#define COLOR_DIM_BLACK   "\x1B[2;30m"
#define COLOR_DIM_RED     "\x1B[2;31m"
#define COLOR_DIM_GREEN   "\x1B[2;32m"
#define COLOR_DIM_YELLOW  "\x1B[2;33m"
#define COLOR_DIM_BLUE    "\x1B[2;34m"
#define COLOR_DIM_MAGENTA "\x1B[2;35m"
#define COLOR_DIM_CYAN    "\x1B[2;36m"
#define COLOR_DIM_WHITE   "\x1B[2;37m"

#define COLOR_RESET "\033[0m"


struct fixture_s {
	char *in;

	char *s;
	URL   u;
};
typedef struct fixture_s Fixture;


static Fixture fixtures[] = {
	{
		"udp://239.255.1.1:8800",
		"udp://239.255.1.1:8800",
		{
			.scheme=URL_SCHEME_UDP,
			.host="239.255.1.1",
			.port=8800,
		},
	},
	{
		"UdP://128.201.1.1",
		"udp://128.201.1.1",
		{
			.scheme=URL_SCHEME_UDP,
			.host="128.201.1.1",
			.port=0,
		},
	},
	{
		"UdP://128.201.1.1/foo/bar/buz",
		"udp://128.201.1.1/foo/bar/buz",
		{
			.scheme=URL_SCHEME_UDP,
			.host="128.201.1.1",
			.path="/foo/bar/buz",
		},
	},
	{
		"239.255.1.1:8800",
		"udp://239.255.1.1:8800",
		{
			.scheme=URL_SCHEME_UDP,
			.host="239.255.1.1",
			.port=8800,
		},
	},
	{
		"file:///mnt/data/ts.ts",
		"file:///mnt/data/ts.ts",
		{
			.scheme=URL_SCHEME_FILE,
			.host="",
			.port=0,
			.path="/mnt/data/ts.ts",
		},
	},
	{
		"file://mnt/data/ts.ts",
		"file:///mnt/data/ts.ts",
		{
			.scheme=URL_SCHEME_FILE,
			.host="",
			.port=0,
			.path="/mnt/data/ts.ts",
		},
	},
	{
		"/tmp/ts.ts",
		"file:///tmp/ts.ts",
		{
			.scheme=URL_SCHEME_FILE,
			.host="",
			.port=0,
			.path="/tmp/ts.ts",
		},
	},
	{
		"ssh://git@github.com/vany-egorov/absolut.git",
		"ssh://git@github.com:22/vany-egorov/absolut.git",
		{
			.scheme=URL_SCHEME_SSH,
			.userinfo="git",
			.host="github.com",
			.port=22,
			.path="/vany-egorov/absolut.git",
		},
	},
	{
		"http://127.0.0.1/google/com?foo=bar&bar=buz",
		"http://127.0.0.1:80/google/com?foo=bar&bar=buz",
		{
			.scheme=URL_SCHEME_HTTP,
			.host="127.0.0.1",
			.port=80,
			.path="/google/com",
			.query="foo=bar&bar=buz",
		},
	},
	{
		"http://127.0.0.1/google/com?foo=bar&bar=buz#section1",
		"http://127.0.0.1:80/google/com?foo=bar&bar=buz#section1",
		{
			.scheme=URL_SCHEME_HTTP,
			.host="127.0.0.1",
			.port=80,
			.path="/google/com",
			.query="foo=bar&bar=buz",
			.fragment="section1",
		},
	},
	{
		"http://127.0.0.1/google/com#section1",
		"http://127.0.0.1:80/google/com#section1",
		{
			.scheme=URL_SCHEME_HTTP,
			.host="127.0.0.1",
			.port=80,
			.path="/google/com",
			.fragment="section1",
		},
	},
	{
		"http://127.0.0.1/google/com#section1",
		"http://127.0.0.1:80/google/com#section1",
		{
			.scheme=URL_SCHEME_HTTP,
			.host="127.0.0.1",
			.port=80,
			.path="/google/com",
			.fragment="section1",
		},
	},
	{
		"/221.1.1.1:5500",
		"file:///221.1.1.1:5500",
		{
			.scheme=URL_SCHEME_FILE,
			.path="/221.1.1.1:5500",
		},
	},

	{
		NULL,
		NULL,
		{ 0 },
	},
};


static int ret = 0,
           pass = 0,
           fail = 0;
static void psep() { printf("[ %s----%s ] ", COLOR_BRIGHT_WHITE, COLOR_RESET); }
static void ppass() { printf("[ %sPASS%s ] ", COLOR_BRIGHT_GREEN, COLOR_RESET); pass++; }
static void pfail() { printf("[ %sFAIL%s ] ", COLOR_BRIGHT_RED, COLOR_RESET); fail++; ret = 1; }


int main(int argc, char **argv) {
	URL u = { 0 };
	char s[255] = { 0 };
	Fixture *fixture;
	clock_t start = clock();

	for (fixture=fixtures; fixture->in; fixture++) {
		psep();
		printf("\"%s\"\n", fixture->in);

		memset(s, 0, sizeof(s));

		url_parse(&u, fixture->in);
		url_sprint(&u, s, sizeof(s));

		(!strcmp(fixture->s, s)) ? ppass() : pfail();
		printf("\"%s\" | \"%s\"\n", fixture->in, s);

		(fixture->u.scheme == u.scheme) ? ppass() : pfail();
		printf("\"%s\" | \"%s\"\n",
			url_scheme_string(fixture->u.scheme), url_scheme_string(u.scheme));

		(!strcmp(fixture->u.userinfo, u.userinfo)) ? ppass() : pfail();
		printf("\"%s\" | \"%s\"\n", fixture->u.userinfo, u.userinfo);

		(!strcmp(fixture->u.userinfo, u.userinfo)) ? ppass() : pfail();
		printf("\"%s\" | \"%s\"\n", fixture->u.userinfo, u.userinfo);

		(!strcmp(fixture->u.host, u.host)) ? ppass() : pfail();
		printf("\"%s\" | \"%s\"\n", fixture->u.host, u.host);

		(fixture->u.port == u.port) ? ppass() : pfail();
		printf("%d | %d\n", fixture->u.port, u.port);

		(!strcmp(fixture->u.path, u.path)) ? ppass() : pfail();
		printf("\"%s\" | \"%s\"\n", fixture->u.path, u.path);

		(!strcmp(fixture->u.query, u.query)) ? ppass() : pfail();
		printf("\"%s\" | \"%s\"\n", fixture->u.query, u.query);

		(!strcmp(fixture->u.fragment, u.fragment)) ? ppass() : pfail();
		printf("\"%s\" | \"%s\"\n", fixture->u.fragment, u.fragment);

		printf("\n");
	}

	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;

	printf("%s-------------------------------%s\n", COLOR_BRIGHT_WHITE, COLOR_RESET);
	printf("Ran %d tests in %f seconds\n", pass+fail, seconds);
	if (ret) {
		printf("%sFAIL!%s -- %s%d Passed%s | %s%d Failed%s\n",
			COLOR_BRIGHT_RED, COLOR_RESET,
			COLOR_BRIGHT_GREEN, pass, COLOR_RESET,
			COLOR_BRIGHT_RED, fail, COLOR_RESET
		);
	} else {
		printf("%sPASS!%s -- %s%d Passed%s | %s%d Failed%s\n",
			COLOR_BRIGHT_GREEN, COLOR_RESET,
			COLOR_BRIGHT_GREEN, pass, COLOR_RESET,
			COLOR_BRIGHT_WHITE, fail, COLOR_RESET
		);
	}

	return ret;
}
