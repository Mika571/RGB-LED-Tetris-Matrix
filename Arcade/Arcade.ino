#include "SmartMatrix_32x32.h"
SmartMatrix matrix;

//Pin Definitions
#define rightArrow      15
#define leftArrow		16		
#define upArrow         17
#define downArrow       18
#define aButton			19 //YES
#define bButton			11 //NO	

//  Matrix Definition
#define cols         8
#define rows        16
#define leftLimit    0
#define rightLimit   7
#define topLimit     0
#define bottomLimit 15
int currentMatrix[rows][cols];

#define blank  { \
    {0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0}, \
}

//Tetris pieces: these piece matrices are represented by their corresponding colors (1-7)
#define Z1 { \
    {0, 0, 0, 0}, \
    {0, 0, 0, 0}, \
    {1, 1, 0, 0}, \
    {0, 1, 1, 0}, \
}
#define Z2 { \
    {0, 0, 0, 0}, \
    {0, 1, 0, 0}, \
    {1, 1, 0, 0}, \
    {1, 0, 0, 0}, \
}
#define S1 { \
    {0, 0, 0, 0}, \
    {0, 0, 0, 0}, \
    {0, 2, 2, 0}, \
    {2, 2, 0, 0}, \
}
#define S2 { \
    {0, 0, 0, 0}, \
    {2, 0, 0, 0}, \
    {2, 2, 0, 0}, \
    {0, 2, 0, 0}, \
}
#define J1 { \
    {0, 0, 0, 0}, \
    {0, 0, 0, 0}, \
    {3, 3, 3, 0}, \
    {0, 0, 3, 0}, \
}
#define J2 { \
    {0, 0, 0, 0}, \
    {0, 3, 0, 0}, \
    {0, 3, 0, 0}, \
    {3, 3, 0, 0}, \
}
#define J3 { \
    {0, 0, 0, 0}, \
    {3, 0, 0, 0}, \
    {3, 3, 3, 0}, \
    {0, 0, 0, 0}, \
}
#define J4 { \
    {0, 0, 0, 0}, \
    {0, 3, 3, 0}, \
    {0, 3, 0, 0}, \
    {0, 3, 0, 0}, \
}
#define L1 { \
    {0, 0, 0, 0}, \
    {0, 0, 0, 0}, \
    {4, 4, 4, 0}, \
    {4, 0, 0, 0}, \
}
#define L2 { \
    {0, 0, 0, 0}, \
    {4, 4, 0, 0}, \
    {0, 4, 0, 0}, \
    {0, 4, 0, 0}, \
}
#define L3 { \
    {0, 0, 0, 0}, \
    {0, 0, 4, 0}, \
    {4, 4, 4, 0}, \
    {0, 0, 0, 0}, \
}
#define L4 { \
    {0, 0, 0, 0}, \
    {0, 4, 0, 0}, \
    {0, 4, 0, 0}, \
    {0, 4, 4, 0}, \
}
#define T1 { \
    {0, 0, 0, 0}, \
    {0, 0, 0, 0}, \
    {6, 6, 6, 0}, \
    {0, 6, 0, 0}, \
}
#define T2 { \
    {0, 0, 0, 0}, \
    {0, 6, 0, 0}, \
    {6, 6, 0, 0}, \
    {0, 6, 0, 0}, \
}
#define T3 { \
    {0, 0, 0, 0}, \
    {0, 6, 0, 0}, \
    {6, 6, 6, 0}, \
    {0, 0, 0, 0}, \
}
#define T4 { \
    {0, 0, 0, 0}, \
    {0, 6, 0, 0}, \
    {0, 6, 6, 0}, \
    {0, 6, 0, 0}, \
}
#define I1 { \
    {0, 0, 0, 0}, \
    {0, 0, 0, 0}, \
    {5, 5, 5, 5}, \
    {0, 0, 0, 0}, \
}
#define I2 { \
    {0, 5, 0, 0}, \
    {0, 5, 0, 0}, \
    {0, 5, 0, 0}, \
    {0, 5, 0, 0}, \
}
#define O1 {  \
    {0, 0, 0, 0}, \
    {0, 0, 0, 0}, \
    {0, 7, 7, 0}, \
    {0, 7, 7, 0}, \
}
//                      0,  1, 2,  3, 4,  5, 6,  7, 8, 9,10, 11,12,13,14, 15,16,17,18
byte pieces[19][4][4]={O1, Z1,Z2, S1,S2, I1,I2, J1,J2,J3,J4, L1,L2,L3,L4, T1,T2,T3,T4};
//                              0,  1,  2,  3,  4,  5,  6
byte piecesGenerated[7][4][4]={O1, Z1, S1, I1, J1, L1, T1};

