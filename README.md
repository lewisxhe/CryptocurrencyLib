CoinMarketCapApi2
=========================
This is an updated version of [witnessmenow/arduino-coinmarketcap-api](https://github.com/witnessmenow/arduino-coinmarketcap-api), because the original author is not maintained, I renamed it `CoinMarketCapApi2`


- For the new API, you can use the currency ID or abbreviated name, such as Bitcoin, you can view the letter after Circulating Supply at https://coinmarketcap.com/, it is BTC

```
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "CoinMarketCapApi.h"

#define APIKEY "YOUR API KEY"

WiFiClientSecure client;
CoinMarketCapApi api(client, APIKEY);

void printTickerData(String ticker){
CMCTickerResponse response = api.GetTickerInfo(ticker, "USD");
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
    //...connect your wifi
    ...
    ...

    //Get cryptocurrency data
    printTickerData("BTC","USD");

}
```
