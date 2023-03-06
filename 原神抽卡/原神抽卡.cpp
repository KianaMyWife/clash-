
#include "pool.h"
#include <random>
#include <cstdlib>
#include <time.h>
#include <windows.h>
double pro_5 = 0.6;  // 五星基础概率
double pro_4 = 5.1;  // 四星基础概率
double pro_3 = 94.3; // 三星基础概率
double arms_5 = 0.7; // 五星武器基础概率
bool wflag;          // 是否为UP，0为歪，1为up
bool nflag;          // 角色为0；武器为1
int sum;             // 总抽数计数
int sum10;           // 10发保底计数
int sum90;           // up与常驻90发保底计数
int sum80;           // 武器池计数
int con;             // 平衡数,防止一直出角色不出武器,设为3 出人-1出武器+1

int up4;     // 统计4星up数
int up5;     // 统计5星up数
bool wflag4; // 判断4星是否歪了up

int arms_55 = arms5.size();
int arms_4 = arms4.size();
int arms_3 = arms3.size();
int p_5 = pro5.size();
int p_4 = pro4.size();

// 是否显示抽卡过程
bool isshow3;
bool isshow4;
bool isshow5;

// 是否显示出货抽数
bool isshowtemp90;
bool isshowtemp10;
vector<int> temp90;
vector<int> temp10;

// 是否显示出的货
bool isshow44;
bool isshow55;
vector<string> star_4;
vector<string> star_5;

int crand(int n) // 随机数生成
{
    timeb timeSeed;
    ftime(&timeSeed);
    srand(timeSeed.time * 751 + timeSeed.millitm * 773); // millitime
    // 生成[0到n-1]范围内的数值，包括b和a
    unsigned int range = n;
    unsigned int crand = rand() % range;
    return crand;
}

int chouka(int sum_10, int sum_90) // 判断当前抽为几星
{
    int c = crand(1000); // 随机数种子
    int s = c;
    if (sum_90 >= 74) // 保底加概率从74后每发概率加6%
        c -= 60 * (sum_90 - 73);
    if (c < 6)
    {
        sum10++;
        temp90.push_back(sum90 + 1);
        sum90 = 0; // 重置保底数
        return 5;  // 出五星
    }
    else
    {
        if (sum_10 == 8)
            s -= 510;
        if (s < 57)
        { // 出四星
            temp10.push_back(sum10 + 1);
            sum10 = 0;
            sum90++;
            return 4;
        }

        else
        {

            if (sum_10 >= 9)
            { // 保底4星
                temp10.push_back(sum10 + 1);
                sum10 = sum10 - 9; // 不为0是因为有可能5星把4星卡掉,导致10发之后才出4星,此时4星概率会偏低
                sum90++;
                return 4;
            }
            else
            { // 出三星
                sum10++;
                sum90++;
                return 3;
            }
        }
    }
}

void wai() // 判断是否歪了 0为没歪,1为歪了
{
    bool c = crand(100) & 1;
    if (wflag == 1)
    {
        wflag = 0;
    }
    else
    {
        wflag = c;
    }
}
void wai4() // 判断是否歪了 0为没歪,1为歪了
{
    bool c = crand(100) & 1;
    if (wflag4 == 1)
        wflag4 = 0;
    else
        wflag4 = c;
}

int typem(int typ) // 0为角色,1为武器   输入为当前物品星级
{
    if (typ == 5)
        return 0;
    if (typ == 3)
        return 1;

    if (typ == 4)
    {
        int c = crand(100) & 1;
        Sleep(1);     // 同样的BUG
        if (con == 3) // 保底人物
        {
            con = 0;
            return 0;
        }

        if (con == -3) // 保底武器
        {
            wflag4 = 1;
            con = 0;
            return 1;
        }

        if (c == 1)
        { // 武器
            if (wflag4 == 1)
                return 0;

            wflag4 = 1;
            if (con >= 0)
                con++;
            else
                con = 0;
            return 1;
        }
        if (!c)
        { // 人物
            if (con <= 0)
                con--;
            else
                con = 0;
            return 0;
        }
    }
}

