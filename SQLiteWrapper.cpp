#include "SQLiteWrapper.hpp"
#include <iostream>
#include <sstream>

// ================================== constructor and destructor ==================================
/**
 * @brief Constructs the SQLiteWrapper object and opens the database.
 * @param databaseName The name of the database file.
 * @param logs_level Logging level (default: DISABLE_ALL).
 */
SQLiteWrapper::SQLiteWrapper(const std::string &databaseName, LogsLevel logs_level) : m_databaseName(databaseName), m_logs_level(logs_level)
{
    openDatabase();
}
/**
 * @brief Destructor that ensures the database is closed.
 */
SQLiteWrapper::~SQLiteWrapper()
{
    closeDatabase();
}

// ================================== Table Management ==================================

/**
 * @brief Sets the table to operate on.
 * @param tableName The name of the table.
 * @return Reference to the current SQLiteWrapper instance.
 */
SQLiteWrapper &SQLiteWrapper::setTable(const std::string &tableName)
{
    m_columns.erase(m_columns.begin(), m_columns.end());
    this->m_tableName = tableName;
    return *this;
}

/**
 * @brief Creates a table using previously set columns.
 * @return True if the table was created successfully, false otherwise.
 */
bool SQLiteWrapper::createTable()
{
    std::ostringstream query;
    if (m_tableName.empty() || m_columns.empty())
    {
        print_Logs("Table name or columns not set!", MessagType::ERROR);
        return false;
    }

    query << "CREATE TABLE IF NOT EXISTS " + m_tableName + "(";
    for (size_t i = 0; i < m_columns.size(); ++i)
    {
        query << m_columns[i].first << " " << m_columns[i].second;
        if (i < m_columns.size() - 1)
            query << ", ";
    }
    query << ");";

    print_Logs(query.str(), MessagType::QUERY);

    bool ret = executeQuery(query.str());
    if (ret)
    {
        print_Logs("Table " + m_tableName + " created successfully", MessagType::INFO);
    }
    return ret;
}

/**
 * @brief Creates a table using a given list of column definitions.
 * @param columns A vector of column name and type pairs.
 */
void SQLiteWrapper::createTable(const std::vector<std::pair<std::string, std::string>> &columns)
{
    m_columns = columns;
    createTable();
}

/**
 * @brief Deletes an entire table from the database.
 *
 * @param table_name The name of the table to delete.
 * @return True if the table is successfully deleted, false otherwise.
 */
bool SQLiteWrapper::deleteTable(const std::string &table_name)
{
    std::ostringstream query{};
    query << "DROP TABLE " + table_name + " ;";
    print_Logs(query.str(), MessagType::QUERY);
    bool ret = executeQuery(query.str());
    if (ret)
    {
        print_Logs("Table " + table_name + " deleted successfully", MessagType::INFO);
    }
    return ret;
}

/**
 * @brief Renames a table in the database.
 *
 * @param oldname The current name of the table.
 * @param newname The new name for the table.
 * @return True if the table is successfully renamed, false otherwise.
 */
bool SQLiteWrapper::renameTable(const std::string &oldname, const std::string &newname)
{
    std::ostringstream query{};
    query << "ALTER TABLE " + oldname + " RENAME TO " + newname + " ;";
    print_Logs(query.str(), MessagType::QUERY);

    bool ret = executeQuery(query.str());
    if (ret)
    {
        print_Logs("Table " + oldname + " is renammed to " + newname + " successfully", MessagType::INFO);
    }
    return ret;
}

// ================================== column Management ==================================
/**
 * @brief Adds a column definition to the current table.
 * @param columnName The name of the column.
 * @param type The data type of the column (e.g., TEXT, INTEGER).
 * @param constraints Constraints for the column (default: NO_CONSTRAINTS).
 * @param Default Default value for the column if there is a default constraint (default: empty).
 * @param check Check condition for the column if there is a check constraint(default: empty).
 * @return Reference to the current SQLiteWrapper instance.
 */

