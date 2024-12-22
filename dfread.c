/*-
 * dfread, a util that helps with reading dev files.
 * Copyright (C) 2022 moatx
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

static inline _Noreturn void
usage(const char *name)
{
	fprintf(stderr, "usage: %s filename\n", name);
	exit(1);
}

int
main(int argc, char **argv)
{
	char *str;
	FILE *fp;

	if (argc != 2 || argc > 2)
		usage(argv[0]);

	if ((fp = fopen(argv[1], "r")) == NULL)
		goto out;

	//(void) fread(str, 1080, 90, fp);
	fgets(str, 1080, fp);

	if (ferror(fp))
		goto out;

	/* remove trailing newline */
	str[strcspn(str, "\n")] = ' ';

	puts(str);
out:
	if (fclose(fp) != 0)
		err(EXIT_FAILURE, " ");
	return 0;
}
