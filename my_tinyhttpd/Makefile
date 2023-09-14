all: htppd client

httpd: httpd.c
	gcc $< -o $@ -Wall

client: client.c
	gcc $< -o $@ -Wall

.PHONY: clean
clean:
	rm -rf client httpd
