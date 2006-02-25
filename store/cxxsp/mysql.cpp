#include "mysql.h"
#include <stdexcept>

using namespace std;

Mysql mysql;

Mysql::Mysql() {
    handle = mysql_init(NULL);
    handle = mysql_real_connect(handle, "localhost","","","internet",3306,"/var/run/mysqld/mysqld.sock",0);
    if(handle==NULL)
        throw runtime_error("Unable to connec to database");
}

Mysql::~Mysql() {
    mysql_close(handle);
}

boost::shared_ptr<MYSQL_RES> Mysql::query(const std::string& query) {
    execute(query);
    boost::shared_ptr<MYSQL_RES> result(mysql_use_result(handle), mysql_free_result);
    if(result.get() == NULL)
        throw mysql_exception(handle, "mysql_use_result");
    return result;
}


std::string Mysql::prepare(std::string query, const params_t& params) {
    for(std::string::size_type i=0; i<query.size(); ++i){
        if(query[i]=='$') {
            std::string::size_type j;
            for(j = i+1; j<query.size() ; ++j) {
                if(isalnum(query[j]) || query[j]=='_')
                    continue;
                break;
            }
            std::string var = query.substr(i+1, j-(i+1));
            params_t::const_iterator val = params.find(var);
            if(val == params.end())
                throw std::runtime_error("Parameter ["+var+"] not found in attribs");
            char buf[250];
            mysql_real_escape_string(
                handle,
                buf,
                val->second.c_str(),
                val->second.size()
            );
            query.replace(i, j-i, buf);
            i = i + val->second.size();
            if(i>=query.size())
                break;
        }
    }
    return query;
}
