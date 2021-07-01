#define NOMINMAX
#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <conio.h>
#include "zoom.h" // Custom made Zoom header for Zoom client launch-related functions

using namespace std;

const int ENTER = 13;

void trim(string& word) // Trims whitespaces from both sides https://stackoverflow.com/a/49253841
{
    if (word.empty()) return;

    // Trim spaces from left side
    while (word.find(" ") == 0)
    {
        word.erase(0, 1);
    }

    // Trim spaces from right side
    size_t len = word.size();
    while (word.rfind(" ") == --len)
    {
        word.erase(len, len + 1);
    }
}

template <typename T>
void remove_duplicates(vector<T>& vec) { // Removes duplicated occurences of vector elements https://stackoverflow.com/a/9237226
    sort(vec.begin(), vec.end());
    vec.erase(unique(vec.begin(), vec.end()), vec.end());
}

void listAll(vector<string> vec) { // Lists all vector elements
    int num = 0;
    for (vector<string>::const_iterator g = vec.begin(); g != vec.end(); ++g) {
        cout << ++num << ". " << *g << '\n';
    }
}

void askSubjects(vector<string>& total, vector<int>& subOnDays, string weekDay) { /* A function which interacts with the user in order
to create a timetable */

    int subCount;
    
    system("cls");
    cout << weekDay << '\n';
    cout << "Enter the subject count\n";
    cin >> subCount;
    while (cin.fail() || subCount <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid value entered. Please try again\n";
        cin >> subCount;
    }
    cout << '\n';

    cin.ignore();

    cout << "Enter each subject one by one\n";
    cout << "(press 'Enter' to move on)\n";

    for (int i = 0; i < subCount; ++i) {
        string toPush;
        getline(cin, toPush);
        trim(toPush);
        total.push_back(toPush);
    }

    subOnDays.push_back(subCount);
    cout << '\n';
}

