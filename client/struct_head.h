#ifndef STRUCT_HEAD_H
#define STRUCT_HEAD_H

struct Exam_Info    //题目信息
{
    char topic[1024];
    char optionA[512];
    char optionB[512];
    char optionC[512];
    char answer[2];
    int id;
};

struct User_Info    //用户信息
{
    char username[15];
    char password[12];

    int score;
};

struct Cmd_Info     //命令信息
{
    char cmd[24];
    struct User_Info user_cmd;
};

struct Result_msg       //处理后的结果
{
    char result[24];
    char username[15];

    Exam_Info exam_info;

    int score;
};

#endif // STRUCT_HEAD_H
