//created by Leonardo Araujo 
//Github leonardaraujo
//leonardoaraujo.oct@gmail.com+

//latch led
int l_rgb = 13;

//alarm
byte alarm_led = A5;
byte configure_alarm_off = A4;
int actualPointer = 0;

//alarm on variables
int alarm_hour = 0;
int alarm_minute = 0;
int alarm_second = 0;
bool alarm_activated = false;

//alarm off variables
int alarm_off_h = 0;
int alarm_off_minute = 0;
int alarm_off_second = 0;

//Clock variables
int crude_hours = 0;
int crude_minutes = 0;
int crude_secs = 0;

//button variables
byte add_one = A0;
byte pin_start_stop = A1;
byte pointer = A2;
byte configure_alarm_on = A3;
bool is_configuring = false;
bool is_setting_alarm = false;
bool is_setting_alarm_off = false;

//latch variables
int l_seconds = 10;
int l_minutes = 11;
int l_hours = 12;

//time
unsigned long time;
unsigned long firstTime = 0;

//bus
String str = "";
String united = "";
String string_decimal = "";
String string_unit = "";

//function binary convert
char decimal[4] = { 0 };
String binary_string = "";

//Función para trabajar con millis
bool timePass(unsigned long &other_time, unsigned long time_passed) {
  if ((time - other_time) >= time_passed) {
    other_time = time;
    return true;
  } else {
    return false;
  }
}

