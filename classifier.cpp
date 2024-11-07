#include <iostream>
#include <fstream>
#include <string>
#include "csvstream.hpp"
#include <cmath>
#include <set>

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

        // REQUIRES: number of training posts >= 0, label is nonempty
        // MODIFIES: nothing
        // EFFECTS: returns a double with either log-prior or log-likelihood
        double calc_log(string label, bool prior, string word="") {
            if (prior) {
                return log((double)posts_per_label[label] / num_posts);
            } else {
                if (word_and_label.find({label,word}) == word_and_label.end()) {
                    if (posts_per_word.find(word) == posts_per_word.end()) {
                        return log((double)1/num_posts);
                    } else {
                        return log((double)posts_per_word[word] / num_posts);
                    }
                }
            return log((double)word_and_label[{label, word}] / posts_per_label[label]);
            }
        }

    public:
        set<string> unique_words(const string &str) {
            istringstream source(str);
            set<string> words;
            string word;
            while (source >> word) {
                words.insert(word);
            }
            return words;
        }
        Classifier(int num_posts_in, vector<string> tags_in, vector<string> contents_in)
         : num_posts(num_posts_in), tags(tags_in), contents(contents_in){
            for (size_t i = 0; i < num_posts; i++) {
                if (posts_per_label.find(tags[i]) == posts_per_label.end()) {
                    posts_per_label[tags[i]] = 1;
                } else {
                    posts_per_label[tags[i]]++;
                }
                set<string> words = unique_words(contents[i]);
                for (auto it = words.begin(); it != words.end(); ++it) {
                    if (posts_per_word.find(*it) == posts_per_word.end()) {
                    posts_per_word[*it] = 1;
                    } else {
                        posts_per_word[*it]++;
                    }
                    if (word_and_label.find({tags[i],*it}) == word_and_label.end()) {
                        word_and_label[{tags[i],*it}] = 1;
                    } else {
                        word_and_label[{tags[i],*it}]++;
                    }
                }
                    
            }
            vocab_size = posts_per_word.size();
        };

        
        
        // REQUIRES: tags and contents are not empty
        // MODIFIES: nothing
        // EFFECTS: prints out information about the input training data
        void print_training_data() {
            cout << "training data:" << endl;
            for (size_t i = 0; i < num_posts; i++) {
                cout << "  label = " << tags[i] << ", content = " << contents[i] << endl;
            }
            cout << endl;
            cout << "trained on " << num_posts << " examples" << endl;
            cout << "vocabulary size = " << vocab_size << endl;
            cout << "classes:" << endl;
            for (const auto& pair : posts_per_label) {
                cout << "  " << pair.first << ", " << pair.second << 
                " examples, log-prior = " << calc_log(pair.first, true) << endl;
            }
            cout << "classifier parameters:" << endl;
            for (const auto& pair : word_and_label) {
                cout << "  " << pair.first.first << ":" << pair.first.second << 
                ", count = " << pair.second << ", log-likelihood = " << 
                calc_log(pair.first.first, false, pair.first.second) << endl;
            }
        }


};

int main(int argc, char* argv[]) {
  cout.precision(3);
  if (argc != 2 && argc != 3) {
    cout << "Usage: classifier.exe TRAIN_FILE [TEST_FILE]" << endl;
    return 1;
  }

  string training = argv[1];
  string testing;
  csvstream csv1(training);
  vector<string> header = csv1.getheader();
  vector<string> tags;
  vector<string> contents;

  int num_posts = 0;
  map<string, string> row;
  while (csv1 >> row) {
    tags.push_back(row["tag"]);
    contents.push_back(row["content"]);
    num_posts++;
    }

Classifier andy = Classifier(num_posts, tags, contents);


// for testing
  if (argc == 3) {
    ifstream fin2;
    testing = argv[2];
    fin2.open(testing);
    if (!fin2.is_open()) {
        cout << "Error opening file: " << testing << endl;
        return 1;
    }
    csvstream csv2(fin2);
  }
andy.print_training_data();

}