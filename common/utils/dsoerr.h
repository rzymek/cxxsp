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

#ifndef DSOERR_H
#define DSOERR_H

#include <stdexcept> //runtime_error
#include <string>
#include <apr_dso.h>

class dso_error : public std::exception{
        std::string msg;
public:
        dso_error(const std::string& func_name, apr_dso_handle_t *handle) {
                static const apr_size_t buf_size = 256;
                char buf[buf_size];
                msg = func_name + ": " + apr_dso_error(handle, buf, buf_size);
        }
        virtual const char* what() const throw()
                { return msg.c_str(); }
        virtual ~dso_error() throw()
                { }
};


#endif//DSOERR_H

