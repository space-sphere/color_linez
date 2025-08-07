/*2152778 数金 魏亭舟*/
#include "90-b2.h"

const int direc[][2] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
const int half_direc[][2] = {{1, 0}, {1, -1}, {0, -1}, {-1, -1}};

const char* outline[] = {"╔", "╚", "╗", "╝", "═", "║", "╦", "╩", "╠", "╣", "╬"};

/*-------------------------------Game-------------------------------*/
Game::Game(int h, int w) : num_remain{0}, num_eli{0}
{
    int i, j;

    height = h;
    width = w;

    for (i = 0; i < MAX_HEIGHT; ++i)
    {
        for (j = 0; j < MAX_WIDTH; ++j)
        {
            board[i][j] = 0;
        }
    }

    avail_pos_num = 0;
    choice = 'A';

    generate_color();

    for (i = 0; i < MAX_HEIGHT * MAX_WIDTH; ++i) // input
    {
        available_pos[i].x = 0;
        available_pos[i].y = 0;
    }
    
    score = 0;
}

void Game::input(char ch)
{
    int i, h, w;

    choice = ch;

    for (; ; )
    {
        cout << "请输入行数（" << MIN_HEIGHT << "-" << MAX_HEIGHT << "）：" << endl;
        cin >> h;

        if (!cin.good() || (h >= MAX_HEIGHT + 1 || h <= MIN_HEIGHT - 1))
        {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }
        else
        {
            break;
        }
    }

    for (; ; )
    {
        cout << "请输入列数（"<< MIN_WIDTH << "-" << MAX_WIDTH << "）：" << endl;
        cin >> w;

        if (!cin.good() || (w >= MAX_WIDTH + 1 || w <= MIN_WIDTH - 1))
        {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            continue;
        }
        else
        {
            break;
        }
    }

    width = w;
    height = h;
    avail_pos_num = width * height;
    num_remain[0] = width * height;

    for (i = 0; i < width * height; ++i)
    {
        available_pos[i].y = i % width;
        available_pos[i].x = i / width;
    }
}

void Game::start()
{
    if (choice >= 'A' && choice <= 'C')
    {
        base_game();
    }

    else if (choice >= 'D' && choice <= 'G')
    {
        cmd_game();
    }

    else // 错误情形
    {
        return;
    }
}

void input_error(int x, int y)
{
    cct_gotoxy(x, y);
    cout << setw(100) << ' ';
    cct_gotoxy(0, y + 1);
    cout << "输入错误，请重新输入.";
    cct_gotoxy(x, y);
}

void swap_cor(coordinate* pos1, coordinate* pos2)
{
    int x, y;

    x = pos2->x;
    y = pos2->y;
    pos2->x = pos1->x;
    pos2->y = pos1->y;
    pos1->x = x;
    pos1->y = y;
}

void Game::swap_board_value(coordinate* src, coordinate* dst)
{
    int tmp, idx;

    tmp = board[dst->x][dst->y];
    board[dst->x][dst->y] = board[src->x][src->y];
    board[src->x][src->y] = tmp;

    for (idx = 0; idx < avail_pos_num; ++idx)
    {
        if (available_pos[idx].x == dst->x && available_pos[idx].y == dst->y)
        {
            available_pos[idx] = *src;
        }
    }
}

void add_cor(coordinate* dst_cor, const coordinate* src_cor)
{
    *dst_cor = *src_cor;
}

void Game::place_ball(int color, int& current_score, bool show_board, int board_x, int board_y)
{
    int idx;
    coordinate dst;

    // set color
    idx = (int)rand() % avail_pos_num;
    board[available_pos[idx].x][available_pos[idx].y] = color;
    ++num_remain[color];
    --num_remain[0];
    dst.x = available_pos[idx].x;
    dst.y = available_pos[idx].y;

    if (show_board)
        show_one_ball(&dst, board_x, board_y, board[dst.x][dst.y]);

    // swap
    swap_cor(&available_pos[idx], &available_pos[--avail_pos_num]);

    // check adj
    clear_ball(&dst, current_score, show_board, board_x, board_y);
}

void Game::count_adj_ball(int x, int y, const int* xy_direc, int& n)
{
    if (x + *xy_direc >= 0 && x + *xy_direc < height && y + *(xy_direc + 1) >= 0 && y + *(xy_direc + 1) < width &&
        board[x][y] == board[x + *xy_direc][y + *(xy_direc + 1)])
    {
        ++n;
        count_adj_ball(x + *xy_direc, y + *(xy_direc + 1), xy_direc, n);
    }
}

