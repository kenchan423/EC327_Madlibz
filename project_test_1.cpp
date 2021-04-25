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
  // will ask for user input on minlength & maxlength ==> may be add a cool user input button
  // connect these to buttons?
  cout << "How big of a madlibz do you want?\n";
  cout << "Minimum Length: ";
  int min_len;
  cin >> min_len;
  cout << "Maximum Length: ";
  int max_len;
  cin >> max_len;
  //try{
    string minlength = "minlength=" + std::to_string(min_len);
    string maxlength = "maxlength=" + std::to_string(max_len);
    //?minlength=5&maxlength=10
    string user_input_length = "?" + minlength + "&" + maxlength;
    //cout << user_input_length << '\n';
    string madlibz_url = "http://madlibz.herokuapp.com/api/random"+user_input_length;
    // catch error --> if the user input min > 1 & max < 10 
    // --> no templates fulfill that --> error
  //}
  //catch(int max_len, int min_len){
  //  cout << "There are no templates that meet the maximum length: "
  //  + max_len " and minimum length: " + min_len + " desired";
  //}
  

  //cout << madlibz_url << '\n';

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
  /*
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
  */

  vector <string> answers;
  // populate with inputs from API

  sf::RenderWindow window(sf::VideoMode(800, 800), "Madlibz Lite");
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
                      cout << question_counter << '\n';
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
      window.draw(header_text);
      window.draw(rectangle);
      window.draw(text_submit);
      window.draw(input_box);
      window.draw(text_input);
      window.draw(title);
      window.display();
  }
  for (int i=0; i<answers.size(); i++) cout<<answers[i]<<'\n';

  
  // making the madlibz
  string value_temp;
  vector<string> final_madlibz;
  for (int q=0; q<answers.size()-1; q++){
    value_temp = value_str.at(q) + answers.at(q);
    final_madlibz.push_back(value_temp);
    value_temp.clear();
  }

  for (int c=0; c<answers.size()-1; c++){
    cout << final_madlibz.at(c) << '\n';
  }
  

  return 0;
}
