/*
* Copyright 2008 Free Software Foundation, Inc.
*
*
* This software is distributed under the terms of the GNU Affero Public License.
* See the COPYING file in the main directory for details.
*
* This use of this software may be subject to additional restrictions.
* See the LEGAL file in the main directory for details.

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Affero General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/




#include "Sockets.h"
#include "Threads.h"
#include <stdio.h>
#include <stdlib.h>


static const int gNumToSend = 10;


void *testReaderIP(void *param)
{
	UDPSocket *readSocket = (UDPSocket *)param;
	readSocket->nonblocking();
	int rc = 0;
	while (rc<gNumToSend) {
		char buf[MAX_UDP_LENGTH];
		int count = readSocket->read(buf, MAX_UDP_LENGTH);
		if (count>0) {
			CERR("read: " << buf);
			rc++;
		} else {
			sleep(2);
		}
	}
	return NULL;
}

int main(int argc, char * argv[] )
{

  UDPSocket readSocket("127.0.0.1", 0);
  UDPSocket socket1("127.0.0.1", 0, "localhost", readSocket.port());

  CERR("socket1: " << socket1.port() << ", readSocket: " << readSocket.port());

  Thread readerThreadIP;
  readerThreadIP.start(testReaderIP, &readSocket);

  // give the readers time to open
  sleep(1);

  for (int i=0; i<gNumToSend; i++) {
    socket1.write("Hello IP land");
    sleep(1);
  }

  readerThreadIP.join();

  printf("Done\n");
}

// vim: ts=4 sw=4