byte currentPieceIndex;        // stores current piece index: from 0 to 18
byte currentPieceRotatedIndex; // stores the index of the rotated version of the current piece
byte currentPiece[4][4];       // stores the current piece matrix

//the top left corner of the current piece is tracked
int currentPieceRow;           // stores the row position of currentPiece[0][0]
int currentPieceCol;           // stores the col position of currentPiece[0][0]
int currentPieceMovedRow;      // stores the row position of currentPiece[0][0] after a certain move
int currentPieceMovedCol;      // stores the col position of currentPiece[0][0] after a certain move

byte numberOfLinesCleared;
boolean disableMove;
boolean pieceDropping;
byte textColor;
boolean gameRunning;
boolean gameOver;
boolean fastDrop;

///////////////////////////////////////////
//            SMART MATRIX
///////////////////////////////////////////

const int defaultBrightness = 50*(255/100);
const int defaultScrollOffset = 10;
const rgb24 defaultBackgroundColor = {0x40, 0, 0};
const rgb24 black = {0,0,0};
// Teensy 3.0 has the LED on pin 13
const int ledPin = 13;

///////////////////////////////////////////
//            Interrupts
///////////////////////////////////////////

volatile unsigned long lastButtonPressTime = millis();
volatile unsigned long time = millis();

void checkUp() {
  time= millis();
  delay(1);
  if (digitalRead(upArrow)==HIGH && time-lastButtonPressTime>110 && disableMove==false && gameRunning==true) {
    rotate();
    lastButtonPressTime=millis();
  }
}

void startDropping() {
  time = millis();
  if (digitalRead(downArrow)==HIGH && time-lastButtonPressTime>110 && disableMove==false && gameRunning==true) {
    pieceDropping=true;
    lastButtonPressTime=millis();
  }
}

void checkDrop() {  
  time= millis();
  delay(1);
  if (digitalRead(downArrow)==HIGH && time-lastButtonPressTime>110 && disableMove==false && gameRunning==true) {
    fastDrop=true;
    lastButtonPressTime=millis();
  }
}

void checkLeft()  {
  time= millis();
  delay(1);
  if (digitalRead(leftArrow)==HIGH && time-lastButtonPressTime>110 && disableMove==false && gameRunning==true) {
    move(3);
    lastButtonPressTime=millis();
  }
}

//modified deleted && gamerunning
void checkRight() {
	time= millis();
	delay(1);
  if (digitalRead(rightArrow)==HIGH && time-lastButtonPressTime>110 && disableMove==false && gameRunning==true) {
    move(4);
    lastButtonPressTime=millis();
  }
}
volatile bool bPlayTetris = false;

void checkAButton() {  
  time= millis();
  delay(1);
  if (digitalRead(rightArrow)==HIGH && time-lastButtonPressTime>110 && bPlayTetris == false) {
	bPlayTetris = true;
    lastButtonPressTime=millis();
  }
}

void checkBButton() {  
	time= millis();
	delay(1);
	if (digitalRead(rightArrow)==HIGH && time-lastButtonPressTime>110 && bPlayTetris == false) {
		bPlayTetris = true;
		lastButtonPressTime=millis();
	}
	if(digitalRead(rightArrow)==HIGH && time-lastButtonPressTime>110 && gameRunning==false)
	{
		Serial.println("restart");
		for (int i=0;i<cols;i++) 
		{
			for (int j=0; j<rows; j++) currentMatrix[j][i]=0;
		}
	
		numberOfLinesCleared=0;
		gameRunning = true;
		gameOver = false;
		pieceDropping = false;
		disableMove = false;
		fastDrop= false;
		generatePiece();
	}
}

