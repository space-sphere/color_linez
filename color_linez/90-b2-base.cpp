/*2152778 数金 魏亭舟*/
#include "90-b2.h"

void Game::cout_board(int bg_color, bool real_board, bool use_fg_white)
{
    int line, idx;

    // first line
    cout << "  " << '|';
    for (idx = 0; idx < width; ++idx)
    {
        cout << setw(3) << idx + 1;
    }
    cout << endl;

    //second line
    cout << "--" << "+";
    cout << setfill('-');
    cout << setw(3 * width) << '-' << '-' << endl;
    cout << setfill(' ');

    // inner array
    for (line = 0; line < height; ++line)
    {
        cout << (char)('A' + line) << ' ' << '|';
        
        if (real_board)
        {
            for (idx = 0; idx < width; ++idx)
            {
                cout << setw(2) << ' ';
                if (!use_fg_white && board[line][idx] > 0)
                    cct_setcolor(bg_color, board[line][idx]);
                else
                    ;
                cout << board[line][idx];
                cct_setcolor();
            }
        }
        else
        {
            for (idx = 0; idx < width; ++idx)
            {
                cout << setw(3) << 0;
                
            }
        }
        cout << endl;
 
    }
}

void Game::base_input_src(char& x, int& y)
{
    int tmp_x, tmp_y;
    char coor[3] = {0};

    cout << "请以字母+数字形式[例：c2]输入要移动球的矩阵坐标：";
    cct_getxy(tmp_x, tmp_y);
    for (;;)
    {
        cin.ignore(INT_MAX, '\n');
        cin.get(coor, 2 + 1);
        if (!cin.good())
        {
            cin.clear();
            input_error(tmp_x, tmp_y);
            continue;
        }

        x = coor[0];

        if (x >= 'a')
            x -= 32;

        if (x < 'A' || x > 'A' + height - 1)
        {
            input_error(tmp_x, tmp_y);
            continue;
        }

        y = coor[1] - '0';

        if (y < 1 || y > width)
        {
            input_error(tmp_x, tmp_y);
            continue;
        }

        if (board[(int)(x - 'A')][y - 1] == 0)
        {
            cout << "起始位置为空，请重新输入." << endl;
            cout << "请以字母+数字形式[例：c2]输入要移动球的矩阵坐标：";
            cct_getxy(tmp_x, tmp_y);
            continue;
        }

        break;
    }
}

void Game::base_input_dst(char &x, int &y)
{
    int tmp_x, tmp_y;
    char coor[3] = {0};

    cout << "请以字母+数字形式[例：c2]输入要移动球的目的坐标：";
    cct_getxy(tmp_x, tmp_y);
    for (;;)
    {
        cin.ignore(INT_MAX, '\n');
        cin.get(coor, 2 + 1);
        if (!cin.good())
        {
            cin.clear();
            input_error(tmp_x, tmp_y);
            continue;
        }

        x = coor[0];

        if (x >= 'a')
            x -= 32;

        if (x < 'A' || x > 'A' + height - 1)
        {
            input_error(tmp_x, tmp_y);
            continue;
        }

        y = coor[1] - '0';

        if (y < 1 || y > width)
        {
            input_error(tmp_x, tmp_y);
            continue;
        }

        if (board[(int)(x - 'A')][y - 1] != 0)
        {
            cout << "目标位置非空，请重新输入." << endl;
            cout << "请以字母+数字形式[例：c2]输入要移动球的目的坐标：";
            cct_getxy(tmp_x, tmp_y);
            continue;
        }

        break;
    }
}

void Game::src_dst(coordinate& src, coordinate& dst)
{
    int y_src, y_dst, tmp_x, tmp_y;
    char x_src, x_dst;
    bool flag = 0;

    base_input_src(x_src, y_src);
    cct_getxy(tmp_x, tmp_y);
    cout << setw(100) << ' ';
    cct_gotoxy(0, tmp_y);
    cout << "输入为" << x_src << "行" << y_src << "列" << endl;

    base_input_dst(x_dst, y_dst);
    cct_getxy(tmp_x, tmp_y);
    cout << setw(100) << ' ';
    cct_gotoxy(0, tmp_y);
    cout << "输入为" << x_dst << "行" << y_dst << "列" << endl;

    src.x = x_src - 'A';
    src.y = y_src - 1;

    dst.x = x_dst - 'A';
    dst.y = y_dst - 1;
}

