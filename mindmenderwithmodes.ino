
//Initializing variables
const int maxLevel = 10;
int sequence[maxLevel];
int your_sequence[maxLevel];
int reverse_sequence[maxLevel];

int level = 1;
int lightDelay = 1000;

const int myButtonPins[] = {2, 4, 6, 8, 10};
const int myLedPins[] = {3, 5, 7, 9, 11};
int numPins = 5;
int buttonState1 = 0;
int buttonState2 = 0;
int buttonPresses = 0;

bool gameInProgress = false;
int ledIndex = 0;

// Game mode variables
const int numModes = 3;
int currentMode = 0; // 0: Simon mode, 1: Reverse mode, 2: Speed mode

void setup() {
  delay(2000);
  for (int i = 0; i < numPins; i++) {
    pinMode(myLedPins[i], OUTPUT);
    digitalWrite(myLedPins[i], LOW);
    pinMode(myButtonPins[i], INPUT_PULLUP);
  }
  pinMode(12, INPUT_PULLUP); // Start/Stop button
  pinMode(13, INPUT_PULLUP); // Mode switch button
  randomSeed(analogRead(0));
}

void startGame() {
  gameInProgress = true;
}

void endGame() {
  gameInProgress = false;
  level = 1;
  lightDelay = 1000;
}

void beatGame() {
  endGame();
}
void generate_and_show_sequence(int level) {  
  int ledIndex; // Declare ledIndex within the function

  for (int i = 0; i < level; i++) {
    ledIndex = random(0, numPins);
    sequence[i] = myLedPins[ledIndex];
  }
  for (int i = 0; i < level; i++){
    digitalWrite(sequence[i], HIGH);
    delay(lightDelay);
    digitalWrite(sequence[i], LOW);
    delay(1000);
  }
}
void wrong_sequence() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < numPins; j++) {
      digitalWrite(myLedPins[j], HIGH);
    }
    delay(200);
    for (int j = 0; j < numPins; j++) {
      digitalWrite(myLedPins[j], LOW);
    }
    delay(200);
  }
  level = 1;
  lightDelay = 1000;
}

void right_sequence() {
  delay(300);
  for (int i = 0; i < numPins; i++) {
    digitalWrite(myLedPins[i], HIGH);
  }
  delay(200);
  for (int i = 0; i < numPins; i++) {
    digitalWrite(myLedPins[i], LOW);
  }
  delay(200);
  if (level < maxLevel) {
    level++;
  }
  // Update light delay for speed mode
  if (currentMode == 2) {
    lightDelay -= 2000;
  }
}

void get_user_sequence(int level) {
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
void reverse(int arr[], int count) {
  int temp;
  for (int i = 0; i < count / 2; ++i) {
    temp = arr[i];
    arr[i] = arr[count - i - 1];
    arr[count - i - 1] = temp;
  }
}

void get_user_reverse_sequence() {
  int correctseq = 0;
  for (int i = 0; i < level; i++) {
    correctseq = 0;
    while (!correctseq) {
      for (int j = 0; j < numPins; j++) {
        if (digitalRead(myButtonPins[j]) == HIGH) {
          digitalWrite(myLedPins[j], HIGH);
          your_sequence[i] = myLedPins[j];
          delay(200); // Debounce delay
          correctseq = 1;
        }
      }
    }
    digitalWrite(myLedPins[your_sequence[i]], LOW);
    if (your_sequence[i] != reverse_sequence[i]) {
      wrong_sequence();
      return;
    }
  }
  right_sequence();
}
void modeButtonPressed() {
  static unsigned long lastDebounceTime = 0;
  static bool lastButtonState = LOW;
  unsigned long currentTime = millis();
  
  // Check for button debounce
  if (currentTime - lastDebounceTime < 50) {
    return;
  }
  
  // Read the button state
  bool buttonState = digitalRead(13);
  
  // Check if the button state has changed
  if (buttonState != lastButtonState) {
    // Update last debounce time
    lastDebounceTime = currentTime;
    
    // Update last button state
    lastButtonState = buttonState;
    
    // If button is pressed, switch to the next game mode
    if (buttonState == HIGH) {
      currentMode = (currentMode + 1) % numModes;
    }
  }
}
void loop() {
  buttonState1 = digitalRead(12); // Button for on/off
  buttonState2 = digitalRead(13); // Button for mode switching
  
  if (buttonState1 == HIGH) {
    buttonPresses++;
    delay(100); // Delay for debouncing
  }
  
  boolean odd = buttonPresses % 2;
  
  if (odd && !gameInProgress) {
    startGame();
  } else if (!odd && gameInProgress) {
    endGame();
  }
  
  // Check if the game is in progress
  if (gameInProgress) {
    // Check for mode switching button press
    if (buttonState2 == HIGH) {
      modeButtonPressed();
    }
    
    // Execute logic based on the current game mode
    switch (currentMode) {
      case 0: // Simon mode
        if (level < maxLevel) {
          generate_and_show_sequence(level);
          get_user_sequence(level);
        } else {
          beatGame();
        }
        break;
      case 1: // Reverse mode
        if (level < maxLevel) {
          generate_and_show_sequence(level);
          get_user_sequence(level);
        } else {
          beatGame();
        }
        break;
      case 2: // Speed mode
        // Implement speed mode logic here
        break;
    }
  }
}