// the setup() method runs once, when the sketch starts
void setup() {
	// initialize the digital pin as an output.
	pinMode(ledPin, OUTPUT);
	pinMode(leftArrow, INPUT_PULLUP);
	pinMode(rightArrow, INPUT_PULLUP);
	pinMode(upArrow, INPUT_PULLUP);
	pinMode(downArrow, INPUT_PULLUP);
	pinMode(aButton, INPUT_PULLUP);
	pinMode(bButton, INPUT_PULLUP);
	
	Serial.begin(38400);
	attachInterrupt(leftArrow, &checkLeft, RISING);
	attachInterrupt(rightArrow, &checkRight, RISING);
	attachInterrupt(upArrow, &checkUp, RISING);
	attachInterrupt(downArrow, &checkDrop, RISING);
	attachInterrupt(aButton, &checkAButton, RISING);
	attachInterrupt(bButton, &checkBButton, RISING);
	
	
	matrix.begin();
	matrix.setBrightness(defaultBrightness);
	
	matrix.setScrollOffsetFromEdge(defaultScrollOffset);
	
	matrix.setColorCorrection(cc24);
	
	//initialize variables
	numberOfLinesCleared=0;
	gameRunning = false;
	gameOver = false;
	disableMove = false;
	fastDrop= false;
	generatePiece();
}

//  used in the main loop to calculate the time interval between moves
unsigned long lastMoveTime=millis();
unsigned long currentMoveTime=millis();

unsigned long lastLandedTime=millis();
unsigned long currentLandedTime=millis();

bool bscrollText = true;

extern const bitmap_font bmo;
extern const bitmap_font bmo2;
unsigned long currentMillis;
uint transitionTime = 150;
int bmoIndex = 1;
	
