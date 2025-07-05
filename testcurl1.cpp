#include <cstdio>
#include <curl/curl.h>
#include "json.hpp"

using json = nlohmann::json;

size_t WriteResponse(void *contents, size_t size, size_t nmemb, std::string *response)
{
    size_t totalSize = size * nmemb;
    response->append((char *)contents, totalSize);
    return totalSize;
}

int main()
{
    CURL *curl = curl_easy_init();

    std::string response;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/posts/1/comments");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteResponse);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        try
        {
            if (res == CURLE_OK)
            {
                json data = json::parse(response);
                printf("Response JSON:\n%s\n", data.dump(4).c_str());
            }
            else
            {
                printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }
        }
        catch (const std::exception &e)
        {
            printf("Exception occurred: %s\n", e.what());
        }
    }
    else
    {
        printf("Failed to initialize CURL.\n");
    }
    return 0;
}
