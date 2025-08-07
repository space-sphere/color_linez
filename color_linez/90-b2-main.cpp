/*2152778 数金 魏亭舟*/
#include "90-b2.h"

char menu()
{
    int mx, my, maction, keycode1, keycode2;

    cout << "-----------------------------------------" << endl;
    cout << "A.内部数组,随机生成初始5个球" << endl;
    cout << "B.内部数组,随机生成60%的球,寻找移动路径" << endl;
    cout << "C.内部数组,完整版" << endl;
    cout << "D.画出n*n的框架(无分隔线),随机显示5个球" << endl;
    cout << "E.画出n*n的框架(有分隔线)，随机显示5个球" << endl;
    cout << "F.n*n的框架,60%的球,支持鼠标,完成一次移动" << endl;
    cout << "G.cmd图形界面完整版" << endl;
    cout << "Q.退出" << endl;
    cout << "-----------------------------------------" << endl;
    cout << "[请选择:] ";
    
    for (; ; )
    {
        cct_read_keyboard_and_mouse(mx, my, maction, keycode1, keycode2);

        if (keycode1 >= 'a')
        {
            keycode1 -= 32;
        }

        if (keycode1 < 'A' || keycode1 > 'G' && keycode1 != 'Q')
            continue;

        return keycode1;
    }
}

int main()
{
    char choice = 'A';
    Game game(MIN_HEIGHT, MIN_WIDTH);

    cct_setconsoleborder(CONSOLE_COLS, CONSOLE_LINES, -1, BUFFER_LINES);
    cct_setfontsize("新宋体", FONT_SIZE);

    srand((unsigned int)time(0));

    for (; choice != 'Q'; )
    {
        choice = menu();

        if (choice != 'Q')
        {
            cct_cls();
            game.input(choice);
            game.start();
        }
    }

    cct_cls();
    game.end();
    return 0;
}
