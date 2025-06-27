//============================================================================
// Name        : Project Two
// Author      : Jordan Bankston
// Version     : 1.0
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Course {
    string courseId;
    string courseName;
    vector<string> preList;
};

class BinarySearchTree {
private:
    struct Node {
        Course course;
        Node* left;
        Node* right;
        Node(Course aCourse) : course(aCourse), left(nullptr), right(nullptr) {}
    };

    Node* root;
    void inOrder(Node* node);
    int size = 0;

public:
    BinarySearchTree() : root(nullptr) {}
    void Insert(Course aCourse);
    Course Search(string courseId);
    void InOrder() { inOrder(root); }
    int Size() { return size; }
};

void BinarySearchTree::Insert(Course aCourse) {
    Node** current = &root;
    while (*current) {
        if (aCourse.courseId < (*current)->course.courseId) {
            current = &((*current)->left);
        } else {
            current = &((*current)->right);
        }
    }
    *current = new Node(aCourse);
    ++size;
}

Course BinarySearchTree::Search(string courseId) {
    Node* current = root;
    while (current) {
        if (current->course.courseId == courseId) return current->course;
        current = (courseId < current->course.courseId) ? current->left : current->right;
    }
    return {};
}

void BinarySearchTree::inOrder(Node* node) {
    if (!node) return;
    inOrder(node->left);
    cout << node->course.courseId << ", " << node->course.courseName << endl;
    inOrder(node->right);
}

vector<string> Split(const string& line, char delim = ',') {
    vector<string> tokens;
    string token;
    for (char ch : line) {
        if (ch == delim) {
            tokens.push_back(token);
            token.clear();
        } else {
            token += ch;
        }
    }
    if (!token.empty()) tokens.push_back(token);
    return tokens;
}

void loadCourses(const string& filename, BinarySearchTree* bst) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        auto tokens = Split(line);
        if (tokens.size() >= 2) {
            Course course;
            course.courseId = tokens[0];
            course.courseName = tokens[1];
            for (size_t i = 2; i < tokens.size(); ++i) {
                course.preList.push_back(tokens[i]);
            }
            bst->Insert(course);
        }
    }
    file.close();
}

void displayCourse(const Course& course) {
    cout << course.courseId << ", " << course.courseName << endl;
    cout << "Prerequisites: ";
    if (course.preList.empty()) {
        cout << "None";
    } else {
        for (size_t i = 0; i < course.preList.size(); ++i) {
            cout << course.preList[i];
            if (i < course.preList.size() - 1) cout << ", ";
        }
    }
    cout << endl;
}

void toUpper(string& s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);
}

int main() {
    BinarySearchTree courseTree;
    int choice = 0;
    string filename, inputId;

    while (choice != 9) {
        cout << "\nMenu:\n";
        cout << "  1. Load Courses\n";
        cout << "  2. Display All Courses\n";
        cout << "  3. Find Course\n";
        cout << "  9. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter filename to load: ";
                cin >> filename;
                loadCourses(filename, &courseTree);
                cout << courseTree.Size() << " courses loaded.\n";
                break;
            case 2:
                courseTree.InOrder();
                break;
            case 3:
                cout << "Enter course ID: ";
                cin >> inputId;
                toUpper(inputId);
                {
                    Course found = courseTree.Search(inputId);
                    if (!found.courseId.empty())
                        displayCourse(found);
                    else
                        cout << "Course not found.\n";
                }
                break;
            case 9:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}
