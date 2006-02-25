#include <string>
#include <map>
#include <mysql/mysql.h>
#include <boost/shared_ptr.hpp>
#include <stdexcept>

class mysql_exception : public std::runtime_error {
public:
    mysql_exception(MYSQL* mysql, const std::string& msg) : 
        runtime_error(msg + ": " + mysql_error(mysql))
        { }
};

class Mysql{
public:
    Mysql();    
    ~Mysql();

    typedef std::map<std::string,std::string> params_t;
    MYSQL *handle;

    inline std::string get(MYSQL_ROW row,unsigned i)
        { return (row[i]?row[i]:""); }

    boost::shared_ptr<MYSQL_RES> query(const std::string& query);
    
    void execute(const std::string& sql) {
        if(mysql_real_query(handle, sql.c_str(), sql.size()) != 0)
            throw mysql_exception(handle, "mysql_real_query("+sql+")");
    }

    std::string prepare(std::string query, const params_t& params);
};

extern Mysql mysql;