int main() {

    fstream fileStream;
    fileStream.open("config");

    if (fileStream.fail()) {

        cout << "It appears that the program is running for the first\n";
        cout << "time or the configuration file was recently wiped\n\n";
        cout << "Do you wish to create a new timetable?\n\n";
        cout << "[Press 'Enter' to continue or any key to quit]\n";

        char userWantsTimetable = _getch();

        if (userWantsTimetable == ENTER) {
            system("cls");
            vector<int> subOnDays;
            vector<string> total;
            string weekDays[5] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday" };

            for (int i = 0; i < 5; ++i) {
                askSubjects(total, subOnDays, weekDays[i]);
            }

            ofstream fd;
            fd.open("config");

            for (unsigned i = 0; i < 5; ++i) {
                fd << ' ' << subOnDays[i];
            }


            for (unsigned i = 0; i < total.size(); ++i) {
                fd << '\n' << total[i];
            }

            vector<string> totalUnique = total;
            remove_duplicates(totalUnique);

            system("cls");
            listAll(totalUnique);

            vector<string> zoomLinks;

            cout << "Please enter Zoom links in this order, from top to bottom\n";

            for (unsigned i = 0; i < totalUnique.size(); ++i) {
                string link;
                getline(cin, link);
                trim(link);
                bool linkCorrect = false;
                if (link.find(string("web.zoom.us/j/")) != string::npos) {
                    linkCorrect = true;
                }
                while (linkCorrect == false || findConfId(link) == "Error" || findConfPass(link) == "Error") {
                    cin.clear();
                    cout << "Invalid link entered for subject \'" << totalUnique[i] << "\'. Please try again\n";
                    getline(cin, link);
                    trim(link);

                    if (link.find(string("web.zoom.us/j/")) != string::npos) {
                        linkCorrect = true;
                    }
                }
                fd << '\n' << link;
            }
            system("cls");
            fd.close();
            cout << "The configuration file is ready!\n\n";
            cout << "[Press 'Enter' to continue or any key to quit]\n";

            char userWantsSelection = _getch();
            if (userWantsSelection == ENTER) {

            }
            else {
                return 0;
            }
        }

        else {

            return 0;

        }

    }

    char userWantsToRepeat = ENTER;
    while (userWantsToRepeat == ENTER) {

        ifstream fd;
        fd.open("config");

        int subOnDays[5];

        for (int i = 0; i < 5; ++i) {
            fd >> subOnDays[i];
        }

        vector<string> mon, tue, wed, thu, fri;
        vector<string> total;


        {
            string line;

            int i = 0;

            while (i < subOnDays[0]) {
                getline(fd, line);
                if (line != "") {
                    mon.push_back(line);
                    total.push_back(line);
                }
                else {
                    getline(fd, line);
                    mon.push_back(line);
                    total.push_back(line);
                }
                ++i;
            }

        }
        // ----
        {
            string line;

            int i = 0;

            while (i < subOnDays[1]) {
                getline(fd, line);
                if (line != "") {
                    tue.push_back(line);
                    total.push_back(line);
                }
                else {
                    getline(fd, line);
                    tue.push_back(line);
                    total.push_back(line);
                }
                ++i;
            }

        }
        // ----
        {
            string line;

            int i = 0;

            while (i < subOnDays[2]) {
                getline(fd, line);
                if (line != "") {
                    wed.push_back(line);
                    total.push_back(line);
                }
                else {
                    getline(fd, line);
                    wed.push_back(line);
                    total.push_back(line);
                }
                ++i;
            }

        }
        // ----
        {
            string line;

            int i = 0;

            while (i < subOnDays[3]) {
                getline(fd, line);
                if (line != "") {
                    thu.push_back(line);
                    total.push_back(line);
                }
                else {
                    getline(fd, line);
                    thu.push_back(line);
                    total.push_back(line);
                }
                ++i;
            }

        }
        // ----
        {
            string line;

            int i = 0;

            while (i < subOnDays[4]) {
                getline(fd, line);
                if (line != "") {
                    fri.push_back(line);
                    total.push_back(line);
                }
                else {
                    getline(fd, line);
                    fri.push_back(line);
                    total.push_back(line);
                }
                ++i;
            }

        }

        vector<string> totalUnique = total;
        remove_duplicates(totalUnique);

        system("cls");

        int weekDay;
        cout << "Enter a week day\n";
        cin >> weekDay;
        while (cin.fail() || weekDay > 5 || weekDay <= 0) {
            system("cls");
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid value entered. Please try again\n";
            cin >> weekDay;
        }

        system("cls");

        switch (weekDay) {
        case 1:
            listAll(mon);
            break;
        case 2:
            listAll(tue);
            break;
        case 3:
            listAll(wed);
            break;
        case 4:
            listAll(thu);
            break;
        case 5:
            listAll(fri);
            break;
        }

        vector<string> zoomLinks;

        for (unsigned i = 0; i < totalUnique.size(); ++i) {
            string toPush;
            fd >> toPush;
            zoomLinks.push_back(toPush);
        }

        int subLaunch;
        cout << "\nSelect a subject to launch\n";
        cin >> subLaunch;

        ifstream zoomClientExists;
        zoomClientExists.open(zoomLocation());

        if (zoomClientExists) {
            for (unsigned i = 0; i < totalUnique.size(); ++i) {
                if (weekDay == 1 && mon[subLaunch - 1] == totalUnique[i]) {
                    ShellExecuteA(0, 0, zoomLocation().c_str(), zoomLaunchArgument(zoomLinks[i]).c_str(), 0, 0);
                }
                else if (weekDay == 2 && tue[subLaunch - 1] == totalUnique[i]) {
                    ShellExecuteA(0, 0, zoomLocation().c_str(), zoomLaunchArgument(zoomLinks[i]).c_str(), 0, 0);
                }
                else if (weekDay == 3 && wed[subLaunch - 1] == totalUnique[i]) {
                    ShellExecuteA(0, 0, zoomLocation().c_str(), zoomLaunchArgument(zoomLinks[i]).c_str(), 0, 0);
                }
                else if (weekDay == 4 && thu[subLaunch - 1] == totalUnique[i]) {
                    ShellExecuteA(0, 0, zoomLocation().c_str(), zoomLaunchArgument(zoomLinks[i]).c_str(), 0, 0);
                }
                else if (weekDay == 5 && fri[subLaunch - 1] == totalUnique[i]) {
                    ShellExecuteA(0, 0, zoomLocation().c_str(), zoomLaunchArgument(zoomLinks[i]).c_str(), 0, 0);
                }
            }
        }

        else {
            system("cls");
            cout << "It appears you do not have Zoom client installed\n";
            cout << "or the current installation's files are corrupted!\n\n";
            cout << "Do you wish to download Zoom client now?\n\n";
            cout << "[Press 'Enter' to continue or any key to quit]\n";
            cin.ignore();
            char userWantsZoom = _getch();

            if (userWantsZoom == ENTER) {
                ShellExecuteA(0, 0, "https://zoom.us/download", 0, 0, SW_SHOW);
            }
            else {
                return 0;
            }
        }

        zoomClientExists.close();

        system("cls");
        cout << "Do you wish to go back to the selection?\n\n";
        cout << "[Press 'Enter' to continue or any key to quit]\n";

        cin.ignore();
        userWantsToRepeat = _getch();
    }

}
