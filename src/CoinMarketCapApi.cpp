/////////////////////////////////////////////////////////////////
/*
  CoinMarketCapApi.cpp - Arduino library Get cryptocurrency value from CoinMarketCapApi.
  Created by Lewis he on March 9, 2020
*/
/////////////////////////////////////////////////////////////////
#include "CoinMarketCapApi.h"

CoinMarketCapApi::CoinMarketCapApi(Client &client, String apikey)
{
    this->client = &client;
    __apikey  = apikey;
}

String CoinMarketCapApi::SendGetToCoinMarketCap(String command)
{
    String body = "";
    body.reserve(700);
    bool findHeaders = false;
    uint32_t now;

    if (client->connect(COINMARKETCAP_HOST, Port)) {
        Serial.println(".... connected to server");
        client->println("GET " + command + " HTTP/1.1");
        client->println("Host: " COINMARKETCAP_HOST);
        client->println(F("User-Agent: arduino/1.0.0"));
        client->println("Accepts: application/json");
        client->print("X-CMC_PRO_API_KEY: ");
        client->println(__apikey);
        client->println();
        now = millis();
        while (millis() - now < 3000) {
            while (client->available()) {
                char c = client->read();
                Serial.write(c);
                if (!findHeaders) {
                    if (c == '{') {
                        findHeaders = true;
                        body = c;
                    }
                } else {
                    if (c == '\r') {
                        break;
                    } else {
                        body += c;
                    }
                }
            }
        }
    }
    client->stop();
    return body;
}

CMCTickerResponse CoinMarketCapApi::GetTickerInfo(String coinId, String currency)
{
    //! new api : https://pro-api.coinmarketcap.com/v1/cryptocurrency/map
    //! info : https://pro-api.coinmarketcap.com/v1/cryptocurrency/info
    String command = "/v1/cryptocurrency/quotes/latest?symbol=" + coinId;
    if (currency != "") {
        command = command + "&convert=" + currency;
    } else {
        currency = "USD";
    }

    Serial.println(command);
    String response = SendGetToCoinMarketCap(command);
    CMCTickerResponse responseObject = CMCTickerResponse();

    responseObject.price_unit = currency;

    DynamicJsonDocument  doc(2048);
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        responseObject.error = error.c_str();
        return responseObject;
    }
#if 0
    String output;
    serializeJson(doc, output);
    Serial.println(output);
#endif
    JsonObject data = doc["data"][coinId];

    responseObject.id = data["id"].as<String>();
    responseObject.name = data["name"].as<String>();
    responseObject.symbol = data["symbol"].as<String>();
    responseObject.cmc_rank = data["cmc_rank"].as<int>();

    JsonObject quote = data["quote"][currency];
    responseObject.percent_change_1h = quote["percent_change_1h"].as<double>();
    responseObject.percent_change_24h = quote["percent_change_24h"].as<double>();
    responseObject.percent_change_7d = quote["percent_change_7d"].as<double>();
    responseObject.price = quote["price"].as<double>();
    responseObject.volume_24h = quote["volume_24h"].as<double>();
    responseObject.market_cap = quote["market_cap"].as<double>();
    responseObject.last_updated = quote["last_updated"].as<String>();

    return responseObject;
}

void CoinMarketCapApi::closeClient()
{
    if (client->connected()) {
        client->stop();
    }
}
