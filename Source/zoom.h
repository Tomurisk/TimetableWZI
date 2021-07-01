#pragma once
#include <string>

/* This header's functions also check for possible mistakes done by the user to prevent unexpected behavior.
Anything unusual will get returned as 'Error' and that works as indicator in TimetableWZI to stop program's
work or for the user to reclarify their actions.*/

std::string findConfId(std::string link) { // This function is used to find Zoom conference's ID

    std::string foundConfId = link.substr(26, 17);
    for (int i = 16; i >= 0; --i) {
        if (i == 0 && foundConfId.find(char('?')) != std::string::npos) {
            foundConfId = foundConfId.substr(0, i);
            return "Error"; // The conference ID cannot be blank
        }
        if (foundConfId.find(char('p')) != std::string::npos) {
            foundConfId = foundConfId.substr(0, i);
        }
        if (foundConfId.find(char('?')) != std::string::npos) {
            foundConfId = foundConfId.substr(0, i);
        }
        if (foundConfId.length() > 16) {
            return "Error"; /* Checks for possible overflow. The limit is set at 16,
            a number which likely won't be reached without all nowadays' generations dying out!*/
        }
    }
    bool containsNum = foundConfId.find_first_not_of("0123456789") == std::string::npos;

    if (containsNum == true) {
        return foundConfId;
    }
    else {
        return "Error";  // Zoom's conference ID cannot contain other characters than numbers
    }
}

std::string findConfPass(std::string link) { // This function is used to find Zoom conference's password
    int confIdLen = findConfId(link).length();
    std::string foundConfPass;
    if (findConfId(link) != "Error") { // Checks if there aren't any errors from the ID
        foundConfPass = link.substr(31 + confIdLen, link.length()); // Moves in accordance to increasing ID's numbers
        if (foundConfPass.length() == 32) {
            return foundConfPass;
        }
        else {
            return "Error";  // The limit of encrypted passwords is 32, so it makes sense to return an overflow error
        }
    }
    else {
        return "Error"; // Returns an error if the ID turns out to have errors
    }
}

std::string zoomLaunchArgument(std::string link) {
    std::string argument = "--url=zoommtg://zoom.us/join?action=join&confno=";
    argument.append(findConfId(link));
    argument.append("&pwd=");
    argument.append(findConfPass(link));
    return argument;
}

std::string zoomLocation() {
    std::string path = getenv("APPDATA");
    path.append("\\Zoom\\bin\\Zoom.exe");
    return path;
}
