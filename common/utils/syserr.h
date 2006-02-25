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

#ifndef SYSERR_H
#define SYSERR_H

#include <string.h> //strerror
#include <stdexcept> //runtime_error
#include <string>
#include <errno.h>

class system_error : public std::runtime_error{
public:
    system_error(const std::string& func_name, int errno_val)
        : std::runtime_error(func_name + ": " +strerror(errno_val))
        { }
    virtual ~system_error() throw()
        { }
};

class not_found_error : public std::runtime_error {
public:
    not_found_error(const std::string& filename)
        : std::runtime_error(filename) { }
};

#endif//SYSERR_H
