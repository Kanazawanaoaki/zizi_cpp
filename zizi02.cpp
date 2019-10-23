#include <iostream>
#include <cstdio>
#include <cstdlib> //rand用

//zizi01からの改良：それぞれの手札用の数列を作る　jokerはなくした オープンソースにした
//あとは画面表示の仕方とか工夫したいのと、関数にしてもう少し見やすいプログラミングにしたい

using namespace std;

class card{
public:
    int mark; //カードのマーク 1:スペード　2:クラブ 3:ハート 4:ダイヤ 
    int number; //カードの番号 1~13 11:J 12:Q 13:K  
    int own; //カードの持ち主  0~3:player1~4 と 5:墓地 6:じじ
    int ori; //カードが誰オリジナルか　0~3
    int pos; //カードの位置 シャッフルの時(通し番号) それ以外手札での位置番号として使う
};
class card card[52];

int x[4]; //手札数
int agariban; //だれかがあがったときの手番
int wins=0; //上ったplayer数
int win[3]; //上ったplayerを入れる 0~3
int lose=0; //負けたplayerを入れる 0~3
int ptehuda[4][13]; //各playerの手札配列　数の通し番号を入れる
int opens[13]; //オープンソース用の配列　i+1のカードが何枚あるか
int a; //画面切り替えの時に入力受ける用の整数a
int zizim,zizin; //じじのマークと数字を入れる

void hantei(void);  //カードの判定をする、全員の手札を探索してペアがあったら墓地に送る
void shuffle1(void); //はじめにカードをシャッフルして配る、じじも作る
void hyouji1(int num); //player num+1 のカードを表示する、手札での位置番号posと手札の枚数x[num]を決める
void hyouji2(int i); //player i+1 の一連の表示動作を行う
void kirikae(void); //30回改行して画面を切り替える
void hiku(int a,int aite,int i); //player i が player aite の位置番号 a のカードを引く
void agarihantei(int num,int k); //player num のあがり判定をする winsも変更する kはその時の番
void owari(void); //playerの番がおわった時に画面切り替え用の一連のどうさを行う
void tehudaseiton(int num,int i,int j); //player num  のiとjを無くして整頓


int main(){
    //カード作成
    for(int i=0;i<52;i++){
        if(i<13){card[i].mark=1;card[i].number=i+1;card[i].pos=i;}  //スペード
        else if(i<26){card[i].mark=2;card[i].number=i-12;card[i].pos=i;} //クラブ
        else if(i<39){card[i].mark=3;card[i].number=i-25;card[i].pos=i;} //ハート
        else {card[i].mark=4;card[i].number=i-38;card[i].pos=i;} //ダイヤ
    }

    shuffle1();
    hantei();
    
    //はじめの手札確認
    for(int i=0;i<4;i++){
        hyouji2(i); //ここでposの意味が変わるs
        agarihantei(i,-1);
    }
    //誰も上っていない間
    while(wins == 0){
        for(int i=0;i<4 && wins==0;i++){
            hyouji1(i);  
            int aite;
            if(i==0)aite=3;
            else aite = i-1;
            cout << " player " << aite+1 << "の手札 1 ~ " << x[aite] << " のどれをとりますか: ";
            cin >> a;
            if(a>x[aite] || a<1){cout << "もう一度入力してください" << endl;i--;continue;}
            hiku(a,aite,i); //カード引く
            hantei();
            hyouji2(i);
            if(x[i]==0){
                wins=1;win[0]=i;cout << "player" << i+1 << " おめでとうございます！ あがりました" << endl;agariban=i;
                owari();
            }
            agarihantei(aite,i);
        }
    }
    //一人あがっている状態
    while(wins == 1){
        int i=agariban+1;
        agariban=-1; //agaribanの初期化
        for(i;i<4 && wins==1;i++){
            if(i==win[0]){
                cout << "player " << i+1 <<" あなたは上っています！ ";
                owari();
                continue;
            }
            hyouji1(i);  
            int aite;
            if(i==0){aite=3;if(aite==win[0])aite -=1;}
            else {aite = i-1;if(aite==win[0])aite -=1;if(aite<0)aite=3;}
            cout << " player " << aite+1 << "の手札 1 ~ " << x[aite] << " のどれをとりますか: ";
            cin >> a;
            if(a>x[aite] || a<1){cout << "もう一度入力してください" << endl;i--;continue;}
            hiku(a,aite,i); //カード引く
            hantei();
            hyouji2(i);
            if(x[i]==0){
                wins=2;win[1]=i;cout << "player" << i+1 << " おめでとうございます！ あがりました" << endl;agariban=i;
                owari();
            }
            agarihantei(aite,i);
        }
    }
    //二人あがっているじょうたい
    while(wins == 2){
        int i=agariban+1;
        agariban=-1; //agaribanの初期化
        for(i;i<4 && wins==2;i++){
            if(i==win[0] || i==win[1]){
                cout << "player " << i+1 <<" あなたはあがっています ";
                owari();
                continue;
            }
            hyouji1(i);  
            int aite;
            for(int j=0;j<4;j++){
                if(j!=i && j!=win[0] && j!=win[1])aite=j;
            }
            cout << " player " << aite+1 << "の手札 1 ~ " << x[aite] << " のどれをとりますか: ";
            cin >> a;
            if(a>x[aite] || a<1){cout << "もう一度入力してください" << endl;i--;continue;}
            hiku(a,aite,i); //カード引く
            hantei();
            hyouji2(i);
            if(x[i]==0){
                wins=3;win[2]=i;lose=aite;cout << "player" << i+1 << " おめでとうございます！ あがりました" << endl;agariban=i;
                owari();
            }
            agarihantei(aite,i);
        }
    }
    cout << "ゲームが終了しました。 結果は"<< endl;
    for(int i=0;i<3;i++){
        cout <<  i+1 <<"位: player " << win[i]+1 << endl;
    }
    cout <<  "負けたのは: player " << lose+1<< endl;
    cout << "じじは　" << zizim << " " << zizin << "  でした！" << endl;
}

