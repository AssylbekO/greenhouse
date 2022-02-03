float temp_internal;    // Stores internal temperature value
float hum_internal;     // Stores internal humidity value
float temp_external;    // Stores external temperature value
float hum_external;     // Stores external humidity value
int state;              // Condition of pins
int valMoisture = 0;    // Stores Moisture value
int kath = 5000;        // Delay
int LimM = 1023 ;       // Stores value after edit
int LimT = 25 ;         // Stores value after edit
int manytimes = 1;
int total;

int MoisturePin = A0;   // Moisture Analog pin
int potention1 = A2;    // Potentiometer to change Temperature Limit
int potention2 = A1;    // Potentiometer to change Moisture Limit
int Relay1_AirIn = 4;   // airRelay1 Blower Air Fan IN
int Relay2_AirOut = 5;  // airRelay2 Blower Air Fan OUT
int Relay4_Water = 7;   // Relay4_Water Pump
int soilPower = 2;      // Moisture Soil power

void setup() {

  Serial.begin(9600);

  dht21.begin();  // DHT21 Starts 
  tft.initR(INITR_BLACKTAB);  // Initialize Iduino 1.8 TFT LCD 128 X 160
  tft.fillScreen(BLACK);
  tft.setRotation(0);

  // Initialize Sensor Moisture
  pinMode(soilPower, OUTPUT);   // Set D7 as an OUTPUT
  digitalWrite(soilPower, LOW); // Set to LOW, no power in the sensor

  // Initialize Relays
  pinMode(potention1, INPUT);
  pinMode(potention2, INPUT);
  pinMode(Relay1_AirIn, OUTPUT);
  pinMode(Relay2_AirOut, OUTPUT);
  pinMode(Relay4_Water, OUTPUT);

  digitalWrite(Relay1_AirIn, LOW);
  digitalWrite(Relay2_AirOut, LOW);
  digitalWrite(Relay3_Lamp, LOW);
  digitalWrite(Relay4_Water, LOW);
}

