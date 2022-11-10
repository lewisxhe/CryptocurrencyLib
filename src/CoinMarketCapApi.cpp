/////////////////////////////////////////////////////////////////
/*
  CoinMarketCapApi.cpp - Arduino library Get cryptocurrency value from CoinMarketCapApi.
  Created by Lewis he on March 9, 2020
*/
/////////////////////////////////////////////////////////////////
#include "CoinMarketCapApi.h"

static const char *rootCA = "-----BEGIN CERTIFICATE-----\n\
MIIEDzCCAvegAwIBAgIBADANBgkqhkiG9w0BAQUFADBoMQswCQYDVQQGEwJVUzEl\n\
MCMGA1UEChMcU3RhcmZpZWxkIFRlY2hub2xvZ2llcywgSW5jLjEyMDAGA1UECxMp\n\
U3RhcmZpZWxkIENsYXNzIDIgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMDQw\n\
NjI5MTczOTE2WhcNMzQwNjI5MTczOTE2WjBoMQswCQYDVQQGEwJVUzElMCMGA1UE\n\
ChMcU3RhcmZpZWxkIFRlY2hub2xvZ2llcywgSW5jLjEyMDAGA1UECxMpU3RhcmZp\n\
ZWxkIENsYXNzIDIgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwggEgMA0GCSqGSIb3\n\
DQEBAQUAA4IBDQAwggEIAoIBAQC3Msj+6XGmBIWtDBFk385N78gDGIc/oav7PKaf\n\
8MOh2tTYbitTkPskpD6E8J7oX+zlJ0T1KKY/e97gKvDIr1MvnsoFAZMej2YcOadN\n\
+lq2cwQlZut3f+dZxkqZJRRU6ybH838Z1TBwj6+wRir/resp7defqgSHo9T5iaU0\n\
X9tDkYI22WY8sbi5gv2cOj4QyDvvBmVmepsZGD3/cVE8MC5fvj13c7JdBmzDI1aa\n\
K4UmkhynArPkPw2vCHmCuDY96pzTNbO8acr1zJ3o/WSNF4Azbl5KXZnJHoe0nRrA\n\
1W4TNSNe35tfPe/W93bC6j67eA0cQmdrBNj41tpvi/JEoAGrAgEDo4HFMIHCMB0G\n\
A1UdDgQWBBS/X7fRzt0fhvRbVazc1xDCDqmI5zCBkgYDVR0jBIGKMIGHgBS/X7fR\n\
zt0fhvRbVazc1xDCDqmI56FspGowaDELMAkGA1UEBhMCVVMxJTAjBgNVBAoTHFN0\n\
YXJmaWVsZCBUZWNobm9sb2dpZXMsIEluYy4xMjAwBgNVBAsTKVN0YXJmaWVsZCBD\n\
bGFzcyAyIENlcnRpZmljYXRpb24gQXV0aG9yaXR5ggEAMAwGA1UdEwQFMAMBAf8w\n\
DQYJKoZIhvcNAQEFBQADggEBAAWdP4id0ckaVaGsafPzWdqbAYcaT1epoXkJKtv3\n\
L7IezMdeatiDh6GX70k1PncGQVhiv45YuApnP+yz3SFmH8lU+nLMPUxA2IGvd56D\n\
eruix/U0F47ZEUD0/CwqTRV/p2JdLiXTAAsgGh1o+Re49L2L7ShZ3U0WixeDyLJl\n\
xy16paq8U4Zt3VekyvggQQto8PT7dL5WXXp59fkdheMtlb71cZBDzI0fmgAKhynp\n\
VSJYACPq4xJDKVtHCN2MQWplBqjlIapBtJUhlbl90TSrE9atvNziPTnNvT51cKEY\n\
WQPJIrSPnNVeKtelttQKbfi3QBFGmh95DmK/D5fs4C8fF5Q=\n\
-----END CERTIFICATE-----\n";

const char *CoinMarketCapApi::getRootCA()
{
    return rootCA;
}


CoinMarketCapApi::CoinMarketCapApi(Client &client, String apikey)
{
    this->client = &client;
    __apikey  = apikey;
}