bool Game::valid_board(int x, int y)
{
    int i, j;
    for (i = 0; i < height; ++i)
    {
        for (j = 0; j < width; ++j)
        {
            if (board[i][j] != 0)
            {
                return 1;
            }
        }
    }

    if (x >= 0 && y >= 0)
    {
        cct_gotoxy(x, y);
    }
    cout << "因所有球均被消除，游戏结束" << endl;
    Sleep(1500);
    return 0;
}

bool Game::clear_ball(coordinate* dst, int& current_score, bool show_board, int board_x, int board_y)
{
    int idx, m, n, k;
    int inver_direc[2];
    bool flag = 0; // 1: clear 0: not
    coordinate tmp;

    if (dst->x == -1 || dst->y == -1)
        return 0; // dst错误

    for (idx = 0; idx < sizeof(half_direc) / sizeof(half_direc[0]); ++idx)
    {
        n = 0;
        m = 0;

        count_adj_ball(dst->x, dst->y, half_direc[idx], n);
        inver_direc[0] = -half_direc[idx][0];
        inver_direc[1] = -half_direc[idx][1];
        count_adj_ball(dst->x, dst->y, inver_direc, m);

        if (m + n >= 5 - 1) // 补上dst位置
        {
            flag = 1;

            for (k = 0; k < n; ++k)
            {
                board[dst->x + (k + 1) * half_direc[idx][0]][dst->y + (k + 1) * half_direc[idx][1]] = 0;
                --num_remain[board[dst->x][dst->y]];
                ++num_eli[board[dst->x][dst->y]];
                ++num_remain[0];
                tmp.x = dst->x + (k + 1) * half_direc[idx][0];
                tmp.y = dst->y + (k + 1) * half_direc[idx][1];

                if (show_board)
                    show_one_ball(&tmp, board_x, board_y);

                add_cor(&available_pos[avail_pos_num++], &tmp);
            }

            for (k = 0; k < m; ++k)
            {
                --num_remain[board[dst->x][dst->y]];
                ++num_eli[board[dst->x][dst->y]];
                ++num_remain[0];
                board[dst->x + (k + 1) * inver_direc[0]][dst->y + (k + 1) * inver_direc[1]] = 0;

                tmp.x = dst->x + (k + 1) * inver_direc[0];
                tmp.y = dst->y + (k + 1) * inver_direc[1];

                if (show_board)
                    show_one_ball(&tmp, board_x, board_y);

                add_cor(&available_pos[avail_pos_num++], &tmp);
            }

            current_score += (m + n) * (m + n - 1);
        }
    }

    if (flag)
    {
        --num_remain[board[dst->x][dst->y]];
        ++num_eli[board[dst->x][dst->y]];
        ++num_remain[0];
        board[dst->x][dst->y] = 0;

         if (show_board)
            show_one_ball(dst, board_x, board_y);

        add_cor(&available_pos[avail_pos_num++], dst);
    }

    score += current_score;

    return flag;
}

void Game::init_board(int n, bool show_board, int board_x, int board_y)
{
    int i, c, tmp_x = (int)rand() % width, tmp_y = (int)rand() % height;
    int current_score = 0;

    for (i = 0; i < n && avail_pos_num > 0; ++i)
    {
        if (RANDOM_COLOR)
            c = rand() % COLOR_NUM + 1;
        else
            c = FIXED_COLOR;
        place_ball(c, current_score, show_board, board_x, board_y);
    }

    score += current_score;
}

void Game::generate_color()
{
    int idx;

    for (idx = 0; idx < num_generated; ++idx)
    {
        if (RANDOM_COLOR)
            next_generated_color[idx] = (int)rand() % COLOR_NUM + 1;
        else
            next_generated_color[idx] = FIXED_COLOR;
    }
}

