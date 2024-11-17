#include <iostream>
#include <string>
using namespace std;

const int MAX_QUESTIONS = 100;  // Maximum number of questions in the quiz
const int MAX_OPTIONS = 4;      // Maximum number of options for multiple choice questions

// --- Admin, Student, and Teacher classes ---

// Abstract class Admin
class Admin {
protected:
    string name;  // Name of person
    int id;       // ID of person
public:
    // Constructor to initialize name and ID
    Admin(string n, int i) : name(n), id(i) {}

    // Virtual function for role, to be implemented by derived classes
    virtual void displayRole() = 0;

    // Function to display basic info (name and ID)
    void displayInfo() {
        cout << "Name: " << name << endl;
        cout << "ID: " << id << endl;
    }

    // Virtual destructor for proper cleanup
    virtual ~Admin() {}
};

// Student class inherits from Admin
class Student : public Admin {
private:
    int score;  // Student's quiz score
public:
    // Constructor to initialize student info
    Student(string n, int i, int s = 0) : Admin(n, i), score(s) {}

    // Display student role
    void displayRole() override {
        cout << "Role: Student" << endl;
    }

    // Display student's quiz score
    void displayScore() {
        cout << "Score: " << score << endl;
    }

    // Function to set the score after taking a quiz
    void setScore(int s) {
        score = s;
    }
};

// Teacher class inherits from Admin
class Teacher : public Admin {
private:
    string subject;  // Subject taught by teacher
public:
    // Constructor to initialize teacher info
    Teacher(string n, int i, string sub) : Admin(n, i), subject(sub) {}

    // Display teacher role
    void displayRole() override {
        cout << "Role: Teacher" << endl;
    }

    // Display the subject taught by the teacher
    void displaySubject() {
        cout << "Subject: " << subject << endl;
    }
};

// --- Question, MultipleChoiceQuestion, and TrueFalseQuestion classes ---

// Abstract class Question
class Question {
protected:
    string questionText;  // Text of the question
public:
    // Constructor to initialize question text
    Question(string qText) : questionText(qText) {}

    // Virtual functions to be implemented by derived classes
    virtual void displayQuestion() = 0;
    virtual bool checkAnswer(int answer) = 0;

    // Virtual destructor
    virtual ~Question() {}
};

// MultipleChoiceQuestion inherits from Question
class MultipleChoiceQuestion : public Question {
private:
    string options[MAX_OPTIONS];  // Array of multiple-choice options
    int correctAnswer;            // Index of the correct answer
    int numOptions;               // Number of options for this question
public:
    // Constructor to initialize the question, options, and correct answer
    MultipleChoiceQuestion(string qText, string opts[], int numOpts, int correctAns)
        : Question(qText), numOptions(numOpts), correctAnswer(correctAns) {
        for (int i = 0; i < numOptions; ++i) {
            options[i] = opts[i];
        }
    }

    // Display the question and options
    void displayQuestion() override {
        cout << questionText << endl;
        for (int i = 0; i < numOptions; ++i) {
            cout << i + 1 << ". " << options[i] << endl;
        }
    }

    // Check if the answer is correct
    bool checkAnswer(int answer) override {
        return answer == correctAnswer;
    }
};

// TrueFalseQuestion inherits from Question
class TrueFalseQuestion : public Question {
private:
    int correctAnswer;  // 1 for True, 2 for False
public:
    // Constructor to initialize question and correct answer
    TrueFalseQuestion(string qText, int correctAns)
        : Question(qText), correctAnswer(correctAns) {}

    // Display the question with True/False options
    void displayQuestion() override {
        cout << questionText << endl;
        cout << "1. True\n2. False" << endl;
    }

    // Check if the answer is correct
    bool checkAnswer(int answer) override {
        return answer == correctAnswer;
    }
};

// --- Quiz Class to manage the quiz ---

class Quiz {
private:
    Question* questions[MAX_QUESTIONS];  // Array of question pointers
    int questionCount;  // Track the number of questions added
public:
    // Constructor to initialize the question count
    Quiz() : questionCount(0) {}

    // Add a question to the quiz
    void addQuestion(Question* q) {
        if (questionCount < MAX_QUESTIONS) {
            questions[questionCount++] = q;
        } else {
            cout << "Question limit reached!" << endl;
        }
    }

    // Start the quiz for a student
    int startQuiz() {
        int score = 0;
        for (int i = 0; i < questionCount; ++i) {
            questions[i]->displayQuestion();
            int answer;
            cout << "Your answer: ";
            cin >> answer;
            if (questions[i]->checkAnswer(answer)) {
                cout << "Correct!\n";
                score += 10;  // Add 10 points for each correct answer
            } else {
                cout << "Wrong!\n";
            }
            cout << endl;
        }
        return score;  // Return the total score
    }

    // Destructor to clean up dynamic memory
    ~Quiz() {
        for (int i = 0; i < questionCount; ++i) {
            delete questions[i];
        }
    }
};

// --- Main Program ---

int main() {
    Quiz quiz;  // Create a quiz object
    int choice;

    cout << "Welcome to the Quiz Management System!" << endl;

    while (true) {
        cout << "\n1. Admin Mode\n2. Student Mode\n3. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        if (choice == 1) {
            int adminChoice;
            cout << "Admin Mode:\n1. Add Multiple Choice Question\n2. Add True/False Question\n";
            cout << "Choose an option: ";
            cin >> adminChoice;

            if (adminChoice == 1) {
                string qText;
                int numOptions, correctAns;
                string options[MAX_OPTIONS];

                cout << "Enter question text: ";
                cin.ignore();
                getline(cin, qText);
                cout << "Enter number of options (up to " << MAX_OPTIONS << "): ";
                cin >> numOptions;
                cin.ignore();

                for (int i = 0; i < numOptions; ++i) {
                    cout << "Enter option " << i + 1 << ": ";
                    getline(cin, options[i]);
                }

                cout << "Enter the correct option number: ";
                cin >> correctAns;

                quiz.addQuestion(new MultipleChoiceQuestion(qText, options, numOptions, correctAns));
                cout << "Question added successfully!\n";
            } else if (adminChoice == 2) {
                string qText;
                int correctAns;

                cout << "Enter True/False question text: ";
                cin.ignore();
                getline(cin, qText);
                cout << "Enter the correct answer (1 for True, 2 for False): ";
                cin >> correctAns;

                quiz.addQuestion(new TrueFalseQuestion(qText, correctAns));
                cout << "Question added successfully!\n";
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

            int score = quiz.startQuiz();  // Start the quiz and get the score
            student.setScore(score);       // Set the student's score

            cout << "Quiz completed! Your score: " << score << endl;
        } else if (choice == 3) {
            cout << "Exiting the system..." << endl;
            break;
        } else {
            cout << "Invalid choice, please try again!" << endl;
        }
    }

    return 0;
}
