#include "FastLED.h"

#define BTN0 PIN_PB0
#define BTN1 PIN_PB1
#define BTN2 PIN_PB2
#define BTN3 PIN_PB3
#define BTN4 PIN_PB4
#define BTN5 PIN_PB5

#define RESET PIN_PD0

#define LDR0 PIN_PC0
#define LDR1 PIN_PC1
#define LDR2 PIN_PC2
#define LDR3 PIN_PC3
#define LDR4 PIN_PC4
#define LDR5 PIN_PC5

#define LED_PIN PIN_PD5

#define NUM_LEDS 30
#define COL 6
#define ROW 5
#define BRIGHTNESS  255 

CRGB leds[NUM_LEDS];
//CRGB color_player1 = CRGB::Blue;
//CRGB color_player2 = CRGB::Red;
CRGB highlight = CRGB::White;
CRGB BG = CRGB::Black; //change to white
CRGB OFF = CRGB::Black;
CRGB COLOR[6] = {CRGB::Blue, CRGB::Yellow, BG, OFF, CRGB::Aqua, CRGB::Green}; //0=p1 1=p2
CRGB POOL[21] ={CRGB::Red,CRGB::OrangeRed,CRGB::Goldenrod,CRGB::Crimson,
                CRGB::Blue,CRGB:: Indigo,CRGB::Purple,CRGB::MidnightBlue,
                CRGB::Green,ColorFromPalette( RainbowColors_p, 90),ColorFromPalette( RainbowColors_p, 85),ColorFromPalette( RainbowColors_p, 80),
                CRGB::LawnGreen,ColorFromPalette( RainbowColors_p, 75),ColorFromPalette( RainbowColors_p, 70),ColorFromPalette( RainbowColors_p, 65),
                CRGB::LimeGreen,CRGB::MediumSpringGreen,CRGB::Aqua,
                CRGB::Blue,CRGB::Yellow,};
                
int ldr[6] = {LDR0, LDR1, LDR2, LDR3, LDR4, LDR5};
int btn[6] = {BTN0, BTN1, BTN2, BTN3, BTN4, BTN5};

volatile int grid[COL][ROW];
volatile bool endgame = false;
volatile bool reset = true;
volatile int current_player;
volatile int last_drop;
volatile int select;

void setLed(int col, int row, int sel);
void drop(int c);
void switchInput();
void switchReset();
void checkFin();
void push(int btn, int col);
void markColome(int col);
void fullDisplay(int sel);
void selectColor(int player);


void setup()
{
    FastLED.addLeds<WS2812B, LED_PIN>(leds, NUM_LEDS)    
           .setDither(BRIGHTNESS < 255);
    FastLED.setBrightness(BRIGHTNESS);

    pinMode(PIN_PD3, OUTPUT);
    pinMode(BTN0, INPUT_PULLUP);
    pinMode(BTN1, INPUT_PULLUP);
    pinMode(BTN2, INPUT_PULLUP);
    pinMode(BTN3, INPUT_PULLUP);
    pinMode(BTN4, INPUT_PULLUP);
    pinMode(BTN5, INPUT_PULLUP);
    pinMode(RESET, INPUT_PULLUP);
    pinMode(LDR0, INPUT);
    pinMode(LDR1, INPUT);
    pinMode(LDR2, INPUT);
    pinMode(LDR3, INPUT);
    pinMode(LDR4, INPUT);
    pinMode(LDR5, INPUT);
}

void loop()
{
  if(reset){
    fullDisplay(4);
    delay(500);
    fullDisplay(5);    
    current_player = 0;
    selectColor();
    selectColor();
    reset = false;
  }
  initial();
  while (!endgame)
  {
    marking();
    switchInput();
    checkFin();
    switchReset();
  }
}

void initial()
{
  endgame = 0;
  for (int c = 0; c < COL; c++)
    for (int r = 0; r < ROW; r++)
      setLed(c, r, 3);
  for (int c = 0; c < COL; c++)
    for (int r = 0; r < ROW; r++)
      setLed(c, r, 2);

  for (int i = 0; i < COL; i++) //rm
    setLed(i, 4, 4);
  digitalWrite(PIN_PD3, HIGH); //rm

//  FastLED.show();
}

void setLed(int col, int row, int sel)
{
  grid[col][row] = sel;
  leds[col * ROW + row] = COLOR[sel];
  FastLED.show();
}

void selectColor(){
  select=19;
  for(;;){
    fullDisplay(3);
    if(!digitalRead(BTN0)){
       select--;
       select = select<0?19:select;
    }
    if(!digitalRead(BTN1)){
      select++;
      select%=19;
    }
    if(current_player==1 && POOL[select]==COLOR[0])
      select++; //check duplicate
    if(!digitalRead(RESET)){
      COLOR[current_player]=POOL[select];
      current_player = !current_player;
      break;
    }
  }
  while(!digitalRead(RESET)){}
}

void drop(int c)
{
  int color = current_player;
  last_drop = c;
  for (int r = ROW - 1; r >= 0; r--)
  {
    setLed(c, r, color);
    delay(150);
    if (r == 0 || grid[c][r - 1] != 2)
      break;
    else
      setLed(c, r, 2);
  }
}

void push(int btn, int col)
{
  if (!digitalRead(btn))
  {
    //****don't forget rm last condition
    if (grid[col][ROW - 1] == 2 || grid[col][ROW - 1] == 4)
    {
      digitalWrite(PIN_PD3, LOW); //rm
      drop(col);
      current_player = !current_player;
    }
    digitalWrite(PIN_PD3, HIGH); //rm
    while(!digitalRead(btn)){}
  }
}

