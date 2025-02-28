#include "SQLiteWrapper.hpp"
int main(int argc, const char **argv)
{

    SQLiteWrapper db1("mydatabase.db", SQLiteWrapper::LogsLevel::ENABLE_ALL);
    db1.setTable("Users")
        .addColumn("ID", "INTEGER", SQLiteWrapper::Constraints::DEFAULT, "0")
        .addColumn("NAME", "TEXT", SQLiteWrapper::Constraints::NOT_NULL)
        .addColumn("AGE", "INTEGER")
        .createTable();

    // std::map<std::string, std::string> data;
    // data.emplace("ID", "1");
    // data.emplace("NAME", "Ali");
    // data.emplace("AGE", "25");
    // // db1.insertRecord(data);
    // // or insert the data direct
    // db1.insertRecord({{"NAME", "Omar"}, {"AGE", "20"}});
    // db1.insertRecord({{"ID", "2"}, {"NAME", "Mahmoud"}, {"AGE", "19"}});
    // db1.insertRecord({{"ID", "3"}, {"NAME", "Gamal"}, {"AGE", "35"}});

    // db1.insertMultipleRecords({{{"ID", "4"}, {"Name", "Ali"}, {"Age", "30"}},
    //                            {{"ID", "5"}, {"Name", "Ahmed"}, {"Age", "22"}},
    //                            {{"ID", "6"}, {"Name", "Osman"}, {"Age", "16"}},
    //                            {{"ID", "7"}, {"Name", "Khaled"}, {"Age", "30"}}});

    // db1.setTable("Products")
    //     .addColumn("ID", "TEXT", SQLiteWrapper::Constraints::DEFAULT, "0")
    //     .addColumn("NAME", "TEXT")
    //     .addColumn("PRICE", "TEXT")
    //     .createTable();
    // db1.setTable("Orders")
    //     .addColumn("ID", "TEXT", SQLiteWrapper::Constraints::DEFAULT, "0")
    //     .addColumn("USER_ID", "TEXT")
    //     .addColumn("PRODUCT_ID", "TEXT")
    //     .createTable();
    // db1.setTable("Products")
    //     .insertMultipleRecords({{{"ID", "101"}, {"Name", "Laptop"}, {"Price", "1000"}},
    //                             {{"ID", "102"}, {"Name", "Phone"}, {"Price", "500"}},
    //                             {{"ID", "103"}, {"Name", "Tablet"}, {"Price", "300"}}});
    // db1.setTable("Orders")
    //     .insertMultipleRecords({{{"ID", "1001"}, {"User_ID", "1"}, {"Product_ID", "101"}},
    //                             {{"ID", "1002"}, {"User_ID", "2"}, {"Product_ID", "102"}},
    //                             {{"ID", "1003"}, {"User_ID", "3"}, {"Product_ID", "103"}}});
    // db1.showAll();

    return 0;
}

// db1.disable_logs();
// db1.showTable("Users");
// std::cout << "==============================" << std::endl;
// db1.showTable("Users", "ID > 3"); // Where ID > 3

// Products table
// db1.setTable("Products")
//     .addColumn("Product_ID", "INTEGER", SQLiteWrapper::Constraints::PRIMARY_KEY)
//     .addColumn("Product_Name", "TEXT")
//     .addColumn("Price", "REAL")
//     .createTable();
// db1.setTable("Orders")
//     .addColumn("Order_ID", "INTEGER", SQLiteWrapper::Constraints::PRIMARY_KEY)
//     .addColumn("User_ID", "TEXT")
//     .addColumn("Total_Amount", "REAL")
//     .createTable();
// db1.insertMultipleRecords({{{"Product_ID", "1"}, {"Product_Name", "Laptop"}, {"Price", "1000"}},
//                            {{"Product_ID", "2"}, {"Product_Name", "Phone"}, {"Price", "500"}},
//                            {{"Product_ID", "3"}, {"Product_Name", "Tablet"}, {"Price", "300"}}});
// db1.insertMultipleRecords({{{"Order_ID", "1"}, {"User_ID", "4"}, {"Total_Amount", "1200"}},
//                            {{"Order_ID", "2"}, {"User_ID", "5"}, {"Total_Amount", "700"}},
//                            {{"Order_ID", "3"}, {"User_ID", "6"}, {"Total_Amount", "400"}}});
// db1.showAll();

