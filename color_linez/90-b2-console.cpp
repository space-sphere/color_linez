/*2152778 数金 魏亭舟*/
#include "90-b2.h"

const int single_width = strlen(outline[0]);
const char* ball[] = {"○", "◎"};

void Game::show_board_wo_line()
{
    int tmp_x, tmp_y, idx;

    cct_getxy(tmp_x, tmp_y);
    
    show_level_line(NULL, width, outline[level], outline[UPPER_LEFT], NULL, outline[UPPER_RIGHT]);

    if (1) // 竖直边框
    {
        ++tmp_y;
        for (idx = 0; idx < height; ++idx, ++tmp_y)
        {
            cct_gotoxy(0, tmp_y);
            show_level_line(board[idx], width, ball[original], outline[vertical], NULL,
                            outline[vertical]);
        }
    }

    if (1) // 底边框
    {
        cct_gotoxy(0, tmp_y);
        show_level_line(NULL, width, outline[level], outline[LOWER_LEFT], NULL,
                        outline[LOWER_RIGHT]);
    }

    cct_setcolor();
}

void show_level_line(const int* color_array, const int content_num, const char* content, const char* left, const char* middle, const char* right)
{
    int tmp_x, tmp_y;
    const int* p;
    bool flag = color_array == NULL;

    cct_getxy(tmp_x, tmp_y);
    cct_showstr(tmp_x, tmp_y, left, BOARD_COLOR, OUTLINE_COLOR);
    cct_getxy(tmp_x, tmp_y);

    for (p = color_array; p - color_array < content_num - 1; ++p)
    {
        if (flag)
            cct_showstr(tmp_x, tmp_y, content, BOARD_COLOR, OUTLINE_COLOR);
        else
            cct_showstr(tmp_x, tmp_y, content, *p + (*p == 0) * BOARD_COLOR + (*p > 0) * BASE_COLOR, BOARD_COLOR);
        cct_getxy(tmp_x, tmp_y);
        
        if (middle != NULL)
        {
            cct_showstr(tmp_x, tmp_y, middle, BOARD_COLOR, OUTLINE_COLOR);
            cct_getxy(tmp_x, tmp_y);
        }
    }

    if (flag)
        cct_showstr(tmp_x, tmp_y, outline[level], BOARD_COLOR, OUTLINE_COLOR);
    else
        cct_showstr(tmp_x, tmp_y, content, *p + (*p == 0) * BOARD_COLOR + (*p > 0) * BASE_COLOR, BOARD_COLOR);

    cct_getxy(tmp_x, tmp_y);
    cct_showstr(tmp_x, tmp_y, right, BOARD_COLOR, OUTLINE_COLOR);
}

void Game::show_board_line()
{
    int tmp_x, tmp_y, idx;

    cct_getxy(tmp_x, tmp_y);
    if (1) // 上边框
    {
        show_level_line(NULL, width, outline[level], outline[UPPER_LEFT], outline[up_T],
                        outline[UPPER_RIGHT]);
        cct_gotoxy(0, ++tmp_y);
    }

    if (1) // 中间
    {
        for (idx = 0; idx < height - 1; ++idx)
        {
            show_level_line(&board[idx][0], width, ball[original], outline[vertical], outline[vertical],
                            outline[vertical]);
            cct_gotoxy(0, ++tmp_y);
            
            show_level_line(NULL, width, outline[level], outline[left_T], outline[cross],
                            outline[right_T]);
            cct_gotoxy(0, ++tmp_y);
        }
        
        show_level_line(&board[idx][0], width, ball[original], outline[vertical], outline[vertical],
                        outline[vertical]);
        cct_gotoxy(0, ++tmp_y);
    }

    if (1)
    {
        show_level_line(NULL, width, outline[level], outline[LOWER_LEFT], outline[low_T],
                        outline[LOWER_RIGHT]);
    }
    
    cct_setcolor();
}

bool Game::valid_place(int& x, int& y, int& board_x, int& board_y)
{
    return (x < board_x + (1 + 2 * width) * single_width && x > board_x && y < board_y + (1 + 2 * height) &&
            y > board_y && (x - board_x) % (2 * single_width) / 2 == 1 && (y - board_y) % 2 == 1);
}

