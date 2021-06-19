// Author: Fuat Bölük <mektup@fuatboluk.com.tr>


#include <regex.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <curl/easy.h>


struct MemoryStruct {
    char * memory;
    size_t size;
};


static size_t WriteMemoryCallback(void * contents, size_t size, size_t nmemb, void * userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct * mem = (struct MemoryStruct *)userp;
 
    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }
 
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
 
    return realsize;
}


void strrev(char *str, const size_t size)
{
    char temp = '\0';
    size_t i = 0;
    size_t j = size - 1;

   while (i < j) {
       temp = str[i];
       str[i] = str[j];
       str[j] = temp;
       i++;
       j--;
   }
}


char retab(char * string)
{
    int one = 2;
    int num = 0;
    const char space[2] = "\t";
    char * token = strtok(string + 1, space);

    printf("Tarih     \tSaat    \tEnlem\t\tBoylam\t\tDerinlik\tMD \tML \tMw \tYer\n");
    printf("----------\t--------\t-------\t\t-------\t\t--------\t---\t---\t---\t---\n");

    while (token != NULL) {

        num++;

        int ret;
        regex_t regex;
        ret = regcomp(&regex, "^([A-Z]+)|([A-Z]+).([A-Z]+)|(.[A-Z]+).([A-Z]+.)$", REG_EXTENDED);
        ret = regexec(&regex, token, 0, NULL, 0);

        switch (num) {
            case 1:  printf("%s\t", token);   break;
            case 2:  if (one == 2) {
                         one = 0;
                         printf("%s\t", token); 
                     } else {
                         printf("%s\t\t", token); 
                     }
                     break;
            case 3:  printf("%s\t\t", token); break;
            case 4:  printf("%s\t\t", token); break;
            case 5:  if (one == 0) {
                         one = 5;
                         printf("%s\t\t", token); 
                     } else {
                         printf("%s\t", token); 
                     }
                     break;
            case 6:  printf("%s\t", token);   break;
            case 7:  if (!ret) {
                         num = 0;
                         printf("%s\t\t", token);
                     } else if (ret == REG_NOMATCH) {
                         printf("%s\t", token);
                     } 
                     break;
            case 8:  if (!ret) {
                         num = 0;
                         printf("%s\t", token);
                     } else if (ret == REG_NOMATCH) {
                         printf("%s\t", token);
                     } 
                     break;
            case 9:  if (!ret) {
                         num = 0;
                         printf("%s\t", token);
                     } else if (ret == REG_NOMATCH) {
                         printf("%s\t", token);
                     } 
                     break;
        }

        regfree(&regex);
        token = strtok(NULL, space);
    }

    printf("\r");
}


char clean(char * string)
{
    char all_line[999999];
    const char newline[2] = "\n";
    char * token = strtok(string, newline);

    while (token != NULL) {

        char * part1;
        char * part2;
        char * part3;
        char * part4;

        char finish[512];

        strrev(token, strlen(token));

        if (part1 = strstr(token, "10EZIVER")) {
            sprintf(finish, "%s\n", part1 + 9);
        } else if (part2 = strstr(token, "20EZIVER")) {
            sprintf(finish, "%s\n", part2 + 9);
        } else if (part3 = strstr(token, "30EZIVER")) {
            sprintf(finish, "%s\n", part3 + 9);
        } else if (part4 = strstr(token, "leskl")) {
            sprintf(finish, "%s\n", part4 + 7);
        } else {
            sprintf(finish, "%s\n", token);
        }

        strrev(finish, strlen(finish));

        strcat(all_line, finish);
        token = strtok(NULL, newline);
    }

    return retab(all_line);
}


char tab(char * string)
{
    char all_line[999999];
    const char space[2] = " ";
    char * token = strtok(string, space);

    while (token != NULL) {

        int ret;
        regex_t regex;
        ret = regcomp(&regex, "^([A-Z]+)|([A-Z]+).([A-Z]+)|(.[A-Z]+).([A-Z]+.)$", REG_EXTENDED);
        ret = regexec(&regex, token, 0, NULL, 0);

        if (!ret) {
            strcat(all_line, token);
            strcat(all_line, " ");
        } else if (ret == REG_NOMATCH) {
            strcat(all_line, token);
            strcat(all_line, "\t");
        }

        regfree(&regex);
        token = strtok(NULL, space);
    }

    return clean(all_line);
}


char line(char * string)
{
    int ret;
    regex_t regex;

    char all_line[999999];
    const char space[2] = "\n";
    char * token = strtok(string, space);

    while (token != NULL) {
        ret = regcomp(&regex, "^(20[1-9]+).([0-9]+).([0-9]+)", REG_EXTENDED);
        ret = regexec(&regex, token, 0, NULL, 0);

        if (!ret) {
            strcat(all_line, token);
            strcat(all_line, "\n");
        }

        regfree(&regex);
        token = strtok(NULL, space);
    }

    return tab(all_line);
}


char crl(char * url)
{
    CURL * curl;
    CURLcode res;

    struct MemoryStruct chunk;
 
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        return line(chunk.memory);

        curl_easy_cleanup(curl);
        free(chunk.memory);
        curl_global_cleanup();
    }
}


int main(int argc, char * argv[])
{
    crl("http://www.koeri.boun.edu.tr/scripts/lst0.asp");
    return 0;
}


// End