void Game::cout_path()
{
    int tmp_x, tmp_y, end_x, end_y;
    coordinate tmp, src, dst;

    src_dst(src, dst);
    Astar search(&board[0][0], &src, &dst, height, width);
    search.find_path();

    if (search.get_pre(&dst)->x == -1)
    {
        cout << "无法找到移动路径" << endl;
    }

    else
    {
        //cct_getconsoleborder(con_cols, con_lines, buf_cols, buf_lines);
        //cct_setconsoleborder(con_cols, con_lines, buf_cols, buf_lines + 30);
        tmp.x = dst.x;
        tmp.y = dst.y;
        cout << endl;
        cout << "查找结果数组：" << endl;
        cct_getxy(tmp_x, tmp_y);
        cout_board(COLOR_BLACK, 0);
        cct_getxy(end_x, end_y);

        cct_gotoxy(tmp_x + 3 * (tmp.y + 1) + 2, tmp_y + tmp.x + 2);
        cout << '*';
        while (tmp.x != src.x || tmp.y != src.y)
        {
            tmp = *search.get_pre(&tmp);
            cct_gotoxy(tmp_x + 3 * (tmp.y + 1) + 2, tmp_y + tmp.x + 2);
            cout << '*';
        }

        tmp.x = dst.x;
        tmp.y = dst.y;

        cct_gotoxy(end_x, end_y);
        cout << endl;
        cout << "移动路径（不同色标识）：" << endl;
        cct_getxy(tmp_x, tmp_y);
        cout_board(COLOR_BLACK, 1, 1);
        cct_getxy(end_x, end_y);

        cct_showch(tmp_x + 3 * (tmp.y + 1) + 2, tmp_y + tmp.x + 2, '0', COLOR_HYELLOW, COLOR_BLUE);
        while (tmp.x != src.x || tmp.y != src.y)
        {
            tmp = *search.get_pre(&tmp);
            cct_showch(tmp_x + 3 * (tmp.y + 1) + 2, tmp_y + tmp.x + 2, board[tmp.x][tmp.y] + '0', COLOR_HYELLOW, COLOR_BLUE);
        }

        cct_setcolor();
        cct_gotoxy(end_x, end_y);
    }
}

void Game::update_cout_board()
{
    int idx;
    int current_score = 0;
    coordinate src, dst;
    
    for (; avail_pos_num > 0;)
    {
        cout << endl;
        cout << "下" << num_generated << "个彩球的颜色分别是：";

        for (idx = 0; idx < num_generated; ++idx)
        {
            cout << next_generated_color[idx] << ' ';
        }
        cout << endl;
        src_dst(src, dst);
        Astar search(&board[0][0], &src, &dst, height, width);
        search.find_path();

        if (search.get_pre(&dst)->x == -1)
        {
            cout << "无法找到移动路径" << endl;
        }

        else
        {
            cout << endl;   
            cout << "移动后的数组（不同颜色）：" << endl;
            swap_board_value(&src, &dst);
            
            current_score = 0;
            if (!clear_ball(&dst, current_score))
            {
                for (idx = 0; idx < num_generated && avail_pos_num > 0; ++idx)
                {
                    place_ball(next_generated_color[idx], current_score);
                }
                generate_color(); // 找到路径 + 没有消除
            }

            cout_board();
            cout << "本次得分：" << current_score << ' ' << "总得分：" << score << endl;

            if (!valid_board())
            {
                break;
            }
        }

        cout << endl;
        cout << "当前数组" << endl;
        cout_board();
    }
}

void Game::base_game()
{
    int tmp_x, tmp_y, idx;

    cct_getxy(tmp_x, tmp_y);
    cct_gotoxy(0, tmp_y + 1);

    if (choice == 'A')
    {
        cout << "初始数组：" << endl;
        init_board(5);
        cout_board();
    }

    else if (choice == 'B')
    {
        cout << "当前数组：" << endl;
        init_board((int)(width * height * 0.6));
        cout_board();
        
        if (valid_board())
        {
            generate_color();
            cout << endl;
            cout << "下" << num_generated << "个彩球的颜色分别是：";

            for (idx = 0; idx < num_generated; ++idx)
            {
                cout << next_generated_color[idx] << ' ';
            }
            cout << endl;

            cout_path();
        }

        else
        {
            cout << endl;
            cout << "本次得分：" << score << ' ' << "总得分：" << score << endl;
            cout << "因所有球均被消除，游戏结束" << endl;
        }
    }

    else if (choice == 'C')
    {
        cout << "当前数组" << endl;
        init_board(5);
        cout_board();
        if (valid_board())
        {
            update_cout_board();
        }
        else
        {
            cout << endl;
            cout << "本次得分：" << score << ' ' << "总得分：" << score << endl;
            cout << "因所有球均被消除，游戏结束" << endl;
        }
    }
    
    cout << endl;
    end();
}