void Game::end()
{
    int tmp_x, tmp_y, i;
    char End[3 + 1], ret;
    cout << "本小题结束，请输入End继续...";
    cct_getxy(tmp_x, tmp_y);

    End[3] = '\0';
    for (;;)
    {
        cin.ignore(INT_MAX, '\n');
        cin.get(End, 3 + 1);

        if (!cin.good() || (strcmp(End, "End") != 0 && strcmp(End, "end") != 0))
        {
            input_error(tmp_x, tmp_y);
            cin.clear();
            // cin.ignore(INT_MAX, '\n'); // 这里如果加到程序中，会出现很奇怪的错
            continue;
        }
        else
        {
            cct_gotoxy(0, 25);
            cout << "请按任意键继续. . . ";
            ret = getchar();
            cct_cls();
            break;
        }
    }

    // 初始化信息
    for (i = 0; i < COLOR_NUM; ++i)
    {
        num_remain[i] = 0;
        num_eli[i] = 0;
    }

    // 初始化棋盘
    for (i = 0; i < width * height; ++i)
    {
        board[i % height][i / height] = 0;
    }

    score = 0;
}

/*------------------------------Astar------------------------------*/
Astar::Astar(int* p_board, coordinate* s, coordinate* d, int h, int w, int t)
    : board(p_board), height(h), width(w), src(s), dst(d), top(t) {}

void Astar::del_node(Node* p)
{
    --top;

    p->x = node_que[top].x;
    p->y = node_que[top].y;
    p->H = node_que[top].H;
    p->G = node_que[top].G;
    p->F = node_que[top].F;
    
    //node[top].F = -1; // 上面搜索条件为找到F为-1的为止
}

void Astar::add_node(Node* p)
{
    node_que[top].x = p->x;
    node_que[top].y = p->y;
    node_que[top].H = p->H;
    node_que[top].G = p->G;
    node_que[top].F = p->F;

    ++top;
}

int Manhattan(int x0, int y0, int x1, int y1)
{
    return abs(x0 - x1) + abs(y0 - y1);
}

bool Astar::valid_node(Node* node)
{
    if (node->x < 0 || node->x >= height || node->y < 0 || node->y >= width)
    {
        return 0;
    }

    if (*(board + node->y + node->x * MAX_WIDTH) > 0)
    {
        return 0;
    }

    return 1;
}

Node* Astar::top_index(void)
{
    Node *p, *min_f;

    for (p = node_que + 1, min_f = node_que; p - node_que < top; ++p)
    {
        if (p->get_f() < min_f->get_f())
            min_f = p;
    }

    return min_f;
}

void Astar::find_path()
{
    int idx; // 模拟node的对列长度
    Node* p_node_current;
    Node node_current, node_next;

    top = 0;
    node_que[top].x = src->x;
    node_que[top].y = src->y;

    node_que[top].G = 0;
    node_que[top].H = Manhattan(src->x, src->y, dst->x, dst->y);
    node_que[top].F = node_que[top].H;
    search_log[src->x][src->y].F = node_que[top].F;
    ++top;

    for (; top > 0;)
    {
        p_node_current = top_index();
        node_current = *p_node_current;
        del_node(p_node_current);
        search_log[node_current.x][node_current.y].visited = 1;
        // arrive at dst
        if (node_current.x == dst->x && node_current.y == dst->y)
            break;

        for (idx = 0; idx < sizeof(direc) / sizeof(direc[0]); ++idx)
        {
            node_next.build(node_current.x + direc[idx][0], node_current.y + direc[idx][1], dst, &node_current);

            if (valid_node(&node_next) && !search_log[node_next.x][node_next.y].visited)
            {
                // log中F小于零：还未加入node_que中
                if (search_log[node_next.x][node_next.y].F < 0 || node_next.F < search_log[node_next.x][node_next.y].F)
                {
                    search_log[node_next.x][node_next.y].pre_coord.x = node_current.x;
                    search_log[node_next.x][node_next.y].pre_coord.y = node_current.y;
                    search_log[node_next.x][node_next.y].F = node_next.F;
                    add_node(&node_next);
                }
            }
        }
    }
}

coordinate* Astar::get_pre(coordinate* pos)
{
    return &search_log[pos->x][pos->y].pre_coord;
}

/*-------------------------------Node-------------------------------*/
void Node::build(int xx, int yy, const coordinate* dst, Node* node)
{
    x = xx;
    y = yy;
    
    //G = node->G + Manhattan(x, y, node->x, node->y);
    G = node->G + 1;
    H = Manhattan(x, y, dst->x, dst->y);
    F = G + H;
}

int Node::get_f()
{
    return F;
}
