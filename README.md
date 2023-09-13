# ewdns

As for now ewdns is a dns echo

``` sh
❯❯❯ meson setup build
❯❯❯ meson test -C build -v
❯❯❯ ./build/dnsproxy & 
[2] 69225
```

``` sh
❯❯❯ dig @localhost -p 5300  google.com -4 TXT
received DNS query (HEX): FF500120000100000000000106676F6F676C6503636F6D000010000100002904FF00000000000C000A000800FFFF3FFF08FF3A
Byte#   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
0000   FA 50 01 20 00 01 00 00 00 00 00 01 06 67 6F 6F  .P. .........goo
0010   67 6C 65 03 63 6F 6D 00 00 10 00 01 00 00 29 04  gle.com.......).
0020   D0 00 00 00 00 00 0C 00 0A 00 08 00 B5 88 3F D5  ..............?.
0030   08 8A 3A                                         ..:.............
;; Warning: query response not set

; <<>> DiG 9.18.16-1-Debian <<>> @localhost -p 5300 google.com -4 TXT
; (2 servers found)
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 64080
;; flags: rd ad; QUERY: 1, ANSWER: 0, AUTHORITY: 0, ADDITIONAL: 1
;; WARNING: recursion requested but not available

;; OPT PSEUDOSECTION:
; EDNS: version: 0, flags:; udp: 1232
; COOKIE: 00b5883fd5088a3a (echoed)
;; QUESTION SECTION:
;google.com.			IN	TXT

;; Query time: 0 msec
;; SERVER: 127.0.0.1#5300(localhost) (UDP)
;; WHEN: Wed Sep 13 02:55:23 CEST 2023
;; MSG SIZE  rcvd: 51
```
