const int buttonPinA = 2;
const int buttonPinB = 3;
const int buttonPinC = 4;
const int ledPinA =  13;
const int ledPinB =  12;
const int ledPinC =  11;
int pinDigitron = 22; //管脚D2连接到数码管的A脚，D3连B... D9连h
int pinWei = 38;  
////设置阴极接口
//int a = 22;//int b = 24;//int c = 26;//int d = 28;//int e = 30;//int f = 32;//int g = 34;//int h = 36;
////设置阳极接口
//int d4 = 38;//int d3 = 40;//int d2 = 42;//int d1 = 44;

//在数码管中显示数字的函数
void displayDigit(unsigned char digit) {
  //定义一个数组表：不同数字的abcdefgh各段的取值
  unsigned char abcdefgh[][8] = {
    {1,1,1,1,1,1,0,0},  //0
    {0,1,1,0,0,0,0,0},  //1
    {1,1,0,1,1,0,1,0},  //2
    {1,1,1,1,0,0,1,0},  //3
    {0,1,1,0,0,1,1,0},  //4
    {1,0,1,1,0,1,1,0},  //5
    {1,0,1,1,1,1,1,0},  //6
    {1,1,1,0,0,0,0,0},  //7
    {1,1,1,1,1,1,1,0},  //8
    {1,1,1,1,0,1,1,0},  //9
    {1,1,1,0,1,1,1,0},  //A
    {0,0,1,1,1,1,1,0},  //b
    {1,0,0,1,1,1,0,0},  //C
    {0,1,1,1,1,0,1,0},  //d
    {1,0,0,1,1,1,1,0},  //E
    {1,0,0,0,1,1,1,0},  //F
    {0,0,0,0,0,0,0,1},  //DOT = 16
    {0,0,0,0,0,0,0,0},  //BLANK = 17
    {0,0,0,0,0,0,1,0},  //MINUS= 18
  };
  
  if ( digit > 18 ) return;
  for (unsigned char x=0; x<8; x++)
    digitalWrite( pinDigitron + 2*x, abcdefgh[digit][x] );
}
//在指定的位，显示指定的数字
void display(unsigned char wei, unsigned char digit,int n) {
  for(int x=0; x<4; x++) {
     if ( x != wei- 1)
       digitalWrite(pinWei + 2*x, HIGH);
  }
  displayDigit(17);//擦除
  if(digit == 0 && wei ==4 && n<1000)return;
  if(digit == 0 && wei ==3 && n<100)return;
  if(digit == 0 && wei ==2 && n<10)return;
  digitalWrite(pinWei + 2*(wei-1) , LOW);//位选
  displayDigit(digit);//显示数字
  delayMicroseconds(55);
}
void shownum(int n){  
  display(1, n%10, n);
  display(2, n%100/10, n);
  display(3, n%1000/100, n);
  display(4, n/1000, n);
}
void setup() {
  Serial.begin(9600);
  for(int x=0; x<4; x++) {
     pinMode(pinWei+2*x, OUTPUT); //设置各脚为输出状态
     digitalWrite(pinWei+2*x, HIGH);//
   }
  for(int x=0; x<8; x++) {
    pinMode(pinDigitron+2*x, OUTPUT); //设置各脚为输出状态
  } 

  for(int x=0; x<4; x++) {
     digitalWrite(pinWei+2*x, LOW);
     for(int i=0; i<8; i++) {
      if(i == 7) continue;
      if(i == 6) continue;  
      digitalWrite( pinDigitron + 2*i,1 );
     } 
  }
  pinMode(buttonPinA, INPUT_PULLUP);
  pinMode(buttonPinB, INPUT_PULLUP);
  pinMode(buttonPinC, INPUT_PULLUP);
  pinMode(ledPinA, OUTPUT);
  pinMode(ledPinB, OUTPUT);
  pinMode(ledPinC, OUTPUT);
  randomSeed(analogRead(0));
}
int gamestar = 0;
int addflag = 0;//是否正在按键
int score = 0;//得分
#define MAXHP 3//最大生命
int hp = MAXHP;//当前生命
int GAMELV=2000;//游戏难度，灯亮着的时长，越大游戏难度越低
int nowdg = 0;//当前进程
int getscore = 0;//敲中得分
int lting = -1;//lighting当前正在亮的灯
int lastlt = -1;//shang ci liang de deng
int bting = -2;//当前按着的键
int gamestop = 0;//游戏是否暂停
int gamelt = 0;//灯有没在闪
int cacheLV =0;//当前游戏难度
// 系统调用，无限循环方法
void loop() {
  
  shownum(score);
  if(gamestar == 0){
    for(int i=0;i < 3;i++){
      digitalWrite(ledPinC+i, HIGH);
    }
    if((score/10) > cacheLV){
      GAMELV = GAMELV*3/4;
      cacheLV += 1;
      if(GAMELV < 200){
        GAMELV = 200;
      }
    }
    delay(GAMELV);
    lting = random(2);//0-2
    lastlt = lting;
    //Serial.println(lting);
    for(int i=0;i < 3;i++){
      digitalWrite(ledPinC+i, LOW);//全熄灭
    }
    digitalWrite(ledPinA-lting, HIGH);//亮一个
    gamestar = 1;
  }
  if(hp <= 0){ //游戏结束
    gamestop = 1;
  }
  if(gamestop == 0){//游戏并未结束
    delay(1);
    //Serial.println(nowdg);
    nowdg += 1;
    if(getscore == 1){//换灯
      nowdg = GAMELV;
    }
    if(nowdg >= GAMELV){
      if(getscore == 0){//到时间没按键
        hp -= 1;
        if(score>0)score-=1;
      }
      do{
        lting = random(3);//0-2
        Serial.println(lting);
      }while(lting == lastlt);
      lastlt = lting;
      //Serial.println(lting);
      for(int i=0;i < 3;i++){
        digitalWrite(ledPinC+i, LOW);//全熄灭
      }
      digitalWrite(ledPinA-lting, HIGH);//亮一个
      nowdg = 0;
      getscore = 0;
    }
    if(digitalRead(buttonPinA)==LOW || digitalRead(buttonPinB)==LOW || digitalRead(buttonPinC)==LOW){
      if(addflag == 0){
        addflag = 1;
        if(lting != -1){
          if(digitalRead(buttonPinA)==LOW){
            bting = 0;
          }else if(digitalRead(buttonPinB)==LOW){
            bting = 1;
          }else if(digitalRead(buttonPinC)==LOW){
            bting = 2;
          }else{
            bting = -2;
          }
          //Serial.println(bting);
          if(lting != bting){//按错灯
            hp -= 1;
            if(score>0)score-=1;
          }else{
            score += 1;
            getscore = 1;
          }
        }else{//没灯亮时候按灯
          hp -= 1;
          if(score>0)score-=1;
        }
      }
    }else{
      addflag = 0;
    }
  }else{
    delay(1);
    nowdg += 1;
    if(nowdg>=1000){
      nowdg = 0;
      if(gamelt == 0){
        gamelt = 1;
        digitalWrite(ledPinA, HIGH);
        digitalWrite(ledPinB, HIGH);
        digitalWrite(ledPinC, HIGH);
      }else{
        gamelt = 0;
        digitalWrite(ledPinA, LOW);
        digitalWrite(ledPinB, LOW);
        digitalWrite(ledPinC, LOW);
      }
    }
  }

}
