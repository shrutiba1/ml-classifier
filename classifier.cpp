#include <iostream>
#include <fstream>
#include <string>
#include "csvstream.hpp"

using namespace std;

class Classifier {
    private:
        int num_posts;
        int vocab_size;
        vector<string> tags;
        vector<string> contents;
        map<string, int> posts_per_word;
        map<string, int> posts_per_label;
        map<pair<string, string>, int> word_and_label;

    public:
        Classifier(int num_posts_in, vector<string> tags_in, vector<string> contents_in)
         : num_posts(num_posts_in), tags(tags_in), contents(contents_in){
            // code to create the other variables
        };

        // REQUIRES: tags and contents are not empty
        // MODIFIES: nothing
        // EFFECTS: prints out information about the input training data
        void print_training_data() {
            cout << "training data:" << endl;
            for (size_t i = 0; i < num_posts; i++) {
                // print data like in the correct file
            }
        }
};

int main(int argc, char* argv[]) {
  cout.precision(3);
  if (argc != 2 || argc != 3) {
    cout << "Usage: classifier.exe TRAIN_FILE [TEST_FILE]" << endl;
    return 1;
  }

  string training = argv[1];
  string testing;
  csvstream csv1(training);
  ifstream fin2;
  // csvstream throws an error by itself so do we still need to return 1
  /*if (!csv1.is_open()) {
    cout << "Error opening file: " << training << endl;
    return 1;
  }*/
  if (argc == 3) {
    testing = argv[2];
    fin2.open(testing);
    if (!fin2.is_open()) {
        cout << "Error opening file: " << testing << endl;
        return 1;
    }
  }
csvstream csv2(fin2);
vector<string> header = csv1.getheader();
int num_posts = stoi(header[0]);
vector<string> tags;
vector<string> contents;
for (size_t i = 0; i < num_posts; i++) {
    map<string, string> row;
    csv1 >> row;
    tags.push_back(row["tag"]);
    contents.push_back(row["content"]);
}
Classifier andy = Classifier(num_posts, tags, contents);

}