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
#ifndef APACHESTREAM_H
#define APACHESTREAM_H

#include <httpd.h>
#include <http_protocol.h>



#include <boost/iostreams/concepts.hpp> // source
#include <boost/iostreams/stream.hpp>

class apache_sink : public boost::iostreams::sink {
    request_rec* r;
public:
    apache_sink(request_rec *request) : r(request)
        {  }
    std::streamsize write(const char* s, std::streamsize n) {
        const int c = ap_rwrite(s, n, r);
        if(c < 0)
            throw std::ios_base::failure("ap_rwrite failed");
        return static_cast<std::streamsize>(c);
    };
};


typedef boost::iostreams::stream<apache_sink> oapachestream;

//~ #include <streambuf>
//~ #include <iostream>

//~ class apache_streambuf : public std::streambuf {
    //~ request_rec* r;
//~ public:
    //~ apache_streambuf(request_rec* request) : r(request) { }
    //~ std::streamsize xsputn(const char* s, std::streamsize n) {
        //~ return static_cast<std::streamsize>(ap_rwrite(s,n,r));
    //~ }
//~ };
//~ class oapachestream : public std::ostream {
    //~ apache_streambuf buf;
//~ public:
    //~ oapachestream(request_rec* r) : std::ostream(&buf),buf(r) { }
//~ };



#endif//APACHESTREAM_H

