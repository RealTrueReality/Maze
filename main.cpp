#include <iostream>//文件读写流
#include<stdlib.h>
#include<time.h>
#include <easyx.h> //Easyx图形库
#include <tchar.h>   //easyx数字转字符
#include <string.h> //memset的头文件
#include <windows.h>
#include<fstream>
#include<string>
/**
 * 8、迷宫问题求解
【问题描述】
编写一个程序求解迷宫问题。迷宫由m行n列的二维数组设置，0表示无障碍，1表示有障碍。设入口为（1，0），出口为（m-2，n-1）
 每次只能从一个无障碍单元移到周围四个方向上任一无障碍单元。编写实现对任意设定的迷宫，求出从入口到出口的通路，如果有多条通路，比较路径长短，如果没有通路给出没有通路的结论。
主要涉及到的知识点:深度优先搜索算法、广度优先搜索算法、文件
【基本要求】
（1）创建迷宫，迷宫可随机生成，也可由文件读取。//TODO 文件读取
（2）在搜索路径时，用两种搜索方式，深度优先搜索和广度优先搜索。//TODO  DFS
（3）算法输入，迷宫规模m（行数）、n（列数）；
（4）算法输出，给出形象迷宫通路图。
（5）小结，比较两种算法的优缺点。（已实现了哪些功能？遇到哪些问题，如何解决的？以后哪些方面需要加强？）
 */

using namespace std;

typedef struct node {
    int x, y;  //记录某点的行数和列数
    int step;  //记录从起点到该点的步数
} node;

node pre[100][100], maze_path[100];  //pre[][]记录某点的前趋结点,maze_path[]记录迷宫最短路径的的各点
//队列结构体
typedef struct Queue_node {
    node node;   //类型为node结构体
    struct Queue_node *next;
} Queue_node;

typedef struct Queue {
    Queue_node *front;
    Queue_node *rear;
} Queue;

//初始化队列，头指针和尾指针为空
void Init(Queue *q) {
    q->front = q->rear = 0;
}

//判断队列是否为空
bool QueueEmpty(Queue *q) {
    return (q->front == NULL); //当队列的队头元素为空时，队列为空
}

//入队
void EnQueue(node &no, Queue *q) {
    Queue_node *temp = new Queue_node();   //生成一个类型为Queue_node的新结点
    temp->node = no;
    temp->next = NULL;
//如果队列为空,则队列的头指针和尾指针都指向新生成的结点temp
    if (q->front == NULL) {
        q->front = temp;
        q->rear = temp;
    } else {
        //若队列不空，则更新尾指针
        q->rear->next = temp;
        q->rear = temp;
    }
}

//出队列，取队头元素
node DeQueue(Queue *q) {
    node n = q->front->node;  //取出队头的node，即取出迷宫中保存某点的各信息，即x,y和step
    Queue_node *que = q->front; //取出队头元素
    if (q->front == q->rear) {
        //如果队头元素等于队尾，即该队列只有一个元素，则出队时，头指针和尾指针都赋值为0
        q->rear = 0;
        q->front = 0;
    } else      //否则更新头指针，指向下一个结点
        q->front = q->front->next;
    free(que); //回收队头元素
    return n; //返回迷宫结点
}

//销毁队列
void DestroyQueue(Queue *q) {
    Queue_node *front = q->front; //取出队头元素
    Queue_node *t;
    while (front != q->rear) {
        //当队头不等于队尾时，头指针指向下一个结点，回收队头元素
        t = front;
        front = front->next;
        free(t);
    }
    q->front = NULL;
    q->rear = NULL;
}

