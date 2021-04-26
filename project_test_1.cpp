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


int main() {
  json all_madlibz;
  // will ask for user input on minlength & maxlength ==> may be add a cool user input button
  // connect these to buttons?
  vector<string> max_min;
  max_min.push_back("Maximum");
  max_min.push_back("Minimum"); 
  vector<string> answers1;
  sf::RenderWindow window1(sf::VideoMode(800, 800), "How big of a Madlibz?");
  sf::RectangleShape rectangle1(sf::Vector2f(110.f, 50.f));
  rectangle1.setFillColor(sf::Color::White);
  rectangle1.setPosition(350, 600);

  sf::RectangleShape input_box1(sf::Vector2f(110.f, 50.f));
  input_box1.setFillColor(sf::Color::White);
  input_box1.setPosition(350, 300);

  sf::Font font1;
  font1.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-BI.ttf");
  sf::Text text_submit1;
  text_submit1.setFont(font1);
  text_submit1.setString("Submit");
  text_submit1.setFillColor(sf::Color::Blue);
  text_submit1.setStyle(sf::Text::Bold);
  text_submit1.setPosition(350,600);
  window1.setFramerateLimit(10);

  string input_text1;

  sf::Text text_input1;
  text_input1.setFont(font1);
  text_input1.setString(input_text1);
  text_input1.setFillColor(sf::Color::Blue);
  text_input1.setStyle(sf::Text::Bold);
  text_input1.setPosition(350,300);


  int length_counter = 0;
  sf::Text header_text1;
  header_text1.setFont(font1);
  header_text1.setString(max_min[length_counter]);
  header_text1.setFillColor(sf::Color::Red);
  header_text1.setStyle(sf::Text::Bold);
  header_text1.setPosition(350,200);
  while (window1.isOpen())
  {
      sf::Event event1;
      while (window1.pollEvent(event1))
      {
          if (event1.type == sf::Event::Closed)
              window1.close();
          // if something typed
          else if (event1.type == sf::Event::TextEntered) {
              if (std::isprint(event1.text.unicode) && length_counter < max_min.size()-1){
                  input_text1 += event1.text.unicode;
                   text_input1.setString(input_text1);
              }
          }
          // if backspace
          else if (event1.type == sf::Event::KeyPressed) {
              if (event1.key.code == sf::Keyboard::BackSpace) {
                  if (!input_text1.empty()) {
                      input_text1.pop_back();
                      text_input1.setString(input_text1);
                    }
              }
              // entering an input
              if (event1.key.code == sf::Keyboard::Return) {
                      if (length_counter < max_min.size()-1){
                      answers1.push_back(input_text1);
                      text_input1.setString("");
                      length_counter++;
                      header_text1.setString(max_min.at(length_counter));
                      input_text1 = "";
                    }
              }
            }
          if (event1.type == sf::Event::MouseButtonPressed){
            double x1 = event1.mouseButton.x;
            double y1 = event1.mouseButton.y;
            if (x1 >= 350 && x1 <= 460) {
              if (y1 >= 600 && y1 <=650) {
                  window1.close();
              }
            }
          }
      }
      window1.clear();
      if (length_counter < max_min.size()-1){
        window1.draw(header_text1);
      }
      window1.draw(rectangle1);
      window1.draw(text_submit1);
      window1.draw(input_box1);
      window1.draw(text_input1);
      window1.display();
  }

  string tempmin = answers1.at(0);
  string tempmax = answers1.at(1);
  string minlength = "minlength=" + tempmin;
  string maxlength = "maxlength=" + tempmax;
  string user_input_length = "?" + minlength + "&" + maxlength;
  string madlibz_url = "http://madlibz.herokuapp.com/api/random"+user_input_length;

  string resp;
  resp = get_url_contents(madlibz_url);
  // printing out resp to see if call to api works
  //cout << resp << '\n';
  json current_madlibz = json::parse(resp);
  //cout << current_madlibz << '\n';
  vector<string> questions = current_madlibz["blanks"];
  int num_output = questions.size();
  vector<string> value_str;
  // the last part of value is always a 0 
  // b/c 0 at end of cstrings
  for (int k=0; k<num_output-1; k++){
    value_str.push_back(current_madlibz["value"].at(k));
  }

  // not always strings --> so used auto
  string title_str = current_madlibz["title"];
  /*
  cout << resp << '\n';
  for(int p=0; p<num_output-1; p++){
    cout << value_str[p] << '\n';
  }
  */
  
  // terminal stuff
  string temp_str;
  for (int o=0; o<num_output-1; o++){
    cout << questions.at(o) << '\n';
  }
  
  /*
  for (int p=0; p<blank_input.size()-1; p++){
    cout << blank_input.at(p) << '\n';
  }
  */
  
  

  all_madlibz.push_back(current_madlibz);

  // saving madlibz into library --> could do an all previous madlibz option
  std::ofstream mylib;
  mylib.open("madlibz.json");
  mylib << all_madlibz << '\n';
  mylib.close();
  // seems to overwrite the madlibz.json each time --> want to add to the file

  // cant figure out how to read from madlibz.json
  json prev_madlibz;

  std::ifstream mylib_rd("madlibz.json", std::ifstream::binary);
  mylib_rd >> prev_madlibz;
  //cout << prev_madlibz << '\n';
  // printing out all previous madlibz --> another button



  // front end portion
  
  vector <string> answers;
  // populate with inputs from API

  sf::RenderWindow window(sf::VideoMode(800, 800), "Madlibz Lite User Input");
  sf::RectangleShape rectangle(sf::Vector2f(110.f, 50.f));
  rectangle.setFillColor(sf::Color::White);
  rectangle.setPosition(350, 600);

  sf::RectangleShape input_box(sf::Vector2f(110.f, 50.f));
  input_box.setFillColor(sf::Color::White);
  input_box.setPosition(350, 300);


  sf::Font font;
  font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-BI.ttf");
  sf::Text text_submit;
  text_submit.setFont(font);
  text_submit.setString("Submit");
  text_submit.setFillColor(sf::Color::Blue);
  text_submit.setStyle(sf::Text::Bold);
  text_submit.setPosition(350,600);
  window.setFramerateLimit(10);

  sf::Text title;
  title.setFont(font);
  title.setString(title_str);
  title.setPosition(350,100);

  string input_text;


  sf::Text text_input;
  text_input.setFont(font);
  text_input.setString(input_text);
  text_input.setFillColor(sf::Color::Blue);
  text_input.setStyle(sf::Text::Bold);
  text_input.setPosition(350,300);


  int question_counter = 0;
  sf::Text header_text;
  header_text.setFont(font);
  header_text.setString(questions[question_counter]);
  header_text.setFillColor(sf::Color::Red);
  header_text.setStyle(sf::Text::Bold);
  header_text.setPosition(350,200);
  while (window.isOpen())
  {
      sf::Event event;
      while (window.pollEvent(event))
      {
          if (event.type == sf::Event::Closed)
              window.close();
          // if something typed
          else if (event.type == sf::Event::TextEntered) {
              if (std::isprint(event.text.unicode) && question_counter < questions.size()-1){
                  input_text += event.text.unicode;
                   text_input.setString(input_text);
              }
          }
          // if backspace
          else if (event.type == sf::Event::KeyPressed) {
              if (event.key.code == sf::Keyboard::BackSpace) {
                  if (!input_text.empty()) {
                      input_text.pop_back();
                      text_input.setString(input_text);
                    }
              }
              // entering an input
              if (event.key.code == sf::Keyboard::Return) {
                      if (question_counter < questions.size()-1){
                      answers.push_back(input_text);
                      text_input.setString("");
                      question_counter++;
                      header_text.setString(questions.at(question_counter));
                      input_text = "";
                    }
              }
            }
          if (event.type == sf::Event::MouseButtonPressed){
            double x = event.mouseButton.x;
            double y = event.mouseButton.y;
            if (x >= 350 && x <= 460) {
              if (y >= 600 && y <=650) {
                  window.close();
              }
            }
          }
      }
      window.clear();
      if (question_counter < questions.size()-1){
        window.draw(header_text);
      }
      window.draw(rectangle);
      window.draw(text_submit);
      window.draw(input_box);
      window.draw(text_input);
      window.draw(title);
      window.display();
  }

  
  // making the madlibz
  string value_temp;
  vector<string> final_madlibz;
  for (int q=0; q<answers.size(); q++){
    value_temp = value_str.at(q) + answers.at(q);
    final_madlibz.push_back(value_temp);
    value_temp.clear();
  }
  string madlibz_lite;
  for (int c=0; c<answers.size(); c++){
    // cout << final_madlibz.at(c) << '\n';
    value_temp.clear();
    value_temp = final_madlibz.at(c) + '\n';
    
    if (value_temp.length() > 40){
      madlibz_lite.append(value_temp.substr(0,20));
      cout << value_temp.substr(0,20);
      madlibz_lite.append("\n");
      madlibz_lite.append(value_temp.substr(21, value_temp.length()-1));
      cout << value_temp.substr(21, value_temp.length()-1);
      continue; 
    }
    
    madlibz_lite.append(value_temp);
  }
  cout << madlibz_lite;
  
  // 2nd terminal --> displaying the final madlibz
  sf::RenderWindow window2(sf::VideoMode(1000, 800), "Madlibz Lite");
  sf::Text madlibz_str;

  madlibz_str.setString(madlibz_lite);
  madlibz_str.setFont(font);
  madlibz_str.setPosition(350,100);
  // can try to do if statements and make this more responsive
  madlibz_str.setCharacterSize(15);

  // customizing text appearance
  madlibz_str.setFillColor(sf::Color::Red);
  madlibz_str.setOutlineColor(sf::Color::Blue);
  madlibz_str.setOutlineThickness(2);
  madlibz_str.setStyle(sf::Text::Bold);

  window.setFramerateLimit(10);
  while (window2.isOpen())
  {
    madlibz_str.setPosition(100,100);
    window2.draw(madlibz_str);
    window2.display();
  }
  


  return 0;
}
