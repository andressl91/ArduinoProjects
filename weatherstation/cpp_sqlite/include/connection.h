#ifndef CONNECTION
#define CONNECTION

#include <string>
#include <sqlite3.h>


class Connection {
    private:

    sqlite3 *DB;
    std::string db_path;

    public:
    Connection(std::string db_path);
    void query(std::string sql);
    void openDB();
    void closeDB();

};

#endif
