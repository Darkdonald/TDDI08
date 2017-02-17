#include "lightsystem.h"

Lightsystem::Lightsystem(sc_module_name name)
  : sc_module(name)
{
  light_NS.initialize(0);
  light_SN.initialize(0);
  light_WE.initialize(0);
  light_EW.initialize(0);

  SC_THREAD(event_trigger_thread);

  SC_METHOD(control_lights_method);
  dont_initialize();
  sensitive << time_event;
}

void Lightsystem::control_lights_method()
{
  sensor[0] = sensor_NS->read();
  sensor[1] = sensor_SN->read();
  sensor[2] = sensor_WE->read();
  sensor[3] = sensor_EW->read();

  cout << "last car " << car[0] << car[1] << car[2] << car[3] << "\n";
  cout << "light" << light[0] << light[1] << light[2] << light[3] << "\n";
  // see if already green
  for (int i = 0; i < 4; i++) {
    if (light[i] == 1) {
      car[i] = car[i] - light[i];
      count[i]++;
    } else {
      count[i] = 0;
    }
    if (car[i] < 0) {
      car[i] = 0;
    }
  }

  for (int i = 0; i < 4; i++) {
    light[i] = 0;
  }

  cout << "sensor " << sensor[0] << sensor[1] << sensor[2] << sensor[3] << "\n";

  // check
  for (int i = 0; i < 4; i++) {
    if (sensor[i] == 1) {
      car[i]++;
    }
  }

  cout << "new car " << car[0] << car[1] << car[2] << car[3] << "\n";

  // no car
  if (sumArray(car) == 0) {
    for (int i = 0; i < 4; i++) {
      light[i] = 0;
    }
    setLight();
    return;
  }

  NSSN = car[0] + car[1] - count[0] - count[1];
  WEEW = car[2] + car[3] - count[2] - count[3];

  if (NSSN >= WEEW) {
    light[2] = 0;
    light[3] = 0;

    if (car[0]) {
      light[0] = 1;
    }
    if (car[1]) {
      light[1] = 1;
    }
    setLight();
    return;
  } else {
    light[0] = 0;
    light[1] = 0;

    if (car[2]) {
      light[2] = 1;
    }
    if (car[3]) {
      light[3] = 1;
    }
    setLight();
    return;
  }
}

void Lightsystem::setLight() {
  cout << "set light" << light[0] << light[1] << light[2] << light[3] << "\n";

  light_NS->write(light[0]);
  light_SN->write(light[1]);
  light_WE->write(light[2]);
  light_EW->write(light[3]);
}

int Lightsystem::sumArray(int* array) {
  int sum = 0;

  for (int i = 0; i < 4; i++) {
    sum = sum + array[i];
  }

  return sum;
}

void Lightsystem::event_trigger_thread()
{
  for (;;) {               // Loop infinitely.
    wait(1, SC_SEC);       // Wait one second.
    time_event.notify();
  }
}
