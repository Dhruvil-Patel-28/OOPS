#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

const int MAX_OPTIONS = 4;

class Admin {
protected:
    string name;
    int id;
public:
    Admin(string n, int i) : name(n), id(i) {}
    virtual void displayRole() = 0;
    void displayInfo() {
        cout << "Name: " << name << endl;
        cout << "ID: " << id << endl;
    }
    virtual ~Admin() {}
};

class Student : public Admin {
private:
    int score;
public:
    Student(string n, int i, int s = 0) : Admin(n, i), score(s) {}
    void displayRole() override {
        cout << "Role: Student" << endl;
    }
    void displayScore() {
        cout << "Score: " << score << endl;
    }
    void setScore(int s) {
        score = s;
    }
    void saveToLeaderboard() {
        ofstream file("leaderboard.txt", ios::app);
        if (file.is_open()) {
            file << setw(20) << name << setw(10) << id << setw(10) << score << endl;
            file.close();
        } else {
            cout << "Unable to open leaderboard file for writing!" << endl;
        }
    }
};

class Quiz {
public:
    void addMultipleChoiceQuestion(string qText, string options[], int numOptions, int correctAns) {
        ofstream file("questions.txt", ios::app);
        if (file.is_open()) {
            file << "MCQ\n";
            file << qText << endl;
            for (int i = 0; i < numOptions; ++i) {
                file << options[i] << endl;
            }
            file << correctAns << endl;
            file.close();
            cout << "Question added successfully!\n";
        } else {
            cout << "Unable to open file for writing!\n";
        }
    }

    void addTrueFalseQuestion(string qText, int correctAns) {
        ofstream file("questions.txt", ios::app);
        if (file.is_open()) {
            file << "TF\n";
            file << qText << endl;
            file << correctAns << endl;
            file.close();
            cout << "Question added successfully!\n";
        } else {
            cout << "Unable to open file for writing!\n";
        }
    }

    int startQuiz() {
        ifstream file("questions.txt");
        if (!file.is_open()) {
            cout << "Unable to open file for reading!\n";
            return 0;
        }

        string line;
        int score = 0;

        while (getline(file, line)) {
            if (line == "MCQ") {
                string qText, options[MAX_OPTIONS];
                int correctAns, numOptions = MAX_OPTIONS;

                getline(file, qText);
                for (int i = 0; i < numOptions; ++i) {
                    getline(file, options[i]);
                }
                file >> correctAns;
                file.ignore();  // Ignore newline

                // Simulating what the user sees at runtime
                cout << "\n" << qText << endl;
                for (int i = 0; i < numOptions; ++i) {
                    cout << i + 1 << ". " << options[i] << endl;  // Displaying options
                }

                int answer;
                cout << "Your answer: ";  // Asking user for their answer
                cin >> answer;

                if (answer == correctAns) {
                    cout << "Correct!\n";  // If correct
                    score += 10;
                } else {
                    cout << "Wrong!\n";  // If incorrect
                }
                cout << endl;
            } else if (line == "TF") {
                string qText;
                int correctAns;

                getline(file, qText);
                file >> correctAns;
                file.ignore();  // Ignore newline

                // Simulating what the user sees for True/False question
                cout << "\n" << qText << endl;
                cout << "1. True\n2. False" << endl;

                int answer;
                cout << "Your answer: ";
                cin >> answer;

                if (answer == correctAns) {
                    cout << "Correct!\n";
                    score += 10;
                } else {
                    cout << "Wrong!\n";
                }
                cout << endl;
            }
        }

        file.close();
        return score;  // Return the total score after the quiz
    }

    void displayLeaderboard() {
        ifstream file("leaderboard.txt");
        if (file.is_open()) {
            cout << "\n------ Leaderboard ------\n";
            cout << setw(20) << "Name" << setw(10) << "ID" << setw(10) << "Score" << endl;
            cout << "----------------------------\n";
            string name;
            int id, score;

            while (file >> name >> id >> score) {
                cout << setw(20) << name << setw(10) << id << setw(10) << score << endl;
            }

            file.close();
        } else {
            cout << "Unable to open leaderboard file!\n";
        }
    }
};

void printBoxedText(string text) {
    int len = text.length() + 4;
    cout << "\n";
    for (int i = 0; i < len; i++) cout << "-";  // Top border
    cout << "\n| " << text << " |\n";  // Centered text
    for (int i = 0; i < len; i++) cout << "-";  // Bottom border
    cout << "\n";
}

void printBigQuizText() {
    // Printing large "QUIZ" title in ASCII art using boxes
    cout << "\n";
    cout << "  QQQQQ   U   U   III   ZZZZZ\n";
    cout << " Q     Q  U   U    I       Z\n";
    cout << " Q     Q  U   U    I      Z\n";
    cout << " Q   Q Q  U   U    I     Z\n";
    cout << "  QQQQQ   UUUUU   III   ZZZZZ\n";
    cout << "\n";
}


int main() {
    Quiz quiz;
    int choice;

    // Displaying the initial menu with a boxed title
    printBigQuizText();
    printBoxedText("Welcome to the Quiz Management System!");

    while (true) {
        // Main menu
        cout << "\n";
        printBoxedText("1. Add a Question");
        printBoxedText("2. Start the Quiz");
        printBoxedText("3. View Leaderboard");
        printBoxedText("4. Exit");

        cout << "\nEnter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string qText, options[MAX_OPTIONS];
            int type, correctAns;

            cout << "Enter 1 for MCQ, 2 for True/False: ";
            cin >> type;
            cin.ignore();  // Ignore newline

            if (type == 1) {
                cout << "Enter the question: ";
                getline(cin, qText);
                cout << "Enter the options:\n";
                for (int i = 0; i < MAX_OPTIONS; ++i) {
                    cout << "Option " << i + 1 << ": ";
                    getline(cin, options[i]);
                }
                cout << "Enter the number of the correct option (1-" << MAX_OPTIONS << "): ";
                cin >> correctAns;

                quiz.addMultipleChoiceQuestion(qText, options, MAX_OPTIONS, correctAns);
            } else if (type == 2) {
                cout << "Enter the True/False question: ";
                getline(cin, qText);
                cout << "Enter 1 for True, 2 for False: ";
                cin >> correctAns;

                quiz.addTrueFalseQuestion(qText, correctAns);
            }
        } else if (choice == 2) {
            string studentName;
            int studentID;

            cout << "Enter your name: ";
            cin.ignore();
            getline(cin, studentName);
            cout << "Enter your student ID: ";
            cin >> studentID;

            Student student(studentName, studentID);

            student.displayInfo();
            student.displayRole();

            int score = quiz.startQuiz();
            student.setScore(score);

            cout << "Quiz completed! Your score: " << score << endl;

            student.saveToLeaderboard();
        } else if (choice == 3) {
            quiz.displayLeaderboard();
        } else if (choice == 4) {
            cout << "Exiting the system...\n";
            break;
        } else {
            cout << "Invalid choice, please try again!\n";
        }
    }

    return 0;
}