SQLiteWrapper &SQLiteWrapper::addColumn(const std::string &columnName, const std::string &type, Constraints constraints, const std::string &Default, const std::string &check)
{
    std::string str{type};

    if (constraints != Constraints::NO_CONSTRAINTS)
    {
        std::map<Constraints, std::string> fullstring = {
            {Constraints::NOT_NULL, " NOT NULL "},
            {Constraints::UNIQUE, " UNIQUE "},
            {Constraints::PRIMARY_KEY, " PRIMARY KEY "},
            {Constraints::NOT_NULL_UNIQUE, " NOT NULL UNIQUE "},
            {Constraints::NOT_NULL_PRIMARY_KEY, " NOT NULL PRIMARY KEY "}};

        if (constraints == Constraints::DEFAULT && !Default.empty())
        {
            fullstring[Constraints::DEFAULT] = " DEFAULT '" + Default + "' ";
        }
        else if (constraints == Constraints::NOT_NULL_DEFAULT && !Default.empty())
        {
            fullstring[Constraints::NOT_NULL_DEFAULT] = " NOT NULL DEFAULT '" + Default + "' ";
        }

        if (constraints == Constraints::CHECK && !check.empty())
        {
            fullstring[Constraints::CHECK] = " CHECK (" + check + ") ";
        }

        auto it = fullstring.find(constraints);
        if (it != fullstring.end())
        {
            str += std::string(it->second);
        }
    }

    m_columns.emplace_back(columnName, str);

    return *this;
}

/**
 * @brief Renames a column in a given table.
 *
 * @param table_name The name of the table containing the column.
 * @param column_name The current name of the column.
 * @param new_column_name The new name for the column.
 * @return True if the column is successfully renamed, false otherwise.
 */
bool SQLiteWrapper::renamecolumn(const std::string &table_name, const std::string &column_name, const std::string &new_column_name)
{
    std::ostringstream query{};
    query << "ALTER TABLE " + table_name + " RENAME COLUMN " + column_name + " TO " + new_column_name + " ;";
    print_Logs(query.str(), MessagType::QUERY);

    bool ret = executeQuery(query.str());
    if (ret)
    {
        print_Logs("column " + column_name + " renamed to " + new_column_name + " successfully ", MessagType::INFO);
    }
    return ret;
}

/**
 * @brief Adds a new column to a table.
 *
 * @param table_name The name of the table to modify.
 * @param column_name The name of the column to add.
 * @param data_type The data type of the new column.
 * @return True if the column is successfully added, false otherwise.
 */
bool SQLiteWrapper::addcolumn(const std::string &table_name, const std::string column_name, const std::string &data_type)
{
    std::ostringstream query{};
    query << "ALTER TABLE " + table_name + " ADD " + column_name + " " + data_type + " ;";
    print_Logs(query.str(), MessagType::QUERY);
    bool ret = executeQuery(query.str());
    if (ret)
    {
        print_Logs("column " + column_name + " Added to " + table_name + " successfully", MessagType::INFO);
    }
    return ret;
}

/**
 * @brief Drops a column from a table.
 *
 * @param table_name The name of the table to modify.
 * @param column_name The name of the column to drop.
 * @return True if the column is successfully dropped, false otherwise.
 */
bool SQLiteWrapper::dropcolumn(const std::string &table_name, const std::string column_name)
{
    std::ostringstream query{};
    query << "ALTER TABLE " + table_name + " DROP COLUMN " + column_name + " ;";
    print_Logs(query.str(), MessagType::QUERY);

    bool ret = executeQuery(query.str());
    if (ret)
    {
        print_Logs("column " + column_name + " dropped successfully from table " + table_name, MessagType::INFO);
    }
    return ret;
}

// ================================== Data Manipulation ==================================

/**
 * @brief Inserts a single record into the current table.
 *
 * @param data A map where keys are column names and values are the corresponding data to be inserted.
 * @return True if the record is inserted successfully, false otherwise.
 */
bool SQLiteWrapper::insertRecord(const std::map<std::string, std::string> &data)
{
    if (m_tableName.empty() || data.empty())
    {
        print_Logs("Table name or columns not set!", MessagType::ERROR);
        return false;
    }

    std::ostringstream query;
    query << "INSERT INTO " << m_tableName << " (";
    std::vector<std::string> keys, values;

    for (const auto &pair : data)
    {
        keys.push_back(pair.first);
        values.push_back("'" + pair.second + "'");
    }
    query << join(keys, ", ") << ") VALUES (" << join(values, ", ") << ");";
    print_Logs(query.str(), MessagType::QUERY);

    bool ret = executeQuery(query.str());
    if (ret)
    {
        print_Logs("data inserted successfully", MessagType::INFO);
    }
    return ret;
}

/**
 * @brief Inserts values into the current table.
 *
 * This method constructs an SQL `INSERT INTO` query and executes it.
 * It assumes the table structure is already set and the provided values
 * match the column order in the table.
 *
 * @param values A vector containing the values to insert.
 * @return `true` if the query executes successfully, `false` otherwise.
 */
