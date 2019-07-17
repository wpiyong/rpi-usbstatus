/*
* Test suite program based of libusb-0.1-compat testlibusb
* Copyright (c) 2013 Nathan Hjelm <hjelmn@mac.ccom>
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include <stdio.h>
#include <string.h>
#include <libusb.h>

#if defined(_MSC_VER) && (_MSC_VER < 1900)
#define snprintf _snprintf
#endif

int verbose = 0;

#define VENDOR_ID      0x1992   // avantes LLC
#define PRODUCT_ID     0x0668   // spectrometer

/* We use a global variable to keep the device handle
 */
static struct libusb_device_handle *devh = NULL;

int main(int argc, char *argv[])
{
	libusb_device **devs;
	ssize_t cnt;
	int r, i;

	struct libusb_device_descriptor desc;
	int ret;

	if (argc > 1 && !strcmp(argv[1], "-v"))
		verbose = 1;

	r = libusb_init(NULL);
	if (r < 0)
		return r;

//	cnt = libusb_get_device_list(NULL, &devs);
//	if (cnt < 0)
//		return (int)cnt;
//
//	for (i = 0; devs[i]; ++i) {
//		ret = libusb_get_device_descriptor(devs[i], &desc);
//		if (ret < 0) {
//			fprintf(stderr, "failed to get device descriptor");
//			return -1;
//		} else {
//			if(desc.idVendor == VENDOR_ID && desc.idProduct == PRODUCT_ID) {
//				fprintf(stderr, "found device\n");
//				ret = libusb_open(devs[i], &devh);
//				fprintf(stderr, "open device: %d\n", ret);
//
//				if(ret == 0){
//					int if_num = 0;
//					//for(if_num = 0; if_num < 2; if_num++)
//					{
//						int rc = libusb_claim_interface(devh, if_num);
//						if (rc < 0) {
//							fprintf(stderr, "Error claiming interface: %d\n", rc);
//						} else {
//							fprintf(stdout, "succeed in claiming interface: %d\n", if_num);
//							fprintf(stdout, "Releasing interface %d...\n", if_num);
//							libusb_release_interface(devh, if_num);
//
//							fprintf(stdout, "Closing device...\n");
//							libusb_close(devh);
//						}
//					}
//				}
//				break;
//			}
//
//		}
//	}
//
//	libusb_free_device_list(devs, 1);
	int count = 0;
	while(1) {
		// sleep 5 seconds
		sleep(5);
		devh = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, PRODUCT_ID);
		if (!devh) {
			fprintf(stderr, "Error finding USB device\n");
		} else {
			fprintf(stdout, "succeed in opening device and getting device handler\n");
			int if_num = 0;
			int rc = libusb_claim_interface(devh, if_num);
			if (rc < 0) {
				fprintf(stderr, "Error claiming interface: %d\n", rc);
				fprintf(stderr, "Error claiming interface: %s\n", libusb_strerror((enum libusb_error)rc));
				count = 0;
			} else {
				fprintf(stdout, "succeed in claiming interface: %d\n", if_num);
				fprintf(stdout, "Releasing interface %d...\n", if_num);
				libusb_release_interface(devh, if_num);

				fprintf(stdout, "Closing device...\n");
				libusb_close(devh);
				count++;
			}

			if(count >= 3) {
				// reboot the system
				fprintf(stdout, "reboot...\n");
				system("sudo reboot");
			}
		}
	}

	libusb_exit(NULL);
	return 0;
}
