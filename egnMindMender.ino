//Initializing variables
//#define playerWaitTime 7000

const int maxLevel  = 10;
int sequence[maxLevel];
int your_sequence[maxLevel];

int level  = 1;
int lightDelay = 1000;

const int myButtonPins[] = {2, 4, 6, 8, 10};
const int myLedPins[] = {3, 5, 7, 9, 11};
int numPins = 5;
int expectedLed = 0;
bool buttonPressed = false;
int inputCount = 0;
int buttonPresses = 0;
int buttonState = 0;

bool gameInProgress = false;
bool waiting = false;
int ledIndex = 0;
unsigned long intputTime = 0;

//Game mode variables
const int numModes = 3;
int currentMode = 0;

//setting the LEDs to Output and buttons to Input
void setup() {
  delay(2000);
  for (int i = 0; i < 5; i++){
    pinMode(myLedPins[i], OUTPUT);
    digitalWrite(myLedPins[i], LOW);
  }
  for (int i = 0; i < 5; i++){
    pinMode(myButtonPins[i], INPUT_PULLUP);
  }
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  randomSeed(analogRead(0));
}

//Start the game if start button is pressed
void startGame(){
  gameInProgress = true;
}

//end the game if the start button is pressed again
void endGame(){
  gameInProgress = false;
}

void beatGame(){
  for(int j = 0; j < 5; j++){
  delay(1000);

  digitalWrite(3, LOW);
  digitalWrite(5, LOW);
  digitalWrite(7, LOW);
  digitalWrite(9, LOW);
  digitalWrite(11, LOW);

  for(int i = 0; i < 5; i++){
    digitalWrite(myLedPins[i], HIGH);
  }

  delay(1000);
  
  digitalWrite(3, LOW);
  digitalWrite(5, LOW);
  digitalWrite(7, LOW);
  digitalWrite(9, LOW);
  digitalWrite(11, LOW);

  delay(200);
  }
  endGame();
}

//Generate a random sequence of LED lights depending on the level
void generate_and_show_sequence() {  

  digitalWrite(3, LOW);
  digitalWrite(5, LOW);
  digitalWrite(7, LOW);
  digitalWrite(9, LOW);
  digitalWrite(11, LOW);

  for (int i = 0; i < level; i++) {
    ledIndex = random(0, 5);
    sequence[i] = myLedPins[ledIndex];
  }
  for (int i = 0; i < level; i++){
    digitalWrite(sequence[i], HIGH);
    delay(lightDelay);
    digitalWrite(sequence[i], LOW);
    delay(1000);
  }
    
}

//if the user inputs the wrong sequence of numbers, reset to level 1
void wrong_sequence(){
  for(int i = 0; i < 3; i++){
    digitalWrite(3, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(11, HIGH);

    delay(200);
    
    digitalWrite(3, LOW);
    digitalWrite(5, LOW);
    digitalWrite(7, LOW);
    digitalWrite(9, LOW);
    digitalWrite(11, LOW);
    
    delay(200);
  }
  level = 1;
  lightDelay = 1000;
}

//if the user inputs the right sequence, go to next level
void right_sequence(){
  delay(300);

  digitalWrite(3, LOW);
  digitalWrite(5, LOW);
  digitalWrite(7, LOW);
  digitalWrite(9, LOW);
  digitalWrite(11, LOW);

  for(int i = 0; i < 5; i++){
    digitalWrite(myLedPins[i], HIGH);
  }

  delay(200);
  
  digitalWrite(3, LOW);
  digitalWrite(5, LOW);
  digitalWrite(7, LOW);
  digitalWrite(9, LOW);
  digitalWrite(11, LOW);

  delay(200);

  if (level < maxLevel){
    level++;
  }

  //lightDelay -= 50;
  delay(500);
}

//get the user input, check if its right or wrong
void get_user_sequence() {
  int correctseq = 0;

  for (int i = 0; i < level; i++) {
    correctseq = 0; // Reset flag for each new button press
    while (!correctseq) {
      if (digitalRead(2) == HIGH) {
        digitalWrite(3, HIGH);
        your_sequence[i] = 3;
        correctseq = 1; // Set flag to exit the loop
      } else if (digitalRead(4) == HIGH) {
        digitalWrite(5, HIGH);
        your_sequence[i] = 5;
        correctseq = 1;
      } else if (digitalRead(6) == HIGH) {
        digitalWrite(7, HIGH);
        your_sequence[i] = 7;
        correctseq = 1;
      } else if (digitalRead(8) == HIGH) {
        digitalWrite(9, HIGH);
        your_sequence[i] = 9;
        correctseq = 1;
      } else if (digitalRead(10) == HIGH) {
        digitalWrite(11, HIGH);
        your_sequence[i] = 11;
        correctseq = 1;
      }
      delay(100); // Delay for debounce
      digitalWrite(3, LOW);
      digitalWrite(5, LOW);
      digitalWrite(7, LOW);
      digitalWrite(9, LOW);
      digitalWrite(11, LOW);
    }
    if (your_sequence[i] != sequence[i]) {
      wrong_sequence();
      return;
    }
  }
  right_sequence();
}
void loop() {
  buttonState = digitalRead(12);
  
  if (buttonState == HIGH) {
    buttonPresses++;
    delay(100); // Delay for debouncing
  }
  
  boolean odd = buttonPresses % 2;
  
  if (odd && !gameInProgress) {
    startGame();
  } else if (!odd && gameInProgress) {
    endGame();
  }
  
  if (gameInProgress) {
    if (level < maxLevel) {
      generate_and_show_sequence();
      get_user_sequence();
    } else {
      beatGame();
    }
  }
}

// Reverse Mode: In this mode, instead of the sequence being shown to the player, 
// they have to memorize the sequence as it's played and then reproduce it in reverse order.
//  For example, if the sequence is 1-2-3-4, the player would have to input 4-3-2-1.

// Speed Mode: In this mode, the speed at which the sequence is played increases with each level, 
// requiring the player to react more quickly.