void single_anime(int x_dst, int y_dst, int x_src, int y_src, int color)
{
    int direc_x = x_dst - x_src;
    int direc_y = y_dst - y_src;

    cct_showstr(x_src, y_src, ball[original], BOARD_COLOR, BOARD_COLOR);
    
    if (direc_y == 0)
    {
        cct_showstr(x_src + ((direc_x > 0) - (direc_x < 0)) * single_width, y_src, ball[chosen], color, BOARD_COLOR);
        Sleep(SLEEP_TIME);
        cct_showstr(x_src + ((direc_x > 0) - (direc_x < 0)) * single_width, y_src, outline[vertical],
                    BOARD_COLOR, OUTLINE_COLOR);
    }
    else
    {
        cct_showstr(x_src, y_src + (direc_y > 0) - (direc_y < 0), ball[chosen], color, BOARD_COLOR);
        Sleep(SLEEP_TIME);
        cct_showstr(x_src, y_src + (direc_y > 0) - (direc_y < 0), outline[level], BOARD_COLOR, OUTLINE_COLOR);
    }
    cct_showstr(x_dst, y_dst, ball[chosen], color, BOARD_COLOR);
    Sleep(SLEEP_TIME);
}

void src_dst_anime(Astar* astar, coordinate* dst, coordinate* src, const int board_x, const int board_y, int color)
{
    int x_dst, y_dst, x_tmp, y_tmp;
    coordinate tmp = *(*astar).get_pre(dst);

    if (tmp.x == src->x && tmp.y == src->y)
    {
        ;
    }
    else
        src_dst_anime(astar, &tmp, src, board_x, board_y, color);

    x_tmp = board_x + single_width + 2 * single_width * tmp.y;
    y_tmp = board_y + 1 + 2 * tmp.x;

    x_dst = board_x + single_width + 2 * single_width * dst->y;
    y_dst = board_y + 1 + 2 * dst->x;

    single_anime(x_dst, y_dst, x_tmp, y_tmp, color);
}

void Game::show_one_ball(coordinate* plc, int board_x, int board_y, int color)
{
    int console_x, console_y;

    if (color <= BASE_COLOR)
        color += BASE_COLOR;

    board_to_console(plc, console_x, console_y, board_x, board_y);
    cct_showstr(console_x, console_y, ball[original], color, BOARD_COLOR);
    
    cct_setcolor();
}

void console_to_board(coordinate* coord, int& x, int& y, int board_x, int board_y)
{
    coord->x = (y - board_y) / 2;
    coord->y = (x - board_x) / (2 * single_width);
}

void board_to_console(coordinate* coord, int& x, int& y, int board_x, int board_y)
{
    y = board_y + 1 + 2 * coord->x;
    x = board_x + single_width + 2 * single_width * coord->y;
}

void Game::show_score(int& board_x, int& board_y, int max_width)
{
    const int warn = 6;
    int tmp_x, tmp_y;

    max_width = (max_width + warn + single_width) / (single_width * 2) + ((max_width + warn + single_width) % (single_width * 2) > 0);
    cct_gotoxy(board_x, board_y);
    show_level_line(NULL, max_width, outline[level],
                    outline[UPPER_LEFT], outline[level],
                    outline[UPPER_RIGHT]);

    cct_gotoxy(board_x, board_y + 1);
    cct_getxy(tmp_x, tmp_y);
    cct_showstr(tmp_x, tmp_y, outline[vertical], BOARD_COLOR, OUTLINE_COLOR);

    cct_getxy(tmp_x, tmp_y);
    cct_showstr(tmp_x, tmp_y, "得分：0", BOARD_COLOR, OUTLINE_COLOR, 1, 4 * max_width - single_width);

    cct_getxy(tmp_x, tmp_y);
    cct_showstr(tmp_x, tmp_y, outline[vertical], BOARD_COLOR, OUTLINE_COLOR);

    cct_gotoxy(board_x, board_y + 2);
    show_level_line(NULL, max_width, outline[level], outline[LOWER_LEFT], outline[level],
                    outline[LOWER_RIGHT]);

    cct_setcolor();
}

void Game::show_next_generated_ball(int& board_x, int& board_y)
{
    cct_gotoxy(board_x, board_y);
    show_level_line(NULL, num_generated, outline[level], outline[UPPER_LEFT], outline[up_T],
                    outline[UPPER_RIGHT]);

    cct_gotoxy(board_x, board_y + 1);
    show_level_line(&next_generated_color[0], num_generated, ball[original], outline[vertical],
                    outline[vertical],
                    outline[vertical]);

    cct_gotoxy(board_x, board_y + 2);
    show_level_line(NULL, num_generated, outline[level], outline[LOWER_LEFT],
                    outline[low_T],
                    outline[LOWER_RIGHT]);

    cct_setcolor();
}

