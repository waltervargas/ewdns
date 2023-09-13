#include <stdio.h>
#include <string.h>

void dns_query_to_hex(const char* buf, char *hex_str, int recv_len) {
    // Pointer to the next position in hex_str to write to
    char *hex_ptr = hex_str;

    for (int i = 0; i < recv_len; i++) {
        // Write the two hex digits for buf[i] into hex_ptr
        snprintf(hex_ptr, 3, "%02X", buf[i]);
        hex_ptr += 2;
    }

    // Null-terminate the string
    *hex_ptr = '\0';
}

void packet_debug_table_to_buffer(const unsigned char *buf, int len, char *out, int out_size) {
    // Pointers for output buffer manipulation
    char *out_ptr = out;
    int written;

    // Add Title
    written = snprintf(out_ptr, out_size, "\nDebugging Packet:\n");
    out_ptr += written;
    out_size -= written;

    // Add Header
    written = snprintf(out_ptr, out_size, "+------+----------------+----------------+\n");
    out_ptr += written;
    out_size -= written;

    written = snprintf(out_ptr, out_size, "| Byte |      Hex       |      ASCII      |\n");
    out_ptr += written;
    out_size -= written;

    written = snprintf(out_ptr, out_size, "+------+----------------+----------------+\n");
    out_ptr += written;
    out_size -= written;

    for (int i = 0; i < len; ++i) {
        // Print byte position
        written = snprintf(out_ptr, out_size, "| %4d |", i);
        out_ptr += written;
        out_size -= written;

        // Print hex value of the byte
        written = snprintf(out_ptr, out_size, "     %02X      |", buf[i]);
        out_ptr += written;
        out_size -= written;

        // Print ASCII value of the byte, but only if it's printable
        if (buf[i] >= 32 && buf[i] <= 126) {
            written = snprintf(out_ptr, out_size, "       %c       |\n", buf[i]);
        } else {
            written = snprintf(out_ptr, out_size, "    (N/A)      |\n");
        }
        out_ptr += written;
        out_size -= written;

        // Print row delimiter
        written = snprintf(out_ptr, out_size, "+------+----------------+----------------+\n");
        out_ptr += written;
        out_size -= written;
    }
}


void format_packet_RFC_style(const unsigned char *packet, int len, char *buffer, int buffer_len) {
    memset(buffer, 0, buffer_len);  // Clear the entire buffer
    char *p = buffer;
    int remaining_len = buffer_len;
    int written = 0;

    // Title
    written = snprintf(p, remaining_len, "Byte#   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
    p += written;
    remaining_len -= written;

    // Packet content
    for (int i = 0; i < len; i += 16) {
        // Row header
        written = snprintf(p, remaining_len, "%04X   ", i);
        p += written;
        remaining_len -= written;

        // Hex content
        for (int j = 0; j < 16; ++j) {
            if (i + j < len) {
                written = snprintf(p, remaining_len, "%02X ", packet[i + j]);
            } else {
                written = snprintf(p, remaining_len, "   ");  // fill in if no more data
            }
            p += written;
            remaining_len -= written;
        }

        // ASCII content
        written = snprintf(p, remaining_len, " ");
        p += written;
        remaining_len -= written;

        for (int j = 0; j < 16; ++j) {
            if (i + j < len && packet[i + j] >= 32 && packet[i + j] <= 126) {
                written = snprintf(p, remaining_len, "%c", packet[i + j]);
            } else {
                written = snprintf(p, remaining_len, ".");
            }
            p += written;
            remaining_len -= written;
        }

        // End of line
        written = snprintf(p, remaining_len, "\n");
        p += written;
        remaining_len -= written;
    }
}
