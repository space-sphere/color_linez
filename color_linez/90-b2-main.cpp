/*2152778 ���� κͤ��*/
#include "90-b2.h"

char menu()
{
    int mx, my, maction, keycode1, keycode2;

    cout << "-----------------------------------------" << endl;
    cout << "A.�ڲ�����,������ɳ�ʼ5����" << endl;
    cout << "B.�ڲ�����,�������60%����,Ѱ���ƶ�·��" << endl;
    cout << "C.�ڲ�����,������" << endl;
    cout << "D.����n*n�Ŀ��(�޷ָ���),�����ʾ5����" << endl;
    cout << "E.����n*n�Ŀ��(�зָ���)�������ʾ5����" << endl;
    cout << "F.n*n�Ŀ��,60%����,֧�����,���һ���ƶ�" << endl;
    cout << "G.cmdͼ�ν���������" << endl;
    cout << "Q.�˳�" << endl;
    cout << "-----------------------------------------" << endl;
    cout << "[��ѡ��:] ";
    
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
    cct_setfontsize("������", FONT_SIZE);

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
