#include <Bounce2.h>
#include <LedControl.h>

const int buttonPin1 = 9;
const int buttonPin2 = 8;
const int buttonPin3 = 7;
const int buttonPin4 = 6;
const int buttonPin5 = 5;
const int buttonPin6 = 4;
const int buttonPin7 = 3;
const int buttonPin8 = 2;
const int buttonPin10p = 14;
const int buttonPinm5p = 15;
const int buttonPin5p = 16;
const int buttonPinNQ = 17;

const int scoreLatchPin = 10;
const int scoreDataPin = 11;
const int scoreClockPin = 13;

const int ledLatchPin = 1;
const int ledDataPin = 18;
const int ledClockPin = 19;

bool answered = false;
int activePlayer = 0;
int team1Score = 0;
int team2Score = 0;
int debounceDelay = 20;
float timer = 0;

LedControl lsScore=LedControl(scoreDataPin, scoreClockPin, scoreLatchPin, 1);

Bounce2::Button button1 = Bounce2::Button();
Bounce2::Button button2 = Bounce2::Button();
Bounce2::Button button3 = Bounce2::Button();
Bounce2::Button button4 = Bounce2::Button();
Bounce2::Button button5 = Bounce2::Button();
Bounce2::Button button6 = Bounce2::Button();
Bounce2::Button button7 = Bounce2::Button();
Bounce2::Button button8 = Bounce2::Button();
Bounce2::Button button10p = Bounce2::Button();
Bounce2::Button button5p = Bounce2::Button();
Bounce2::Button buttonm5p = Bounce2::Button();
Bounce2::Button buttonNQ = Bounce2::Button();

void setup() {
  pinMode(scoreLatchPin, OUTPUT);
  pinMode(scoreDataPin, OUTPUT);
  pinMode(scoreClockPin, OUTPUT);
  pinMode(ledLatchPin, OUTPUT);
  pinMode(ledDataPin, OUTPUT);
  pinMode(ledClockPin, OUTPUT);

  updateLights();

  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lsScore.shutdown(0,false);
  /* Set the brightness to a medium values */
  lsScore.setIntensity(0,8);
  /* and clear the display */
  lsScore.clearDisplay(0);

  updateScoreDisplay();
  
  /*
   Set up the button objects.
   8 player buttons using external pull-down resistors
   */
  button1.attach(buttonPin1, INPUT_PULLUP);
  button1.interval(debounceDelay);
  button1.setPressedState(LOW);

  button2.attach(buttonPin2, INPUT_PULLUP);
  button2.interval(debounceDelay);
  button2.setPressedState(LOW);

  button3.attach(buttonPin3, INPUT_PULLUP);
  button3.interval(debounceDelay);
  button3.setPressedState(LOW);

  button4.attach(buttonPin4, INPUT_PULLUP);
  button4.interval(debounceDelay);
  button4.setPressedState(LOW);

  button5.attach(buttonPin5, INPUT_PULLUP);
  button5.interval(debounceDelay);
  button5.setPressedState(LOW);

  button6.attach(buttonPin6, INPUT_PULLUP);
  button6.interval(debounceDelay);
  button6.setPressedState(LOW);

  button7.attach(buttonPin7, INPUT_PULLUP);
  button7.interval(debounceDelay);
  button7.setPressedState(LOW);

  button8.attach(buttonPin8, INPUT_PULLUP);
  button8.interval(debounceDelay);
  button8.setPressedState(LOW);

  /*
   Set up the score increment and next question buttons.
   4 buttons using the internal pull-up resistor
  */
  button10p.attach(buttonPin10p, INPUT_PULLUP);
  button10p.interval(debounceDelay);
  button10p.setPressedState(LOW);

  button5p.attach(buttonPin5p, INPUT_PULLUP);
  button5p.interval(debounceDelay);
  button5p.setPressedState(LOW);

  buttonm5p.attach(buttonPinm5p, INPUT_PULLUP);
  buttonm5p.interval(debounceDelay);
  buttonm5p.setPressedState(LOW);

  buttonNQ.attach(buttonPinNQ, INPUT_PULLUP);
  buttonNQ.interval(debounceDelay);
  buttonNQ.setPressedState(LOW);

}

