#include <iostream>
#include <cstdio>
#include <curl/curl.h>
#include "json.hpp"

using json = nlohmann::json;

size_t WriteCallBack(void *contents, size_t size, size_t nmemb, std::string *userp)
{
    size_t totalSize = size * nmemb;
    userp->append((char *)contents, totalSize);
    return totalSize;
}

int main()
{
    CURL *curl = curl_easy_init();
    std::string response;
    if (curl)
    {
        // curl_easy_setopt(curl, CURLOPT_URL, "https://tarunnayaka.me");
        curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/posts/1/comments");
        // curl_easy_setopt(curl, CURLOPT_URL, "https://notexistingmadeup.com");

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else
        {
            try
            {
                json data = json::parse(response);
                printf("Response Json:\n%s\n", data.dump(4).c_str());
            }
            catch (const std::exception &e)
            {
                printf("Exception occurred: %s\n", e.what());
            }
            printf("Request completed successfully.\n");
        }

        curl_easy_cleanup(curl);
    }

    json testJson = {
        {"name", "Tarun Nayaka"},
        {"age", 30},
        {"city", "Bangalore"},
        {"skills", {"C++", "Python", "JavaScript"}},
        {"isDeveloper", true},
        {"projects", {{"name", "Project A"}, {"status", "completed"}}},
        {"languages", {{"name", "English"}, {"proficiency", "fluent"}}},

        {"edu", json::array({{{"degree", "Bachelor of Technology"},
                              {"field", "Computer Science"},
                              {"university", "XYZ University"},
                              {"year", 2015}}})},
        {"work", json::array({{{"company", "ABC Corp"},
                               {"position", "Software Engineer"},
                               {"duration", "3 years"}}})},
        {"hobbies", json::array({"Coding", "reding"})}};
    printf("JSON Object:\n%s\n", testJson.dump(4).c_str());
    return 0;
}