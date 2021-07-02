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

const int ENTER = 13; // (char)13 is used as a newline character in _getch()

// Trims whitespaces from both sides https://stackoverflow.com/a/49253841
void trim(string& word)
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

 // Removes duplicated occurences of vector elements https://stackoverflow.com/a/9237226
template <typename T>
void remove_duplicates(vector<T>& vec) {
    sort(vec.begin(), vec.end());
    vec.erase(unique(vec.begin(), vec.end()), vec.end());
}

 // Lists all vector elements
void listAll(vector<string> vec) {
    int num = 0;
    for (vector<string>::const_iterator g = vec.begin(); g != vec.end(); ++g) {
        cout << ++num << ". " << *g << '\n';
    }
}

// A function which interacts with the user in order to create a timetable
void askSubjects(vector<string>& total, vector<int>& subOnDays, string weekDay) {

    int subCount;
    
    system("cls");
    cout << weekDay << '\n';
    cout << "Enter the subject count\n";
    cin >> subCount;

    // Checks if the user has entered a valid number of subjects and has not entered any characters other than numbers
    while (cin.fail() || subCount <= 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid value entered. Please try again\n";
        cin >> subCount; // Asks the user to retry
    }
    cout << '\n';

 // After the user inputs value into subCount, newline character stays in a buffer and gets passed as a subject in toPush
    cin.ignore();

    cout << "Enter each subject one by one\n";
    cout << "(press 'Enter' to move on)\n";

// Pushes back all subjects into a vector of according week day and a vector with holds total subjects
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

// Checks if the configuration file exists
    fstream doesConfigExist;
    doesConfigExist.open("config");

    // If doesn't exist, the user two choices - either to create a timetable or to quit the program
    if (doesConfigExist.fail()) {

    	// Configuration file does not exist, doesConfigExist won't be used anymore
    	doesConfigExist.close();
        
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

                // Checks if the user has typed in a proper link
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

            // User gets to select whether they want to see the newly created timetable or not
            char userWantsSelection = _getch();
            if (userWantsSelection == ENTER) {

            // Left blank, so the program can continue with another function 
            }

            // User does not want to continue to the timetable
            else {
                return 0;
            }
        }

        // User does not want to create a timetable
        else {
        	doesConfigExist.close();
			return 0;
		}

    }

    // userWantsToRepeat is initialized with ENTER to continue
    char userWantsToRepeat = ENTER;
    while (userWantsToRepeat == ENTER) {

        ifstream fd;
        fd.open("config");

        // Stores the number of subjects on each day
        int subOnDays[5];

        for (int i = 0; i < 5; ++i) {
            fd >> subOnDays[i];
        }

        vector<string> mon, tue, wed, thu, fri;
        vector<string> total;

    // In this part subjects are extracted from fd into independent week days and total
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
        // ----------------------------------------
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
        // ----------------------------------------
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
        // ----------------------------------------
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
        // ----------------------------------------
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

        // Stores Zoom links
        vector<string> zoomLinks;

		// Extracts Zoom links that will be used for launching the client
		for (unsigned i = 0; i < totalUnique.size(); ++i) {
            string toPush;
            fd >> toPush;
            zoomLinks.push_back(toPush);
        }

        system("cls");

        int weekDay;
        cout << "Enter a week day\n";
        cin >> weekDay;

        // Prevents the user from typing in characters other than numbers and weekends
        while (cin.fail() || weekDay > 5 || weekDay <= 0) {
            system("cls");
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid value entered. Please try again\n";
            cin >> weekDay;
        }

        system("cls");

        // Lists all subjects of user's selected day
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

        // User can select a subject to launch
        int subLaunch;
        cout << "\nSelect a subject to launch\n";
        cin >> subLaunch;

        // Checks if Zoom client is installed in first place
        ifstream zoomClientExists;
        zoomClientExists.open(zoomLocation());

        // Functions are called from zoom.h, they convert links into arguments and Zoom client is launched based on week day
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

        // If Zoom client is not installed, the user may download and install it
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

        // If user wants to repeat, the function will start over
        userWantsToRepeat = _getch();
    }

}
