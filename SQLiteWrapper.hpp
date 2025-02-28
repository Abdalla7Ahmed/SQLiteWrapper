#ifndef SQLITE_WRAPPER_H
#define SQLITE_WRAPPER_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
class SQLiteWrapper
{
public:
    enum class MessagType : unsigned char
    {
        INFO,
        ERROR,
        QUERY
    };

    enum class LogsLevel : unsigned char
    {
        INFO,
        ERROR,
        QUERY,
        ENABLE_ALL,
        DISABLE_ALL
    };
    enum class Constraints : unsigned char
    {
        NO_CONSTRAINTS = 0,
        NOT_NULL = 1 << 0,
        UNIQUE = 1 << 1,
        PRIMARY_KEY = 1 << 2,
        CHECK = 1 << 3,
        DEFAULT = 1 << 4,

        // Common Constraint Combinations
        NOT_NULL_UNIQUE = NOT_NULL | UNIQUE,
        NOT_NULL_PRIMARY_KEY = NOT_NULL | PRIMARY_KEY,
        NOT_NULL_DEFAULT = NOT_NULL | DEFAULT
    };
    LogsLevel m_logs_level = LogsLevel::DISABLE_ALL;

    // constructor and destructor
    explicit SQLiteWrapper(const std::string &databaseName, LogsLevel logs_level = LogsLevel::DISABLE_ALL);
    SQLiteWrapper() = delete;
    ~SQLiteWrapper();

    // Table Management
    SQLiteWrapper &setTable(const std::string &tableName);
    bool createTable();
    void createTable(const std::vector<std::pair<std::string, std::string>> &columns);
    bool deleteTable(const std::string &table_name);
    bool renameTable(const std::string &oldname, const std::string &newname);

    // column management
    SQLiteWrapper &addColumn(const std::string &columnName, const std::string &type, Constraints constraints = Constraints::NO_CONSTRAINTS, const std::string &Default = "", const std::string &Check = "");
    bool renamecolumn(const std::string &table_name, const std::string &column_name, const std::string &new_column_name);
    bool addcolumn(const std::string &table_name, const std::string column_name, const std::string &data_type);
    bool dropcolumn(const std::string &table_name, const std::string column_name);

    // Data Manipulation
    bool insertRecord(const std::map<std::string, std::string> &data);
    bool insertValues(const std::vector<std::string> &values);
    void insertMultipleRecords(const std::vector<std::map<std::string, std::string>> &records);
    bool update_record(const std::string &table_name, const std::string &column_name, const std::string &value, const std::string &condition = "");
    bool removerecord(std::string table_name = "", const std::string &condition = "");

    // data showing
    std::vector<std::map<std::string, std::string>> fetchTable();
    void showTable(const std::string &table_name, const std::string &condition = "");
    void showAll();

    // filter
    SQLiteWrapper &setFilter(const std::string &column, const std::string &value, const std::string &comparisonoperator);
    SQLiteWrapper &disableFilter();

    // logs management
    void enable_logs(LogsLevel logs_level = LogsLevel::ENABLE_ALL);
    void disable_logs();

    // custom queues management
    bool customquery(const std::string &query);

private:
    // member variables
    sqlite3 *m_db = nullptr;
    std::string m_databaseName;
    std::string m_tableName;
    std::string m_filter;
    std::vector<std::pair<std::string, std::string>> m_columns;
    bool m_logs_flag;

    // helper functions
    std::string join(const std::vector<std::string> &elements, const std::string &delimiter);
    static int callback(void *data, int argc, char **argv, char **colNames);
    void print_Logs(const std::string &log, MessagType type);
    bool executeQuery(const std::string &query);
    void openDatabase(void);
    void closeDatabase();
};

#endif // SQLITE_WRAPPER_H
