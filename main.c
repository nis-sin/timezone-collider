#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <regex.h>

struct memory {
    char* response;
    size_t size;
};

void printAvailableTimezones();
int searchTimezone(char* timezone);
size_t cb(void* data, size_t size, size_t nmemb, void* clientp);    // Callback function to write the data to the memory

int main(){

    unsigned int numTimezones = 0;
    char timezone[BUFSIZ];

    while (numTimezones != 1){
        printf("Enter timezone: \n");
        scanf("%s", timezone);

        numTimezones = searchTimezone(timezone);

    }

    char* url = malloc(sizeof(char) * 100);
    snprintf(url, 100, "https://timeapi.io/api/Time/current/zone?timeZone=");
    int size = (strlen(timezone)+1)*sizeof(char);
    memcpy(url+50, timezone, size);     // append timezone to url

    CURL* curl;
    CURLcode responseCode;
    char errorBuffer[CURL_ERROR_SIZE];
    struct memory chunk;
    chunk.response = malloc(1);
    chunk.size = 0;
    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();

    if (curl){

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) &chunk);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
        responseCode = curl_easy_perform(curl);

        if (responseCode != CURLE_OK){
            printf("Request failed: %s\n", curl_easy_strerror(responseCode));
            printf("Error: %s\n", errorBuffer);
        }

        cJSON* json = cJSON_Parse(chunk.response);
        cJSON* e = NULL;
        char* str = NULL;

        if (json == NULL){
            const char* error_ptr = cJSON_GetErrorPtr();
            if (error_ptr != NULL){
                fprintf(stderr, "Error: %s\n", error_ptr);
            }
            cJSON_Delete(json);
            return 1;
        }

        cJSON *time = cJSON_GetObjectItem(json, "time");
        if (cJSON_IsString(time) && (time->valuestring != NULL)){
            printf("Time: %s\n", time->valuestring);
        }
        /*
        cJSON_ArrayForEach(e, json){
            str = cJSON_Print(e);
            if (str == NULL){
                fprintf(stderr, "Failed to print.\n");
            }
            else {
                printf("%s\n", str);
            }
        };
        */

        curl_easy_cleanup(curl);
        cJSON_Delete(json);
    }
    else {
        fprintf(stderr, "curl_easy_init() failed\n");
        return 1;
    }
    curl_global_cleanup();

    return 0;
}


int searchTimezone(char* timezone){
    FILE* fp = fopen("available_timezones.txt", "r");

    if (fp == NULL){
        fprintf(stderr, "Failed to open file\n");
        return 0;
    }

    char buffer[1024] = "";
    regex_t regex;
    int reti;

    reti = regcomp(&regex, timezone, REG_ICASE);
    if (reti){
        fprintf(stderr, "Could not compile regex\n");
        return 0;
    }


    char c = '1';
    unsigned int i = 0;
    unsigned int count = 0;
    printf("Timezones found: \n");

    while (c != EOF){
        c = fgetc(fp);
        if (c == '\n' || c == EOF){
            buffer[i] = '\0';
            reti = regexec(&regex, buffer, 0, NULL, 0);
            if (reti == 0){
                count++;
                printf("%s\n", buffer);
            }
            i = 0;
        }
        else{
            buffer[i++] = c;
        }
    }

    regfree(&regex);
    fclose(fp);

    if (count == 0){
        printf("No timezones found\nPlease enter a valid timezone\n");
    }
    else if (count > 1){
        printf("Multiple timezones found\nPlease enter a more specific timezone\n");
    }

    return count;
}


void printAvailableTimezones(){
    FILE* fp = fopen("available_timezones.txt", "r");

    if (fp == NULL){
        fprintf(stderr, "Failed to open file\n");
        return;
    }

    char buffer[1024];

    while (fgets(buffer, 1024, fp) != NULL){
        printf("%s\n", buffer);
    }

    fclose(fp);
}


size_t cb(void* data, size_t size, size_t nmemb, void* clientp){
    size_t realsize = size * nmemb;     // Calculate the size of the data
    struct memory* mem = (struct memory*) clientp;       // Cast the stream to a memory struct

    char* ptr = realloc(mem->response, mem->size + realsize + 1);       // Reallocate the memory to the new size, mem->response means grab the contents of the response field pointed by mem variable. Or (*mem).response
    if (!ptr){
        return 0;       // Return 0 if the memory allocation fails
    }

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;    // Null terminate the string

    return realsize;
}