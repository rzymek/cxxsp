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
#include "wrapper.h"
/*
     #include <execinfo.h>
     #include <stdio.h>
     #include <stdlib.h>
void print_trace() {
    const size_t SIZE = 50;
    void *array[SIZE];
    size_t size;
    char **strings;
    size_t i;

    size = backtrace (array, SIZE);
    strings = backtrace_symbols (array, size);

    printf ("Obtained %zd stack frames.\n"
    "=====================================================\n", size);

    for (i = 0; i < size; i++)
        printf ("%s\n", strings[i]);
    printf("-------------------------------------------------------\n");
    free (strings);
}
*/
#include <iostream>
using namespace std;

wrapper::wrapper() {
}
wrapper::wrapper(const wrapper& rhs) : value(rhs.value){
    ++value->ref_count;
}

wrapper& wrapper::operator=(const wrapper& rhs) {
    if(value == rhs.value)
        return *this;
    if(--value->ref_count == 0)
        delete value;
    value = rhs.value;
    ++value->ref_count;
    return *this;
};

wrapper::wrapper(void* an_obj, free_t a_func)
    : value(new value_t(an_obj,a_func))
{
}
wrapper::~wrapper() {
    if(--value->ref_count == 0)
        delete value;
}


