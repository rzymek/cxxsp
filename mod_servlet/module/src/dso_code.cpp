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
#include "dso_code.h"
#include <utils/aprerr.h>
#include <utils/dsoerr.h>
#include <apr_file_info.h>

#include <iostream> //dbg

using namespace std;

dso_code::dso_code(const string& a_filename) : filename(a_filename) {
//     cout << "dso_code\t" << filename << endl;
    load();
    try{
        mtime = file_mtime();
    }catch(...){
        unload();
        throw;
    }
}

apr_time_t dso_code::file_mtime() {
    apr_finfo_t finfo;
    apr_status_t status = apr_stat(
        &finfo,
        filename.c_str(),
        APR_FINFO_MTIME,
        mem_pool
    );
    if(status != APR_SUCCESS)
        throw apr_error("apr_stat", status);
    return finfo.mtime;
}

void dso_code::load() {
//     cout << "dso_code::load\t" << filename << endl;
    apr_status_t status = apr_pool_create(&mem_pool, NULL);
    if(status != APR_SUCCESS)
        throw apr_error("apr_pool_create", status);
    status = apr_dso_load(&handle, filename.c_str(), mem_pool);
    if(status != APR_SUCCESS) {
        apr_pool_destroy(mem_pool);
        throw dso_error("apr_dso_load", handle);
    }

}
void dso_code::unload() {
//     cout << "dso_code::unload" << endl;
    apr_pool_destroy(mem_pool);
    if(apr_dso_unload(handle) != APR_SUCCESS)
            throw dso_error("apr_dso_unload", handle);
}

bool dso_code::reload_if_needed(apr_time_t mtime) {
//     cout << "dso_code::reload_if_changed\t"
//          << "filename: "<<filename << endl;
//     cout << "this->mtime: " << this->mtime << endl;
//     cout << "      mtime: " << mtime << endl;
    if(this->mtime != mtime) {
        unload();
        load();
        this->mtime = mtime;
        return true;
    }
    return false;
}

bool dso_code::reload_if_needed() {
    apr_time_t mtime = file_mtime();
    return reload_if_needed(mtime);
}

template<>
void* dso_code::call<void*>(const std::string& name) {
//     cout << "dso_code::call\t"<<name << endl;
    apr_dso_handle_sym_t sym;
    if(apr_dso_sym(&sym, handle, name.c_str()) != APR_SUCCESS)
            throw dso_error("apr_dso_sym", handle);

    typedef void*(*dso_func_t) ();
    dso_func_t dso_func = (dso_func_t) sym;
    return dso_func();
}
