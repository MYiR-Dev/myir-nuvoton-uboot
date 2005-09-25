#include <stdio.h>
#include <stdlib.h>

#if defined(__linux__)
#include <stdint.h>
#else
#ifdef __CYGWIN__
#include "elf.h"
#else
#include <inttypes.h>
#endif
#endif

typedef struct bitmap_s {		/* bitmap description */
	uint16_t width;
	uint16_t height;
	uint8_t	palette[256*3];
	uint8_t	*data;
} bitmap_t;

#define DEFAULT_CMAP_SIZE	16	/* size of default color map	*/

/*
 * Neutralize little endians.
 */
uint16_t le_short(uint16_t x)
{
    uint16_t val;
    uint8_t *p = (uint8_t *)(&x);

    val =  (*p++ & 0xff) << 0;
    val |= (*p & 0xff) << 8;

    return val;
}

void skip_bytes (FILE *fp, int n)
{
	while (n-- > 0)
		fgetc (fp);
}

int main (int argc, char *argv[])
{
	int	i, x;
	FILE	*fp;
	bitmap_t bmp;
	bitmap_t *b = &bmp;
	uint16_t data_offset, n_colors;

	if (argc < 2) {
		fprintf (stderr, "Usage: %s file\n", argv[0]);
		exit (EXIT_FAILURE);
	}

	if ((fp = fopen (argv[1], "rb")) == NULL) {
		perror (argv[1]);
		exit (EXIT_FAILURE);
	}

	if (fgetc (fp) != 'B' || fgetc (fp) != 'M') {
		fprintf (stderr, "%s is not a bitmap file.\n", argv[1]);
		exit (EXIT_FAILURE);
	}

	/*
	 * read width and height of the image, and the number of colors used;
	 * ignore the rest
	 */
	skip_bytes (fp, 8);
	fread (&data_offset, sizeof (uint16_t), 1, fp);
	skip_bytes (fp, 6);
	fread (&b->width,   sizeof (uint16_t), 1, fp);
	skip_bytes (fp, 2);
	fread (&b->height,  sizeof (uint16_t), 1, fp);
	skip_bytes (fp, 22);
	fread (&n_colors, sizeof (uint16_t), 1, fp);
	skip_bytes (fp, 6);

	/*
	 * Repair endianess.
	 */
	data_offset = le_short(data_offset);
	b->width = le_short(b->width);
	b->height = le_short(b->height);
	n_colors = le_short(n_colors);

	/* assume we are working with an 8-bit file */
	if ((n_colors == 0) || (n_colors > 256 - DEFAULT_CMAP_SIZE)) {
		/* reserve DEFAULT_CMAP_SIZE color map entries for default map */
		n_colors = 256 - DEFAULT_CMAP_SIZE;
	}

	printf ("/*\n"
		" * Automatically generated by \"tools/bmp_logo\"\n"
		" *\n"
		" * DO NOT EDIT\n"
		" *\n"
		" */\n\n\n"
		"#ifndef __BMP_LOGO_H__\n"
		"#define __BMP_LOGO_H__\n\n"
		"#define BMP_LOGO_WIDTH\t\t%d\n"
		"#define BMP_LOGO_HEIGHT\t\t%d\n"
		"#define BMP_LOGO_COLORS\t\t%d\n"
		"#define BMP_LOGO_OFFSET\t\t%d\n"
		"\n",
		b->width, b->height, n_colors,
		DEFAULT_CMAP_SIZE);

	/* allocate memory */
	if ((b->data = (uint8_t *)malloc(b->width * b->height)) == NULL) {
		fclose (fp);
		printf ("Error allocating memory for file %s.\n", argv[1]);
		exit (EXIT_FAILURE);
	}

	/* read and print the palette information */
	printf ("unsigned short bmp_logo_palette[] = {\n");

	for (i=0; i<n_colors; ++i) {
		b->palette[(int)(i*3+2)] = fgetc(fp);
		b->palette[(int)(i*3+1)] = fgetc(fp);
		b->palette[(int)(i*3+0)] = fgetc(fp);
		x=fgetc(fp);

		printf ("%s0x0%X%X%X,%s",
			((i%8) == 0) ? "\t" : "  ",
			(b->palette[(int)(i*3+0)] >> 4) & 0x0F,
			(b->palette[(int)(i*3+1)] >> 4) & 0x0F,
			(b->palette[(int)(i*3+2)] >> 4) & 0x0F,
			((i%8) == 7) ? "\n" : ""
		);
	}

	/* seek to offset indicated by file header */
	fseek(fp, (long)data_offset, SEEK_SET);

	/* read the bitmap; leave room for default color map */
	printf ("\n");
	printf ("};\n");
	printf ("\n");
	printf ("unsigned char bmp_logo_bitmap[] = {\n");
	for (i=(b->height-1)*b->width; i>=0; i-=b->width) {
		for (x = 0; x < b->width; x++) {
			b->data[(uint16_t) i + x] = (uint8_t) fgetc (fp) \
						+ DEFAULT_CMAP_SIZE;
		}
	}
	fclose (fp);

	for (i=0; i<(b->height*b->width); ++i) {
		if ((i%8) == 0)
			putchar ('\t');
		printf ("0x%02X,%c",
			b->data[i],
			((i%8) == 7) ? '\n' : ' '
		);
	}
	printf ("\n"
		"};\n\n"
		"#endif /* __BMP_LOGO_H__ */\n"
	);

	return (0);
}
