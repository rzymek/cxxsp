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

#include <stdlib.h>

#include <utils/dir.h>
#include <utils/syserr.h>

std::string get_cwd() {
    size_t size = 256;
    char* buf = (char*)malloc(size);
    for(;;){
        if(getcwd(buf, size) == NULL) {
            if(errno == ERANGE) {
                size = 2*size;
                buf = (char*)realloc(buf, size);
            }else{
                free(buf);
                throw system_error("getcwd",errno);
            }
        }else break;
    }
    std::string cwd = buf;
    free(buf);
    return cwd;
}
