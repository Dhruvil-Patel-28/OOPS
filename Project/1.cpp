#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <chrono>
#include <thread>
using namespace std;

class Question {
protected:
    string questionText;
    vector<string> options;
    int correctAnswer;
public:
    virtual void askQuestion() = 0;
    virtual bool checkAnswer(int answer) = 0;
    virtual ~Question() {}
};

class MultipleChoiceQuestion : public Question {
public:
    MultipleChoiceQuestion(string qText, vector<string> opts, int correctAns) {
        questionText = qText;
        options = opts;
        correctAnswer = correctAns;
    }

    void askQuestion() override {
        cout << questionText << endl;
        for (size_t i = 0; i < options.size(); ++i) {
            cout << i + 1 << ". " << options[i] << endl;
        }
    }

    bool checkAnswer(int answer) override {
        return answer == correctAnswer;
    }
};

class TrueFalseQuestion : public Question {
public:
    TrueFalseQuestion(string qText, int correctAns) {
        questionText = qText;
        options = {"True", "False"};
        correctAnswer = correctAns;
    }

    void askQuestion() override {
        cout << questionText << endl;
        cout << "1. True\n2. False" << endl;
    }

    bool checkAnswer(int answer) override {
        return answer == correctAnswer;
    }
};

class Quiz {
private:
    vector<Question*> questions;
    int score;
    string playerName;

public:
    Quiz(string name) : score(0), playerName(name) {}

    void addQuestion(Question* q) {
        questions.push_back(q);
    }

    void startQuiz() {
        auto startTime = chrono::steady_clock::now();
        for (auto& q : questions) {
            q->askQuestion();
            int answer;
            cout << "Your answer: ";
            cin >> answer;
            if (q->checkAnswer(answer)) {
                cout << "Correct!\n";
                score += 10;
            } else {
                cout << "Wrong!\n";
            }
        }
        auto endTime = chrono::steady_clock::now();
        auto elapsed = chrono::duration_cast<chrono::seconds>(endTime - startTime);
        cout << "\nQuiz finished in " << elapsed.count() << " seconds!" << endl;

        saveScore();
    }

    void saveScore() {
        ofstream file("leaderboard.txt", ios::app);
        if (file.is_open()) {
            file << playerName << " " << score << endl;
            file.close();
        } else {
            cout << "Unable to save score!" << endl;
        }
    }

    ~Quiz() {
        for (auto& q : questions) {
            delete q;
        }
    }
};

class Admin {
public:
    void addQuestions(Quiz& quiz) {
        string qText;
        int correctAns, numOptions;
        cout << "Enter multiple-choice question: ";
        cin.ignore();
        getline(cin, qText);
        cout << "Enter number of options: ";
        cin >> numOptions;

        vector<string> options(numOptions);
        for (int i = 0; i < numOptions; ++i) {
            cout << "Option " << i + 1 << ": ";
            cin.ignore();
            getline(cin, options[i]);
        }

        cout << "Enter correct option number: ";
        cin >> correctAns;
        quiz.addQuestion(new MultipleChoiceQuestion(qText, options, correctAns));
        cout << "Question added successfully!\n";
    }
};

void viewLeaderboard() {
    ifstream file("leaderboard.txt");
    string line;
    if (file.is_open()) {
        cout << "\n--- Leaderboard ---\n";
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    } else {
        cout << "Unable to open leaderboard!" << endl;
    }
}

int main() {
    int choice;
    cout << "Welcome to the Quiz Management System!" << endl;
    cout << "1. Admin Mode\n2. Player Mode\n3. View Leaderboard\n4. Exit\n";
    cout << "Choose an option: ";
    cin >> choice;

    if (choice == 1) {
        Admin admin;
        Quiz dummyQuiz(""); // Creating a dummy quiz object
        admin.addQuestions(dummyQuiz);
    } else if (choice == 2) {
        string name;
        cout << "Enter your name: ";
        cin.ignore();
        getline(cin, name);

        Quiz quiz(name);
        quiz.addQuestion(new MultipleChoiceQuestion("What is 2 + 2?", {"1", "2", "4", "5"}, 3));
        quiz.addQuestion(new TrueFalseQuestion("The Earth is flat.", 2));
        
        quiz.startQuiz();
    } else if (choice == 3) {
        viewLeaderboard();
    } else {
        cout << "Exiting the system." << endl;
    }

    return 0;
}