void Game::remaining_info(int& board_x, int& board_y, int max_eli_num)
{
    const int info_width = 20;
    int tmp_x, tmp_y, idx;

    max_eli_num = (max_eli_num + info_width) / (single_width) +
                  ((max_eli_num + info_width) % (single_width) > 0);
    cct_gotoxy(board_x, board_y);
    show_level_line(NULL, max_eli_num, outline[level], outline[UPPER_LEFT], NULL,
                    outline[UPPER_RIGHT]);

    cct_gotoxy(board_x, board_y + 1);
    cct_getxy(tmp_x, tmp_y);
    for (idx = 0; idx < COLOR_NUM + 1; ++idx)
    {
        cct_showstr(tmp_x, tmp_y, outline[vertical], BOARD_COLOR, OUTLINE_COLOR);
        cct_getxy(tmp_x, tmp_y);
        cct_showstr(tmp_x, tmp_y, ball[original], idx + BASE_COLOR + (idx == 0) * (BOARD_COLOR - BASE_COLOR), BOARD_COLOR);
        cct_setcolor(BOARD_COLOR, OUTLINE_COLOR);

        cout << setiosflags(ios::right);
        cout << ':' << setfill('0') << setw(2) << num_remain[idx] << "/(" << setfill(' ') << fixed << setw(5)
             << setprecision(2) << num_remain[idx] * 100 / height * width * 0.01 << "%) 消除-";
        cout << defaultfloat;
        cout << resetiosflags(ios::right);
        cout << setiosflags(ios::left) << setw(max_eli_num * single_width - info_width) << num_eli[idx];

        cct_getxy(tmp_x, tmp_y);
        cct_showstr(tmp_x, tmp_y, outline[vertical], BOARD_COLOR, OUTLINE_COLOR);
        cct_gotoxy(board_x, ++tmp_y);
        cct_getxy(tmp_x, tmp_y);
    }

    show_level_line(NULL, max_eli_num, outline[level], outline[LOWER_LEFT], NULL,
                    outline[LOWER_RIGHT]);
    cct_setcolor();
}

void Game::visualize_info(int info_x, int info_y)
{
    int next_genr_bx, next_genr_by, ball_record_x, ball_record_y;
    
    // 每个info版面的左上角
    next_genr_bx = info_x;
    next_genr_by = info_y + 4;
    ball_record_x = next_genr_bx;
    ball_record_y = next_genr_by + 4;

    show_score(info_x, info_y);
    show_next_generated_ball(next_genr_bx, next_genr_by);
    remaining_info(ball_record_x, ball_record_y);
}

void Game::update_info(int info_x, int info_y)
{
    int next_genr_bx, next_genr_by, ball_record_x, ball_record_y, idx;

    // 每个info版面的左上角
    next_genr_bx = info_x;
    next_genr_by = info_y + 4;
    ball_record_x = next_genr_bx;
    ball_record_y = next_genr_by + 4;

    cct_setcolor(BOARD_COLOR, OUTLINE_COLOR);
    // update score
    cct_gotoxy(info_x + 6 + single_width, info_y + 1);
    cout << setw(MAX_SCORE_WIDTH) << score;

    //update next generate ball
    cct_gotoxy(next_genr_bx, next_genr_by + 1);
    show_level_line(next_generated_color, num_generated, ball[original], outline[vertical],
                    outline[vertical], outline[vertical]);

    // update remaining
    cct_setcolor(BOARD_COLOR, OUTLINE_COLOR);
    for (idx = 0; idx < COLOR_NUM + 1; ++idx)
    {
        cct_gotoxy(ball_record_x + 2 * single_width + 1, ball_record_y + 1 + idx);
        cout << setiosflags(ios::right);
        cout << setfill('0') << setw(2) << num_remain[idx] << "/(" << setfill(' ') << fixed << setw(5)
             << setprecision(2) << num_remain[idx] * 100 / height * width * 0.01 << "%) 消除-";
        cout << defaultfloat;
        cout << resetiosflags(ios::right);
        cout << setiosflags(ios::left) << setw(MAX_ELI_BALL_WIDTH) << num_eli[idx];
    }

    cct_setcolor();
}

