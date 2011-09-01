#include	"soupcup32.h"

#include	"fs.h"
#include	"serial.h"

#include	"config.h"

#define	MAX_LINE_LENGTH 192

unsigned char ser_linebuf[MAX_LINE_LENGTH];
int ser_linebuf_p;

int sd_printing = 0;
unsigned char sd_linebuf[MAX_LINE_LENGTH];
int sd_linebuf_p;


void process_line(unsigned char *line) {
}

void soupcup32() {
	if (fs_init() != FR_OK) {
		serial_writestr("SD init failed! rebooting!\n");
		return;
	}

	for (;;) {
		if (serial_rxchars()) {
			unsigned char c;
			ser_linebuf[ser_linebuf_p++] = c = serial_popchar();
			if (c < 32) {
				ser_linebuf[ser_linebuf_p] = 0;
				ser_linebuf_p = 0;
				process_line(ser_linebuf);
			}
		}
		
		if (sd_printing) {
			int n = 0;
			unsigned char c;
			if (f_read(&file, &c, 1, &n) == FR_OK) {
				if (n > 0) {
					sd_linebuf[sd_linebuf_p] = c;
					sd_linebuf_p += n;
					if (c < 32) {
						sd_linebuf[sd_linebuf_p] = 0;
						sd_linebuf_p = 0;
						process_line(sd_linebuf);
					}
				}
			}
			else {
				sd_printing = 0;
			}
		}
	}
}