void switchInput()
{
  for(int i=0;i<6;i++){
    if(i==2 || i==3 || i==4 || i==5) continue; //set btn off
    push(btn[i],i);
  }
}

void switchReset(){
  if (!digitalRead(RESET)){
    reset = true;
    endgame = true;
    while(!digitalRead(btn)){}
  }
}

void checkFin()
{
  for (int i = 0; i <= COL - 4; i++)
  {
    for (int j = 0; j <= ROW - 4; j++)
    {
      //col
      if (grid[i][j] != 2)
      {
        if (grid[i][j] == grid[i][j + 1] && grid[i][j] == grid[i][j + 2] && grid[i][j] == grid[i][j + 3])
        {
          leds[i * ROW + j] = CRGB::Green;
          delay(100);
          endgame = true;
        }
//        row
        else if (grid[i][j] == grid[i + 1][j] && grid[i][j] == grid[i + 2][j] && grid[i][j] == grid[i + 3][j])
          endgame = true;
        //crossup
        else if (grid[i][j] == grid[i + 1][j + 1] && grid[i][j] == grid[i + 2][j + 2] && grid[i][j] == grid[i + 3][j + 3])
          endgame = true;
        else if (grid[i][j + 3] != 2 && grid[i][j + 3] == grid[i + 1][j + 2] && grid[i][j + 3] == grid[i + 2][j + 1] && grid[i][j + 3] == grid[i + 3][j])
          endgame = true;
        if(endgame){
          fullDisplay(!current_player);
        }
      }
    }
  }
//  if (grid[0][ROW - 1] != 2 && grid[1][ROW - 1] != 2 && grid[2][ROW - 1] != 2 && grid[3][ROW - 1] != 2 && grid[4][ROW - 1] != 2 && grid[5][ROW - 1] != 2)
  if (grid[0][ROW - 1] != 4 && grid[1][ROW - 1] != 4 && grid[2][ROW - 1] != 4 && grid[3][ROW - 1] != 4 && grid[4][ROW - 1] != 4 && grid[5][ROW - 1] != 4)
  {
    //draw
    endgame = true;
    return 0;
  }
}

void markColome(int col)
{
  for (int r = 0; r < ROW; r++)
    leds[col * ROW + r] = highlight;
  FastLED.show();
  delay(180);
  for (int r = 0; r < ROW; r++)
    setLed(col, r, grid[col][r]);
//  FastLED.show();
  delay(500);
}

void marking()
{
  int get_light[COL+1];
  for(int i=0;i<COL;i++){
    get_light[i]=analogRead(ldr[i]);
  }
  get_light[COL] = 1024;

  //check min
  int mn = COL;
  for(int i=0;i<COL;i++){
    if(i==0||i==1||i==2||i==3||i==4||i==5) continue; //set ldr off
    mn = get_light[i]<get_light[mn] ? i:mn;
  }
  if(analogRead(ldr[mn]) < 800 && mn !=COL){
    digitalWrite(PIN_PD3, LOW); //rm
    delay(100); //rm
    digitalWrite(PIN_PD3, HIGH);//rm
    markColome(1);
  }
}

void fullDisplay(int sel){
  
  //winner
  if(sel==0 || sel==1){
    for(int c=0;c<COL;c++)
        for(int r=0;r<ROW;r++)
          setLed(c,r,sel);
    delay(200);
  }
  
  //draw
  else if(sel==2){
    for(int c=0;c<COL/2;c++)
      for(int r=0;r<ROW;r++)
        setLed(c,r,0);
    for(int c=COL/2;c<COL;c++)
      for(int r=0;r<ROW;r++)
        setLed(c,r,1);
    delay(200);
  }
  
  //select
  else if(sel==3){
    for(int i = 0;i<NUM_LEDS;i++)
      leds[i]=POOL[select];
    if(current_player==0){
      leds[2*ROW]=BG;
      leds[2*ROW+4]=BG;
      for(int i=0;i<ROW;i++)
        leds[3*ROW+i]=BG;
      leds[4*ROW]=BG;
    }
    else if(current_player==1){
      for(int i=0;i<ROW;i++){
        if(i==3) continue;
        leds[2*ROW+i]=BG; 
      }
      for(int i=0;i<ROW;i+=2)
        leds[3*ROW+i]=BG; 
      for(int i=0;i<ROW;i++){
        if(i==1) continue;
        leds[4*ROW+i]=BG;
      }           
    }
    FastLED.show();
    delay(100);
  }
  
  //reset
  else if(sel==4){
    //set Rainbow
    //part1
    for(int i=0;i<5;i++){
      int x=0;
      for(int j=0;j<i+1;j++){
        leds[x*ROW+(i-j)]=ColorFromPalette( RainbowColors_p, i*20+25);
        x++;
      }
      FastLED.show();
      delay(100);
    }
    //part2
    for(int i=1;i<COL;i++){
      int y=5;
      for(int j=i;j<COL;j++){
        y--;
        leds[j*ROW+y]=ColorFromPalette( RainbowColors_p, i*25+100);
      }
      FastLED.show();
      delay(100);      
    }
  }
  //disappear
  else if(sel==5){
    for(int i=0;i<5;i++){
      int x=0;
      for(int j=0;j<i+1;j++){
        leds[x*ROW+(i-j)]=OFF;
        x++;
      }
      FastLED.show();
      delay(100);
    }
    //part2
    for(int i=1;i<COL;i++){
      int y=5;
      for(int j=i;j<COL;j++){
        y--;
        leds[j*ROW+y]=OFF;
      }
      FastLED.show();
      delay(100);      
    }
  }
}

