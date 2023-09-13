#ifndef UTILS_H_
#define UTILS_H_

/* dns_query_to_hex transforms a DNS query into hex format */
void dns_query_to_hex(const unsigned char* buf, char *hex_str, int recv_len);

// This function constructs a "packet" debug table in both hex and ASCII,
// and stores it in the output buffer 'out'.
void packet_debug_table_to_buffer(const unsigned char *buf, int len, char *out, int out_size);

// Fills in a buffer with a graphical representation of the DNS packet, closely mimicking the DNS RFCs.
void format_packet_RFC_style(const unsigned char *packet, int len, char *buffer, int buffer_len);

#endif // UTILS_H_
