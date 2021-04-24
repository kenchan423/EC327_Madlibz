#include <cstdio>
#include <iostream>
#include <fstream>

#include <string>
#include <vector>

#include <curl/curl.h>

#include "json.hpp"
using nlohmann::json;

#include <SFML/Graphics.hpp>

using std::string;
using std::vector;
using std::cout;
using std::cin;

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
  string madlibz_url = "http://madlibz.herokuapp.com/api/random?minlength=5&maxlength=10";
  // will ask for user input on minlength & maxlength ==> may be add a cool user input button

  string resp;
  resp = get_url_contents(madlibz_url);
  // printing out resp to see if call to api works
  //cout << resp << '\n';
  json current_madlibz = json::parse(resp);
  //cout << current_madlibz << '\n';
  vector<string> user_inputs = current_madlibz["blanks"];
  int num_output = current_madlibz["value"].size();
  vector<string> value_str;
  // the last part of value is always a 0 
  // b/c 0 at end of cstrings
  for (int k=0; k<num_output-1; k++){
    value_str.push_back(current_madlibz["value"].at(k));
  }
  
  // not always strings --> so used auto
  string title_str = current_madlibz["title"];

  cout << resp << '\n';
  for(int p=0; p<num_output-1; p++){
    cout << value_str[p] << '\n';
  }

  all_madlibz.push_back(current_madlibz);

  // saving madlibz into library --> could do an all previous madlibz option
  std::ofstream mylib;
  mylib.open("madlibz.json");
  mylib << all_madlibz << '\n';
  mylib.close();

  // front end portion

  sf::RenderWindow window(sf::VideoMode(1000,1000), "SFML works!");
  sf::Text blanks;
  sf::Text values;
  sf::Text title;
  // font of the blanks
  sf::Font font;
  font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-BI.ttf");
  blanks.setFont(font);
  // for loop to print out each blank individually
  int total_entries = user_inputs.size();
  string user_inputs_str;
  for (int i=0; i<total_entries; i++){
    user_inputs_str.append(user_inputs.at(i));
    user_inputs_str.append(": ");
    user_inputs_str.append("\n");
  }

  blanks.setString(user_inputs_str);

  // customizing text appearance
  blanks.setFillColor(sf::Color::White);
  blanks.setOutlineColor(sf::Color::Blue);
  blanks.setOutlineThickness(2);

  window.setFramerateLimit(10);
  while (window.isOpen())
  {
    blanks.setPosition(100,100);
    window.draw(blanks);
    window.display();
  }


  return 0;
}
