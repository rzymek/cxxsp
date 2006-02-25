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
#ifndef DSO_CODE_H
#define DSO_CODE_H

#include <string>
#include <apr_dso.h>
#include <apr_time.h>

class dso_code {
    apr_pool_t *mem_pool;
    apr_dso_handle_t *handle;
    apr_time_t mtime; //modification time of the dso file
    std::string filename;

    void load();
    void unload();
    //Returns the mtime of the 'filename'
    apr_time_t file_mtime();
public:
    dso_code(const std::string& filename);
    dso_code(const std::string& a_filename, apr_time_t mtime) : filename(a_filename)
        { load(); this->mtime = mtime; }
    ~dso_code()
        { unload(); }
    bool reload_if_needed(apr_time_t mtime);
    bool reload_if_needed();
    bool file_changed()
        { return file_mtime() > mtime; }
    template<typename T>
    T call(const std::string& name);
};

template<typename T>
T dso_code::call(const std::string& name) {
    return reinterpret_cast<T>( call<void*>(name) );
}



#endif//DSO_CODE_H

