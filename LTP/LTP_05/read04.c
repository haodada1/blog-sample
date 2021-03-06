/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write the Free Software Foundation, Inc.
 *
 */
/**********************************************************
 *
 *    TEST IDENTIFIER   : read04
 *
 *    TEST TITLE        : Basic tests for read(2)
 *
 *    TEST CASE TOTAL   : 1
 *
 *    AUTHOR            : jitwxs
 *						  <jitwxs@foxmail.com>
 *
 *    DESCRIPTION
 *      Testcase to check if read returns the number of bytes read correctly.
 *
 *    ALGORITHM
 *		Create a file and write some bytes out to it.
 *		Attempt to read more than written.
 *		Check the return count, and the read buffer. The read buffer should be
 *		same as the write buffer.
 *
 **********************************************************/

#include <stdio.h>
#include <errno.h>
#include "tst_test.h"
#include "tst_safe_macros.h"

#define TST_SIZE 27
#define TST_DATA "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define FILENAME "tmpfile"

int rfd, wfd;

static void my_test(void)
{
	char prbuf[BUFSIZ];

	SAFE_WRITE(0, wfd, TST_DATA, TST_SIZE);

	rfd = SAFE_OPEN(FILENAME, O_RDONLY, 0700);
	if (rfd == -1)
		tst_res(TBROK | TTERRNO, "can't open for reading");

	TEST(read(rfd, prbuf, BUFSIZ));

	if (TEST_RETURN == -1)
		tst_res(TFAIL, "call failed unexpectedly");

	if (TEST_RETURN != TST_SIZE)
		tst_res(TFAIL, "Bad read count - got %ld - expected %d",
				TEST_RETURN, TST_SIZE);

	if (memcmp(TST_DATA, prbuf, TST_SIZE) != 0)
		tst_res(TFAIL, "read buffer not equal to write buffer");

	tst_res(TPASS, "functionality of read() is correct");
}

static void setup(void)
{
	wfd = SAFE_OPEN(FILENAME, O_WRONLY | O_CREAT, 0700);
	if (wfd == -1)
		tst_res(TBROK | TTERRNO,
				"open %s failed", FILENAME);
}

static void cleanup(void)
{
	if (rfd > 0)
		SAFE_CLOSE(rfd);
	if (wfd > 0)
		SAFE_CLOSE(wfd);
}

static struct tst_test test = {
	.test_all = my_test,
	.setup = setup,
	.cleanup = cleanup,
	.needs_tmpdir = 1
};
