/////////////////////////////////////////////////////////////////
/*
  CoinMarketCapApi.h - Arduino library Get cryptocurrency value from CoinMarketCapApi.
  Created by Lewis he on March 9, 2020
*/
/////////////////////////////////////////////////////////////////
#pragma once

#include <Arduino.h>
#include <Client.h>
#include <cJSON.h> //ESP32 Core comes with cJSON

#define COINMARKETCAP_HOST "pro-api.coinmarketcap.com"

struct CMCTickerResponse {
    int id;
    String name;
    String symbol;
    String price_unit;

    int cmc_rank;
    double price;
    double price_btc;
    double volume_24h;
    double market_cap;
    uint32_t total_supply;
    uint32_t circulating_supply;

    double percent_change_1h;
    double percent_change_24h;
    double percent_change_7d;
    String last_updated;

    String error;
};


class CoinMarketCapApi
{
public:
    CoinMarketCapApi (Client &client, String apikey);
    const char *getRootCA();
    CMCTickerResponse update(String coinId, String currency = "");
    int Port = 443;
protected:
    String get(String command);
private:
    Client *client;
    void closeClient();
    String __apikey;
};

