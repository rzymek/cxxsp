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
#ifndef IPR_HPP
#define IPR_HPP

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <memory>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
//~ #include <boost/archive/text_iarchive.hpp>
//~ #include <boost/archive/text_oarchive.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/read_write_mutex.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/any.hpp>
#include <boost/shared_ptr.hpp>
#include "unix_socket.h"

class ipr {
    //ipr is a class and not a namespace because I want this private:
    typedef boost::archive::binary_iarchive iarchive;
    typedef boost::archive::binary_oarchive oarchive;
    //~ typedef boost::archive::text_iarchive iarchive;
    //~ typedef boost::archive::text_oarchive oarchive;
    typedef boost::iostreams::stream<boost::iostreams::file_descriptor> fdstream;

    enum operation_type { OT_SET, OT_GET, OT_HAS, OT_OPEN, OT_LIST, OT_REMOVE };

    //====================================================================
public:
    class server {
        typedef std::map<std::string, std::string> map_type;

        void main_loop(net::unix_socket& sock);

        void process(int sockfd);
        server(const std::string& addr_path);

        struct repository {
            repository() : mutex(new boost::mutex()) { }
            boost::shared_ptr<boost::mutex> mutex;
            map_type data;
            std::string id;
        };
        typedef std::map<std::string, repository> reps_map;

        boost::mutex repositories_mutex;

        reps_map repositories;
    public:
        static pid_t dispatch(const std::string& addr_path);
    };

    //====================================================================

    class client {
        net::unix_socket sock;
        fdstream stream;
        void sync();
    public:
        client(const std::string& addr_path);

        void open(const std::string& rep);

        std::string get(const std::string& id); //operates just like std::map::operator[]

        void set(const std::string& id, const std::string& value);

        bool has(const std::string& id);
        std::auto_ptr< std::vector<std::string> > get_names();
        void remove(const std::string& id);
    };
};


#endif//IPR_HPP

