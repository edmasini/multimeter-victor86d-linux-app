#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include "hidapi.h"
#include <getopt.h>
#include <math.h>

#define VICTOR86D_VENDOR_ID 0x1244
#define VICTOR86D_PRODUCT_ID 0xd237

#define VICTOR86D_DIGIT0_0 0x81
#define VICTOR86D_DIGIT0_1 0x01
#define VICTOR86D_DIGIT0_2 0xC1
#define VICTOR86D_DIGIT0_3 0x41
#define VICTOR86D_DIGIT0_4 0xA1
#define VICTOR86D_DIGIT0_5 0x21
#define VICTOR86D_DIGIT0_6 0xE1
#define VICTOR86D_DIGIT0_7 0x61
#define VICTOR86D_DIGIT0_8 0x91
#define VICTOR86D_DIGIT0_9 0x11
#define VICTOR86D_DIGIT0__ 0x71

#define VICTOR86D_DIGIT1_0 0x6F
#define VICTOR86D_DIGIT1_1 0xEF
#define VICTOR86D_DIGIT1_2 0xAF
#define VICTOR86D_DIGIT1_3 0x2F
#define VICTOR86D_DIGIT1_4 0x8F
#define VICTOR86D_DIGIT1_5 0x0F
#define VICTOR86D_DIGIT1_6 0xCF
#define VICTOR86D_DIGIT1_7 0x4F
#define VICTOR86D_DIGIT1_8 0x7F
#define VICTOR86D_DIGIT1_9 0xFF
#define VICTOR86D_DIGIT1__ 0xBF

#define VICTOR86D_DIGIT2_0 0x71
#define VICTOR86D_DIGIT2_1 0xF1
#define VICTOR86D_DIGIT2_2 0xB1
#define VICTOR86D_DIGIT2_3 0x31
#define VICTOR86D_DIGIT2_4 0x91
#define VICTOR86D_DIGIT2_5 0x11
#define VICTOR86D_DIGIT2_6 0xD1
#define VICTOR86D_DIGIT2_7 0x51
#define VICTOR86D_DIGIT2_8 0x81
#define VICTOR86D_DIGIT2_9 0x01

#define VICTOR86D_DIGIT3_0 0x77
#define VICTOR86D_DIGIT3_1 0xF7
#define VICTOR86D_DIGIT3_2 0xB7
#define VICTOR86D_DIGIT3_3 0x37
#define VICTOR86D_DIGIT3_4 0x97
#define VICTOR86D_DIGIT3_5 0x17
#define VICTOR86D_DIGIT3__ 0x67
/*
#define VICTOR86D_DIGIT3_6	0x00
#define VICTOR86D_DIGIT3_7	0x00
#define VICTOR86D_DIGIT3_8	0x00
#define VICTOR86D_DIGIT3_9	0x00
*/

/* Flag set by ‘--verbose’. */
static int verbose_flag = 0;
static int rawtime_flag = 0;
static unsigned char loop_flag = 0;
static unsigned char show_time = 1;
static unsigned char json = 0;
unsigned char counter = 0;
int minus, decimal;
char *unit;
int digit[4] = {0, 0, 0, 0};

