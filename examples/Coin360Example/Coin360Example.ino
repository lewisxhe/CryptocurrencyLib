/**
 * @file      Coin360Example.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2022  lewis he
 * @date      2022-11-08
 *
 */

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "Coin360Api.h"


const char *ssid     = "WIFI SSID";
const char *password = "WIFI PASSWORD";

WiFiClientSecure client;
Coin360Api api(client);

void printTickerData(String ticker)
{
    Coin360Response response = api.update(ticker, "USD");
    if (response.error == "") {

        Serial.println("=====================");
        Serial.print("24H Volume: ");
        Serial.println(response.volume_24h);
        Serial.print("1H Change: ");
        Serial.println(response.change_1h);
        Serial.print("24H Change: ");
        Serial.println(response.change_24h);

        Serial.print("Price: ");
        Serial.println(response.price);

        // !Using println will return ovf
        // Serial.print("Volume: ");
        // Serial.println(response.volume);
        // Serial.print("Market Cap: ");
        // Serial.println(response.market_cap);
        Serial.printf("Volume:%.6f \nMarket Cap: %.6f\n", response.volume, response.market_cap);

        Serial.print("Last Updated: ");
        Serial.println(response.last_updated);
    }
}

void setup()
{
    Serial.begin(115200);

    Serial.println("Start.");

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(20);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Get data..");

    // SET Client Secure CA
    client.setCACert(api.getCoin360CA());

}


void loop()
{
    printTickerData("BTC");
    delay(60000);
}