void loop() {
  if (answered) {
    changeScore();
    checkNextQuestionButton();
  } else {
    checkPlayerButtons();
  }
  updateLights();
}


void checkPlayerButtons() {
  button1.update();
  button2.update();
  button3.update();
  button4.update();
  button5.update();
  button6.update();
  button7.update();
  button8.update();
  if ( button1.pressed() ) {
    answered = true;
    activePlayer = 1;
    return;
  } else if ( button2.pressed() ) {
    answered = true;
    activePlayer = 2;
    return;
  } else if ( button3.pressed() ) {
    answered = true;
    activePlayer = 3;
    return;
  } else if ( button4.pressed() ) {
    answered = true;
    activePlayer = 4;
    return;
  } else if ( button5.pressed() ) {
    answered = true;
    activePlayer = 5;
    return;
  } else if ( button6.pressed() ) {
    answered = true;
    activePlayer = 6;
    return;
  } else if ( button7.pressed() ) {
    answered = true;
    activePlayer = 7;
    return;
  } else if ( button8.pressed() ) {
    answered = true;
    activePlayer = 8;
    return;
  } 
}

void checkNextQuestionButton() {
  buttonNQ.update();
  
  if (buttonNQ.pressed()) {
    answered = false;
    activePlayer = 0;
  }
}

void changeScore() {
  button10p.update();
  button5p.update();
  buttonm5p.update();
  int deltaScore = 0;
  if (button10p.pressed()) {
    deltaScore = 10;
  } else if (button5p.pressed()) {
    deltaScore = 5;
  } else if (buttonm5p.pressed()) {
    deltaScore = -5;
  }
  if (deltaScore != 0) {
    switch (activePlayer) {
      case 1:
      case 2:
      case 3:
      case 4:
        team1Score += deltaScore;
        updateScoreDisplay();
        break;
      case 5:
      case 6:
      case 7:
      case 8:
        team2Score += deltaScore;
        updateScoreDisplay();
        break;
      default:
        return;
    }
  }
}

void displayNumber3(int value, int startDigit) {
  // Convert to string
  char buf[5];
  sprintf(buf, "%4d", value);   // produces right‑aligned: "  45", " 123", " -45"
  // Write characters into 3 digits
  for (int i = 0; i < 3; i++) {
    char c = buf[1 + i];  // use last 3 positions
    int digit = startDigit - (2 - i);
    
    if (c == ' ') {
      lsScore.setChar(0, digit, ' ', false);
    } else if (c == '-') {
      // Display minus sign
      lsScore.setChar(0, digit, '-', false);
    } else {
      lsScore.setDigit(0, digit, c - '0', false);
    }
  }
}

void updateScoreDisplay() {
  displayNumber3(team1Score, 2);
  displayNumber3(team2Score, 5);
}

void updateLights() {
  if (!answered) {
    digitalWrite(ledLatchPin, LOW);
    shiftOut(ledDataPin, ledClockPin, MSBFIRST, 0b00000000);
    shiftOut(ledDataPin, ledClockPin, MSBFIRST, 0b00000000);
    shiftOut(ledDataPin, ledClockPin, MSBFIRST, 0b00000000);
    digitalWrite(ledLatchPin, HIGH);
  } else {
    digitalWrite(ledLatchPin, LOW);
    shiftOut(ledDataPin, ledClockPin, MSBFIRST, 1 << (activePlayer - 1));
    shiftOut(ledDataPin, ledClockPin, MSBFIRST, 1 << (activePlayer - 1));
    shiftOut(ledDataPin, ledClockPin, MSBFIRST, 1 << (activePlayer < 5 ? 0 : 1));
    digitalWrite(ledLatchPin, HIGH);
  }
}

