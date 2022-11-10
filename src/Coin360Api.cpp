/////////////////////////////////////////////////////////////////
/*
  Coin360Api.cpp - Arduino library Get cryptocurrency value from Coin360Api.
  Created by Lewis he on November 8, 2022
*/
/////////////////////////////////////////////////////////////////
#include "Coin360Api.h"

static const char *rootCA = "-----BEGIN CERTIFICATE-----\n\
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n\
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n\
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n\
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n\
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n\
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n\
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n\
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n\
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n\
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n\
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n\
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n\
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n\
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n\
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n\
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n\
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n\
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n\
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n\
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n\
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n\
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n\
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n\
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n\
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n\
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n\
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n\
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n\
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n\
-----END CERTIFICATE-----\n";

Coin360Api::Coin360Api(Client &client)
{
    this->client = &client;
}

const char *Coin360Api::getCoin360CA()
{
    return rootCA;
}

String Coin360Api::get(String command)
{
    String body = "";
    body.reserve(700);
    bool findHeaders = false;
    uint32_t now;


    if (client->connect(COIN360_HOST, Port)) {
        client->println("GET " + command + " HTTP/1.1");
        client->println("Host: " COIN360_HOST);
        client->println(F("User-Agent: CoinLib"));
        client->println("Accept: application/json");
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

Coin360Response Coin360Api::update(String coinId, String currency)
{
    // ! https://api.coin360.com/coin/latest?coin=BTC&convert=USD
    String command = "/coin/latest?coin=" + coinId;
    if (currency != "") {
        command = command + "&convert=" + currency;
    } else {
        currency = "USD";
    }

    String response = get(command);
    Coin360Response responseObject = Coin360Response();

    responseObject.price_unit = currency;

#if 0
    Serial.println("=======================");
    Serial.println(response);
    Serial.println("=======================");
#endif

    cJSON *root = cJSON_Parse(response.c_str());
    if (!root) {
        responseObject.error = "cJSON parse failed!";
        return responseObject;
    }

    cJSON *sub =  cJSON_GetObjectItem(root, coinId.c_str());
    if (!sub) {
        return responseObject;
    }
    cJSON *volume_24h =  cJSON_GetObjectItem(sub, "volume_24h");
    if (volume_24h) {
        responseObject.volume_24h = volume_24h->valuedouble;
    }
    cJSON *change_1h =  cJSON_GetObjectItem(sub, "change_1h");
    if (change_1h) {
        responseObject.change_1h = change_1h->valuedouble;
    }
    cJSON *change_24h =  cJSON_GetObjectItem(sub, "change_24h");
    if (volume_24h) {
        responseObject.change_24h = change_24h->valuedouble;
    }

    cJSON *quotes =  cJSON_GetObjectItem(sub, "quotes");
    if (quotes) {
        cJSON *currency2 =  cJSON_GetObjectItem(quotes, currency.c_str());
        if (currency2) {
            cJSON *price =  cJSON_GetObjectItem(currency2, "price");
            if (price) {
                responseObject.price = price->valuedouble;
            }
            cJSON *volume =  cJSON_GetObjectItem(currency2, "volume");
            if (volume) {
                responseObject.volume = volume->valuedouble;
            }
            cJSON *market_cap =  cJSON_GetObjectItem(currency2, "market_cap");
            if (market_cap) {
                responseObject.market_cap = market_cap->valuedouble;
            }
        }

    }
    cJSON *timestamp =  cJSON_GetObjectItem(sub, "timestamp");
    if (timestamp) {
        struct tm *tm_t;
        tm_t = localtime((const time_t *)&timestamp->valueint);
        char buf[128];
        strftime(buf, 128, "%F %T", tm_t);
        responseObject.last_updated = buf;
    }

    cJSON_Delete(root);

    return responseObject;
}

void Coin360Api::closeClient()
{
    if (client->connected()) {
        client->stop();
    }
}
