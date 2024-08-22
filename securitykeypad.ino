#include <Servo.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <EEPROM.h>

//servo
Servo myservo;

//lcd
LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);

//keypad
const byte rows=4;
const byte cols=3;
char key[rows][cols]={
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[rows]={5,4,3,2};
byte colPins[cols]={6,7,8};
Keypad keypad= Keypad(makeKeymap(key),rowPins,colPins,rows,cols);

//globalni
String password="";
String code="";
int waitTime=1;
bool reset=false;
bool access=false;
int keySignal;
int closeSignal;

//inicijalizacija
void setup()
{
  readpass();
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  myservo.attach(9);
  myservo.write(180);
  lcd.begin(32,2);
  welcome();
}

//glava jamka
void loop()
{
  if(code==""&&!access)
  {
    welcome();
  }
  
  char key=keypad.getKey();
  keySignal=digitalRead(10);
  closeSignal=digitalRead(11);
  
  if (key!=NO_KEY){
    code+=key;
  }
  
  if(key=='*')
  {
    reset=true;
    code="";
  }
  
  if(key=='#')
  {
    reset=false;
    code="";
  }
  
  if(code!=""&&key!=NO_KEY&&!access)
  {
    int passResult=passwordcheck(code);
    if(passResult==1)
    {      
      unlockdoor();
      access=true;
    }
    if(passResult==0)
    {
      incorrect();
      reset=false;
    }
    passResult=2;
  }
  
  if(reset&&access&&key!=NO_KEY)
  {
    lcd.clear();
    lcd.print("ENTER NEW PASSWORD:");
    lcd.setCursor(7,1);
    lcd.print(code);
    if(code.length()==6)
    {
      passwordreset();
      reset=false;
      code="";
    }
  }

  if(keySignal)
  {
    unlockdoor();
    access=true;
    code="";
    waitTime=1;
  }

  if(closeSignal)
  {
    lockdoor();
    access=false;
    code="";
  }
}

//poceten ekran
void welcome()
{
  lcd.setCursor(0,0);
  lcd.print("ENTER CODE");
  lcd.print(password);
}

//citaj lozinka
void readpass()
{
  password="";
  char readchar="";
  for(int i=0;i<6;i++)
  {
    EEPROM.get(i,readchar);
    password+=readchar;
  }
}

//reset za lozinka
void passwordreset()
{
  if(code.length()==6)
  {
    lcd.clear();
    password=code;
    lcd.setCursor(0,0);
    lcd.print("WRITTEN ");
    for(int i=0;i<6;i++)
    {
      EEPROM.put(i,code[i]);
      delay(1000);
      lcd.setCursor(8,0);
      lcd.print(i);
    }
    lcd.clear();
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("PASSWORD MUST BE 6 CHARACTERS LONG");
    delay(4000);
  }
}

//proverka za lozinka
int passwordcheck(String localpass)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PASSWORD:");
  lcd.setCursor(7,0);
  lcd.print(" ");
  lcd.setCursor(7,1);
  //pisuvaj dzvezdi
  for(int l=0;l<code.length();l++)
  {
    lcd.print("*");
  }
  lcd.setCursor(10,0);
  lcd.print(code);
  //porverka za lozinka
  if (password.length()==localpass.length())
  {
    if(password==localpass)
    {
      code="";
      waitTime=1;
      return 1;
    }
    else
    {
      waitTime+=waitTime;
      code="";
      return 0;
    }
  }
  else
  {
    return 2;
  }
}

//odkluci
void unlockdoor()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("UNLOCKING");
  digitalWrite(LED_BUILTIN, HIGH);
  myservo.write(0);
  delay(4000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("UNLOCKED");
}

//zakluci
void lockdoor()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println("LOCKING");  
  myservo.write(180);
  digitalWrite(LED_BUILTIN, LOW);  
  delay(4000); 
  lcd.clear();
  welcome();
}

//pogresen vnes
void incorrect()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("INCORRECT CODE");
  delay(waitTime*1000);
  lcd.clear();
  welcome();
}