//setup
void setup() {
  pinMode(add_one, INPUT);
  pinMode(pointer, INPUT);
  pinMode(configure_alarm_on, INPUT);
  pinMode(pin_start_stop, INPUT);
  pinMode(configure_alarm_off, INPUT);
  pinMode(alarm_led, OUTPUT);
  for (int i = 2; i <= 12; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(l_rgb, OUTPUT);
  Serial.begin(9600);
}

//convierte un numero a su forma binaria
String converts(int number) {
  decimal[4] = { 0 };
  binary_string = "";
  number += 16;
  itoa(number, decimal, 2);
  binary_string = decimal + 1;
  return binary_string;
}

//activa el latch especificado
void burnLatch(int pin) {
  digitalWrite(pin, HIGH);
  digitalWrite(pin, LOW);
}

//salida del bus
void busOut(int number) {
  if (number < 10) {
    str = "0" + String(number);
  } else {
    str = String(number);
  }
  string_decimal = converts((int)str[0] - 48);
  string_unit = converts((int)str[1] - 48);
  united = string_decimal + string_unit;
  for (int i = 0; i < 8; i++) {
    if (united[i] == '0') {
      digitalWrite(i + 2, LOW);
    } else {
      digitalWrite(i + 2, HIGH);
    }
  }
}
//actualiza los leds con los valores dados
void updateLeds(int hour, int min, int sec) {
  busOut(sec);
  burnLatch(l_seconds);
  busOut(min);
  burnLatch(l_minutes);
  busOut(hour);
  burnLatch(l_hours);
}

//clock
void clockMachine(int &crude_hours, int &crude_minutes, int &crude_secs) {
  if (crude_secs != 60) {
  }
  if (crude_secs == 60) {
    crude_secs = 0;
    crude_minutes++;
  }
  if (crude_minutes == 60) {
    crude_minutes = 0;
    crude_hours++;
  }
  if (crude_hours == 24) {
    crude_hours = 0;
  }
}
void alarmSound(bool state) {
  if (state) {
    digitalWrite(alarm_led, HIGH);
  } else {
    digitalWrite(alarm_led, LOW);
  }
}

//Funcion activar alarma
void alarmActivate() {
  Serial.print("Hora de prendido ");
  Serial.print(alarm_hour);
  Serial.print(":");
  Serial.print(alarm_minute);
  Serial.print(":");
  Serial.print(alarm_second);
  Serial.println("");
  if (crude_secs == alarm_second && crude_minutes == alarm_minute && crude_hours == alarm_hour) {
    Serial.println("Alarm activated ");
    alarm_hour = 0;
    alarm_minute = 0;
    alarm_second = 0;
    alarmSound(true);
  }
}

//Funcion desactivar alarma
void alarmDesactivate() {
  Serial.print("Hora de apagado ");
  Serial.print(alarm_off_h);
  Serial.print(":");
  Serial.print(alarm_off_minute);
  Serial.print(":");
  Serial.print(alarm_off_second);
  Serial.println("");
  if (crude_secs == alarm_off_second && crude_minutes == alarm_off_minute) {
    if (crude_hours == alarm_off_h) {
      Serial.println("Alarm desactivated");
      alarm_off_h = 0;
      alarm_off_minute = 0;
      alarm_off_second = 0;
      alarmSound(false);
    }
  }
}

//variables del contexto
String actual_context_adder = "";
String string_time = "time";
String string_alarm_on = "alarmon";
String string_alarm_off = "alarmoff";

//Define el contexto al quien esta añadiendo unidades
void contextAdder(bool state, String to) {
  if (state) {
    Serial.println("Changing context adder");
    actual_context_adder = to;
  } else {
    actual_context_adder = "";
  }
  Serial.println(actual_context_adder);
}
//Añade una unidad al contexto que se tiene
void addOneTo(String to) {
  Serial.println(actual_context_adder);
  if (to == string_time) {
    if (is_configuring) {
      Serial.println("Adding one in clock");
      if (actualPointer == 0) {
        crude_secs = crude_secs + 1;
      } else if (actualPointer == 1) {
        crude_minutes = crude_minutes + 1;
      } else if (actualPointer == 2) {
        crude_hours = crude_hours + 1;
      }
      clockMachine(crude_hours, crude_minutes, crude_secs);
      updateLeds(crude_hours, crude_minutes, crude_secs);
    }
  } else if (to == string_alarm_on) {
    if (is_setting_alarm) {
      Serial.println("Adding one in on/Alarm");
      if (actualPointer == 0) {
        alarm_second = alarm_second + 1;
      } else if (actualPointer == 1) {
        alarm_minute = alarm_minute + 1;
      } else if (actualPointer == 2) {
        alarm_hour = alarm_hour + 1;
      }
      clockMachine(alarm_hour, alarm_minute, alarm_second);
      updateLeds(alarm_hour, alarm_minute, alarm_second);
    }
  } else if (to == string_alarm_off) {
    if (is_setting_alarm_off) {
      Serial.println("Adding one in off/Alarm");
      if (actualPointer == 0) {
        alarm_off_second = alarm_off_second + 1;
      } else if (actualPointer == 1) {
        alarm_off_minute = alarm_off_minute + 1;
      } else if (actualPointer == 2) {
        Serial.println("ADDING HOUR ");
        alarm_off_h = alarm_off_h + 1;
      }
      clockMachine(alarm_off_h, alarm_off_minute, alarm_off_second);
      updateLeds(alarm_off_h, alarm_off_minute, alarm_off_second);
    }
  }
}

//variables para los leds
String red = "red";
String green = "green";
String blue = "blue";
String yellow = "yellow";

//Guarda el color actual en los rgb
void outColor(bool r, bool g, bool b) {
  if (r) {
    digitalWrite(2, HIGH);
  }
  if (g) {
    digitalWrite(3, HIGH);
  }
  if (b) {
    digitalWrite(4, HIGH);
  }
  digitalWrite(l_rgb, HIGH);
  digitalWrite(l_rgb, LOW);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
}

//define los colores utilizados
void ledColorState(String color) {
  if (color == red) {
    outColor(true, false, false);
  } else if (color == green) {
    outColor(false, true, false);
  } else if (color == blue) {
    outColor(false, false, true);
  } else if (color == yellow) {
    outColor(true, true, false);
  }
}

//loop
void loop() {
  time = millis();
  //button functions

  //añadir una unidad
  if (digitalRead(add_one)) {
    addOneTo(actual_context_adder);
    delay(250);
  }

  //cambiar de unidad a decimal a minutos a horas
  if (digitalRead(pointer)) {  
    if (is_configuring || is_setting_alarm || is_setting_alarm_off) {
      Serial.println("pointer is ");
      actualPointer++;
      Serial.println(actualPointer);
      if (actualPointer == 3) {
        actualPointer = 0;
      }
      delay(250);
    }
  }
  //para el reloj y lo renauda 
  //entra en modo configuración
  if (digitalRead(pin_start_stop)) {
    if (is_setting_alarm || is_setting_alarm_off) {
      is_setting_alarm = false;
      is_setting_alarm_off = false;
    }
    ledColorState(blue);
    updateLeds(crude_hours, crude_minutes, crude_secs);
    is_configuring = !is_configuring;
    contextAdder(is_configuring, string_time);
    delay(250);
  }

  // entra en modo configuración de prendido
  if (digitalRead(configure_alarm_on)) {
    if (is_configuring || is_setting_alarm_off) {
      is_configuring = false;
      is_setting_alarm_off = false;
    }
    ledColorState(green);
    updateLeds(alarm_hour, alarm_minute, alarm_second);
    is_setting_alarm = !is_setting_alarm;
    contextAdder(is_setting_alarm, string_alarm_on);
    delay(250);
  }

  // entra en modo configuración de apagado
  if (digitalRead(configure_alarm_off)) {
    if (is_configuring || is_setting_alarm) {
      is_configuring = false;
      is_setting_alarm = false;
    }
    ledColorState(yellow);
    updateLeds(alarm_off_h, alarm_off_minute, alarm_off_second);
    is_setting_alarm_off = !is_setting_alarm_off;
    contextAdder(is_setting_alarm_off, string_alarm_off);
    delay(250);
  }


  //clock loop 
  if (timePass(firstTime, 1000) && !is_configuring) {
    crude_secs = crude_secs + 1;
    clockMachine(crude_hours, crude_minutes, crude_secs);
    if (is_setting_alarm) {
      {
        updateLeds(alarm_hour, alarm_minute, alarm_second);
      }
    } else if (is_setting_alarm_off) {
      updateLeds(alarm_off_h, alarm_off_minute, alarm_off_second);
    } else {
      actualPointer = 0;
      ledColorState(red);
      updateLeds(crude_hours, crude_minutes, crude_secs);
    }
    alarmActivate();
    alarmDesactivate();
    Serial.print(crude_hours);
    Serial.print(":");
    Serial.print(crude_minutes);
    Serial.print(":");
    Serial.print(crude_secs);
    Serial.println("");
  }
  
}
