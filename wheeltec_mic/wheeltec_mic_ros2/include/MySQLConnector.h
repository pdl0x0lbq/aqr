#ifndef MYSQL_CONNECTOR_H
#define MYSQL_CONNECTOR_H

#include <iostream>
#include <string>
#include <mysql/mysql.h>

class MySQLConnector {
private:
    MYSQL* conn;

public:
    // 数据库连接参数
    static constexpr const char* HOST = "47.108.74.127";
    static constexpr const char* USER = "aqr";
    static constexpr const char* PW = "TrcZcacaKH6KdLD2";
    static constexpr const char* DATABASE_NAME = "aqr";
    static const int PORT = 3306;

    MySQLConnector() {
        // 初始化数据库连接
        conn = mysql_init(nullptr);

        // 连接到数据库
        if (!mysql_real_connect(conn, HOST, USER, PW, DATABASE_NAME, PORT, nullptr, 0)) {
            std::cerr << "连接到数据库失败: " << mysql_error(conn) << std::endl;
            mysql_close(conn);
            conn = nullptr;
        }
    }

    ~MySQLConnector() {
        // 断开数据库连接
        if (conn) {
            mysql_close(conn);
            conn = nullptr;
        }
    }

    int findIdByAttributes(const std::string& location) {
        if (!conn) {
            std::cerr << "未连接到数据库" << std::endl;
            return -1;
        }

          // 构造查询语句，只根据locationType筛选
       std::string query = "SELECT id FROM YourTableName WHERE locationType = '" + location + "'";

        // 执行查询
        if (mysql_query(conn, query.c_str())) {
            std::cerr << "执行查询失败: " << mysql_error(conn) << std::endl;
            return -1;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        int id = -1;

        if (res) {
            MYSQL_ROW row;
            if ((row = mysql_fetch_row(res))) {
                id = std::stoi(row[0]);
            }
            mysql_free_result(res);
        }

        return id;
    }
};

#endif // MYSQL_CONNECTOR_H
