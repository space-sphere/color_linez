/*2152778 数金 魏亭舟*/
#pragma once

#include <iostream>
#include <conio.h>
#include <iomanip>
#include "cmd_console_tools.h"
#include <cstring>
#include <ctime>
#include <cmath>
#include <Windows.h>
using namespace std;

// board
#define MAX_WIDTH 9
#define MIN_WIDTH 7
#define MAX_HEIGHT 9
#define MIN_HEIGHT 7

// console
#define CONSOLE_COLS 80
#define CONSOLE_LINES 25
#define BUFFER_LINES 200                    // 缓冲区大小

#define FONT_SIZE 24 + 4

// default cout
#define MAX_SCORE_WIDTH 4                   // 分数显示宽度
#define MAX_ELI_BALL_WIDTH 4                // 消除球个数显示宽度
#define SLEEP_TIME 100                      // 动画延时

// outline
#define UPPER_LEFT 0
#define LOWER_LEFT 1
#define UPPER_RIGHT 2
#define LOWER_RIGHT 3
#define level 4
#define vertical 5
#define up_T 6
#define low_T 7
#define left_T 8
#define right_T 9
#define cross 10

// ball shape
#define chosen 1
#define original 0

// color setting
#define RANDOM_COLOR 1
#define FIXED_COLOR 5
#define BASE_COLOR (COLOR_HBLACK - 1)       // 内部数组加该颜色，得到显示颜色
#define BOARD_COLOR COLOR_HWHITE
#define OUTLINE_COLOR COLOR_BLACK
#define COLOR_NUM 7                         // 共有几种颜色
#define num_generated 3                     // 一次增加多少个球

extern const char* ball[];      // ball shape
extern const char* outline[];   // outline shape
extern const int single_width;  // width of outline and ball, make sure strlen(ball) == strlen()outline

// 数组坐标
struct coordinate
{
    int x;
    int y;
};

void input_error(int x, int y); // check error in input function

class Game;
class Astar;

int Manhattan(int x, int y, int x1, int y1); // calc distance

// 调整available_pos数组中的坐标
void swap_cor(coordinate* pos1, coordinate* pos2);
void add_cor(coordinate* dst_cor, const coordinate* src_cor);

void console_to_board(coordinate* coord, int& x, int& y, int board_x, int board_y);
void board_to_console(coordinate* coord, int& x, int& y, int board_x, int board_y);

// 动图
void show_level_line(const int* color_array, const int content_num, const char* content, const char* left,
                     const char* middle, const char* right);
void single_anime(int x_dst, int y_dst, int x_src, int y_src, int color);                                           // src到dst的动图
void src_dst_anime(Astar* astar, coordinate* dst, coordinate* src, const int board_x, const int board_y, int color);// 递归调用single_anime

// 搜索过程信息
class Node
{
  private:
    int x;
    int y;
    int G; // 从起点到当前点的损失
    int H; // 从当前点到终点的损失
    int F; // F= G + H
  public:
    Node() : x(-1), y(-1), G(-1), H(-1), F(-1){};
    int get_f(); // get F
    void build(int xx, int yy, const coordinate* dst, Node* node); // rebuild the node
    friend Astar;
    friend Game;
};

// 记录搜索过程中的信息
class node_log
{
  private:
    coordinate pre_coord;
    int F;
    bool visited;
    friend Astar;

  public:
    node_log() : pre_coord{-1, -1}, F(-1), visited(0){};
};

// 搜索实现
class Astar
{
  private:
    Node node_que[MAX_WIDTH * MAX_HEIGHT + 1]; // priority que
    node_log search_log[MAX_HEIGHT][MAX_WIDTH];
    const coordinate* src;
    const coordinate* dst;
    const int* board;
    const int height;
    const int width;
    int top;

  public:
    Astar(int* p_board, coordinate* s, coordinate* d, int h, int w, int t = 0);
    void del_node(Node* p);                             // 从优先对列中pop
    void add_node(Node* p);                             // 从优先对列中push
    void find_path();                                   // 找到路径
    coordinate* get_pre(coordinate* pos);
    bool valid_node(Node* node);
    Node* top_index(void);
};

class Game
{
  private:
    coordinate available_pos[MAX_HEIGHT * MAX_WIDTH];
    int board[MAX_HEIGHT][MAX_WIDTH];
    int next_generated_color[num_generated];
    int num_remain[COLOR_NUM + 1];
    int num_eli[COLOR_NUM + 1];
    int avail_pos_num; // available pos top stack ptr
    int height;
    int width;
    int score;
    char choice;

  public:
    Game(int h, int w);
    // 基本工具
    void input(char ch);
    void init_board(int n, bool show_board = 0, int board_x = -1, int board_y = -1);                        // 初始放置彩球
    bool valid_board(int x = -1, int y = -1);
    void swap_board_value(coordinate* src, coordinate* dst);
    void generate_color();                                                                                  // 获得三种颜色 -> next_generated_color[]
    void place_ball(int color, int& current_score, bool show_board = 0, int board_x = -1, int board_y = -1);// 随机按照color放置一颗球
    // 判断，消除
    bool clear_ball(coordinate* dst, int& current_score, bool show_board = 0, int board_x = -1, int board_y = -1);
    void count_adj_ball(int x, int y, const int* xy_direc, int& n);
    // 开始结束
    void start();                                                                                           // start game
    void end();                                                                                             // end game

    //
    // 数组方式
    void base_input_src(char& x, int& y);
    void base_input_dst(char& x, int& y);
    void cout_board(int bg_color = COLOR_HYELLOW, bool real_board = true, bool use_fg_color = 0);           // 数组方式打印board
    void update_cout_board();
    void base_game();                                                                                       // 数组方式
    void src_dst(coordinate& src, coordinate& dst);                                                         // 获得src dst
    void cout_path();

    //
    // 控制台方式
    void cmd_game();                                                                                        // 控制台方式
    void show_board_wo_line();                                                                              // 无边框
    void show_board_line();                                                                                 // 有边框
    // 实时监控
    bool valid_place(int& tmp_x, int& tmp_y, int& board_x, int& board_y);                                   // 鼠标当前坐标是否是小球可放位置
    void control_board_mouse(int board_x, int board_y, coordinate* dst, coordinate* src, int max_place_num = -1, bool show_info = 0); // 使用鼠标控制
    // 展示信息
    void visualize_info(int info_x, int info_y);                                                            // 展示信息板块
    void remaining_info(int& board_x, int& board_y, int max_eli_num = MAX_ELI_BALL_WIDTH);
    void show_score(int& board_x, int& board_y, int max_width = MAX_SCORE_WIDTH);                           // 展示分数
    void show_next_generated_ball(int& board_x, int& board_y);                                              // 展示即将生成的球
    void update_info(int info_x, int info_y);                                                               // 更新信息板块
    // 动图
    bool src_to_dst(coordinate* dst, coordinate* src, const int board_x, const int board_y);                // 判断src是否能到dst，并调用src_dst_anime
    void show_one_ball(coordinate* plc, int board_x, int board_y, int color = BOARD_COLOR);                 // 展示一个球，用于消除放置
    
    // void print_level_line(bool use_board_item = 0, const char* left = double_outline[left_T],
    //                       const char* middle = double_outline[cross], const char* right = double_outline[right_T],
    //                       int line_index = 0);
};
