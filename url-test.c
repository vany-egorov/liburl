#include "url.h"


static const char *raws[] = {
	"udp://239.255.1.1:8800",
	"UdP://128.201.1.1",
	"UdP://128.201.1.1/foo/bar/buz",
	"239.255.1.1:8800",
	"file:///mnt/data/ts.ts",
	"file://mnt/data/ts.ts",
	"/tmp/ts.ts",
	"ssh://git@github.com/vany-egorov/absolut.git",
	"http://127.0.0.1/google/com?foo=bar&bar=buz",
	"http://127.0.0.1/google/com?foo=bar&bar=buz#section1",
	"http://127.0.0.1/google/com#section1",
	"/221.1.1.1:5500",
	NULL,
};


int main(int argc, char **argv) {
	URL u = { 0 };
	char json[255] = { 0 };
	char s[255] = { 0 };
	const char **raw;

	for (raw=raws; *raw; raw++) {
		memset(json, 0, sizeof(json));
		memset(s, 0, sizeof(s));

		url_parse(&u, *raw);
		url_sprint_json(&u, json, sizeof(json));
		url_sprint(&u, s, sizeof(s));

		printf("===>\n");
		printf("%s\n", *raw);
		printf("%s\n", s);
		printf("%s\n", json);
	}

	return 0;
}
