#include <cstdio>
#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"
using nlohmann::json;

using std::string;
using std::cout;

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
  data->append((char*) ptr, size * nmemb);
  return size * nmemb;
}

string get_url_contents(string theurl) {
  CURL *curl;
  CURLcode res;
  string response_string;
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, theurl.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return response_string;
}


int main(int argc, char** argv) {
  json all_madlibz;
  string madlibz_url = "http://madlibz.herokuapp.com/api/random";

  string resp;
  resp = get_url_contents(madlibz_url);
  // printing out resp to see if call to api works
  cout << resp << '\n';
  all_madlibz.push_back(json::parse(resp));
  
  // saving madlibz into library -->  

  std::ofstream mylib;
  mylib.open("madlibz.json");
  mylib << all_madlibz;
  mylib.close();

  return 0;
}
