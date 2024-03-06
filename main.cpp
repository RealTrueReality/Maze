#include <iostream>//�ļ���д��
#include<stdlib.h>
#include<time.h>
#include <easyx.h> //Easyxͼ�ο�
#include <tchar.h>   //easyx����ת�ַ�
#include <string.h> //memset��ͷ�ļ�
#include <windows.h>
#include<fstream>
#include<string>
/**
 * 8���Թ��������
������������
��дһ����������Թ����⡣�Թ���m��n�еĶ�ά�������ã�0��ʾ���ϰ���1��ʾ���ϰ��������Ϊ��1��0��������Ϊ��m-2��n-1��
 ÿ��ֻ�ܴ�һ�����ϰ���Ԫ�Ƶ���Χ�ĸ���������һ���ϰ���Ԫ����дʵ�ֶ������趨���Թ����������ڵ����ڵ�ͨ·������ж���ͨ·���Ƚ�·�����̣����û��ͨ·����û��ͨ·�Ľ��ۡ�
��Ҫ�漰����֪ʶ��:������������㷨��������������㷨���ļ�
������Ҫ��
��1�������Թ����Թ���������ɣ�Ҳ�����ļ���ȡ��//TODO �ļ���ȡ
��2��������·��ʱ��������������ʽ��������������͹������������//TODO  DFS
��3���㷨���룬�Թ���ģm����������n����������
��4���㷨��������������Թ�ͨ·ͼ��
��5��С�ᣬ�Ƚ������㷨����ȱ�㡣����ʵ������Щ���ܣ�������Щ���⣬��ν���ģ��Ժ���Щ������Ҫ��ǿ����
 */

using namespace std;

typedef struct node {
    int x, y;  //��¼ĳ�������������
    int step;  //��¼����㵽�õ�Ĳ���
} node;

node pre[100][100], maze_path[100];  //pre[][]��¼ĳ���ǰ�����,maze_path[]��¼�Թ����·���ĵĸ���
//���нṹ��
typedef struct Queue_node {
    node node;   //����Ϊnode�ṹ��
    struct Queue_node *next;
} Queue_node;

typedef struct Queue {
    Queue_node *front;
    Queue_node *rear;
} Queue;

//��ʼ�����У�ͷָ���βָ��Ϊ��
void Init(Queue *q) {
    q->front = q->rear = 0;
}

//�ж϶����Ƿ�Ϊ��
bool QueueEmpty(Queue *q) {
    return (q->front == NULL); //�����еĶ�ͷԪ��Ϊ��ʱ������Ϊ��
}

//���
void EnQueue(node &no, Queue *q) {
    Queue_node *temp = new Queue_node();   //����һ������ΪQueue_node���½��
    temp->node = no;
    temp->next = NULL;
//�������Ϊ��,����е�ͷָ���βָ�붼ָ�������ɵĽ��temp
    if (q->front == NULL) {
        q->front = temp;
        q->rear = temp;
    } else {
        //�����в��գ������βָ��
        q->rear->next = temp;
        q->rear = temp;
    }
}

//�����У�ȡ��ͷԪ��
node DeQueue(Queue *q) {
    node n = q->front->node;  //ȡ����ͷ��node����ȡ���Թ��б���ĳ��ĸ���Ϣ����x,y��step
    Queue_node *que = q->front; //ȡ����ͷԪ��
    if (q->front == q->rear) {
        //�����ͷԪ�ص��ڶ�β�����ö���ֻ��һ��Ԫ�أ������ʱ��ͷָ���βָ�붼��ֵΪ0
        q->rear = 0;
        q->front = 0;
    } else      //�������ͷָ�룬ָ����һ�����
        q->front = q->front->next;
    free(que); //���ն�ͷԪ��
    return n; //�����Թ����
}

//���ٶ���
void DestroyQueue(Queue *q) {
    Queue_node *front = q->front; //ȡ����ͷԪ��
    Queue_node *t;
    while (front != q->rear) {
        //����ͷ�����ڶ�βʱ��ͷָ��ָ����һ����㣬���ն�ͷԪ��
        t = front;
        front = front->next;
        free(t);
    }
    q->front = NULL;
    q->rear = NULL;
}

//��������������⣬Ϊ�Զ����ɻ��ļ���ȡ
void input(int **maze, int m, int n) {
START:    int input_type; //ѡ�����������
    cout << "��ʾ:����һ����������0��ʾ�����Զ������Թ�������1Ϊ�ֶ�����:";
    cin >> input_type;
    if (input_type == 0) {
        //�����ֵΪ0����ʾ��������Ϊ�Զ����룬�ɵ����Զ�����
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                maze[i][j] = rand() % 2; //�������0��1�������ż����Ϊ0������Ϊ1
            }
        }
        //����Թ�������
        for (int x = 0; x < m; x++) {
            for (int y = 0; y < n; y++) {
                cout << maze[x][y] << " "; //����0����ʾΪͨ·��1��ʾǽ
            }
            cout << endl;
        }
        cout << endl;
    } else {
        //������ļ��ж�ȡ //TODO
        ifstream readFile;
        readFile.open("E:\\ClionProjects\\Maze\\file.txt", ios::in);
        if (readFile.is_open()) {
            cout << "�ļ��򿪳ɹ���" << endl;
            char buffer;
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    if ((buffer = readFile.get()) != EOF) {
                        maze[i][j] = buffer - '0'; //����0����ʾΪͨ·��1��ʾǽ
                    } else {
                        cout << "�ļ����Թ����Թ��趨ֵ��ƥ��!" << endl;
                        goto START;
                    }
                }
            }
        } else {
            cout << "�ļ���ʧ�ܣ�" << endl;
        }
        readFile.close();
    }
}