bool SQLiteWrapper::insertValues(const std::vector<std::string> &values)
{
    if (m_tableName.empty() || values.empty())
    {
        print_Logs("Table name or columns not set!", MessagType::ERROR);
        return false;
    }
    std::ostringstream query;
    query << "INSERT INTO " << m_tableName << " VALUES (";

    for (size_t i = 0; i < values.size(); i++)
    {
        query << "'" + values[i] + "'";
        if (i < values.size() - 1)
            query << ", ";
    }
    query << ");";
    print_Logs(query.str(), MessagType::QUERY);

    bool ret = executeQuery(query.str());
    if (ret)
    {
        print_Logs("data inserted successfully", MessagType::INFO);
    }
    return ret;
}

/**
 * @brief Inserts multiple records into the current table.
 *
 * @param records A vector of maps, where each map represents a single record.
 */
void SQLiteWrapper::insertMultipleRecords(const std::vector<std::map<std::string, std::string>> &records)
{
    for (const auto &record : records)
    {
        insertRecord(record);
    }
}

/**
 * @brief Updates a record in a table.
 *
 * @param table_name The name of the table containing the record.
 * @param column_name The column to update.
 * @param value The new value for the column.
 * @param condition (Optional) The condition to determine which records to update.
 * @return True if the record is successfully updated, false otherwise.
 */
bool SQLiteWrapper::update_record(const std::string &table_name, const std::string &column_name, const std::string &value, const std::string &condition)
{
    std::string query{};
    query = "UPDATE " + table_name + " SET " + column_name + " = " + value;
    query = condition.empty() ? query + " ;" : query + " WHERE " + condition + " ;";

    print_Logs(query, MessagType::QUERY);
    bool ret = executeQuery(query);
    if (ret)
    {
        print_Logs("Record updated successfully", MessagType::INFO);
    }
    return ret;
}

/**
 * @brief Removes records from a table based on a condition.
 *
 * @param table_name (Optional) The name of the table to remove records from. If empty, the currently set table is used.
 * @param condition (Optional) The condition to determine which records to remove.
 * @return True if the records are successfully removed, false otherwise.
 */
bool SQLiteWrapper::removerecord(std::string table_name, const std::string &condition)
{
    if (table_name.empty())
    {
        table_name = this->m_tableName;
    }

    std::string query = "DELETE FROM " + table_name;
    query = condition.empty() ? query + " ;" : query + " WHERE " + condition + " ;";
    print_Logs(query, MessagType::QUERY);

    bool ret = executeQuery(query);
    if (ret)
    {
        print_Logs("Record deleted successfully from table " + table_name, MessagType::INFO);
    }
    return ret;
}

// ================================== Data showing ==================================

/**
 * @brief Fetches all records from the current table.
 *
 * @return A vector of maps, where each map represents a row with column names as keys.
 */
std::vector<std::map<std::string, std::string>> SQLiteWrapper::fetchTable()
{
    std::vector<std::map<std::string, std::string>> results;
    if (m_tableName.empty())
    {
        print_Logs("Table name is not set!", MessagType::ERROR);
        return results;
    }
    std::string query = "SELECT * FROM " + m_tableName;
    query = m_filter.empty() ? query + ";" : query + " " + m_filter + " ;";
    print_Logs(query, MessagType::QUERY);

    char *errMsg = nullptr;
    if (sqlite3_exec(m_db, query.c_str(), callback, &results, &errMsg) != SQLITE_OK)
    {
        print_Logs("SQL error: " + std::string(errMsg), MessagType::ERROR);
        sqlite3_free(errMsg);
    }

    return results;
}

/**
 * @brief Displays all records from the specified table.
 * @param table_name The name of the table.
 * @param condition The WHERE clause condition (optional).
 */
