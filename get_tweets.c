#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include "mylibs.h"

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    FILE *fptr;
    fptr = fopen("output.json", "a");
    fprintf(fptr,"%s", buffer);
    fclose(fptr);
    return size * nmemb;
}

int tweets_get()
{
    int buf_size = 20000;
    time_t t = time(NULL);
    char timestamp[11];
    snprintf(timestamp, sizeof(timestamp), "%zu", (size_t)t);

    FILE *fptr;
    fptr = fopen("output.json", "w");
    fprintf(fptr,"%s", "");
    fclose(fptr);

    char *nonce = malloc(buf_size);
    if (nonce == NULL) return 1;
    strcpy(nonce, generate_nonce());

    char *signature = malloc(buf_size);
    if (signature == NULL) return 1;

    char consumer_key[64] = "oNTQbAtR4Es7Ewk80PIFAd1rQ";
    char consumer_secret[64] = "c49TIQTQ4ayJvkEenQ09mc7Uo6MyF7jfkuJTRRo6i2XDknoTjG";
    char access_token[64] = "2679666494-HIEIqzucuO9zU2FHvp0lyppaLAsXGxs9jkmqhe9";
    char access_token_secret[64] = "HaomhOvKMVEiyMOSde0WiFVuO35pi13Zi3sawzTTHAMHi";

    char *hmac_key = malloc(buf_size);
    if (hmac_key == NULL) return 1;
    strcat(hmac_key, consumer_secret);
    strcat(hmac_key, "&");
    strcat(hmac_key, access_token_secret);

    strcpy(signature, compute_signature(timestamp, nonce, consumer_key, access_token, hmac_key));

    char *post = malloc(buf_size);
    if (post == NULL) {
        return 1;
    }
    strcpy(post, create_post(timestamp, nonce, signature, consumer_key, access_token));

    CURL *curl;
    CURLcode res;
    
    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk, post);
        curl_easy_setopt(curl, CURLOPT_URL, "https://stream.twitter.com/1.1/statuses/sample.json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return 0;
}

int main()
{
    tweets_get();
    return 0;
}