int dfsmin = 9999;
int dfsvisit[100][100] = {0};

void dfs(int bx, int by, int ex, int ey, int **maze, int step, int m, int n) {//TODO bfs�����Թ�

//��������յ�����һ����ǽ�����޷��߳��Թ�
    if (maze[bx][by] == 1 || maze[ex][ey] == 1) {
        dfsmin = -1;   //stepΪ-1ʱ�����޷��߳��Թ���
        return;
    }
    if (bx == ex && by == ey) {
        //p��qΪ�յ�����
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

//bfs�����Թ�
void bfs(int bx, int by, int ex, int ey, int **maze, int &step, Queue *queue, int m, int n) {
    int move_x[] = {-1, 1, 0, 0};   //�ĸ������x��y
    int move_y[] = {0, 0, 1, -1};
//��������յ�����һ����ǽ�����޷��߳��Թ�
    if (maze[bx][by] == 1 || maze[ex][ey] == 1) {
        step = -1;   //stepΪ-1ʱ�����޷��߳��Թ���
        return;
    }
    step = -1;

    Init(queue);            //��ʼ������
    node point, newpoint;    //���������ṹ��

    int visit[m + 1][n + 1];   //����visit��¼ĳ���Ƿ��ѷ���
    memset(visit, 0, sizeof(visit));    //��ʼ����������Ϊ0
    memset(pre, 0, sizeof(pre));

    visit[bx][by] = 1;        //���������㣬��ֵΪ1
    point.x = bx;            //��¼�����Ϣ
    point.y = by;
    point.step = 0;

    EnQueue(point, queue);        //�����������
    while (!QueueEmpty(queue)) {
        node po = DeQueue(queue);        //ȡ����ͷ�Ľṹ��
        if ((po.x == ex) && (po.y == ey)) {
            //�����ͷΪ�յ㣬�˳�bfs
            step = po.step;   //step��¼����㵽�յ�����·��
            return;
        }
//4���������
        for (int i = 0; i < 4; i++) {
            newpoint.x = po.x + move_x[i];  //��ǰ������Ϊpo��newpointΪpo����һ������
            newpoint.y = po.y + move_y[i];
            if (newpoint.x >= 0 && newpoint.x < m && newpoint.y >= 0 && newpoint.y < n &&
                !visit[newpoint.x][newpoint.y] &&
                maze[newpoint.x][newpoint.y] == 0) {   //���newpoint���Թ��С�û�б����ʹ�����Ϊͨ·���ͼ������
                visit[newpoint.x][newpoint.y] = 1;  //���õ���Ϊ�ѷ���
                newpoint.step = po.step + 1;      //�õ��stepΪ����㵽ǰһ��po��step+1
                pre[newpoint.x][newpoint.y] = po;  //��¼�õ��ǰ����Ϊpo
                EnQueue(newpoint, queue);    //��newpoint�������
            }
        }
    }

    DestroyQueue(queue); //���ٶ���
}

//���յ�λ�ÿ�ʼ�ݹ飬xx��yy��ʾ��ǰ���ʵĺ�������
void print(int startx, int starty, int xx, int yy, int &j) {
    if ((xx == startx) && (yy == starty)) //�����ǰ���ʵص�Ϊ���λ�ã������˳�
    {
        maze_path[j].x = xx, maze_path[j].y = yy;
        j++;
        return;
    } else {
        print(startx, starty, pre[xx][yy].x, pre[xx][yy].y, j);
        maze_path[j].x = xx, maze_path[j].y = yy; //��¼��j����ĺ�������
        j++;
    }

}

//������·������ͼ
void image(int &j, int **maze, int m, int n) {

    initgraph(800, 800); //���崰�ڴ�С,��ʾ����̨

    settextcolor(WHITE); //���û��ʵ���ɫ�ǰ�ɫ
    settextstyle(20, 0, "KaiTi"); //�����������ʽ
    int ox = 40;
    int oy = 40;
    for (int r = 0; r < m; r++) {
        //����Ļ������Թ�����
        for (int k = 0; k < n; k++) {
            TCHAR tch = maze[r][k] + '0';  //�����黯Ϊ�ַ�
            outtextxy(ox, oy, tch); //����Ļ�����tch
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
//�����
/*������ʼ��35����ֹ��m*50+35�������50*/
    for (int x = 35 ;x<=m*50+35; x = x + 50) {
        for (int y = 35; y <= n*50+35; ++y)
        {
            putpixel(x, y, DARKGRAY);      /*����*/
            putpixel(y, x, DARKGRAY);
        }
}
int width = textwidth(_T('1')) / 2;  //��ȡ���ֵĿ��
    int height = textheight(_T('1')) / 2; //��ȡ���ֵĸ߶�

    for (int e = 0; e < j - 1; e += 1) {
        //��ԲȦ
        setfillcolor(BLUE);
        circle((maze_path[e].y + 1) * 50 + width, (maze_path[e].x + 1) * 50 + height, 15);
        fillcircle((maze_path[e].y + 1) * 50 + width, (maze_path[e].x + 1) * 50 + height, 15);
        //��ֱ��
        Sleep(800);
        line((maze_path[e].y + 1) * 50 + width, (maze_path[e].x + 1) * 50 + height,
             (maze_path[e + 1].y + 1) * 50 + width, (maze_path[e + 1].x + 1) * 50 + height);
    }
    circle((maze_path[j - 1].y + 1) * 50 + width, (maze_path[j - 1].x + 1) * 50 + height, 15); //�����һ�����ֻ�Բ
    fillcircle((maze_path[j - 1].y + 1) * 50 + width, (maze_path[j - 1].x + 1) * 50 + height, 15);
    getchar();
    closegraph(); //�رջ�ͼ����
}

int main() {
    ios::sync_with_stdio(false);
    while (1) {
        srand(time(NULL)); //Ϊ�˷�ֹ�����ÿ���ظ���ʹ��ϵͳʱ������ʼ��
        cout << "��ʾ:��������m��n���ÿո��������������������������ʾ�Թ��Ĵ�С:";
        int m, n;  //��ʾ�Թ�Ϊm*n����
        cin >> m >> n; //����

        while (m <= 0 || n <= 0) {
            cout << endl;
            cout << "��ʾ:����m������n�������0  ������������:";
            cin >> m >> n;
        }
        cout << endl;
        int **maze = new int *[m + 2];
        for (int i = 0; i < (m + 2); i++) {
            maze[i] = new int[n + 2];
        }
        input(maze, m, n); //���ú���input(),ѡ�����뷽ʽ����ѡ���Զ����ɻ����ֶ���������

        int begin_end_type;

        cout << "��ʾ:������һ��������0��ʾĬ����ںͳ��ڵĺ������꣬1��ʾ�ֶ�����:";

        cin >> begin_end_type;

        cout << endl;

        int begin_x, begin_y, end_x, end_y; //��ں������꣬���ں�������
//�����ֵΪ0����ʾĬ�����Ϊ���ϽǺͳ���Ϊ���½�
        if (begin_end_type == 0) {
            begin_x = 0;
            begin_y = 0;
            end_x = m - 1;
            end_y = n - 1;
        } else {
            //�������1����ʾ�ֶ�������ںͳ�������
            cout << "��ʾ:���������������������ÿո��������ʾ�������(�Թ��ӵ�0�е�0�п�ʼ):";
            cin >> begin_x >> begin_y;
            cout << endl;
            cout << "��ʾ:���������������������ÿո��������ʾ��������(�Թ��ӵ�0�е�0�п�ʼ):";
            cin >> end_x >> end_y;
            cout << endl;
        }
        int step = 0; //��¼����ڵ����ڵ����·��
        Queue queue;
        cout << "��ʾ:��ѡ��ʹ��DFS(1)��BFS(2)�����Թ�:";
        int choice;
        cin >> choice;
        if (choice == 1) {
            dfsmin=999;
            dfsvisit[begin_x][begin_y] = 1;
            dfs(begin_x, begin_y, end_x, end_y, maze, 0, m, n);
            bfs(begin_x, begin_y, end_x, end_y, maze, step, &queue, m, n); //;bfs����Ѱ�����·��
            int j = 0;
            if (dfsmin == -1) {
                cout << "��ʾ:�޷��߳��Թ�" << endl;
            } else {
                print(begin_x, begin_y, end_x, end_y, j); //�ݹ��¼���·�����������
                dfsmin++;
                image(dfsmin, maze, m, n); //�����·��ͼ
                cout << "��ʾ:���߳��Թ����ڣ����·��Ϊ:" << --dfsmin << endl;
                cout << endl;
            }
        } else {
            bfs(begin_x, begin_y, end_x, end_y, maze, step, &queue, m, n); //;bfs����Ѱ�����·��
            int j = 0;
            if (step == -1) {
                cout << "��ʾ:�޷��߳��Թ�" << endl;
            } else {
                print(begin_x, begin_y, end_x, end_y, j); //�ݹ��¼���·�����������
                step++;
                image(step, maze, m, n); //�����·��ͼ
                cout << "��ʾ:���߳��Թ����ڣ����·��Ϊ:" << --step << endl;
                cout << endl;
            }
        }
        fflush(stdin); //������뻺����
        char exit;
        cout << "��ʾ:�Ƿ��ٴ����г���? ����YΪ���� ����NΪ�˳����� Y/N:";
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