void SQLiteWrapper::showTable(const std::string &table_name, const std::string &condition)
{
    if (table_name.empty())
    {
        print_Logs("Table name is not set! ", MessagType::ERROR);
        return;
    }

    std::vector<std::map<std::string, std::string>> results;
    sqlite3_stmt *stmt = nullptr; // Ensure stmt is always initialized

    std::vector<std::string> column_order;
    std::string pragma_query = "PRAGMA table_info(" + table_name + ");";

    if (sqlite3_prepare_v2(m_db, pragma_query.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            column_order.push_back(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
        }
    }
    sqlite3_finalize(stmt); // Always finalize prepared statements

    std::string query = "SELECT * FROM " + table_name;
    if (!condition.empty())
    {
        query += " WHERE " + condition;
    }
    query += ";";
    print_Logs(query, MessagType::QUERY);

    char *errMsg = nullptr;
    if (sqlite3_exec(m_db, query.c_str(), callback, &results, &errMsg) != SQLITE_OK)
    {
        print_Logs("SQL error: " + std::string(errMsg), MessagType::ERROR);
        sqlite3_free(errMsg); // Free the error message buffer
    }

    if (results.empty())
    {
        print_Logs("No records found", MessagType::ERROR);
        return;
    }
    int counter{};
    for (const auto &row : results)
    {
        std::cout << "Record: " << counter++ << " | ";
        for (const auto &col : column_order)
        {
            auto it = row.find(col);
            if (it != row.end())
            {
                std::cout << col << ": " << it->second << " | ";
            }
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Displays all tables in the database.
 */
void SQLiteWrapper::showAll()
{
    int table_count = 0;
    const char *query = "SELECT name FROM sqlite_master WHERE type='table';";
    sqlite3_stmt *stmt = nullptr; // Ensure it is initialized

    if (sqlite3_prepare_v2(m_db, query, -1, &stmt, nullptr) != SQLITE_OK)
    {
        print_Logs("Error preparing statement: " + std::string(sqlite3_errmsg(m_db)), MessagType::ERROR);
        closeDatabase();
        return;
    }

    std::cout << "Tables in database:" << std::endl;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        table_count++;
        std::string tableName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));

        std::string pragmaQuery = "PRAGMA table_info(" + tableName + ");";
        sqlite3_stmt *colStmt = nullptr;
        int column_count = 0;

        if (sqlite3_prepare_v2(m_db, pragmaQuery.c_str(), -1, &colStmt, nullptr) == SQLITE_OK)
        {
            while (sqlite3_step(colStmt) == SQLITE_ROW)
                column_count++;
        }
        sqlite3_finalize(colStmt); // Finalize after use

        std::string countQuery = "SELECT COUNT(*) FROM " + tableName + ";";
        sqlite3_stmt *countStmt = nullptr;
        int row_count = 0;

        if (sqlite3_prepare_v2(m_db, countQuery.c_str(), -1, &countStmt, nullptr) == SQLITE_OK)
        {
            if (sqlite3_step(countStmt) == SQLITE_ROW)
                row_count = sqlite3_column_int(countStmt, 0);
        }
        sqlite3_finalize(countStmt); // Finalize after use

        std::cout << "Table: " << tableName << std::endl;
        std::cout << "Columns: " << column_count << std::endl;
        std::cout << "Records: " << row_count << std::endl;
        showTable(tableName);
        std::cout << "====================================" << std::endl;
    }

    sqlite3_finalize(stmt);
    std::cout << table_count << " Table(s) found" << std::endl;
}

// ================================== Filter ==================================

/**
 * @brief Sets a filter condition for retrieving records.
 *
 * @param column The column to filter by.
 * @param value The value to compare against.
 * @param comparisonoperator The comparison operator (e.g., "=", ">", "<").
 * @return Reference to the current SQLiteWrapper instance.
 */
SQLiteWrapper &SQLiteWrapper::setFilter(const std::string &column, const std::string &value, const std::string &comparisonoperator)
{
    if (m_filter.empty())
    {
        m_filter = "WHERE ";
    }
    else
    {
        m_filter += " AND ";
    }
    m_filter += column + " " + comparisonoperator + " '" + value + "'";
    return *this;
}

/**
 * @brief Disables any previously set filter conditions.
 * @return Reference to the current SQLiteWrapper instance.
 */
SQLiteWrapper &SQLiteWrapper::disableFilter()
{
    m_filter = "";
    return *this;
}
// ================================== logs management ==================================
/**
 * @brief Enables logging with the specified log level.
 *
 * @param logs_level The logging level to set.
 */
void SQLiteWrapper::enable_logs(LogsLevel logs_level)
{
    this->m_logs_level = logs_level;
}

/**
 * @brief Disables all logging.
 */
void SQLiteWrapper::disable_logs()
{
    this->m_logs_level = LogsLevel::DISABLE_ALL;
}

// ================================== custom queues management ==================================
/**
 * @brief Executes a custom SQL query.
 * @param query The SQL query string.
 * @return True if executed successfully, false otherwise.
 */
bool SQLiteWrapper::customquery(const std::string &query)
{
    print_Logs(query, MessagType::QUERY);
    bool ret = executeQuery(query);
    if (ret)
    {
        print_Logs("Query executed successfully", MessagType::INFO);
    }
    return ret;
}
// ================================== helper functions ==================================

/**
 * @brief Opens the SQLite database specified in the constructor.
 * If opening fails, logs an error and closes the database.
 */
void SQLiteWrapper::openDatabase(void)
{
    if (sqlite3_open(m_databaseName.c_str(), &m_db) != SQLITE_OK)
    {
        print_Logs(sqlite3_errmsg(m_db), MessagType::ERROR);
        if (m_db)
            closeDatabase();
    }
    else
    {
        print_Logs("Database created or opened if exist successfully!", MessagType::INFO);
    }
}

/**
 * @brief Closes the SQLite database connection.
 */
void SQLiteWrapper::closeDatabase()
{
    if (m_db)
    {
        print_Logs("Closing database...", MessagType::INFO);
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}
/**
 * @brief Executes an SQL query.
 * @param query The SQL query string.
 * @return True if successful, false otherwise.
 */
bool SQLiteWrapper::executeQuery(const std::string &query)
{
    char *messaggeError = nullptr;
    if (!m_db)
    {
        openDatabase();
    }

    if (sqlite3_exec(m_db, query.c_str(), nullptr, nullptr, &messaggeError) != SQLITE_OK)
    {
        print_Logs("SQL error: " + std::string(messaggeError), MessagType::ERROR);
        sqlite3_free(messaggeError);
        return false;
    }
    return true;
}
/**
 * @brief SQLite callback function for processing query results.
 *
 * This function is called for each row in the result set when executing a query.
 * It stores the retrieved data in a vector of maps, where each map represents
 * a row with column names as keys and values as the respective data.
 *
 * @param data Pointer to a vector of maps where results will be stored.
 * @param argc Number of columns in the result row.
 * @param argv Array of column values (as C-strings).
 * @param colNames Array of column names.
 * @return Always returns 0 to indicate successful execution.
 */
int SQLiteWrapper::callback(void *data, int argc, char **argv, char **colNames)
{
    auto *results = static_cast<std::vector<std::map<std::string, std::string>> *>(data);
    std::map<std::string, std::string> row;

    for (int i = 0; i < argc; ++i)
    {
        row[colNames[i]] = argv[i] ? argv[i] : "NULL";
    }
    results->push_back(row);
    return 0;
}

/**
 * @brief Joins elements of a vector into a single string with a specified delimiter.
 *
 * @param elements The vector of strings to join.
 * @param delimiter The string used to separate elements.
 * @return A single concatenated string with elements separated by the delimiter.
 */

std::string SQLiteWrapper::join(const std::vector<std::string> &elements, const std::string &delimiter)
{
    std::ostringstream result;
    for (size_t i = 0; i < elements.size(); ++i)
    {
        result << elements[i];
        if (i < elements.size() - 1)
            result << delimiter;
    }
    return result.str();
}

/**
 * @brief Logs messages with a specified message type and formatting.
 *
 * This function prints logs to the console based on the current log level.
 * Messages are color-coded and formatted for better readability.
 *
 * @param log The message to log.
 * @param type The type of message (INFO, ERROR, QUERY).
 */
void SQLiteWrapper::print_Logs(const std::string &log, MessagType type)
{
    if (m_logs_level == LogsLevel::DISABLE_ALL)
        return;

    // Define log types, colors, and output streams
    static const std::map<MessagType, std::pair<std::ostream &, std::string>> logConfig = {
        {MessagType::INFO, {std::cout, "\e[32m\e[1m\e[3mINFO  : \e[0m \e[32m"}},
        {MessagType::ERROR, {std::cerr, "\e[31m\e[1m\e[3mERROR : \e[0m \e[31m"}},
        {MessagType::QUERY, {std::cout, "\e[34m\e[1m\e[3mQUERY : \e[0m \e[34m"}}};

    // Check if logging is enabled for the specific type
    if (m_logs_level == LogsLevel::ENABLE_ALL ||
        (m_logs_level == LogsLevel::QUERY && type == MessagType::QUERY) ||
        (m_logs_level == LogsLevel::ERROR && type == MessagType::ERROR) ||
        (m_logs_level == LogsLevel::INFO && type == MessagType::INFO))
    {
        auto it = logConfig.find(type);
        if (it != logConfig.end())
        {
            it->second.first << it->second.second << log << "\e[0m" << std::endl;
        }
    }
}
