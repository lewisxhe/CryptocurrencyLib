/////////////////////////////////////////////////////////////////
/*
  Coin360Api.h - Arduino library Get cryptocurrency value from Coin360Api.
  Created by Lewis he on March 9, 2020
*/
/////////////////////////////////////////////////////////////////
#pragma once

#include <Arduino.h>
// #include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <cJSON.h>  //ESP32 Core comes with cJSON

#define COIN360_HOST "api.coin360.com"

struct Coin360Response {
    String price_unit;
    String last_updated;
    String error;
    double price;
    double volume;
    double volume_24h;
    double change_1h;
    double change_24h;
    double market_cap;
};


class Coin360Api
{
public:
    Coin360Api (Client &client);
    const char *getCoin360CA();

    Coin360Response update(String coinId, String currency = "");

    int Port = 443;

protected:
    String get(String command);

private:
    Client *client;
    void closeClient();
};

