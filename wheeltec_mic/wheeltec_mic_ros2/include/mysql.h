#pragma once
#include <mysql/mysql.h>
#include <iostream>
#include <string>
#include <vector>
 
// 定义学生结构体
struct Student{
public:
    int student_id;
    std::string student_name;
    std::string class_id;
};
 
class StudentManager{
    StudentManager(); // 构造函数
    ~StudentManager(); // 析构函数
public:
    static StudentManager* GetInstance(){ // 单例模式
        static StudentManager StudentManager;
        return &StudentManager;
    }
public:
    bool insert_student(Student& stu); // 插入
    bool update_student(Student& stu); // 更新
    bool delete_student(int student_id); // 删除
    std::vector<Student> get_student(std::string condition = ""); // 查询
 
private:
    MYSQL* con;
    // 定义数据库连接参数
const char* host = "";
const char* user = "aqr";
const char* pw = "";
const char* database_name = "aqr";
const int port = 3306;
};