//解决输入类型问题，为自动生成或文件读取
void input(int **maze, int m, int n) {
START:    int input_type; //选择输入的类型
    cout << "提示:输入一个数，整数0表示电脑自动生成迷宫，整数1为手动输入:";
    cin >> input_type;
    if (input_type == 0) {
        //如果该值为0，表示输入类型为自动输入，由电脑自动生成
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                maze[i][j] = rand() % 2; //随机产生0和1，如果是偶数则为0，否则为1
            }
        }
        //输出迷宫的数据
        for (int x = 0; x < m; x++) {
            for (int y = 0; y < n; y++) {
                cout << maze[x][y] << " "; //输入0，表示为通路；1表示墙
            }
            cout << endl;
        }
        cout << endl;
    } else {
        //否则从文件中读取 //TODO
        ifstream readFile;
        readFile.open("E:\\ClionProjects\\Maze\\file.txt", ios::in);
        if (readFile.is_open()) {
            cout << "文件打开成功！" << endl;
            char buffer;
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    if ((buffer = readFile.get()) != EOF) {
                        maze[i][j] = buffer - '0'; //输入0，表示为通路；1表示墙
                    } else {
                        cout << "文件中迷宫与迷宫设定值不匹配!" << endl;
                        goto START;
                    }
                }
            }
        } else {
            cout << "文件打开失败！" << endl;
        }
        readFile.close();
    }
}

int dfsmin = 9999;
int dfsvisit[100][100] = {0};

void dfs(int bx, int by, int ex, int ey, int **maze, int step, int m, int n) {//TODO bfs搜索迷宫

//如果起点或终点中有一个是墙，则无法走出迷宫
    if (maze[bx][by] == 1 || maze[ex][ey] == 1) {
        dfsmin = -1;   //step为-1时代表“无法走出迷宫”
        return;
    }
    if (bx == ex && by == ey) {
        //p，q为终点坐标
        if (step < dfsmin)
            dfsmin = step;
        return;
    }
    if (by + 1 < n && maze[bx][by + 1] != 1 && dfsvisit[bx][by + 1] == 0) {
        dfsvisit[bx][by + 1] = 1;
        dfs(bx, by + 1, ex, ey, maze, step + 1, m, n);
        dfsvisit[bx][by + 1] = 0;
    }
    if (bx + 1 < m && maze[bx + 1][by] != 1 && dfsvisit[bx + 1][by] == 0) {
        dfsvisit[bx + 1][by] = 1;
        dfs(bx + 1, by, ex, ey, maze, step + 1, m, n);
        dfsvisit[bx + 1][by] = 0;
    }
    if (by - 1 >= 0 && maze[bx][by - 1] != 1 && dfsvisit[bx][by - 1] == 0) {
        dfsvisit[bx][by - 1] = 1;
        dfs(bx, by - 1, ex, ey, maze, step + 1, m, n);
        dfsvisit[bx][by - 1] = 0;
    }
    if (bx - 1 >= 0 && maze[bx - 1][by] != 1 && dfsvisit[bx - 1][by] == 0) {
        dfsvisit[bx - 1][by] = 1;
        dfs(bx - 1, by, ex, ey, maze, step + 1, m, n);
        dfsvisit[bx - 1][by] = 0;
    }
    return;

}

