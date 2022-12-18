/**
 * @file      CoinMarketCapExample.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2022  lewis he
 * @date      2022-11-08
 *
 */
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "CoinMarketCapApi.h"


const char *ssid     = "WIFI SSID";
const char *password = "WIFI PASSWORD";
#define APIKEY "YOUR API KEY"

WiFiClientSecure client;
CoinMarketCapApi api(client, APIKEY);

void printTickerData(String ticker)
{
    CMCTickerResponse response = api.update(ticker, "USD");
    if (response.error == "") {
        Serial.print("ID: ");
        Serial.println(response.id);
        Serial.print("Name: ");
        Serial.println(response.name);
        Serial.print("Symbol: ");
        Serial.println(response.symbol);

        Serial.print("Rank: ");
        Serial.println(response.cmc_rank);

        Serial.print("Price: ");
        Serial.println(response.price);

        Serial.print("24h Volume: ");
        Serial.println(response.volume_24h);
        Serial.print("Market Cap: ");
        Serial.println(response.market_cap);

        Serial.print("Circulating Supply: ");
        Serial.println(response.circulating_supply);
        Serial.print("Total Supply: ");
        Serial.println(response.total_supply);

        Serial.print("Percent Change 1h: ");
        Serial.println(response.percent_change_1h);
        Serial.print("Percent Change 24h: ");
        Serial.println(response.percent_change_24h);
        Serial.print("Percent Change 7d: ");
        Serial.println(response.percent_change_7d);
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
}


void loop()
{
    //Get cryptocurrency data
    printTickerData("BTC");
    delay(60000);
}

