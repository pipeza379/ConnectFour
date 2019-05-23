#include "FastLED.h"
#include <pt.h>

#define PT_DELAY(pt, ms, ts) \
    ts = millis(); \
    PT_WAIT_UNTIL(pt, millis()-ts < (ms));

#define BTN0 PIN_PB0
#define BTN1 PIN_PB1
#define BTN2 PIN_PB2
#define BTN3 PIN_PB3
#define BTN4 PIN_PB4
#define BTN5 PIN_PB5
#define VCC_COM 700
#define VCC_PB 600

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
#define POOL_SIZE 20

CRGB leds[NUM_LEDS];
//CRGB color_player1 = CRGB::Blue;
//CRGB color_player2 = CRGB::Red;
CRGB highlight = CRGB::White;
CRGB BG = CRGB::White; //change to white
CRGB OFF = CRGB::Black;
CRGB COLOR[6] = {CRGB::Blue, CRGB::Yellow, BG, OFF, CRGB::Aqua, CRGB::Green}; //0=p1 1=p2
CRGB POOL[20] = {
    CRGB::Red,
    CRGB::OrangeRed,
    CRGB::Goldenrod,
    CRGB::Crimson,

    CRGB::Blue,
    CRGB::Indigo,
    CRGB::Purple,
    CRGB::MidnightBlue,
    
    CRGB::Green,
    ColorFromPalette(RainbowColors_p, 90),
    ColorFromPalette(RainbowColors_p, 85),
    ColorFromPalette(RainbowColors_p, 80),
    
//    CRGB::LawnGreen,
    ColorFromPalette(RainbowColors_p, 75),
    ColorFromPalette(RainbowColors_p, 70),
    ColorFromPalette(RainbowColors_p, 65),
    
    CRGB::LimeGreen,
    CRGB::MediumSpringGreen,
    CRGB::Aqua,
    //default color for p1 p2
    CRGB::Blue,
    CRGB::Yellow,
};

int ldr[6] = {LDR0, LDR1, LDR2, LDR3, LDR4, LDR5};
int btn[6] = {BTN0, BTN1, BTN2, BTN3, BTN4, BTN5};
int point[30] = {-1};
int count = 0;
int point_p1 = 0;
int point_p2 = 0;

volatile int grid[COL][ROW];
volatile bool endgame = false;
volatile bool reset = true;
volatile bool draw = false;
volatile int current_player;
volatile int last_drop;
volatile int select;
volatile int brightness = 255;

//struct pt pt_marking;
struct pt pt_switchInput;
//struct pt pt_switchReset;

void setLed(int col, int row, int sel);
void drop(int c);
//void switchInput();
void switchReset();
void checkFin();
void push(int btn, int col);
void markColome(int col);
void fullDisplay(int sel);
void selectColor(int player);
void addPoint();
void calPoint();
void setBright(int w,int x,int y ,int z);

static void switchInput(struct pt* pt);
static void marking(struct pt* pt);

void setup()
{
  FastLED.addLeds<WS2812B, LED_PIN>(leds, NUM_LEDS);
//      .setDither(brightness < 255);
  FastLED.setBrightness(brightness);

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

//  PT_INIT(&pt_marking);
  PT_INIT(&pt_switchInput);
}

void loop()
{
  if (reset)
  {
    fullDisplay(4);
    delay(500);
    fullDisplay(5);
    current_player = 0;
    selectColor();
    selectColor();
    count = 0;
    for(int i=0;i<NUM_LEDS;i++)
      point[i] = -1;    
    point_p1 = 0;
    point_p2 = 0;
    reset = false;
  }
  initial();
  while (!endgame)
  {
    marking();
//    marking(&pt_marking);
//    switchInput();
    switchInput(&pt_switchInput);
    switchReset();
  }
  if(!reset)
    addPoint();
  else //endgame
    calPoint();  
}

