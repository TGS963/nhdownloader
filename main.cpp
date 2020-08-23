#include <bits/stdc++.h>
#include <curl/curl.h>
//#include <libxml2/libxml/HTMLparser.h>
#include <fstream>

using namespace std;

void strfind(string &name, string &imglink, string &last, string &type){
  ifstream ifs("page", ios::in | ios::binary);
  string a;
  do{
    ifs >> a;
    //cout << a;
    if (a.find("title") < a.size()){
      name.append(a.substr(9));
      do{
        ifs >> a;
        name.append(" ");
        name.append(a);
      }while(a != "-");
      break;
    }
  }while(1);
  do {
    ifs >> a;
    //cout << a;
    if (a.find("num-pages") < a.size()){
      last = a.substr(18);
      break;
    }
  }while(1);
  do {
    ifs >> a;
    //cout << a;
    if (a.find("i.nhentai") < a.size()){
      imglink = a.substr(5);
      if (a.find("png") < a.size()){
        type = "png";
      }
      else{
        type = "jpg";
      }
      while (imglink[imglink.size()-1] != '/'){
        imglink.pop_back();
      }
      break;
    }
  }while(1);

  last = last.substr(0, last.find("</sp"));
  name.pop_back();
  name.pop_back();
  ifs.close();
  system("rm -f page");
}

void downloadimages(const char url[], const char name[]){
  CURL *curl;
  CURLcode res;
  FILE *img;



  curl_global_init(CURL_GLOBAL_ALL);

  curl = curl_easy_init();

  if (curl){
    img = fopen(name, "w");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, img);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK){
      cout << "error is " << curl_easy_strerror(res) << endl;
    }

    curl_easy_cleanup(curl);
  }
  fclose(img);
  curl_global_cleanup();
}

void generatehtml(const char *url){
  CURL *curl;
  CURLcode res;
  FILE *html;

  curl_global_init(CURL_GLOBAL_ALL);
  string a;

  curl = curl_easy_init();

  if (curl){
    html = fopen("page", "w");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, html);

    res = curl_easy_perform(curl);
    a = curl_easy_strerror(res);

    if (res != CURLE_OK){
      cout << "error is " << curl_easy_strerror(res) << endl;
    }
    else{
      //cout << strfind(a, "title", "Page");
    }

    curl_easy_cleanup(curl);
  }
  fclose(html);
  curl_global_cleanup();
}

int main(int argc, char *argv[]){
  if (argc <= 1){
    cout << "no numbers" << endl;
    return -1;
  }


  string numbers = argv[1];

  string name, imglink, last, type;

  numbers = "https://nhentai.net/g/" + numbers + "/1/";
  //cout << numbers.c_str();

  generatehtml(numbers.c_str());

  strfind(name, imglink, last, type);
  cout << "Name of doujin: " << name << endl << "Number of pages: " << last << endl;
  //cout << endl << name << endl << imglink << endl << last << endl;
  string mkdir = "mkdir '" + name + "'";
  system(mkdir.c_str());

  for (int i = 1; i <= stoi(last); ++i){
    string tmp, fln, cmd;
    if (type == "png"){
      tmp = imglink + to_string(i) + ".png";
      fln = to_string(i) + ".png";
      downloadimages(tmp.c_str(), fln.c_str());
    }
    else {
      tmp = imglink + to_string(i) + ".jpg";
      fln = to_string(i) + ".jpg";
      downloadimages(tmp.c_str(), fln.c_str());
    }
    cmd = "mv " + fln + " '" + name + "'";
    system(cmd.c_str());
  }
  cout << "Done!";
  return 0;
}