string pro(int typ, bool typem) // 输出当前抽到的东西
{

    if (typ == 3)
    {
        int n = crand(arms_3);
        return arms3[n];
    }

    if (typ == 4 && typem == 0)
    { // 4星角色

        int n = crand(p_4);
        int up = crand(100) & 1;

        if (wflag4 == 1)
            up = 1;

        if (up == 1)
        {
            wflag4 = 0;
            up4++;
            return uppro[crand(3)];
        }
        if (up == 0)
        {
            wflag4 = 1;
            return pro4[n];
        }
    }

    if (typ == 4 && typem == 1)
    { // 4星武器
        int n = crand(arms_4);
        return arms4[n];
    }

    if (typ == 5 && typem == 0)
    { // 5星角色
        wai();
        if (!wflag)
        {
            up5++;
            return uppro1;
        }
        else
        {
            Sleep(1); // 不知道为什么此处有BUG只能Sleep,不然常驻只会出莫纳迪卢克提哪里,即n只会取1,3,5.sleep1后bug消失
            int n = crand(p_5);

            return pro5[n];
        }
    }
}

void ck(int sum_10, int sum_90) // 抽卡输出
{

    int star = chouka(sum_10, sum_90);
    int typ = typem(star);
    string gets = pro(star, typ);

    if (star == 3)
    {
        if (isshow3)
            cout << "你抽到了:" << gets << "  当前抽数:" << sum90 << " 当前10发内抽数" << sum10 << endl;
    }
    if (star == 4)
    {
        if (isshow4)
        {
            cout << "你抽到了:" << gets << "  当前抽数:" << sum90 << " 当前10发内抽数" << temp10.back() << endl;
        }

        star_4.push_back(gets);
    }

    if (star == 5)
    {

        cout << "你抽到了:" << gets << "*****"
             << "  当前抽数:" << temp90.back() << " 当前10发内抽数" << sum10 << endl;

        star_5.push_back(gets);
    }
    // cout << endl;
}
int main()
{
    printf("--------------------------------\n");
    printf("          原神抽卡模拟\n");
    printf("--------------------------------\n");
    int n;
    cout << "输入抽卡次数:(输入0关闭)";
    cin >> n;
    if (n == 0)
        return 0;
    cout << "是否显示3*抽卡过程:(0不,1显示)";
    cin >> isshow3;
    cout << "是否显示4*抽卡过程:(0不,1显示)";
    cin >> isshow4;
    cout << "是否显示4*抽卡结果:(0不,1显示):";
    cin >> isshow44;
    cout << "是否显示4*出货抽数:(0不,1显示):";
    cin >> isshowtemp10;

    for (int i = 0; i < n; i++)
    {
        ck(sum10, sum90);
        Sleep(1);
    }

    cout << "4星共有:" << star_4.size() << "个:";
    if (isshow44)
    {
        for (int i = 0; i < star_4.size(); i++)
        {
            if (i % 10 == 0)
                cout << endl;
            cout << star_4[i] << " ";
        }
    }
    cout << endl;
    cout << "5星共有:" << star_5.size() << "个:";
    for (auto &i : star_5)
        cout << i << " ";
    cout << endl;
    if (isshowtemp10)
    {
        cout << "4星出货抽数:";
        for (int i = 0; i < temp10.size(); i++)
        {
            if (i % 10 == 0)
                cout << endl;
            cout << temp10[i] << " ";
        }
    }
    cout << endl;
    cout << "5星出货抽数:";
    for (auto &i : temp90)
        cout << i << " ";
    cout << endl;
    cout << "up4*数:" << up4;
    cout << endl;
    cout << "up5*数:" << up5;
    cout << endl;
    main();
    return 0;
}