//bfs搜索迷宫
void bfs(int bx, int by, int ex, int ey, int **maze, int &step, Queue *queue, int m, int n) {
    int move_x[] = {-1, 1, 0, 0};   //四个方向的x和y
    int move_y[] = {0, 0, 1, -1};
//如果起点或终点中有一个是墙，则无法走出迷宫
    if (maze[bx][by] == 1 || maze[ex][ey] == 1) {
        step = -1;   //step为-1时代表“无法走出迷宫”
        return;
    }
    step = -1;

    Init(queue);            //初始化队列
    node point, newpoint;    //声明两个结构体

    int visit[m + 1][n + 1];   //数组visit记录某点是否已访问
    memset(visit, 0, sizeof(visit));    //初始化两个数组为0
    memset(pre, 0, sizeof(pre));

    visit[bx][by] = 1;        //访问入口起点，赋值为1
    point.x = bx;            //记录起点信息
    point.y = by;
    point.step = 0;

    EnQueue(point, queue);        //将起点加入队列
    while (!QueueEmpty(queue)) {
        node po = DeQueue(queue);        //取出队头的结构体
        if ((po.x == ex) && (po.y == ey)) {
            //如果队头为终点，退出bfs
            step = po.step;   //step记录从起点到终点的最短路径
            return;
        }
//4个方向遍历
        for (int i = 0; i < 4; i++) {
            newpoint.x = po.x + move_x[i];  //当前遍历点为po，newpoint为po的下一个方向
            newpoint.y = po.y + move_y[i];
            if (newpoint.x >= 0 && newpoint.x < m && newpoint.y >= 0 && newpoint.y < n &&
                !visit[newpoint.x][newpoint.y] &&
                maze[newpoint.x][newpoint.y] == 0) {   //如果newpoint在迷宫中、没有被访问过、且为通路，就加入队列
                visit[newpoint.x][newpoint.y] = 1;  //将该点标记为已访问
                newpoint.step = po.step + 1;      //该点的step为从起点到前一点po的step+1
                pre[newpoint.x][newpoint.y] = po;  //记录该点的前趋点为po
                EnQueue(newpoint, queue);    //将newpoint加入队列
            }
        }
    }

    DestroyQueue(queue); //销毁队列
}

//从终点位置开始递归，xx和yy表示当前访问的横纵坐标
void print(int startx, int starty, int xx, int yy, int &j) {
    if ((xx == startx) && (yy == starty)) //如果当前访问地点为起点位置，返回退出
    {
        maze_path[j].x = xx, maze_path[j].y = yy;
        j++;
        return;
    } else {
        print(startx, starty, pre[xx][yy].x, pre[xx][yy].y, j);
        maze_path[j].x = xx, maze_path[j].y = yy; //记录第j个点的横纵坐标
        j++;
    }

}

//输出最短路径，画图
void image(int &j, int **maze, int m, int n) {

    initgraph(800, 800); //定义窗口大小,显示控制台

    settextcolor(WHITE); //设置画笔的颜色是白色
    settextstyle(20, 0, "KaiTi"); //设置字体的样式
    int ox = 40;
    int oy = 40;
    for (int r = 0; r < m; r++) {
        //在屏幕中输出迷宫数据
        for (int k = 0; k < n; k++) {
            TCHAR tch = maze[r][k] + '0';  //将数组化为字符
            outtextxy(ox, oy, tch); //在屏幕中输出tch
            if (maze[r][k]==0){
                setfillcolor(BLACK);
                solidrectangle(ox,oy,ox+40,oy+40);
            } else{
                setfillcolor(LIGHTGRAY);
                solidrectangle(ox,oy,ox+40,oy+40);
            }
            ox += 50;
        }
        oy += 50;
        ox = 40;
    }
//画表格
/*设置起始点35，终止点m*50+35，表格宽度50*/
    for (int x = 35 ;x<=m*50+35; x = x + 50) {
        for (int y = 35; y <= n*50+35; ++y)
        {
            putpixel(x, y, DARKGRAY);      /*画点*/
            putpixel(y, x, DARKGRAY);
        }
}
int width = textwidth(_T('1')) / 2;  //获取文字的宽度
    int height = textheight(_T('1')) / 2; //获取文字的高度

    for (int e = 0; e < j - 1; e += 1) {
        //画圆圈
        setfillcolor(BLUE);
        circle((maze_path[e].y + 1) * 50 + width, (maze_path[e].x + 1) * 50 + height, 15);
        fillcircle((maze_path[e].y + 1) * 50 + width, (maze_path[e].x + 1) * 50 + height, 15);
        //画直线
        Sleep(800);
        line((maze_path[e].y + 1) * 50 + width, (maze_path[e].x + 1) * 50 + height,
             (maze_path[e + 1].y + 1) * 50 + width, (maze_path[e + 1].x + 1) * 50 + height);
    }
    circle((maze_path[j - 1].y + 1) * 50 + width, (maze_path[j - 1].x + 1) * 50 + height, 15); //给最后一个数字画圆
    fillcircle((maze_path[j - 1].y + 1) * 50 + width, (maze_path[j - 1].x + 1) * 50 + height, 15);
    getchar();
    closegraph(); //关闭绘图窗口
}

