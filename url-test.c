#include "url.h"


int main(int argc, char **argv) {
	URL u = { 0 };

	url_parse(&u, "udp://239.255.1.1:8800");
	url_parse(&u, "UdP://128.201.1.1");
	url_parse(&u, "UdP://128.201.1.1/foo/bar/buz");
	url_parse(&u, "239.255.1.1:8800");
	url_parse(&u, "file:///mnt/data/ts.ts");
	url_parse(&u, "file://mnt/data/ts.ts");
	url_parse(&u, "/tmp/ts.ts");
	url_parse(&u, "ssh://git@github.com/vany-egorov/absolut.git");
	url_parse(&u, "http://127.0.0.1/google/com?foo=bar&bar=buz");
	url_parse(&u, "http://127.0.0.1/google/com?foo=bar&bar=buz");
	url_parse(&u, "http://127.0.0.1/google/com?foo=bar&bar=buz#section1");
	url_parse(&u, "http://127.0.0.1/google/com#section1");

	return 0;
}