// std::map<std::string, std::string> data;
// data.emplace("ID", "1");
// data.emplace("NAME", "Ali");
// data.emplace("AGE", "25");
// db1.insertRecord(data);
// // or insert the data direct
// db1.insertRecord({{"NAME", "Omar"}, {"AGE", "20"}});
// db1.insertRecord({{"ID", "2"}, {"NAME", "Mahmoud"}, {"AGE", "19"}});
// db1.insertRecord({{"ID", "3"}, {"NAME", "Gamal"}, {"AGE", "35"}});
// db1.setTable("Topics");
// db1.addColumn("Topic_ID", "INT");
// db1.addColumn("TOPIC_PRICE", "FLOAT");
// db1.addColumn("TOPIC_Instructor", "TEXT");
// db1.createTable();
// db1.insertRecord({{"Topic_ID", "0"}, {"TOPIC_PRICE", "200 $"}, {"TOPIC_Instructor", "Ali"}});
// db1.insertRecord({{"Topic_ID", "1"}, {"TOPIC_PRICE", "300 $"}, {"TOPIC_Instructor", "Abdallah"}});
// // db1.renamecolumn("Topics", "TOPIC_ID", "TOPICID");
// // void SQLiteWrapper::insertMultipleRecords(const std::vector<std::map<std::string, std::string>> &records)

// db1.insertMultipleRecords({{{"Topic_ID", "1"}, {"TOPIC_PRICE", "300 $"}, {"TOPIC_Instructor", "Abdallah"}},
//                            {{"Topic_ID", "2"}, {"TOPIC_PRICE", "400 $"}, {"TOPIC_Instructor", "Ali"}},
//                            {{"Topic_ID", "3"}, {"TOPIC_PRICE", "500 $"}, {"TOPIC_Instructor", "Omar"}},
//                            {{"Topic_ID", "4"}, {"TOPIC_PRICE", "600 $"}, {"TOPIC_Instructor", "Mahmoud"}}});

// db1.deleteTable("Topics");
// db1.disable_logs();

// db1.addprimarykey("Users", "ID");
// db1.customquery("UPDATE Users SET ID = 200 WHERE ID = 100 ;");
// db1.update_record("Users", "ID", "100", "ID = 10");
// db1.setTable("Users").insertRecord({{"ID", "0"}, {"NAME", "Omar"}, {"AGE", "15"}});
// db1.setTable("Topics").insertRecord({{"Topic_ID", "12"}, {"TOPIC_PRICE", "125 $"}, {"TOPIC_Instructor", "Rana"}});

// db1.showAll();
// auto records = db1.fetchTable("Topics");
// for (const auto &row : records)
// {
//     std::cout << "ID: " << row.at("ID")
//               << ", Name: " << row.at("NAME")
//               << ", Age: " << row.at("AGE") << "\n";
// }
// db1.showTable("Topics");
// db1.showTable("Users", "ID =20");

// auto records = db1.setTable("Users").setFilter("ID", "20", ">").setFilter("NAME", "Abdallah", "=").fetchTable();
// for (const auto &row : records)
// {
//     std::cout << "ID: " << row.at("ID")
//               << ", Name: " << row.at("NAME")
//               << ", Age: " << row.at("AGE") << "\n";
// }
// db1.disableFilter();

// auto records2 = db1.fetchTable();
// for (const auto &row : records2)
// {
//     std::cout << "ID: " << row.at("ID")
//               << ", Name: " << row.at("NAME")
//               << ", Age: " << row.at("AGE") << "\n";
// }

// std::vector<std::pair<std::string, std::string>> columns2 = {{"Number3", "TEXT"}, {"Number4", "TEXT"}, {"AGE", "TEXT"}};
// SQLiteWrapper db2("mydatabase.db", db1.LogsLevel::ENABLE_ALL);
// auto db2 = SQLiteWrapper::Get_Instance("mydatabase.db");
// // SQLiteWrapper db2("mydatabase.db");
// db1.setTable("Numbers1").createTable(columns2);
// db1.setTable("Numbers3").createTable(columns2);
// db1.setTable("Numbers4").createTable(columns2);
// db1.setTable("Numbers5").createTable(columns2);
// db1.setTable("Numbers6").createTable(columns2);

// std::vector<std::pair<std::string, std::string>> myColumns = {{"id", "INTEGER PRIMARY KEY"}, {"name", "TEXT"}};
// db1.createTable(myColumns);
// // db1.showTable("Users");
// // db1.setTable("Topics");
// // db1.removerecord();
// db1.showTable("Numbers6");

// db1.setTable("Users").insertValues({"0", "Abotaleb", "20"});
