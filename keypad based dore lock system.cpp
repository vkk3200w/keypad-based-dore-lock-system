#include <Keypad.h>

#include <LiquidCrystal.h>
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

#include <Servo.h>
Servo myservo;

#define Password_Length 7 // Give enough room for six chars + NULL char

int pos = 0;    // variable to store the servo position

char Data[Password_Length]; // 6 is the number of chars it can hold + the null char = 7

char Master[Password_Length] = "123456";

byte data_count = 0, master_count = 0;

bool Pass_is_good;

char customKey;

const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

bool door = true;

byte rowPins[ROWS] = {12, 10,9, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 6, 5}; //connect to the column pinouts of the keypad

Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); //initialize an instance of class NewKeypad

#define buzzer 2

int i = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(buzzer, OUTPUT);
    digitalWrite(buzzer, LOW);
   
  myservo.attach(3);

  ServoClose();
 
  lcd.begin(16, 2);
 
  lcd.setCursor(0, 0);
  lcd.print("  Keypad Based  ");
  lcd.setCursor(0, 1);
  lcd.print("   Door Lock    ");

  delay(2000);

  lcd.clear();
  delay(100);
}

void loop()
{
  if (door == 0)
  {
    customKey = customKeypad.getKey();

    if (customKey == '#')
    {
      lcd.clear();

      ServoClose();

      lcd.setCursor(0, 0);
      lcd.print(" Door is closed ");

      delay(3000);

      door = 1;
    }
  }
  else Open();
}

void clearData()
{
  while (data_count != 0)
  { // This can be used for any array size,
    Data[data_count--] = 0; //clear array for new data
  }
  return;
}

void ServoOpen()
{
  for (pos = 180; pos >= 0; pos -= 5) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void ServoClose()
{
  for (pos = 0; pos <= 180; pos += 5) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void Open()
{
  lcd.setCursor(0, 0);
  lcd.print(" Enter Password");
 
  customKey = customKeypad.getKey();

  if (customKey) // makes sure a key is actually pressed, equal to (customKey != NO_KEY)
  {
    Data[data_count] = customKey; // store char into data array
   
    lcd.setCursor(data_count, 1); // move cursor to show each new char
    lcd.print(Data[data_count]); // print char at said cursor

    data_count++; // increment data array by 1 to store new char, also keep track of the number of chars entered
  }

  if (data_count == Password_Length - 1) // if the array index is equal to the number of expected chars, compare data to master
  {
    if (!strcmp(Data, Master)) // equal to (strcmp(Data, Master) == 0)
    {
     
      lcd.clear();

      ServoOpen();

      lcd.setCursor(0, 0);
      lcd.print("  Door is Open  ");

      door = 0;
    }
   
    else
    {
      i++;
     
      digitalWrite(buzzer, HIGH);
     
      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print(" Wrong Password ");
     
      delay(2000);

      digitalWrite(buzzer, LOW);

      door = 1;
     
      if(i >= 3)
      {
        lcd.setCursor(0, 1);
        lcd.print("  Door Locked   ");
       
        while(1);
      }
    }
   
    clearData();
  }
}
BD_DoorLockSystem.png