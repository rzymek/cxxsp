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

#include <utils/http.h>
#include <httpd.h> //ap_unescape_url
#include <boost/scoped_array.hpp>
#include <stdexcept>

using namespace std;

string unescape(const string& str) {
    boost::scoped_array<char> buf(new char[str.size()+1]);
    strcpy(buf.get(), str.c_str());
    if (ap_unescape_url(buf.get()) != 0)
            throw runtime_error("ap_unescape_url failed for " + str);
    return buf.get();
}
