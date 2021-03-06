/*
this code requires wiringPi lib

git clone git://git.drogon.net/wiringPi
cd wiringPi
./build

gcc -o dht11 dht11.c -L/usr/local/lib -lwiringPi -lpthread
*/

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define MAX_TIME 85
#define DHT11PIN 7
#define ATTEMPTS 5
int dht11_val[5] = {0,0,0,0,0};

char * val(){
  uint8_t lststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;
  for(i = 0; i < 5; i++)
     dht11_val[i] = 0;

  pinMode(DHT11PIN, OUTPUT);
  digitalWrite(DHT11PIN, LOW);
  delay(18);
  digitalWrite(DHT11PIN, HIGH);
  delayMicroseconds(40);
  pinMode(DHT11PIN, INPUT);
  for(i = 0; i < MAX_TIME; i++)
  {
    counter = 0;
    while (digitalRead(DHT11PIN) == lststate){
      counter++;
      delayMicroseconds(1);
      if(counter == 255)
        break;
    }
    lststate = digitalRead(DHT11PIN);
    if (counter == 255)
       break;
    // top 3 transistions are ignored
    if ((i >= 4) &&( i % 2 == 0)) {
      dht11_val[j / 8] <<= 1;
      if (counter > 50)
        dht11_val[j / 8] |= 1;
      j++;
    }
  }
  char * res = malloc(sizeof(char) * 15);
  // verify checksum and print the verified data
  if (j >= 40 && dht11_val[4] == ((dht11_val[0] + dht11_val[1] + dht11_val[2] + dht11_val[3]) & 0xFF)) {
    sprintf(res, "%d.%d%, %d.%d*C\n", dht11_val[0],dht11_val[1],dht11_val[2],dht11_val[3]);
  } else {
    res = "error\n";
  }
  return res;
}

int main(void) {
  int attempts = ATTEMPTS;
  char * r = 0;
  if (wiringPiSetup() == -1)
    exit(1);
  while (attempts)  {
    delay(1000);
    r = val();
    if (r[0] != 'e') {
      printf("%s", r);
      free(r);
      return 0;
    }
    attempts--;
  }
  printf("%s", r);
  free(r);
  return 2;
}