String CoinMarketCapApi::get(String command)
{
    String body = "";
    body.reserve(700);
    bool findHeaders = false;
    uint32_t now;

    if (client->connect(COINMARKETCAP_HOST, Port)) {
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
                // Serial.write(c);
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

CMCTickerResponse CoinMarketCapApi::update(String coinId, String currency)
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
    String response = get(command);
    CMCTickerResponse responseObject = CMCTickerResponse();

    responseObject.price_unit = currency;


#if 0
    Serial.println("=======================");
    Serial.println(response);
    Serial.println("=======================");
#endif

#if 1

    cJSON *root = cJSON_Parse(response.c_str());
    if (!root) {
        Serial.println("Get root failed!");
        return responseObject;
    }


    cJSON *data = cJSON_GetObjectItem(root, "data");
    if (!data) {
        Serial.println("Get data failed!");
        cJSON_Delete(root);
        return responseObject;
    }

    // Serial.print("coinId:");
    // Serial.println(coinId);

    cJSON *__coinId = cJSON_GetObjectItem(data, coinId.c_str());
    if (!__coinId) {
        Serial.println("Get coinId failed!");
        cJSON_Delete(root);
        return responseObject;
    }

    cJSON *id = cJSON_GetObjectItem(__coinId, "id");
    cJSON *name = cJSON_GetObjectItem(__coinId, "name");
    cJSON *symbol = cJSON_GetObjectItem(__coinId, "symbol");
    cJSON *cmc_rank = cJSON_GetObjectItem(__coinId, "cmc_rank");
    cJSON *total_supply = cJSON_GetObjectItem(__coinId, "total_supply");

    cJSON *quote = cJSON_GetObjectItem(__coinId, "quote");
    if (!quote) {
        Serial.println("Get __coinId failed!");
        cJSON_Delete(root);
        return responseObject;
    }

    cJSON *__currency = cJSON_GetObjectItem(quote, currency.c_str());
    if (!__currency) {
        Serial.println("Get __currency failed!");
        cJSON_Delete(root);
        return responseObject;
    }

    cJSON *price = cJSON_GetObjectItem(__currency, "price");
    cJSON *volume_24h = cJSON_GetObjectItem(__currency, "volume_24h");
    cJSON *volume_change_24h = cJSON_GetObjectItem(__currency, "volume_change_24h");
    cJSON *percent_change_1h = cJSON_GetObjectItem(__currency, "percent_change_1h");
    cJSON *percent_change_24h = cJSON_GetObjectItem(__currency, "percent_change_24h");
    cJSON *percent_change_7d = cJSON_GetObjectItem(__currency, "percent_change_7d");

    cJSON *percent_change_30d = cJSON_GetObjectItem(__currency, "percent_change_30d");
    cJSON *percent_change_60d = cJSON_GetObjectItem(__currency, "percent_change_60d");
    cJSON *percent_change_90d = cJSON_GetObjectItem(__currency, "percent_change_90d");
    cJSON *market_cap = cJSON_GetObjectItem(__currency, "market_cap");
    cJSON *market_cap_dominance = cJSON_GetObjectItem(__currency, "market_cap_dominance");
    cJSON *fully_diluted_market_cap = cJSON_GetObjectItem(__currency, "fully_diluted_market_cap");
    cJSON *last_updated = cJSON_GetObjectItem(__currency, "last_updated");

    if (id) {
        responseObject.id = id->valueint;
    }
    if (name) {
        responseObject.name = name->valuestring;
    }
    if (symbol) {
        responseObject.symbol = symbol->valuestring;
    }
    if (cmc_rank) {
        responseObject.cmc_rank = cmc_rank->valueint;
    }
    if (total_supply) {
        responseObject.total_supply = total_supply->valueint;
    }
    if (price) {
        responseObject.price = price->valuedouble;
    }
    if (volume_24h) {
        responseObject.volume_24h = volume_24h->valuedouble;
    }
    if (volume_change_24h) {
        // responseObject.volume_change_24h = volume_change_24h->valuedouble;
    }
    if (percent_change_1h) {
        responseObject.percent_change_1h = percent_change_1h->valuedouble;
    }
    if (percent_change_24h) {
        responseObject.percent_change_24h = percent_change_24h->valuedouble;
    }
    if (percent_change_7d) {
        responseObject.percent_change_7d = percent_change_7d->valuedouble;
    }
    if (percent_change_30d) {
        // responseObject.percent_change_30d = percent_change_30d->valuedouble;
    }
    if (percent_change_60d) {
        // responseObject.percent_change_60d = percent_change_60d->valuedouble;
    }
    if (percent_change_90d) {
        // responseObject.percent_change_90d = percent_change_90d->valuedouble;
    }
    if (market_cap) {
        responseObject.market_cap = market_cap->valuedouble;
    }
    if (market_cap_dominance) {
        // responseObject.market_cap_dominance = market_cap_dominance->valuedouble;
    }
    if (fully_diluted_market_cap) {
        // responseObject.fully_diluted_market_cap = fully_diluted_market_cap->valuedouble;
    }
    if (last_updated) {
        responseObject.last_updated = last_updated->valuestring;
    }

    cJSON_Delete(root);

#else

    DynamicJsonDocument  doc(2048);
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        responseObject.error = error.c_str();
        return responseObject;
    }

    // String output;
    // serializeJson(doc, output);
    // Serial.println(output);

    JsonObject data = doc["data"][coinId];

    responseObject.id = data["id"].as<String>();
    responseObject.name = data["name"].as<String>();
    responseObject.symbol = data["symbol"].as<String>();
    responseObject.cmc_rank = data["cmc_rank"].as<int>();
    responseObject.total_supply = data["total_supply"].as<uint32_t>();
    responseObject.circulating_supply = data["circulating_supply"].as<uint32_t>();

    JsonObject quote = data["quote"][currency];
    responseObject.percent_change_1h = quote["percent_change_1h"].as<double>();
    responseObject.percent_change_24h = quote["percent_change_24h"].as<double>();
    responseObject.percent_change_7d = quote["percent_change_7d"].as<double>();
    responseObject.price = quote["price"].as<double>();
    responseObject.volume_24h = quote["volume_24h"].as<double>();
    responseObject.market_cap = quote["market_cap"].as<double>();
    responseObject.last_updated = quote["last_updated"].as<String>();
#endif

    return responseObject;
}

void CoinMarketCapApi::closeClient()
{
    if (client->connected()) {
        client->stop();
    }
}
