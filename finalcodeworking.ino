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

void generate_and_show_sequence() {
  for (int i = 0; i < level; i++) {
    int ledIndex = random(0, numPins); // Randomize the LED index
    sequence[i] = myLedPins[ledIndex];
  }
  for (int i = 0; i < level; i++) {
    digitalWrite(sequence[i], HIGH);
    delay(lightDelay);
    digitalWrite(sequence[i], LOW);
    delay(1000);
  }
}

void wrong_sequence() {
  for (int j = 0; j < 5; j++) {
    for (int i = 0; i < numPins; i++) {
      digitalWrite(myLedPins[i], HIGH);
    }
    delay(200);
    for (int i = 0; i < numPins; i++) {
      digitalWrite(myLedPins[i], LOW);
    }
    delay(200);
  }
  endGame();
}

void right_sequence() {
  for (int j = 0; j < 1; j++) {
    for (int i = 0; i < numPins; i++) {
      digitalWrite(myLedPins[i], HIGH);
    }
    delay(200);
    for (int i = 0; i < numPins; i++) {
      digitalWrite(myLedPins[i], LOW);
    }
    delay(200);
  }
  if (level < maxLevel) {
    level++;
  }
  // Update light delay for speed mode
  if (currentMode == 2) {
    lightDelay -= 2000;
  }
}

void get_user_sequence() {
  int buttonPressed;
  
  for (int i = 0; i < level; i++) {
    buttonPressed = waitForButtonPress();
    if (buttonPressed == -1 || buttonPressed != sequence[i]) {
      wrong_sequence();
      return;
    }
  }
  right_sequence();
}

int waitForButtonPress() {
  int buttonPressed = -1;
  while (buttonPressed == -1) {
    for (int j = 0; j < numPins; j++) {
      if (digitalRead(myButtonPins[j]) == HIGH) {
        buttonPressed = myLedPins[j];
        digitalWrite(buttonPressed, HIGH); // Turn on corresponding LED
        delay(500); // Wait for half a second
        digitalWrite(buttonPressed, LOW); // Turn off LED
        return buttonPressed;
      }
    }
  }
  return buttonPressed;
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
          generate_and_show_sequence();
          get_user_sequence();
        } else {
          beatGame();
        }
        break;
      case 1: // Reverse mode
        if (level < maxLevel) {
          generate_and_show_sequence();
          get_user_sequence();
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