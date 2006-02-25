/*
 *  Copyright (C) 2005 Krzysztof Rzymkowski <rzymek@users.sourceforge.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <sys/types.h>//dbg
#include <signal.h>//dbg
#include <unistd.h>//dbg
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

inline void dbg_stop() {
//     std::cout << "attach " << getpid() << std::endl;
    char buf[64];
    snprintf(buf, 64, "xterm -e gdb attach %d&", (int)getpid());
    system(buf);
    kill(getpid(), 19/*STOP*/);
}