///////////////////////////////////
//////MULTITASK////////////////////
///////////////////////////////////
//static void marking(struct pt *pt)
//{
//  static uint32_t ts;
//  static int get_light[COL + 1];
//  static unsigned long timestamp = 0;     
//  PT_BEGIN(pt);
//
//  for(;;){
////    for (int i = 0; i < COL; i++)
////      get_light[i] = analogRead(ldr[i]);
////    get_light[COL] = 1024;
////  
////    //check min
////    static int mn = COL;
////    for (int i = 0; i < COL; i++){
////      mn = get_light[i] < get_light[mn] ? i : mn;
//////      PT_DELAY(pt,10,ts);
////    }
////    if (analogRead(ldr[mn]) < 800 && mn != COL){
////      markColome(mn);
////    }
//    if(analogRead(LDR0) <820)
//      markColome(0);
//    timestamp = millis();
//    PT_WAIT_UNTIL(pt, millis() - timestamp > 10  || !(!digitalRead(BTN0)||!digitalRead(BTN1)||!digitalRead(BTN2)||!digitalRead(BTN3)||!digitalRead(BTN4)||!digitalRead(BTN5)) );
//    if(analogRead(LDR1) <820)
//      markColome(1);
//    timestamp = millis();
//    PT_WAIT_UNTIL(pt, millis() - timestamp > 10  || !(!digitalRead(BTN0)||!digitalRead(BTN1)||!digitalRead(BTN2)||!digitalRead(BTN3)||!digitalRead(BTN4)||!digitalRead(BTN5)) );
//    if(analogRead(LDR2) <820)
//      markColome(2);
//    timestamp = millis();
//    PT_WAIT_UNTIL(pt, millis() - timestamp > 10  || !(!digitalRead(BTN0)||!digitalRead(BTN1)||!digitalRead(BTN2)||!digitalRead(BTN3)||!digitalRead(BTN4)||!digitalRead(BTN5)) );
//    if(analogRead(LDR3) <820)
//      markColome(3);
//    timestamp = millis();
//    PT_WAIT_UNTIL(pt, millis() - timestamp > 10  || !(!digitalRead(BTN0)||!digitalRead(BTN1)||!digitalRead(BTN2)||!digitalRead(BTN3)||!digitalRead(BTN4)||!digitalRead(BTN5)) );
//    if(analogRead(LDR4) <820)
//      markColome(4);
//    timestamp = millis();
//    PT_WAIT_UNTIL(pt, millis() - timestamp > 10  || !(!digitalRead(BTN0)||!digitalRead(BTN1)||!digitalRead(BTN2)||!digitalRead(BTN3)||!digitalRead(BTN4)||!digitalRead(BTN5)) );
//    if(analogRead(LDR5) <820)
//      markColome(5);
//    timestamp = millis();
//    PT_WAIT_UNTIL(pt, millis() - timestamp > 10  || !(!digitalRead(BTN0)||!digitalRead(BTN1)||!digitalRead(BTN2)||!digitalRead(BTN3)||!digitalRead(BTN4)||!digitalRead(BTN5)) );
//  }
//  
//  PT_END(pt);
//}

static void switchInput(struct pt *pt)
{
  static uint32_t ts;
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
 
  for (;;)
  {
   if(!digitalRead(BTN0)||!digitalRead(BTN1)||!digitalRead(BTN2)||!digitalRead(BTN3)||!digitalRead(BTN4)||!digitalRead(BTN5)){    
      for (int i = 0; i < 6; i++)
        push(btn[i], i);
      checkFin();
  //    PT_YIELD(pt);
    }
      timestamp = millis();
   PT_WAIT_UNTIL(pt, millis() - timestamp > 100);
  }
 
  PT_END(pt);
}

///////////////////////////////////


void initial()
{
  endgame = false;
  draw = false;
  for (int c = 0; c < COL; c++)
    for (int r = 0; r < ROW; r++)
      setLed(c, r, 3);
  for (int c = 0; c < COL; c++)
    for (int r = 0; r < ROW; r++)
      setLed(c, r, 2);

//  for (int i = 0; i < COL; i++) //rm
//    setLed(i, 4, 4);
  digitalWrite(PIN_PD3, HIGH); //rm
}


void switchInput()
{
    for (int i = 0; i < 6; i++)
      push(btn[i], i);
  checkFin();
}

void switchReset()
{
  if (!digitalRead(RESET))
  {
    reset = true;
    endgame = true;
    while (!digitalRead(RESET)){}
  }
}

void setLed(int col, int row, int sel)
{
  grid[col][row] = sel;
  leds[col * ROW + row] = COLOR[sel];
  delay(20);
  FastLED.show();
}

void selectColor()
{
  select = POOL_SIZE-2;
  for (;;)
  {
    if (!digitalRead(BTN0))
    {
      --select;
      select = select < 0 ? POOL_SIZE-3 : select;
    }
    if (!digitalRead(BTN1))
    {
      ++select;
      select = select > POOL_SIZE-3 ? 0 : select;
    }
    //check duplicate
    if (current_player == 1 && POOL[select] == COLOR[0])
      if (!digitalRead(BTN0))
        --select;
      else
        ++select;
      
    fullDisplay(3);
    if (!digitalRead(BTN2)) //enter
    {
      COLOR[current_player] = POOL[select];
      current_player = !current_player;
      break;
    }
  }
  while (!digitalRead(BTN2)){}
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
    while (!digitalRead(btn)){}
  }
}


