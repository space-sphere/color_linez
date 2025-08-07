/*2152778 ���� κͤ��*/
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

    cout << "������ĸ+������ʽ[����c2]����Ҫ�ƶ���ľ������꣺";
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
            cout << "��ʼλ��Ϊ�գ�����������." << endl;
            cout << "������ĸ+������ʽ[����c2]����Ҫ�ƶ���ľ������꣺";
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

    cout << "������ĸ+������ʽ[����c2]����Ҫ�ƶ����Ŀ�����꣺";
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
            cout << "Ŀ��λ�÷ǿգ�����������." << endl;
            cout << "������ĸ+������ʽ[����c2]����Ҫ�ƶ����Ŀ�����꣺";
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
    cout << "����Ϊ" << x_src << "��" << y_src << "��" << endl;

    base_input_dst(x_dst, y_dst);
    cct_getxy(tmp_x, tmp_y);
    cout << setw(100) << ' ';
    cct_gotoxy(0, tmp_y);
    cout << "����Ϊ" << x_dst << "��" << y_dst << "��" << endl;

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
        cout << "�޷��ҵ��ƶ�·��" << endl;
    }

    else
    {
        //cct_getconsoleborder(con_cols, con_lines, buf_cols, buf_lines);
        //cct_setconsoleborder(con_cols, con_lines, buf_cols, buf_lines + 30);
        tmp.x = dst.x;
        tmp.y = dst.y;
        cout << endl;
        cout << "���ҽ�����飺" << endl;
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
        cout << "�ƶ�·������ͬɫ��ʶ����" << endl;
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
        cout << "��" << num_generated << "���������ɫ�ֱ��ǣ�";

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
            cout << "�޷��ҵ��ƶ�·��" << endl;
        }

        else
        {
            cout << endl;   
            cout << "�ƶ�������飨��ͬ��ɫ����" << endl;
            swap_board_value(&src, &dst);
            
            current_score = 0;
            if (!clear_ball(&dst, current_score))
            {
                for (idx = 0; idx < num_generated && avail_pos_num > 0; ++idx)
                {
                    place_ball(next_generated_color[idx], current_score);
                }
                generate_color(); // �ҵ�·�� + û������
            }

            cout_board();
            cout << "���ε÷֣�" << current_score << ' ' << "�ܵ÷֣�" << score << endl;

            if (!valid_board())
            {
                break;
            }
        }

        cout << endl;
        cout << "��ǰ����" << endl;
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
        cout << "��ʼ���飺" << endl;
        init_board(5);
        cout_board();
    }

    else if (choice == 'B')
    {
        cout << "��ǰ���飺" << endl;
        init_board((int)(width * height * 0.6));
        cout_board();
        
        if (valid_board())
        {
            generate_color();
            cout << endl;
            cout << "��" << num_generated << "���������ɫ�ֱ��ǣ�";

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
            cout << "���ε÷֣�" << score << ' ' << "�ܵ÷֣�" << score << endl;
            cout << "�������������������Ϸ����" << endl;
        }
    }

    else if (choice == 'C')
    {
        cout << "��ǰ����" << endl;
        init_board(5);
        cout_board();
        if (valid_board())
        {
            update_cout_board();
        }
        else
        {
            cout << endl;
            cout << "���ε÷֣�" << score << ' ' << "�ܵ÷֣�" << score << endl;
            cout << "�������������������Ϸ����" << endl;
        }
    }
    
    cout << endl;
    end();
}