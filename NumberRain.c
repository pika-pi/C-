#include<stdio.h>
#include<graphics.h>
#define WIDTH 960
#define HEIGTH 640
#define STR_SIZE 20
#define STR_NUM 128
#define STR_WIDTH 15

struct Rain {
    int x;//数字雨横向位置
    int y;// Y坐标
    int speed;//速度
    char str[STR_SIZE];//数字雨数组
}rain[STR_NUM];

char CreateChar(){
    char temp=0;
    int flag = rand() % 3; //产生0,1,2
    if (flag == 0){
        temp = rand() % 26 + 'a';
    }else if (flag == 1){
        temp = rand() % 26 + 'A';
    }else {
        temp = rand() % 10 + '0';
    }
    return temp;
}
void GameInit(){
    for (int i=0 ; i < STR_NUM ; i++) {
        rain[i].x = i*STR_WIDTH;
        rain[i].y = rand() % HEIGTH;
        rain[i].speed = rand() % 5 + 5; //最低速度5,最高10
    }
    //这里应该可以优化，赋值字符串的二次循环可以放在上面一起赋值，节省一次循环
    for (int i=0 ; i<STR_NUM ; i++){
        for (int j=0 ; j<STR_SIZE;j++){
            rain[i].str[j] = CreateChar();
        }
    }
}
void GameDraw(){
    cleardevice();//清屏
    for (int i=0 ; i<STR_NUM ; i++){
        for (int j=0 ; j<STR_SIZE ; j++){
            outtextxy(rain[i].x , rain[i].y-STR_WIDTH*j , rain[i].str[j]);
            settextcolor(RGB(0,255-13*j,0));//设置字体颜色，并逐渐变淡
        }
    }
}
void GamePlay(){
    for (int i=0 ; i<STR_NUM ; i++){
        rain[i].y += rain[i].speed;//移动字符下落
        //使用字符尾巴判断，尾巴超过之后再初始化坐标
        if(rain[i].y-STR_SIZE*STR_WIDTH > HEIGTH){ //如果字符坐标超过屏幕高度，初始化坐标
            rain[i].y = 0;
        }
    }
}
void ChangeChar(){
    for (int i=0 ; i<STR_NUM ; i++){
        //随机找到一个结构体，再随机改变一个字符。
        rain[rand()%STR_NUM].str[rand()%STR_SIZE] = CreateChar();
    }
}

int main(){
    initgraph(WIDTH,HEIGTH);
    srand(GetTickCount());//埋下一个随机数种子
     
    GameInit();

    DWORD t1,t2;
    t1=t2=GetTickCount();

    while (1)
    {
       BeginBatchDraw();//缓冲函数，需要了解是什么原理
       GameDraw();
       if(t2-t1 > 20){//判断延时是不是过了20毫秒
           GamePlay();
           t1 = t2; //如果超过20毫秒，初始化开始计算延时，校准t1=t2
       }
       t2 = GetTickCount();//获取一次t2,获取延时
       
       EndBatchDraw();//缓冲结束
       //sleep(20)延时，速度过快的解决办法
    }
    getchar();
    closegraph();
    return 0;
}