void setBright(int w,int x,int y ,int z){
          for(int k=0;k<4;k++){
            leds[w].maximizeBrightness(10);
            leds[x].maximizeBrightness(10);
            leds[y].maximizeBrightness(10);
            leds[z].maximizeBrightness(10);
            FastLED.show();   
            delay(100);

            leds[w].maximizeBrightness(100);
            leds[x].maximizeBrightness(100);
            leds[y].maximizeBrightness(100);
            leds[z].maximizeBrightness(100);
            FastLED.show();   
            delay(100);

            leds[w].maximizeBrightness(255);
            leds[x].maximizeBrightness(255);
            leds[y].maximizeBrightness(255);
            leds[z].maximizeBrightness(255);
            FastLED.show();   
            delay(100);
          }
}

void checkFin()
{
  for(int i=0; i<=COL-4;i++){
    for(int j=0;j<ROW;j++){
      if (grid[i][j] != 2)
      {
        //row
        if (grid[i][j] == grid[i + 1][j] && grid[i][j] == grid[i + 2][j] && grid[i][j] == grid[i + 3][j])
        {
          setBright(i*ROW+j,(i+1)*ROW+j,(i+2)*ROW+j,(i+3)*ROW+j);
          endgame = true;
        }      
      }
    }
  }

  for(int i=0;i<COL;i++){
    for(int j=0;j<=ROW-4;j++){
      if (grid[i][j] != 2){
        //col
        if (grid[i][j] == grid[i][j + 1] && grid[i][j] == grid[i][j + 2] && grid[i][j] == grid[i][j + 3]){
          setBright(i*ROW+j,i*ROW+j+1,i*ROW+j+2,i*ROW+j+3);
          endgame = true;
        }        
      }
    }
  }
   
  for (int i = 0; i <= COL-4; i++){
    for (int j = 0; j <= ROW-4; j++){
      if (grid[i][j] != 2){
        //crossup
        if (grid[i][j] == grid[i + 1][j + 1] && grid[i][j] == grid[i + 2][j + 2] && grid[i][j] == grid[i + 3][j + 3]){
          setBright(i*ROW+j,(i+1)*ROW+j+1,(i+2)*ROW+j+2,(i+3)*ROW+j+3);
          endgame = true;
        }
        else if (grid[i][j + 3] != 2 && grid[i][j + 3] == grid[i + 1][j + 2] && grid[i][j + 3] == grid[i + 2][j + 1] && grid[i][j + 3] == grid[i + 3][j]){
          setBright(i*ROW+j+3,(i+1)*ROW+j+2,(i+2)*ROW+j+1,(i+3)*ROW+j);
          endgame = true;
        }
      }
    }
  }
    if (grid[0][ROW - 1] != 2 && grid[1][ROW - 1] != 2 && grid[2][ROW - 1] != 2 && grid[3][ROW - 1] != 2 && grid[4][ROW - 1] != 2 && grid[5][ROW - 1] != 2){
      //draw
      draw = true;
      endgame = true;
    }
    if (endgame){
//    fullDisplay(!current_player);
//    delay(200);
      fullDisplay(5);
//    return 0;
    }
}

void markColome(int col)
{
  push(btn[col],col);
  for (int r = ROW-1; r >= 0; r--){
    if(grid[col][r] == 2){
      grid[col][r] = 3;
      leds[col * ROW + r] = COLOR[3];
    }
//      setLed(col,r,3);
    if(!digitalRead(BTN0)||!digitalRead(BTN1)||!digitalRead(BTN2)||!digitalRead(BTN3)||!digitalRead(BTN4)||!digitalRead(BTN5))
      break;
  }
  FastLED.show();
  delay(200);
  for (int r = ROW-1; r >=0; --r){
    if(grid[col][r] == 3){
      grid[col][r] = 2;
      leds[col * ROW + r] = COLOR[2];
    }
//      setLed(col,r,2);
  }
  FastLED.show();
  push(btn[col],col);
  checkFin();
  delay(200);
}