void victor86d_decode(unsigned char *buf, char *str)
{
	//Decode digit D0
	switch (buf[6])
	{
	case VICTOR86D_DIGIT0_0:
		digit[0] = 0;
		break;
	case VICTOR86D_DIGIT0_1:
		digit[0] = 1;
		break;
	case VICTOR86D_DIGIT0_2:
		digit[0] = 2;
		break;
	case VICTOR86D_DIGIT0_3:
		digit[0] = 3;
		break;
	case VICTOR86D_DIGIT0_4:
		digit[0] = 4;
		break;
	case VICTOR86D_DIGIT0_5:
		digit[0] = 5;
		break;
	case VICTOR86D_DIGIT0_6:
		digit[0] = 6;
		break;
	case VICTOR86D_DIGIT0_7:
		digit[0] = 7;
		break;
	case VICTOR86D_DIGIT0_8:
		digit[0] = 8;
		break;
	case VICTOR86D_DIGIT0_9:
		digit[0] = 9;
		break;
	case VICTOR86D_DIGIT0__:
		digit[0] = 0;
		break;
	default:
		printf("Invalid digit0 = 0x%x\n", buf[6]);
		break;
	}

	//Decode digit D1
	switch (buf[9])
	{
	case VICTOR86D_DIGIT1_0:
		digit[1] = 0;
		break;
	case VICTOR86D_DIGIT1_1:
		digit[1] = 1;
		break;
	case VICTOR86D_DIGIT1_2:
		digit[1] = 2;
		break;
	case VICTOR86D_DIGIT1_3:
		digit[1] = 3;
		break;
	case VICTOR86D_DIGIT1_4:
		digit[1] = 4;
		break;
	case VICTOR86D_DIGIT1_5:
		digit[1] = 5;
		break;
	case VICTOR86D_DIGIT1_6:
		digit[1] = 6;
		break;
	case VICTOR86D_DIGIT1_7:
		digit[1] = 7;
		break;
	case VICTOR86D_DIGIT1_8:
		digit[1] = 8;
		break;
	case VICTOR86D_DIGIT1_9:
		digit[1] = 9;
		break;
	case VICTOR86D_DIGIT1__:
		digit[1] = 0;
		break;
	default:
		printf("Invalid digit1 = 0x%x\n", buf[9]);
		break;
	}

	//Decode digit D2
	switch (buf[3])
	{
	case VICTOR86D_DIGIT2_0:
		digit[2] = 0;
		break;
	case VICTOR86D_DIGIT2_1:
		digit[2] = 1;
		break;
	case VICTOR86D_DIGIT2_2:
		digit[2] = 2;
		break;
	case VICTOR86D_DIGIT2_3:
		digit[2] = 3;
		break;
	case VICTOR86D_DIGIT2_4:
		digit[2] = 4;
		break;
	case VICTOR86D_DIGIT2_5:
		digit[2] = 5;
		break;
	case VICTOR86D_DIGIT2_6:
		digit[2] = 6;
		break;
	case VICTOR86D_DIGIT2_7:
		digit[2] = 7;
		break;
	case VICTOR86D_DIGIT2_8:
		digit[2] = 8;
		break;
	case VICTOR86D_DIGIT2_9:
		digit[2] = 9;
		break;
	default:
		printf("Invalid digit2 = 0x%x\n", buf[3]);
		break;
	}

	//Decode digit D3
	switch (buf[10])
	{
	case VICTOR86D_DIGIT3_0:
		digit[3] = 0;
		break;
	case VICTOR86D_DIGIT3_1:
		digit[3] = 1;
		break;
	case VICTOR86D_DIGIT3_2:
		digit[3] = 2;
		break;
	case VICTOR86D_DIGIT3_3:
		digit[3] = 3;
		break;
	case VICTOR86D_DIGIT3_4:
		digit[3] = 4;
		break;
	case VICTOR86D_DIGIT3_5:
		digit[3] = 5;
		break;
	case VICTOR86D_DIGIT3__:
		digit[3] = 0;
		break;
	default:
		printf("Invalid digit3 = 0x%x\n", buf[10]);
		break;
	}

	//Decode measurement unit
	switch (buf[8])
	{
	case 0xA9:
		unit = "oC";
		break;
	case 0xE9:
		unit = "oF";
		break;
	case 0x79:
		unit = "Hz";
		break;
	case 0x6B:
		switch (buf[12])
		{
		case 0x6A:
			unit = "uA";
			break;
		case 0x6B:
			unit = "mA";
			break;
		default:
			unit = "A";
			break;
		}
		break;
	case 0x89:
		unit = "F";
		break;
	case 0x6D:
		unit = "Ohm";
		break;
	case 0x6A:
		if (buf[12] == 0x6B)
		{
			unit = "mV";
		}
		else
		{
			unit = "V";
		}
		break;
	case 0x71:
		unit = "HFE";
		break;
	case 0x69:
		unit = "%";
		break;
	default:
		printf("Invalid unit = 0x%x\n", buf[8]);
		break;
	}

	//Decode decimal value
	decimal = (buf[5] & 0xE0) >> 5;
	if (decimal == 0x00)
	{
		decimal = 2;
	}
	else if (decimal == 0x06)
	{
		decimal = 1;
	}
	else if (decimal == 0x05)
	{
		decimal = 0;
	}

	minus = buf[4] & 0x01;

	sprintf(str, "%s%d%s%d%s%d%s%d",
			minus ? "-" : "",
			digit[3],
			decimal == 2 ? "." : "",
			digit[2],
			decimal == 1 ? "." : "",
			digit[1],
			decimal == 0 ? "." : "",
			digit[0]);

	return;
}

double get_seconds()
{
	struct timeval tv;
	double time;

	gettimeofday(&tv, NULL);
	time = (double)tv.tv_sec + (double)tv.tv_usec / 1000 / 1000;

	return time;
}

int check_elapsed_time(int now, int interval)
{
	static int last = 0;
	if (interval == 0)
		return 1;
	if ((now - last) >= interval)
	{
		last = now;
		return 1;
	}
	return 0;
}

void print_help()
{
	printf("Usage: victor86d [options]\n");
	printf("Options:\n");
	printf("   -a --value				Return read value only \n");
	printf("   -j --json				Return JSON object\n");
	printf("   -p --posix				Set time as UNIX time format\n");
	// printf("   -v --verbose				Turn on verbose output\n");
	printf("   -h --help				Print this help message\n");
	printf("Loop functions\n");
	printf("   -i --interval			Loop Interval in seconds (>1 second)\n");
	printf("   -d --duration			Duration of capture in seconds\n");
}