int main() {
    ios::sync_with_stdio(false);
    while (1) {
        srand(time(NULL)); //为了防止随机数每次重复，使用系统时间来初始化
        cout << "提示:输入整数m和n，用空格隔开，代表行数和列数，即表示迷宫的大小:";
        int m, n;  //表示迷宫为m*n矩阵
        cin >> m >> n; //输入

        while (m <= 0 || n <= 0) {
            cout << endl;
            cout << "提示:行数m和列数n必须大于0  请您重新输入:";
            cin >> m >> n;
        }
        cout << endl;
        int **maze = new int *[m + 2];
        for (int i = 0; i < (m + 2); i++) {
            maze[i] = new int[n + 2];
        }
        input(maze, m, n); //调用函数input(),选择输入方式，即选择自动生成还是手动输入数据

        int begin_end_type;

        cout << "提示:请输入一个整数，0表示默认入口和出口的横纵坐标，1表示手动输入:";

        cin >> begin_end_type;

        cout << endl;

        int begin_x, begin_y, end_x, end_y; //入口横纵坐标，出口横纵坐标
//如果该值为0，表示默认入口为左上角和出口为右下角
        if (begin_end_type == 0) {
            begin_x = 0;
            begin_y = 0;
            end_x = m - 1;
            end_y = n - 1;
        } else {
            //如果等于1，表示手动定义入口和出口坐标
            cout << "提示:请输入两个数，两个数用空格隔开，表示入口坐标(迷宫从第0行第0列开始):";
            cin >> begin_x >> begin_y;
            cout << endl;
            cout << "提示:请输入两个数，两个数用空格隔开，表示出口坐标(迷宫从第0行第0列开始):";
            cin >> end_x >> end_y;
            cout << endl;
        }
        int step = 0; //记录从入口到出口的最短路径
        Queue queue;
        cout << "提示:请选择使用DFS(1)或BFS(2)遍历迷宫:";
        int choice;
        cin >> choice;
        if (choice == 1) {
            dfsmin=999;
            dfsvisit[begin_x][begin_y] = 1;
            dfs(begin_x, begin_y, end_x, end_y, maze, 0, m, n);
            bfs(begin_x, begin_y, end_x, end_y, maze, step, &queue, m, n); //;bfs搜索寻找最短路径
            int j = 0;
            if (dfsmin == -1) {
                cout << "提示:无法走出迷宫" << endl;
            } else {
                print(begin_x, begin_y, end_x, end_y, j); //递归记录最短路径各点的坐标
                dfsmin++;
                image(dfsmin, maze, m, n); //画最短路径图
                cout << "提示:能走出迷宫出口，最短路径为:" << --dfsmin << endl;
                cout << endl;
            }
        } else {
            bfs(begin_x, begin_y, end_x, end_y, maze, step, &queue, m, n); //;bfs搜索寻找最短路径
            int j = 0;
            if (step == -1) {
                cout << "提示:无法走出迷宫" << endl;
            } else {
                print(begin_x, begin_y, end_x, end_y, j); //递归记录最短路径各点的坐标
                step++;
                image(step, maze, m, n); //画最短路径图
                cout << "提示:能走出迷宫出口，最短路径为:" << --step << endl;
                cout << endl;
            }
        }
        fflush(stdin); //清空输入缓冲区
        char exit;
        cout << "提示:是否再次运行程序? 输入Y为继续 输入N为退出程序 Y/N:";
        cin >> exit;
        if (exit == 'Y' || exit == 'y') {
            system("cls");
            continue;
        } else {
            break;
        }
    }
    return 0;
}