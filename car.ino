#include<MsTimer2.h>   //需要用到库文件MsTimer2
void T2Int_Process(void);
int En1 = A0;          //L293D的EN1对应开发板的A0
int In1 = A1;          //L293D的IN1对应开发板的A1
int In2 = A2;          //L293D的IN2对应开发板的A2
int In4 = A3;          //L293D的IN4对应开发板的A3
int In3 = A4;          //L293D的IN3对应开发板的A4
int En2 = A5;          //L293D的EN2对应开发板的A5
int NY_en = 8;         //蓝牙使能控制
unsigned char pwmval_left  = 0;           //变量定义      
unsigned char pwmval_right = 0;

unsigned char pwmval_left_init  = 3;     //左电机占空比调节 ，调节值在0到20之间     
unsigned char pwmval_right_init = 3;     //右电机占空比调节 ，调节值在0到20之间     

unsigned char right_pwm = 1;              //右电机PWM开关,为1时打开   
unsigned char left_pwm  = 1;              //左电机PWM开关,为1时打开  

String comdata = "";

unsigned char forward_flag       = 0;     //前进标志
unsigned char left_flag          = 0;     //左转标志
unsigned char right_flag         = 0;     //右转标志
unsigned char back_flag          = 0;     //后退标志
unsigned char stop_flag          = 1;     //停止标志

void setup() {
  // put your setup code here, to run once:
  pinMode(En1,OUTPUT);   //将管脚设置成输出 
  pinMode(In1,OUTPUT);   //将管脚设置成输出  
  pinMode(In2,OUTPUT);   //将管脚设置成输出  
  pinMode(En2,OUTPUT);   //将管脚设置成输出  
  pinMode(In3,OUTPUT);   //将管脚设置成输出  
  pinMode(In4,OUTPUT);   //将管脚设置成输出  

  pinMode(NY_en,OUTPUT); //将管脚设置成输出 
  
  Serial.begin(9600);   //串口初始化
  digitalWrite(NY_en,LOW);

  MsTimer2::set(1,T2Int_Process);    
  MsTimer2::start();                 //定时器启动
}

void forward(void)               //小车前进控制函数
{        
 if(forward_flag == 0)
 {
  MsTimer2::stop();              //定时器停止
  pwmval_right = 0;
  pwmval_left  = 0;
  pwmval_left_init  = 6;
  pwmval_right_init = 6;
  MsTimer2::start();             //定时器启动
    
  digitalWrite(In1,HIGH);         
  digitalWrite(In2,LOW);
        
  digitalWrite(In3,HIGH);     
  digitalWrite(In4,LOW);
 }
 forward_flag = 1;
}

void left_turn(void)             //智能小车左转
{
 if(left_flag == 0)
 {
  MsTimer2::stop();              //定时器停止
  pwmval_right = 0;
  pwmval_left  = 0;
  pwmval_left_init  = 6;
  pwmval_right_init = 0;
  MsTimer2::start();             //定时器启动
         
  digitalWrite(In1,HIGH);         
  digitalWrite(In2,LOW);
      
  digitalWrite(In3,HIGH);       
  digitalWrite(In4,LOW);
 }
 left_flag = 1;
}

void right_turn(void)            //智能小车右转
{
 if(right_flag == 0)
 {
  MsTimer2::stop();              //定时器停止
  pwmval_right = 0;
  pwmval_left  = 0;
  pwmval_left_init  = 0;
  pwmval_right_init = 6;
  MsTimer2::start();             //定时器启动
       
  digitalWrite(In1,HIGH);        
  digitalWrite(In2,LOW);
     
  digitalWrite(In3,HIGH);         
  digitalWrite(In4,LOW);
 }
 right_flag = 1;
}
void back(void)                 //智能小车后退
{
 if(back_flag == 0)
 {
  MsTimer2::stop();             //定时器停止
  pwmval_right = 0;
  pwmval_left  = 0;
  pwmval_left_init  = 6;
  pwmval_right_init = 6;
  MsTimer2::start();            //定时器启动
         
  digitalWrite(In1,LOW);       
  digitalWrite(In2,HIGH);
        
  digitalWrite(In3,LOW);       
  digitalWrite(In4,HIGH);
 }
 back_flag = 1;
}

void stop_car(void)                //智能小车停止
{         
 digitalWrite(In1,LOW);            
 digitalWrite(In2,LOW);
         
 digitalWrite(In3,LOW);            
 digitalWrite(In4,LOW);

 forward_flag       = 0;
 left_flag          = 0;
 right_flag         = 0;
 back_flag          = 0;
 forward_left_flag  = 0;
 forward_right_flag = 0;
 back_left_flag     = 0;
 back_right_flag    = 0; 
}

void left_moto(void)            //左轮速度控制函数
{  
 if(left_pwm)
 {
  if(pwmval_left <= pwmval_left_init)
  {digitalWrite(En1,HIGH);}
  else 
  {digitalWrite(En1,LOW);}
  if(pwmval_left >= 20)
  {pwmval_left = 0;}
 }
 else    
 {digitalWrite(En1,LOW);}
}

void right_moto(void)          //右轮速度控制函数
{ 
 if(right_pwm)
 { 
  if(pwmval_right <= pwmval_right_init)      
  {digitalWrite(En2,HIGH);}
  else if(pwmval_right > pwmval_right_init)
  {digitalWrite(En2,LOW);}
  if(pwmval_right >= 20)
  {pwmval_right = 0;}
 }
 else    
 {digitalWrite(En2,LOW);}
}

void T2Int_Process(void)       //T2中断处理函数
{
 pwmval_left  = pwmval_left  + 1;
 pwmval_right = pwmval_right + 1;

 left_moto();
  right_moto();
}

void PROCESS(void)
{   
 if(comdata=="w")        //接收到前进指令
 {
  delay(50);
  forward();
 }
 else if(comdata=="a")   //接收到左转指令
 {
  delay(50);
  left_turn();
 }
 else if(comdata=="s")   //接收到后退指令
 {
  delay(50);
  back();
 }
 else if(comdata=="d")   //接收到右转指令
 {
  delay(50);
  right_turn();
 }
 else if(comdata=="x") {stop_car();}
 else                                        
 {
  stop_car();
 }
}

void loop() {
  // put your main code here, to run repeatedly: 
  while(Serial.available()>0)
  {
    comdata+=char(Serial.read());
    delay(10);
  }
  if(comdata.length()>0)
  {
   PROCESS(); 
   Serial.print(comdata);
   comdata = "";
  }
}