void loop() {

  // This Function send power and then reads from Moisture Sensor
  int readSoil() ;
  digitalWrite(soilPower, HIGH); // turn D2 "On"
  delay(10); // wait 10 milliseconds giving time to Moisture Sensor
  // 3. Read the SIG value (data) from the sensor and counted percent degrees
  valMoisture = (analogRead(MoisturePin) - 23) / 10;
  digitalWrite(soilPower, LOW); // turn power on D2 "Off"

  // 4. Read from potentiometers limit of Temperature and Humidity
  LimT = (analogRead(potention1) - 23) / 10; // Read limit from Potentiometer
  LimM = (analogRead(potention2) - 21) / 10; // Read limit from Potentiometer

  // ***************************************************************************

  // Print readed values of External and Internal Temperature
  tft.fillRect(0, 0, 64, 20, RED);
  printText1(temp_external, WHITE, 2, 2, 2);
  tft.fillRect(64, 0, 64, 20, MAGENTA);
  printText1(temp_internal, WHITE, 66, 2, 2);
  tft.fillRect(43, 20, 45, 20, CYAN);
  printText("EXT", WHITE, 2, 23, 2);
  tft.setCursor(50, 23);
  tft.setTextColor(BLACK);
  tft.print(LimT);
  printText("INT", WHITE, 90, 23, 2);

  // ***************************************************************************

  // Print readed values of Internal and Internal Humidity
  tft.fillRect(0, 40, 64, 20, VIOLET);
  printText1(hum_external, WHITE, 2, 43, 2);
  tft.fillRect(64, 40, 64, 20, YELLOW);
  printText1(hum_internal, BLACK, 66, 43, 2);
  tft.fillRect(58, 60, 128, 20, CYAN);
  printText("LimM", WHITE, 2, 63, 2);
  tft.setCursor(65,63);
  tft.setTextColor(BLACK);
  tft.print(LimM);
  tft.print(" %");

  // ***************************************************************************
  // Print on Serial Window readed values
  Serial.print("Counter = ");
  Serial.println(manytimes);
  Serial.print("TOTAL = ");
  Serial.println(total);
  state = digitalRead(Relay1_AirIn);
  Serial.print("Air In = ");
  Serial.println(state);
  state = digitalRead(Relay2_AirOut);
  Serial.print("Air Out = ");
  Serial.println(state);
  state = digitalRead(Relay3_Lamp);
  Serial.print("Lamp = ");
  Serial.println(state);
  state = digitalRead(Relay4_Water);
  Serial.print("Water = ");
  Serial.println(state);
  Serial.println("");

  // ***************************************************************************
  // 1. Check limit value for Moisture Soil Sensor and control Relay4_Water
  Serial.print("Soil: ");
  Serial.println(valMoisture);
  printText("SOIL", WHITE, 2, 83, 2);
  printText("WATER", WHITE, 5, 143, 2);
  if (valMoisture >= LimM) {
    digitalWrite(Relay4_Water, HIGH);
    tft.fillRect(58, 80, 128, 20, AQUA);
    printText1(valMoisture, WHITE, 65, 83, 2);
    tft.fillRect(80, 140, 128, 20, AQUA);
    printText("ON", WHITE, 85, 143, 2);
  }
  else {
    digitalWrite(Relay4_Water, LOW);
    tft.fillRect(58, 80, 128, 20, BLUE);
    printText1(valMoisture, WHITE, 65, 83, 2);
    tft.fillRect(80, 140, 128, 20, RED);
    printText("OFF", WHITE, 85, 143, 2);
  }

  // ***************************************************************************
  printText("AIR ", WHITE, 2, 103, 2);
  printText("LAMP", WHITE, 5, 123, 2);

  // 2. Check greenhouse's Internal Condition and control Relays of Fans 
    if (temp_internal > temp_external) {
      Serial.println("1. t_int >= LimT & t_int > t_ext");
      Serial.print(temp_internal);
      Serial.print(" >= ");
      Serial.print(LimT);
      Serial.print(" & > ");
      Serial.print(temp_external);
     

      tft.fillRect(40, 100, 42, 20, GREEN);
      printText("IN ", BLACK, 45, 103, 2);
      state = digitalRead(Relay1_AirIn);
      if (state == 1) {
        // do nothing
      }
      else {
        digitalWrite(Relay1_AirIn, HIGH);
      }

      tft.fillRect(80, 100, 128, 20, GREEN);
      printText("OUT", BLACK, 85, 103, 2);
      state = digitalRead(Relay2_AirOut);
      if (state == 1) {
        // do nothing
      }
      else {
        digitalWrite(Relay2_AirOut, HIGH);
      }

      tft.fillRect(80, 120, 128, 20, RED);
      printText("OFF", WHITE, 85, 123, 2);
      state = digitalRead(Relay3_Lamp);
      if (state == 0) {
        // do nothing
      }
      else {
        digitalWrite(Relay3_Lamp, LOW);
      }
    }
    else {
      Serial.println("else 1. t_int >= LimT & t_int < t_ext");
      Serial.print(temp_external);
      Serial.print(" > ");
      Serial.print(temp_internal);
      Serial.print(" >= ");
      Serial.print(LimT);
      Serial.println("   AirIn Off - AirOut On - Lamp Off");

      tft.fillRect(40, 100, 42, 20, RED);
      printText("IN ", WHITE, 45, 103, 2);
      state = digitalRead(Relay1_AirIn);
      if (state == 0) {
        // do nothing
      }
      else {
        digitalWrite(Relay1_AirIn, LOW);
      }
      tft.fillRect(80, 100, 128, 20, GREEN);
      printText("OUT", BLACK, 85, 103, 2);
      state = digitalRead(Relay2_AirOut);
      if (state == 1) {
        // do nothing
      }
      else {
        digitalWrite(Relay2_AirOut, HIGH);
      }

      tft.fillRect(80, 120, 128, 20, RED);
      printText("OFF", WHITE, 85, 123, 2);
      state = digitalRead(Relay3_Lamp);
      if (state == 0) {
        // do nothing
      }
      else {
        digitalWrite(Relay3_Lamp, LOW);
      }
    }
  }

  if (temp_internal < LimT) {
    if (temp_internal <= temp_external) {
      Serial.println("2. t_int < LimT & t_int >= t_ext");
      Serial.print(temp_external);
      Serial.print(" <= ");
      Serial.print(temp_internal);
      Serial.print(" < ");
      Serial.print(LimT);
      Serial.println("   AirIn ON - AirOut Off - Lamp On");

      tft.fillRect(40, 100, 42, 20, GREEN);
      printText("IN", BLACK, 50, 103, 2);
      if (state == 1) {
        // do nothing
      }
      else {
        digitalWrite(Relay1_AirIn, HIGH);
      }

      tft.fillRect(80, 100, 128, 20, RED);
      printText("OUT", WHITE, 85, 103, 2);
      state = digitalRead(Relay2_AirOut);
      if (state == 0) {
        // do nothing
      }
      else {
        digitalWrite(Relay2_AirOut, LOW);
      }

      tft.fillRect(80, 120, 128, 20, GREEN);
      printText("ON", BLACK, 85, 123, 2);
      state = digitalRead(Relay3_Lamp);
      if (state == 1) {
        // do nothing
      }
      else {
        digitalWrite(Relay3_Lamp, HIGH);
      }
    }
    else {
      Serial.println("else 2. t_int < LimT & t_int < t_ext");
      Serial.print(temp_internal);
      Serial.print(" < ");
      Serial.print(LimT);
      Serial.print(" & ");
      Serial.print(temp_external);
      Serial.println("AirIn OFF - AirOut Off - Lamp On");

      tft.fillRect(40, 100, 42, 20, RED);
      printText("IN", WHITE, 44, 103, 2);
      state = digitalRead(Relay1_AirIn);
      if (state == 0) {
        // do nothing
      }
      else {
        digitalWrite(Relay1_AirIn, LOW);
      }

      tft.fillRect(80, 100, 128, 20, RED);
      printText("OUT", WHITE, 85, 103, 2);
      state = digitalRead(Relay2_AirOut);
      if (state == 0) {
        // do nothing
      }
      else {
        digitalWrite(Relay2_AirOut, LOW);
      }

      tft.fillRect(80, 120, 128, 20, RED);
      printText("OFF", WHITE, 85, 123, 2);
      state = digitalRead(Relay3_Lamp);
      if (state == 1) {
        // do nothing
      }
      else {
        digitalWrite(Relay3_Lamp, HIGH);
      }
    }
  }

  // 3. Refresh screen after 50 times
  manytimes++;
  total++;

  if (manytimes == 50) {

    tft.fillScreen(BLACK);
    printText("Smart", LIME, 15, 20, 2);
    printText("GreenHouse", GREEN, 5, 50, 2);
    printText("v4.0", AQUA, 15, 80, 2);
    delay(3000);
    manytimes = 0;
  }

  delay(kath);
  tft.setTextSize(1);
  tft.fillRect(50, 80, 128, 40, BLACK);
  tft.setTextColor(WHITE);
}

// ***************************************************************************
// Function Print Text
void printText(char *text, uint16_t color, int x, int y, int textSize)
{
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(textSize);
  tft.setTextWrap(true);
  tft.print(text);
  // printText("MAX", RED,18,130,1);
}

// ***************************************************************************
// Function Print float
void printText1(float num, uint16_t color, int x, int y, int textSize)
{
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(textSize);
  tft.setTextWrap(true);
  tft.print(num);
  // printText1(value, RED,18,130,1);
}