bool Game::src_to_dst(coordinate* dst, coordinate* src, const int board_x, const int board_y)
{
    int idx;
    int current_score = 0;
    Astar astar(&board[0][0], src, dst, height, width);
    //coordinate genr_c;
    astar.find_path();

    if (astar.get_pre(dst)->x == -1)
        return 0;

    else
    {
        src_dst_anime(&astar, dst, src, board_x, board_y, board[src->x][src->y] + BASE_COLOR);
        cct_showstr(board_x + single_width + 2 * single_width * dst->y, board_y + 1 + 2 * dst->x, ball[original],
                    board[src->x][src->y] + BASE_COLOR, BOARD_COLOR);
        cct_setcolor();

        swap_board_value(src, dst);

        current_score = 0;
        if (!clear_ball(dst, current_score, 1, board_x, board_y))
        {
            for (idx = 0; idx < num_generated && avail_pos_num > 0; ++idx)
                place_ball(next_generated_color[idx], current_score, 1, board_x, board_y);
            
            generate_color(); // 找到路径 + 没有消除
        }
    }

    return 1;
}

void Game::control_board_mouse(int board_x, int board_y, coordinate* dst, coordinate* src, int max_place_num, bool show_info)
{
    int mx, my, x, y, maction, keycode1, keycode2, idx = 0, ret, color, tmp_x, tmp_y;
    int info_x, info_y;
    bool valid_flag = 0;
    bool src_clicked = 0;

    cct_getxy(x, y);
    cct_gotoxy(0, ++y);
    info_x = x + 2;
    info_y = 1;
    cct_getxy(x, y);
    cout << "[当前光标]  位置非法";
    cct_enable_mouse();
    cct_setcursor(CURSOR_INVISIBLE);

    if (show_info)
    {
        visualize_info(info_x, info_y);
        update_info(info_x, info_y);
    }

    do
    {
        if (avail_pos_num == 0)
            break;

        src_clicked = 0;
        for (; ; ) // 读取mouse信息
        {
            ret = cct_read_keyboard_and_mouse(mx, my, maction, keycode1, keycode2);

            if (ret == CCT_MOUSE_EVENT)
            {
                if (maction == MOUSE_ONLY_MOVED)
                {
                    cct_gotoxy(x, y);
                    if (valid_place(mx, my, board_x, board_y))
                    {
                        cout << setw(30) << ' ';
                        cct_gotoxy(x, y);
                        cout << "[当前光标]  " << (char)((my - board_y) / 2 + 'A') << "行"
                             << (mx - board_x) / (single_width * 2) + 1 << "列";
                        valid_flag = 1;
                    }
                    else if (valid_flag)
                    {
                        cout << setw(30) << ' ';
                        cct_gotoxy(x, y);
                        cout << "[当前光标]  位置非法";
                        valid_flag = 0;
                    }
                    else
                        ;
                }
                else if (maction == MOUSE_LEFT_BUTTON_CLICK)
                {
                    if (valid_place(mx, my, board_x, board_y))
                    {
                        color = board[(my - board_y) / 2][(mx - board_x) / (single_width * 2)];
                        if (color > 0)
                        {
                            if (src_clicked) // 将之前选中的变为original
                            {
                                board_to_console(src, tmp_x, tmp_y, board_x, board_y);
                                cct_showstr(tmp_x, tmp_y, ball[original], board[src->x][src->y] + BASE_COLOR, BOARD_COLOR);
                                cct_setcolor();
                            }
                            console_to_board(src, mx, my, board_x, board_y);
                            cct_showstr(mx - mx % single_width, my, ball[chosen], color + BASE_COLOR,
                                        BOARD_COLOR);
                            cct_setcolor();
                            src_clicked = 1;
                        }
                        else if (src_clicked == 1) // 已经判断 && color == 0
                        {
                            dst->x = (my - board_y) / 2;
                            dst->y = (mx - board_x) / (single_width * 2);
                            if (src_to_dst(dst, src, board_x, board_y))
                                break;
                            else
                            {
                                cct_gotoxy(x, y);
                                cout << "[错误] 无法从[" << (char)(src->x + 'A') << src->y + 1 << "]到["
                                     << (char)(dst->x + 'A') << dst->y + 1 << ']';
                            }
                        }
                        else
                            ;
                    }
                }

                else if (maction == MOUSE_RIGHT_BUTTON_CLICK)
                {
                    cct_setcursor(CURSOR_VISIBLE_NORMAL);
                    cct_gotoxy(x, y);
                    cct_disable_mouse();
                    return; // end game
                }
            }
            else if (ret == CCT_KEYBOARD_EVENT)
                ; // 忽略
            else
                return; // 错误
        }

        if (show_info)
            update_info(info_x, info_y);

        ++idx;
    } while ((max_place_num < 0 || idx < max_place_num) && valid_board(x, y));

    cct_setcursor(CURSOR_VISIBLE_NORMAL);
    cct_disable_mouse();
    cct_gotoxy(x, y);
    return;
}

