#include <iostream>
#include <cstdio>
#include <cstdlib> //rand用

//あとは画面表示の仕方とか工夫したいのと、関数にしてもう少し見やすいプログラミングにしたい

using namespace std;

class card{
public:
    int mark; //カードのマーク 1:スペード　2:クラブ 3:ハート 4:ダイヤ 5;joker1 6;joker2
    int number; //カードの番号 1~13 11:J 12:Q 13:K  0:joker
    int own; //カードの持ち主  0~3:player1~4 と 5:墓地 6:じじ
    int pos; //カードの位置 シャッフルの時(通し番号) それ以外手札での位置番号として使う
};
class card card[54];

int x[4]; //手札数
int agariban; //だれかがあがったときの手番
int wins=0; //上ったplayer数
int win[3]; //上ったplayerを入れる 0~3
int lose=0; //負けたplayer1~4を入れる

void hantei(void);  //カードの判定をする、全員の手札を探索してペアがあったら墓地に送る
void shuffle1(void); //はじめにカードをシャッフルして配る、じじも作る
void hyouji1(int num); //player num+1 のカードを表示する、手札での位置番号posと手札の枚数x[num]を決める
void hyouji2(int i,int a); //player i+1 の一連の表示動作を行う
void kirikae(void); //30回改行して画面を切り替える
void hiku(int a,int aite,int i); //player i が player aite の位置番号 a のカードを引く
void agarihantei(int num,int k); //player num のあがり判定をする winsも変更する kはその時の番


int main(){
    //カード作成
    for(int i=0;i<54;i++){
        if(i<13){card[i].mark=1;card[i].number=i+1;card[i].pos=i;}  //スペード
        else if(i<26){card[i].mark=2;card[i].number=i-12;card[i].pos=i;} //クラブ
        else if(i<39){card[i].mark=3;card[i].number=i-25;card[i].pos=i;} //ハート
        else if(i<52){card[i].mark=4;card[i].number=i-38;card[i].pos=i;} //ダイヤ
        else {card[i].mark=i-47;card[i].number=0;card[i].pos=i;} //ジョーカー
    }

    shuffle1();
    hantei();
    
    //はじめの手札確認
    for(int i=0;i<4;i++){
        int a;
        hyouji2(i,a);
        agarihantei(i,-1);
    }
    //誰も上っていない間
    while(wins == 0){
        for(int i=0;i<4 && wins==0;i++){
            int a;
            hyouji1(i);  
            int aite;
            if(i==0)aite=3;
            else aite = i-1;
            cout << " player " << aite+1 << "の手札 1 ~ " << x[aite] << " のどれをとりますか: ";
            cin >> a;
            if(a>x[aite] || a<1){cout << "もう一度入力してください" << endl;i--;continue;}
            hiku(a,aite,i); //カード引く
            hantei();
            hyouji2(i,a);
            if(x[i]==0){
                wins=1;win[0]=i;cout << "player" << i+1 << " おめでとうございます！ あがりました" << endl;agariban=i;
                cout << " 見終わったらなにか数字を入力してください: ";
                cin >> a;
                kirikae();
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
                int a;
                cout << " player " << i+1 <<" あなたは上っています！ なにか数字を入力してください:";
                cin >> a;
                continue;
            }
            int a;
            hyouji1(i);  
            int aite;
            if(i==0){aite=3;if(aite==win[0])aite -=1;}
            else {aite = i-1;if(aite==win[0])aite -=1;if(aite<0)aite=3;}
            cout << " player " << aite+1 << "の手札 1 ~ " << x[aite] << " のどれをとりますか: ";
            cin >> a;
            if(a>x[aite] || a<1){cout << "もう一度入力してください" << endl;i--;continue;}
            hiku(a,aite,i); //カード引く
            hantei();
            hyouji2(i,a);
            if(x[i]==0){
                wins=2;win[1]=i;cout << "player" << i+1 << " おめでとうございます！ あがりました" << endl;agariban=i;
                cout << " 見終わったらなにか数字を入力してください: ";
                cin >> a;
                kirikae();
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
                int a;
                cout << " player " << i+1 <<" あなたはあがっています なにか数字を入力してください:";
                cin >> a;
                continue;
            }
            int a;
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
            hyouji2(i,a);
            if(x[i]==0){
                wins=3;win[2]=i;lose=aite;cout << "player" << i+1 << " おめでとうございます！ あがりました" << endl;agariban=i;
                cout << " 見終わったらなにか数字を入力してください: ";
                cin >> a;
                kirikae();
            }
            agarihantei(aite,i);
        }
    }
    cout << "ゲームが終了しました。 結果は"<< endl;
    for(int i=0;i<3;i++){
        cout <<  i+1 <<"位: player " << win[i]+1 << endl;
    }
    cout <<  "負けたのは: player " << lose+1<< endl;
}

void hantei(void){
    for(int i=0;i<54;i++){
        for(int j=i+1;j<54;j++){
            if(card[i].number==card[j].number && card[i].own ==card[j].own && card[i].own != 5)card[i].own = card[j].own=5;
        }
    }
}

void shuffle1(void){
    srand((unsigned) time(NULL));
    for(int i=0;i<54;i++){
        int j = rand()%54; 
        int temp = card[i].pos;
        card[i].pos = card[j].pos;
        card[j].pos = temp;
        //cout << j << endl;
    }

    for(int i=0;i<54;i++){
        int j = card[i].pos; 
        if(j<13)card[i].own=0;
        else if(j<26)card[i].own=1;
        else if(j<39)card[i].own=2;
        else if(j<52)card[i].own=3;
        else if(j==52)card[i].own=0;
        else card[i].own=6; //じじにする
    }
}

void hyouji1(int num){
    x[num]=0;
    cout << "player " << num+1 << "の手札" << endl;
    for(int i=0;i<54;i++){
        if(card[i].own==num){x[num]++;card[i].pos=x[num];cout << card[i].mark << " " << card[i].number << endl;}
    }
    cout << "手札数:" << x[num] << endl;
}

void hyouji2(int i,int a){
    hyouji1(i);  //ここでposの意味合い変わる 手札の番号になる 枚数も計算する
    cout << " 見終わったらなにか数字を入力してください: ";
    cin >> a;
    kirikae();
}

void kirikae(void){
    for(int i=0;i<30;i++){
        cout << endl;
    }
}

void hiku(int a,int aite,int i){
    for(int j=0;j<54;j++){
        if(card[j].own == aite && card[j].pos ==a){
            card[j].own=i;x[i]++;card[j].pos = x[i];
            cout << "弾いたのはこのカードです：" << card[j].mark << " " << card[j].number << endl;
        }
    }
}

void agarihantei(int num,int k){
    x[num]=0;
    for(int i=0;i<54;i++){
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
    int a;
    cout << " 見終わったらなにか数字を入力してください: ";
    cin >> a;
    kirikae();
    }
}