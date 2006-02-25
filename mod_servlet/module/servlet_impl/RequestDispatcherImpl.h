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
#ifndef REQUEST_DISPATCHER_IMPL_H
#define REQUEST_DISPATCHER_IMPL_H

#include <RequestDispatcher.h>

class RequestDispatcherImpl : public servlet::RequestDispatcher{
   std::string path;
public:
    RequestDispatcherImpl(const std::string& the_path) : path(the_path)
        { }
    void  forward(servlet::ServletRequest& request, servlet::ServletResponse& response) const;
    void  include(servlet::ServletRequest& request, servlet::ServletResponse& response) const;
};

#endif/*REQUEST_DISPATCHER_IMPL_H*/