void Game::cmd_game()
{
    int tmp_x, tmp_y, board_x, board_y;
    char ret;
    coordinate src, dst;

    cct_getxy(tmp_x, tmp_y);
    cct_gotoxy(0, tmp_y + 1);

    if (choice == 'D' || choice == 'E')
    {
        cout << "初始数组：" << endl;
        init_board(5);
        cout_board();
        cout << endl;
        cout << "按回车键显示图形..." << endl;

        while ((ret = _getch()) != '\r' && ret != '\n')
            ;

        cout << endl;

        if (choice == 'D')
            show_board_wo_line();
        else if (choice == 'E')
            show_board_line();
        else
            ; // 错误
        
        cout << endl;
    }

    if (choice == 'F')
    {
        cct_cls();
        cout << "鼠标移动，左键单机选择，右键退出" << endl;
        init_board((int)(height * width * 0.6));
        cct_getxy(board_x, board_y);
        show_board_line();
        cct_getxy(tmp_x, tmp_y);
        cct_setcolor();

        if (valid_board(0, tmp_y + 1))
            control_board_mouse(board_x, board_y, &dst, &src, 1);
    }

    if (choice == 'G')
    {
        cct_cls();
        cout << "鼠标移动，左键单机选择，右键退出" << endl;
        init_board(5);
        cct_getxy(board_x, board_y);
        show_board_line();
        cct_getxy(tmp_x, tmp_y);
        cct_setcolor();

        if (valid_board(0, tmp_y + 1))
            control_board_mouse(board_x, board_y, &dst, &src, -1, 1);
    }

    end();
}

/*-------------------------------------unused-------------------------------------*/
// if (1) // 上边框
//{
//     cct_showstr(tmp_x, tmp_y, double_outline[upper_left], BOARD_COLOR, OUTLINE_COLOR);

//    cct_getxy(tmp_x, tmp_y);
//    cct_showstr(tmp_x, tmp_y, double_outline[level], BOARD_COLOR, OUTLINE_COLOR, width);

//    cct_getxy(tmp_x, tmp_y);
//    cct_showstr(tmp_x, tmp_y, double_outline[upper_right], BOARD_COLOR, OUTLINE_COLOR, 1);
//}

// void Game::print_level_line(bool use_board_item, const char* left, const char* middle, const char* right, int
// line_index)
//{
//     int tmp_x, tmp_y, k;
//
//     cct_getxy(tmp_x, tmp_y);
//     cct_showstr(tmp_x, tmp_y, left, BOARD_COLOR, OUTLINE_COLOR);
//     cct_getxy(tmp_x, tmp_y);
//
//     for (k = 0; k < width - 1; ++k)
//     {
//         if (use_board_item)
//         {
//             cct_showstr(tmp_x, tmp_y, ball[original],
//                         board[line_index][k] + (board[line_index][k] == 0) * BOARD_COLOR +
//                             (board[line_index][k] > 0) * BASE_COLOR,
//                         BOARD_COLOR);
//         }
//         else
//             cct_showstr(tmp_x, tmp_y, double_outline[level], BOARD_COLOR, OUTLINE_COLOR);
//         cct_getxy(tmp_x, tmp_y);
//         cct_showstr(tmp_x, tmp_y, middle, BOARD_COLOR, OUTLINE_COLOR);
//         cct_getxy(tmp_x, tmp_y);
//     }
//
//     if (use_board_item)
//         cct_showstr(tmp_x, tmp_y, ball[original],
//                     board[line_index][k] + (board[line_index][k] == 0) * BOARD_COLOR +
//                         (board[line_index][k] > 0) * BASE_COLOR,
//                     BOARD_COLOR);
//     else
//         cct_showstr(tmp_x, tmp_y, double_outline[level], BOARD_COLOR, OUTLINE_COLOR);
//
//     cct_getxy(tmp_x, tmp_y);
//     cct_showstr(tmp_x, tmp_y, right, BOARD_COLOR, OUTLINE_COLOR);
// }
/*-------------------------------------unused-------------------------------------*/
