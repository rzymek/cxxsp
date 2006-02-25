package store;


import javax.servlet.ServletException;
import java.sql.*;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;


public class Mysql {
    private Mysql() {
    }

    private static Map pool = new TreeMap();

    public static Connection get() throws ServletException {
        Integer thread_id = new Integer(Thread.currentThread().hashCode());
        Connection connection = (Connection) pool.get(thread_id);
        if(connection==null) {
            connection = createNew();
            pool.put(thread_id, connection);
        }
        return connection;
    }

    private static Connection createNew() throws ServletException {
        try {
            return DriverManager.getConnection("jdbc:mysql://localhost/internet");
        } catch (Throwable e) {
            e.printStackTrace();
            throw new ServletException(e);
        }
    }

    public synchronized void release(Connection conn) {
        try {
            conn.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    public static void free(ResultSet result, Statement statement) {
        if (result != null) {
            try {
                result.close();
            } catch (SQLException ex) {  ex.printStackTrace(); }
            if (statement != null) {
                try {
                    statement.close();
                } catch (SQLException ex) {  ex.printStackTrace(); }
            }
        }
    }

    public static ResultSet query(Connection connection, String sql) throws SQLException {
        return connection.createStatement().executeQuery(sql);
    }

    public static String prepare(String query1, Map params) {
        StringBuffer query = new StringBuffer(query1);
        for(int i=0; i<query.length(); ++i){
            if(query.charAt(i)=='$') {
                int j;
                for(j = i+1; j<query.length() ; ++j) {
                    if(Character.isLetterOrDigit(query.charAt(j)) || query.charAt(j)=='_')
                        continue;
                    break;
                }
                String var = query.substring(i+1, j);
                final String val = (String) params.get(var);
                if(val == null)
                    throw new RuntimeException("Parameter ["+var+"] not found in attribs");
//                char buf[250];
//                mysql_real_escape_string(
//                    handle,
//                    buf,
//                    val->second.c_str(),
//                    val->second.size()
//                );
                query.replace(i, j, val);
                i = i + val.length();
                if(i>=query.length())
                    break;
            }
        }
        return query.toString();
    }

    public static boolean execute(Connection connection, String sql) throws SQLException {
        return connection.createStatement().execute(sql);
    }

}