int main(int argc, char **argv)
{
	// int ret;
	// unsigned char buf[256];
	// char str[256];
	hid_device *handle;
	double now;
	double start;
	int duration = 0;
	// double interval = 1.0;
	int interval = 1;
	/* int option_index = 0; */
	// long int counter = 0;

	while (1)
	{
		/*int this_option_optind = optind ? optind : 1;*/
		int c;
		int option_index = 0;
		static struct option long_options[] = {
			{"value", no_argument, 0, 'a'},
			{"help", no_argument, 0, 'h'},
			{"verbose", no_argument, &verbose_flag, 1},
			{"posix", no_argument, 0, 'p'},
			{"json", no_argument, 0, 'j'},
			{"duration", required_argument, 0, 'd'},
			{"interval", required_argument, 0, 'i'},
			{0, 0, 0, 0}};

		c = getopt_long(argc, argv, "ahvpjd:i:", long_options, &option_index);

		if (c == -1)
			break;

		switch (c)
		{
		case 'i':
			if (optarg == NULL || atoi(optarg) == 0)
			{
				printf("ERROR: Missing or incorrect -i parameter\n\n");
				print_help();
				exit(1);
			}
			// interval = (double)atol(argv[optind]);
			interval = (double)atol(optarg);
			loop_flag = 1;
			break;
		case 'd':
			if (optarg == NULL || atoi(optarg) == 0)
			{
				printf("ERROR: Missing or incorrect -d parameter\n\n");
				print_help();
				exit(1);
			}
			duration = atoi(optarg);
			loop_flag = 1;
			break;
		case 'p':
			rawtime_flag = 1;
			break;
		case 'a':
			show_time = 0;
			break;
		case 'j':
			json = 1;
			break;
		case 'h':
		default:
			print_help();
			exit(1);
			break;
		}
	}

	start = get_seconds();

	handle = hid_open(VICTOR86D_VENDOR_ID, VICTOR86D_PRODUCT_ID, NULL);
	if (!handle)
	{
		printf("Cannot find Victor86D device.\n");
		exit(1);
	}

	void printfunc(unsigned int timeout)
	{
		int ret;
		unsigned char buf[256];
		char str[256];
		if (timeout > 0)
		{
			ret = hid_read_timeout(handle, buf, sizeof(buf), 100);
		}
		else
		{
			ret = hid_read(handle, buf, sizeof(buf));
		}
		if (ret <= 0 || buf[0] == 0)
			return;

		now = get_seconds();
		if (check_elapsed_time((int)now, interval) == 0)
			return;

		victor86d_decode(buf, str);

		char date[80];
		char tm[80];
		time_t rawtime;
		struct tm *timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		double integral;
		double s0 = modf(now, &integral);
		s0 = timeinfo->tm_sec + s0;
		strftime(date, 80, "%d-%m-%Y", timeinfo);
		strftime(tm, 80, "%H:%M:", timeinfo);

		if (counter > 0)
		{
			if (json == 1)
			{
				printf(",");
			}
			printf("\n");
		}
		counter = 1;

		if (json == 1)
		{
			printf("{");
		}

		// Print date
		if (show_time == 1)
		{

			if (rawtime_flag == 0)
			{
				if (json == 1)
				{
					printf("date: '%s', ", date);
				}
				else
				{
					printf("%s ", date);
				}
			}
			else
			{
				if (json == 1)
				{
					printf("timestamp: '%lf', ", now);
				}
				else
				{
					printf("%lf ", now);
				}
			}

			// print time
			if (rawtime_flag == 0)
			{
				if (json == 1)
				{
					printf("time: '%s%lf', ", tm, s0);
				}
				else
				{
					printf("%s%lf ", tm, s0);
				}
			}
		}

		// print value
		if (json == 1)
		{
			printf("value: %s,", str);
		}
		else
		{
			printf("%s ", str);
		}

		// print unit
		if (json == 1)
		{
			printf(" unit: '%s'", (char *)unit);
		}
		else
		{
			printf("%s", (char *)unit);
		}

		if (json == 1)
		{
			printf("}");
		}
		fflush(stdout);
	}

	if (loop_flag == 0)
	{
		printfunc(0);
		printf("\n");
	}
	else
	{
		if (duration > 0)
		{
			do
			{
				printfunc(10);
				// sleep(interval);
			} while ((now - start) < duration);
		}
		else
		{
			do
			{
				printfunc(10);
			} while (1);
		}
	}
	hid_close(handle);
	hid_exit();

	return 0;
}