void marking()
{
  int get_light[COL + 1];
  for (int i = 0; i < COL; i++){
    get_light[i] = analogRead(ldr[i]);
//    if(!digitalRead(BTN0)||!digitalRead(BTN1)||!digitalRead(BTN2)||!digitalRead(BTN3)||!digitalRead(BTN4)||!digitalRead(BTN5)){
//      switchInput();
//      return 0;
//    }    
  }
  get_light[COL] = 1024;
  
  //check min
  int mn = COL;
  for (int i = 0; i < COL; i++){
    mn = get_light[i] < get_light[mn] ? i : mn;
//    if(!digitalRead(BTN0)||!digitalRead(BTN1)||!digitalRead(BTN2)||!digitalRead(BTN3)||!digitalRead(BTN4)||!digitalRead(BTN5)){
//      switchInput();
//      return 0;
//    }
  }
  if (analogRead(ldr[mn]) < VCC_PB && mn != COL)
    markColome(mn);
}

void fullDisplay(int sel)
{

  //winner
  if (sel == 0 || sel == 1)
  {
    for (int c = 0; c < COL; c++)
      for (int r = 0; r < ROW; r++)
        leds[c*ROW+r]=COLOR[sel];
    FastLED.show();
    delay(500);
  }

  //draw
  else if (sel == 2)
  {
    for (int c = 0; c < COL / 2; c++)
      for (int r = 0; r < ROW; r++)
        leds[c*ROW+r]=COLOR[0];      
    for (int c = COL / 2; c < COL; c++)
      for (int r = 0; r < ROW; r++)
        leds[c*ROW+r]=COLOR[1];
    FastLED.show();
    delay(500);
  }

  //select
  else if (sel == 3)
  {
    for (int i = 0; i < NUM_LEDS; i++)
      leds[i] = POOL[select];
    if (current_player == 0)
    {
      leds[2 * ROW] = BG;
      leds[2 * ROW + 4] = BG;
      for (int i = 0; i < ROW; i++)
        leds[3 * ROW + i] = BG;
      leds[4 * ROW] = BG;
    }
    else if (current_player == 1)
    {
      for (int i = 0; i < ROW; i++)
      {
        if (i == 3)
          continue;
        leds[2 * ROW + i] = BG;
      }
      for (int i = 0; i < ROW; i += 2)
        leds[3 * ROW + i] = BG;
      for (int i = 0; i < ROW; i++)
      {
        if (i == 1)
          continue;
        leds[4 * ROW + i] = BG;
      }
    }
    FastLED.show();
    delay(100);
  }

  //reset
  else if (sel == 4)
  {
    //set Rainbow
    //part1
    for (int i = 0; i < 5; i++)
    {
      int x = 0;
      for (int j = 0; j < i + 1; j++)
      {
        leds[x * ROW + (i - j)] = ColorFromPalette(RainbowColors_p, i * 20 + 25);
        x++;
      }
      FastLED.show();
      delay(100);
    }
    //part2
    for (int i = 1; i < COL; i++)
    {
      int y = 5;
      for (int j = i; j < COL; j++)
      {
        y--;
        leds[j * ROW + y] = ColorFromPalette(RainbowColors_p, i * 25 + 100);
      }
      FastLED.show();
      delay(100);
    }
  }
  //disappear
  else if (sel == 5)
  {
    for (int i = 0; i < 5; i++)
    {
      int x = 0;
      for (int j = 0; j < i + 1; j++)
      {
        leds[x * ROW + (i - j)] = OFF;
        x++;
      }
      FastLED.show();
      delay(70);
    }
    //part2
    for (int i = 1; i < COL; i++)
    {
      int y = 5;
      for (int j = i; j < COL; j++)
      {
        y--;
        leds[j * ROW + y] = OFF;
      }
      FastLED.show();
      delay(70);
    }
  }
}

void addPoint(){
  if(draw)
    return 0;
  if(!current_player==0)
    point_p1+=1;
  else if(!current_player==1)
    point_p2+=1;
    
  point[count] = !current_player;
  for (int c = 0; c < COL; c++)
    for (int r = 0; r < ROW; r++)
      setLed(c, r, 2);
  for(int i=0;i<count;i++)
      setLed(i%5,4-i/6,point[i]);
  delay(500);
  setLed(count%5,4-count/6,point[count]);
  count+=1;
  delay(1000);
}

void calPoint(){
  fullDisplay(5);    
  if(point_p1>point_p2)
    fullDisplay(0);
  else if(point_p2>point_p1)
    fullDisplay(1);
  else if(point_p1==point_p2)
    fullDisplay(2);
  delay(1000);
  fullDisplay(5);    
}