void hantei(void){
    for(int i=0;i<52;i++){
        for(int j=i+1;j<52;j++){
            if(card[i].number==card[j].number && card[i].own ==card[j].own && card[i].own != 5){
                int temp=card[i].own;
                card[i].own = card[j].own=5;  //カードを墓地に送る
                tehudaseiton(temp,i,j);
                opens[card[i].number-1]+=2;
            }
        }
    }
}

void shuffle1(void){
    srand((unsigned) time(NULL));
    for(int i=0;i<52;i++){
        int j = rand()%52; 
        int temp = card[i].pos;
        card[i].pos = card[j].pos;
        card[j].pos = temp;
    }

    for(int i=0;i<52;i++){
        int j = card[i].pos; 
        if(j<13){card[i].own=0;card[i].ori=0;ptehuda[0][j]=i;}
        else if(j<26){card[i].own=1;card[i].ori=1;ptehuda[1][j-13]=i;}
        else if(j<39){card[i].own=2;card[i].ori=2;ptehuda[2][j-26]=i;}
        else if(j<51){card[i].own=3;card[i].ori=3;ptehuda[3][j-39]=i;}
        else {card[i].own=6;zizim=card[i].mark;zizin=card[i].number;}//じじにする
    }
    ptehuda[3][12]=-1;
}

void hyouji1(int num){
    for(int i=0;i<13;i++)cout << i+1 << "は " << opens[i] << "枚,";
    cout << "捨てられています" << endl << endl;
    for(int i=0;i<13;i++){if(ptehuda[num][i]!=-1)cout << ptehuda[num][i]/13+1 << " " << ptehuda[num][i]%13+1 << endl;}
    cout << endl;
    x[num]=0;
    cout << "player " << num+1 << "の手札" << endl;
    for(int i=0;i<52;i++){
        if(card[i].own==num){x[num]++;card[i].pos=x[num];cout << card[i].mark << " " << card[i].number << endl;}
    }
    cout << "手札数:" << x[num] << endl;
}

void hyouji2(int i){
    hyouji1(i);  //ここでposの意味合い変わる 手札の番号になる 枚数も計算する
    owari();
}

void kirikae(void){
    for(int i=0;i<30;i++){
        cout << endl; //ターミナルの画面切り替え
    }
}

void hiku(int a,int aite,int i){
    for(int j=0;j<52;j++){
        if(card[j].own == aite && card[j].pos ==a){
            card[j].own=i;x[i]++;card[j].pos = x[i];
            cout << "弾いたのはこのカードです：" << card[j].mark << " " << card[j].number << endl;
        }
    }
}

void agarihantei(int num,int k){
    x[num]=0;
    for(int i=0;i<52;i++){
        if(card[i].own==num){x[num]++;card[i].pos=x[num];}
    }
    if(x[num]==0){
        cout << "player " << num+1 << "があがりました。おめでとうございます！"  << endl;
        win[wins]=num;
        wins++;
        if(wins==3){
            for(int j=0;j<4;j++){
                if(j!= win[0] && j!=win[1] && j!= win[2])lose=j;
            }
        }
    agariban=k;
    owari();
    }
}

void owari(void){
    cout << "見終わったら1~10の好きな数字を入力してください: ";
    cin >> a;
    kirikae();
}

void tehudaseiton(int num,int i,int j){
    int index=0;
    for(int k=0;k<13;k++){
        if(ptehuda[num][k]!=i && ptehuda[num][k]!=j){ptehuda[num][index]=ptehuda[num][k];index++;}
    }
    ptehuda[num][11]=ptehuda[num][12]=-1;
}