void drawBMO()
{
	rgb24 *buffer = matrix.backBuffer();
	if(millis() - currentMillis > transitionTime)
	{
		if(bmoIndex==1)
		{
			for (int i = 0; i < 32 * 32; i++) 
			{
				buffer[i].red = bmo.Bitmap[i * 3 + 0];
				buffer[i].green = bmo.Bitmap[i * 3 + 1];
				buffer[i].blue = bmo.Bitmap[i * 3 + 2];
			}
				
			matrix.swapBuffers(false);
			bmoIndex = 2;
			currentMillis = millis();
		}
	}
		
	if(millis() - currentMillis > transitionTime)
	{
		if(bmoIndex==2)
		{
			for (int i = 0; i < 32 * 32; i++) {
			buffer[i].red = bmo2.Bitmap[i * 3 + 0];
			buffer[i].green = bmo2.Bitmap[i * 3 + 1];
			buffer[i].blue = bmo2.Bitmap[i * 3 + 2];
		}
			
		matrix.swapBuffers(false);
		bmoIndex = 1;
		currentMillis = millis();
	}
}

void scrollText(){
delay(1000);
	//pause to prevent screen display straight after power on

	rgb24 *buffer = matrix.backBuffer();

	extern const bitmap_font bmo;
	extern const bitmap_font bmo2;
	
	
	//Scroll messages
	matrix.setScrollColor({0xff, 0xff, 0xff});
	matrix.setScrollMode(wrapForward);
	matrix.setScrollSpeed(37);
	matrix.setScrollFont(font5x7);
	matrix.setScrollOffsetFromEdge(1);
	matrix.scrollText("Hi Lauren, I'm BMO", 1);

	currentMillis = millis();
	
	while(matrix.isScrollingText())
	{
		drawBMO();
	}
	matrix.scrollText("I hear its your birthday...",1);
	while(matrix.isScrollingText())
	{
		drawBMO();	
	}
	matrix.scrollText("Happy Birthday!",1);
	while(matrix.isScrollingText())
	{
		drawBMO();
	}
	
	matrix.scrollText("Introducing the new MAV-BOX-ONE...", 1);
	while(matrix.isScrollingText())
	{
		drawBMO();
	}
	matrix.scrollText("It has the latest in flashing light technology!!", 1);
	while(matrix.isScrollingText())
	{
		drawBMO();
	}
	matrix.scrollText("It comes with the brand new game...TETRIS!!",1);
	while(matrix.isScrollingText())
	{
		drawBMO();
	}
	matrix.scrollText("(GASP)",1);
	
	for (int i = 0; i < 32 * 32; i++) 
	{
		buffer[i].red = bmo2.Bitmap[i * 3 + 0];
		buffer[i].green = bmo2.Bitmap[i * 3 + 1];
		buffer[i].blue = bmo2.Bitmap[i * 3 + 2];
	}
			
	matrix.swapBuffers(true);
	while(matrix.isScrollingText())
	{
	
	}
	matrix.scrollText("Exciting right!", 1);
	while(matrix.isScrollingText())
	{
		drawBMO();
	}
	matrix.fillScreen(black);
	bscrollText = false;
	
	//Make pretty changing text colour
	unsigned int rgbColour[3];
	
	while(bPlayTetris == false){
		
		for (int decColour = 0; decColour < 3; decColour+= 1)
		{
			int incColour = decColour == 2 ? 0 : decColour + 1;
			
			for(int i =0; i<255; i+=1)
			{
				if(bPlayTetris == true)
				{
					break;
				}
				rgbColour[decColour] -= 1;
				rgbColour[incColour] += 1;
				matrix.drawString(8,2, {rgbColour[0], rgbColour[1], rgbColour[2]}, "PLAY");
				matrix.drawString(4,10, {rgbColour[0], rgbColour[1], rgbColour[2]}, "TETRIS?");
				//Make Y and N the colour of the buttons for clarity?
				matrix.drawString(10,18, {rgbColour[0], rgbColour[1], rgbColour[2]}, "Y n");
				matrix.swapBuffers(true);
				
			}
				
		}
	}	
	matrix.fillScreen(black);
	matrix.swapBuffers(true);
	
	//initialize variables
	numberOfLinesCleared=0;
	gameRunning = true;
	gameOver = false;
	disableMove = false;
	fastDrop= false;
    generatePiece();
}

void loop(){
	///////////////////
	//		INTRO
	///////////////////
	if (bscrollText) scrollText();
	///////////////////
	//	TETRIS START
	///////////////////
	displayFrame();
	
	currentMoveTime=millis();
    if (fastDrop==true) {
      while (!checkIfLanded()) move(2);
      displayFrame();
      fastDrop=false;
    }
		
    if (digitalRead(downArrow)==LOW) pieceDropping=false;
    int timeInterval;
	//upped time decrease;
    if (!pieceDropping) timeInterval = 600-numberOfLinesCleared*25;
    else timeInterval= 120;
    if (!checkIfLanded() && currentMoveTime-lastMoveTime>timeInterval) {
      move(2);
      lastMoveTime=millis();
    }
    else if (checkIfLanded()) {
      lastLandedTime=millis();
      while (currentLandedTime<lastLandedTime+400) {
        currentLandedTime=millis();
        displayFrame();
      }
      if (!checkIfLanded()) return;
      else {
        disableMove = true;
        convertToDeadBlock();
        checkLinesCleared();
        if (generatePiece()==false) {
          Serial.println("gameover");
		  gameOverFunc();
		  displayTextOverlay(0);
        }
        else disableMove=false;
      }
    }
  }

  void gameOverFunc() {
  gameRunning = false;
  gameOver =true;
}
  
void clearTrace() {
  for (int i=0;i<4;i++) {
    for (int j=0;j<4;j++) {
      if ((currentPieceRow+j>-1 && currentPieceRow+j<rows) && (currentPieceCol+i>-1 && currentPieceCol+i<cols)) {
        if (currentMatrix[currentPieceRow+j][currentPieceCol+i]>0) currentMatrix[currentPieceRow+j][currentPieceCol+i]=0;
      }
    }
  }
}



void move(int moveDirection){  //updates: currentPieceRow, currentPieceCol, currentMatrix
  //debug
  Serial.println("move");
  
  // moveDirection: 2-down; 3-left; 4-right
  if (moveDirection==2) {
    currentPieceMovedRow=currentPieceRow+1;
    currentPieceMovedCol=currentPieceCol;
  }
  else if (moveDirection==3) {
    currentPieceMovedRow=currentPieceRow;
    currentPieceMovedCol=currentPieceCol-1;
  }
  else if (moveDirection==4) {
    currentPieceMovedRow=currentPieceRow;
    currentPieceMovedCol=currentPieceCol+1;
  }
  // check if move is valid
  if (!checkOverlapAndOutOfBoundary(currentPieceIndex)) {
    clearTrace();
    currentPieceRow=currentPieceMovedRow;
    currentPieceCol=currentPieceMovedCol;
    updateCurrentMatrix();
  }
  
}

boolean checkOverlapAndOutOfBoundary(byte pieceIndex) {
  int row=currentPieceMovedRow;
  int col=currentPieceMovedCol;
  for (int i=0;i<4;i++) {
    for (int j=0; j<4;j++) {
      if (pieces[pieceIndex][j][i]>0) {   // (row+j>-1 && row+j<rows)
        if (!( row+j<rows && (col+i>-1 && col+i<cols))) return true;  // checks out of boundary
        else if (currentMatrix[row+j][col+i]<0) return true;  // checks overlap
      }
    }
  }
  return false;
}

void rotate() {  // updates: currentPieceRotatedIndex, currentPieceIndex, currentPiece, and currentMatrix
  // obtain the currentPieceRotatedIndex
  if (currentPieceIndex==0) return;
  else if (currentPieceIndex<3) currentPieceRotatedIndex=3-currentPieceIndex;
  else if (currentPieceIndex<5) currentPieceRotatedIndex=7-currentPieceIndex;
  else if (currentPieceIndex<7) currentPieceRotatedIndex=11-currentPieceIndex;
  else if ((currentPieceIndex % 4)==2) currentPieceRotatedIndex=currentPieceIndex-3;
  else currentPieceRotatedIndex=currentPieceIndex+1;
  
  currentPieceMovedRow=currentPieceRow;
  currentPieceMovedCol=currentPieceCol;
    
  // check if the rotation is valid 
  if (!checkOverlapAndOutOfBoundary(currentPieceRotatedIndex)) {
    clearTrace();
    currentPieceIndex=currentPieceRotatedIndex;
    for (int i=0;i<4;i++) {
      for (int j=0;j<4;j++) {
        currentPiece[j][i]=pieces[currentPieceRotatedIndex][j][i];
      }
    }
    updateCurrentMatrix();
  }
}

boolean checkIfLanded() {
  for (int i=0;i<4;i++) {
    for (int j=0; j<4;j++) {
      if ((currentPieceRow+j>-1 && currentPieceRow+j<rows) && (currentPieceCol+i>-1 && currentPieceCol+i<cols)) {
        if (currentMatrix[currentPieceRow+j][currentPieceCol+i]>0) {
          if (currentPieceRow+j==bottomLimit) return true;            // if the piece has dropped on the bottom
          else if (currentMatrix[currentPieceRow+j+1][currentPieceCol+i]<0) return true;  //if there is any deadblock just below the piece
        }
      }
    }
  }
  return false;
}

boolean generatePiece() {  //updates: currentPiece, currentMatrix, currentPieceRow, currentPieceCol, currentPieceIndex
  byte pieceIndex=random(7);
  for (int i=0; i<4; i++) {
    for (int j=0; j<4; j++) {
      currentPiece[j][i]=piecesGenerated[pieceIndex][j][i];
    }
  }
  for (int i=0; i<4; i++) {
    for (int j=0; j<2; j++) {
      if (piecesGenerated[pieceIndex][j+2][i]>0) {
        if (currentMatrix[j][i+2]<0) return false;
      }
    }
  }
  for (int i=0; i<4; i++) {
    for (int j=0; j<2; j++) {
      if (piecesGenerated[pieceIndex][j+2][i]>0) {
        currentMatrix[j][i+2]=piecesGenerated[pieceIndex][j+2][i];
      }
    }
  }
  currentPieceRow = -2;
  currentPieceCol = 2;
  switch (pieceIndex) {
    case 0:
      currentPieceIndex=0;
      break;
    case 1:
      currentPieceIndex=1;
      break;
    case 2:
      currentPieceIndex=3;
      break;
    case 3:
      currentPieceIndex=5;
      break;
    case 4:
      currentPieceIndex=7;
      break;
    case 5:
      currentPieceIndex=11;
      break;
    case 6:
      currentPieceIndex=15;
      break;
  }
  return true;
}

void convertToDeadBlock() {
  for (int i=0;i<cols;i++) {
    for (int j=0;j<rows;j++) {
      if (currentMatrix[j][i]>0) {
        currentMatrix[j][i]=-currentMatrix[j][i];
      }
    }
  }
}

void checkLinesCleared() {
  byte lineErased[rows];
  byte rowCounter=0;
  for (int j=0;j<rows;j++) {
    byte colCounter=0;
    for (int i=0;i<cols;i++) {
      if (currentMatrix[j][i]==0) {
        lineErased[j]=0;
        break;
      }
      else colCounter+=1;
    }
    if (colCounter==8) {
      lineErased[j]=1;
      rowCounter+=1;
    }  
  }
  
  if (rowCounter==0) return;  // if no lines are cleared
  
  int tempMatrix[rows-rowCounter][cols];
  byte rowCounter2=0;
  for (int j=0;j<rows;j++) {
    if (lineErased[j]==0) {
      for (int i=0;i<cols;i++) tempMatrix[rowCounter2][i]=currentMatrix[j][i]+0;
      rowCounter2+=1;
    }
  }
  for (int j=0;j<rowCounter;j++) {
    for (int i=0;i<cols;i++) {
      currentMatrix[j][i]=0;
    }
  }
  for (int j=0;j<rows-rowCounter;j++) {
    for (int i=0;i<cols;i++) {
      currentMatrix[j+rowCounter][i]=tempMatrix[j][i];
    }
  }
  numberOfLinesCleared+=rowCounter;
  //DEBUG
  Serial.println(numberOfLinesCleared,DEC);
}


void updateCurrentMatrix() {
  for (int i=0;i<4;i++) {
    for (int j=0;j<4;j++) {
      if ((currentPieceRow+j>-1 && currentPieceRow+j<rows) && (currentPieceCol+i>-1 && currentPieceCol+i<cols)) {
        if (currentPiece[j][i]!=0) currentMatrix[currentPieceRow+j][currentPieceCol+i]=currentPiece[j][i];
      }
    }
  }
}

void displayFrame()	{
	rgb24 colour;
	int c;
	//draw border
	matrix.drawRectangle(11, 14, 20, 31, {27,121,226});
	//draw current score....look at having leaderboard?
	matrix.setFont(font3x5);
	matrix.drawString(0,6,{0xff,0xff,0xff},"SCORE: ");
	
	matrix.fillRectangle(24, 6, 32, 11, {0,0,0});
	char temp[16];
	String score  = numberOfLinesCleared;
	score.toCharArray(temp,16);
	matrix.drawString(24,6,{0xff,0xff,0xff}, temp);
	for(int i=0; i<8; i++) {
		for(int j=0; j<16; j++){
			
			//offset draw function to middle of screen
			if (currentMatrix[j][i]==0){
				matrix.drawPixel(i+12,j+15,{0,0,0});
			}
			else{
				if(currentMatrix[j][i] < 0) c=-currentMatrix[j][i];
				else c = currentMatrix[j][i];
				
				switch (c) 
				{
					case(1):
					matrix.drawPixel(i+12,j+15,{0xff,0,0});
					break;
					
					case(2):
					matrix.drawPixel(i+12,j+15,{255,128,198});
					break;
					
					case(3):
					matrix.drawPixel(i+12,j+15,{0,0xff,0});
					break;
					
					case(4):
					matrix.drawPixel(i+12,j+15,{0,0xff,0xff});
					break;
					
					case(5):
					matrix.drawPixel(i+12,j+15,{216,248,33});
					break;
					
					case(6):
					matrix.drawPixel(i+12,j+15,{220,140,61});
					break;
					
					case(7):
					matrix.drawPixel(i+12,j+15,{222,54,226});
					break;
					}
				//matrix.drawPixel(i+12,j+15,{0xff,0,0});
			}
		}
	}
	
	matrix.swapBuffers(true);
}

void displayTextOverlay(int index) {  // i=0: GAMEOVER;
	matrix.setScrollColor({0xff, 0, 0});
	matrix.setScrollMode(bounceForward);
	matrix.setScrollSpeed(40);
	matrix.setScrollFont(font3x5);
	matrix.setScrollOffsetFromEdge(0);
	while(gameOver){
		matrix.scrollText("GAME OVER	PLAY AGAIN?", 1);
		while(matrix.isScrollingText())
		{
			if(!gameOver) break;
		}
	}
	
	matrix.setScrollOffsetFromEdge(32);
	matrix.setScrollSpeed(100);
		
	for(int ii =0; ii<32; ii++)
	{ 
		matrix.drawFastHLine(0, 32, ii, {0xff,0,0});
		matrix.swapBuffers(true);
	}
	for(int ii =0; ii<32; ii++)
	{
		matrix.drawFastHLine(0, 32, ii, {0,0,0});
		matrix.swapBuffers(true);
	}
}
