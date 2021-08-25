#include <stdio.h>
#include <string.h>     //strcmpを使うために必要
#include <unistd.h>     //sleepを使うために必要
#include <termios.h>    //エコー処理などで必要

int Cjudge(char cc[]);
int Cchange(void);
int Auto(void);
int Manual(void);
//ターミナルで作業してる風に見えるプログラム

int main(void){
    char d[100];

    while(1){   //exitで終了するので無限ループ

        printf("What do u wanna do?（color,code）");  //何をするかの質問
        scanf("%s",d);

        if( strcmp(d,"color")==0){    //color→色を変えるCchangeへ
            Cchange();
        }
        else if( strcmp(d,"code")==0){  //code→コード表示する処理へ
            printf("auto or manual?");  
            scanf("%s",d);
            if( strcmp(d,"auto")==0){      //オートとマニュアルで関数を変える
                Auto();
            }
            else if( strcmp(d,"manual")==0){
                Manual();
            }
            else{
                printf("Please auto or manual\n");
            }
        }
        else if(strcmp(d,"exit")==0){       //exitでプログラムの終了
            printf("End\n");
            break;
        }
        else{
            printf("Please color,code,exit\n"); //例外入力の対応
        }
    }
    return 0;
}

int Cjudge(char cc[]){  //入力された色を数値に変換する関数
    int i;
    char cl[10][10]={{"black"},{"red"},{"green"},{"yellow"},{"blue"},{"magenta"},{"cyan"}};
    for(i=0;i<10;i++){
        if(strcmp(cc,cl[i])==0){    //strcmpとforで文字列を判定
            return i;       //一致したらiが対応した数値となるのでiを返す
            break;
        }
    }
    return -1;                 //対応しない場合負の数を返す
}

int Cchange(void){      //色を変える関数

    char yn[]={},cc[]={};
    int c;
    printf("Background?(yes,no)");    //背景色を変えるかの質問
    scanf("%s",yn);
    if( strcmp(yn,"yes")==0){
        printf("Choice color(black,red,green,yellow,blue,magenta,cyan)");
        scanf("%s",cc);
        c=Cjudge(cc);       //色を判定する関数へ
        if(c>0){
            printf("\x1b[4%dm",c);  //cの値によって対応した色になる
            printf("Complete!\n");
        }
        else{
            printf("\x1b[49m");      // 例外入力の場合はデフォルト色に変更
            printf("An invalid color was selected. Reset to default.\n");
        }
    }
    else if(strcmp(yn,"no")==0){  //背景色でない場合は文字の色を変える
        printf("Change text-color\n");
        printf("Choice color(black,red,green,yellow,blue,magenta,cyan)");
        scanf("%s",cc);
        c=Cjudge(cc);   //色の判定は背景色と同じ
        if(c>0){
            printf("\x1b[3%dm",c);  //文字色の場合は[3<-ここが3になる
            printf("Complete!\n");
        }
        else{
            printf("\x1b[39m");      // 例外入力の場合はデフォルト色に変更
            printf("An invalid color was selected. Reset to default.\n");
        }
    }
    else{
        printf("Please yes or no\n");
    }
    
    return 0;
}
//自動で表示されるコードをグローバル変数で宣言
char sp[]={"\
    /Maximum and minimum taxi prices/\n\
    import math\n\
    n = list(map(int, input().split()))\n\
    taxi = [list(map(int, input().split())) for i in range (n[0])]\n\
    money_all = []\n\
    for i in range(n[0]):\n\
        if(n[1] > taxi[i][0]):\n\
            d = (n[1] - taxi[i][0])/taxi[i][2]\n\
            if(d.is_integer() == True):\n\
                d = d+1\n\
            money =taxi[i][1]+math.ceil(d)*taxi[i][3]\n\
            money_all.append(money)\n\
        elif(n[1] < taxi[i][0]):\n\
            money = taxi[i][1]\n\
            money_all.append(money)\n\
        else:\n\
            money = taxi[i][1]+taxi[i][3]\n\
            money_all.append(money)\n\
    print(min(money_all),max(money_all))\n\
    \n\
    /The missing magic square code/\n\
    n = int(input())\n\
    t = [list(map(int, input().strip().split())) for x in range(n)]\n\
    s = n * (n**2+1)//2\n\
    a = []\n\
    for i,i_v in enumerate(t):\n\
        for j,j_v in enumerate(i_v):\n\
            if j_v == 0:\n\
                a.append((i,j))\n\
    ai1,aj1 = a[0]\n\
    ai2,aj2 = a[1]\n\
    if ai1 != ai2:\n\
        t[ai1][aj1] = s - sum([t[ai1][j] for j in range(n)])\n\
        t[ai2][aj2] = s - sum([t[ai2][j] for j in range(n)])\n\
    elif aj1 != aj2:\n\
        t[ai1][aj1] = s - sum([t[i][aj1] for i in range(n)])\n\
        t[ai2][aj2] = s - sum([t[i][aj2] for i in range(n)])\n\
    for k in t:\n\
        print(' '.join(map(str, k)))\n"};   //ここでは過去に自分が作ったpythonのコードを使用

int Auto(void){     //自動でコードを一文字ずつ出力する関数
    int i=0,ms=0.03*1000000; //msはミリセカンド、掛けられる数が秒数になっている
    printf("Auto mode starting!\n");
    while(sp[i] != '\0'){
        printf("%c",sp[i]); //1文字出力
        fflush(stdout);     //バッファリングを防いで処理を行う
        usleep(ms);         //ms秒間待つ
        i++;
    }
    printf("\n");
    return 0;
}

int Manual(void){   //適当なキー入力に合わせてコードを出力する関数
    printf("Manual mode started!\nPlease press any key!\n");
    int i=0; char ch;
    struct termios st;  //構造体の宣言
    ch=tcgetattr(0,&st);
    st.c_lflag &= ~(ICANON|ECHO);    //非カノニカルモード（即実行）とエコー無くす（入力が見えなくなる）
    ch=tcsetattr(0,TCSANOW,&st);    //設定の変更を保存
    while(sp[i] != '\0'){
        if(getchar() != '~'){   //'~'を打った時以外出力
            printf("%c",sp[i]); //1文字出力
            i++;
        }
    }
    st.c_lflag |= (ICANON|ECHO);    //カノニカルモードとエコー戻す
    ch=tcsetattr(0,TCSANOW,&st);    //設定の変更を保存
    return 0;
}