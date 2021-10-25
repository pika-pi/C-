#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <graphics.h>
//数据设计
const int WIDTH = 640;//窗口属性
const int HEIGHT = 500;
//游戏正确数和错误数
int right=0,error=0;
//文字封装为一个结构体
struct Target{
    //每个字符串的坐标
    int x;
    int y;
    char *str;
};
//用户输入的值
struct UserKey{
    int x;
    int y;
    char str[20];
}userKey = {320,500-30,""};

//在指定位置输出整数
void outtextxy_int(int x,int y,char *format,int num){
    char str[20] = "";
    sprintf(str,format,num);//字符串转换函数,与print类似
    // outtextxy_int(200,200,"正确数:%d",right);
    outtextxy(x,y,str);
}
//在c语言中函数虽然一样，但还是要写两次，java中可以重载，c++中有模板可以精简
void outtextxy_double(int x,int y,char *format,double num){
    char str[20] = "";
    sprintf(str,format,num);
    outtextxy(x,y,str);
}
void initTarget(struct Target words[],int n){
    static char str[6][10]={"main","include","void","while","true","false"};
    words[n].str = str[rand()%6];
    //判断重复，如果重复，就重新生成
    while (words[n].str == words[(n+1)%3].str || words[n].str == words[(n+2)%3].str){
        words[n].str = str[rand()%6];//使用取余方式，避免下标越界，循环队列取数据对比。
    }
    words[n].x = rand()%(WIDTH-200);
    words[n].y = -20;]
}
void divWindow(){
    line(WIDTH-100,0,WIDTH-100,HEIGHT-40);
    line(0,HEIGHT-40,WIDTH+50,HEIGHT-40);
    line(WIDTH-100,130,WIDTH+50,130);
}
void showScore(){
    settextcolor(LIGHTBLUE);
    settextstyle(25,0,"黑体");
    //软件信息输出
    outtextxy(WIDTH-90,25,"标题显示");

    outtextxy(WIDTH-90,225,"正确数");
    outtextxy_int(WIDTH-90,225+25,"%d",right);
    outtextxy(WIDTH-90,285,"错误数");
    outtextxy_int(WIDTH-90,285+25,"%d",error);
    outtextxy(WIDTH-90,225+285-225,"正确率");
    //分类讨论
    if (right+right ==0){
        outtextxy_double(WIDTH-90,225+285-225+25,"%.2f%%",0.00);
    }else {
        double sum = right + error;
        outtextxy_double(WIDTH-90,225+285-225+25,"%.2f%%",right/sum *100);
    }
    
}
int main (){
    srand((unsigned int )time(NULL));
    initgraph(WIDTH,HEIGHT);//创建窗口
    struct Target words[3];
    //随机产生掉落的字符串
    for (int i=0;i<3;i++){
        initTarget(words,i);
        words[i].y = -15 -i*30;//形成不等高，单词上下分开
    }
    int i=0;
    while (1){
        cleardevice();
        divWindow();
        for (int n=0;n<3;n++){
            words[n].y +=2;
            //如果过线就初始化字符串
            if (words[n].y>(HEIGHT-40-textheight(words[n].str))){
                initTarget(words,n);
            }
        }
        //打印文字
        for (int n=0;n<3;n++){
            settextcolor(RED);
            outtextxy(words[n].x,words[n].y,words[n].str);
        }
        if (_kbhit()){ //kbhit 检测键盘，有按键返回非0
            //字符串变为字符处理
            char target;//接受用户的值
            if ((target=_getch()) != '\r'){
                userKey.str[i++]= target;
            }else {//输出回车后
                int flagError = 0;
                for (i=0;i<3;i++){
                    if (strcmp(userKey.str,words[i].str) == 0){
                        initTarget(words,i);
                        right++;
                        flagError=1;
                    }
                }
                if (flagError == 0 ) error++;
                i = 0;
                userKey.x = 320;
                memset(userKey.str,0,20);
            }

        }
        outtextxy(userKey.x,userKey.y,userKey.str);
        showScore();
        sleep(100);
    }

    closegraph();//关闭窗口
    return 0;
}