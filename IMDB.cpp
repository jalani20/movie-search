
#include <iostream>
#include <cctype>
#include <vector>
#include <sstream>
#include <algorithm>
#include <string>
#include <fstream>

using namespace std;

/**************************************************************************************/

// The splitString function takes a line and split it on the delimiter and push the parts into items
void splitString(string line, char delimiter, vector<string> &items)
{
    stringstream s_stream(line);
    while (s_stream.good())
    {
        string item;
        getline(s_stream, item, delimiter);
        items.push_back(item);
    }
}
// struct for titles
struct TitleRecord
{
    string titleId;
    string primaryTitle;
    string titleComparison;
    string startYear;
    string genres;
};

// store content of title file into vector
void titleContent(string titlesFile, vector<TitleRecord> &title)
{
    ifstream inFS;
    inFS.open(titlesFile);
    vector<string> titles;
    // calling struct to access data members
    TitleRecord content;
    string s = "";
    string lowerCaseTitle = "";
    // gets the first line of the file which is the header that is not needed
    getline(inFS, s);
    while (getline(inFS, s))
    {
        splitString(s, '\t', titles);

        content.titleId = titles[0];
        content.primaryTitle = titles[2];

        // Save the lowercase version so we don't have to do any additional processing later on
        for (int i = 0; i < titles[2].size(); i++)
        {
            lowerCaseTitle += tolower(titles[2][i]);
        }
        content.titleComparison = lowerCaseTitle;
        lowerCaseTitle = "";

        content.startYear = titles[5];

        replace(titles[8].begin(), titles[8].end(), ',', ' ');
        content.genres = titles[8];
        title.push_back(content);

        titles.clear();
    }
    inFS.close();
}
// struct for information of actors/actresses
struct NameRecord
{
    string nameId;
    string primaryName;
    string birthYear;
    string deathYear;
    string primaryProfession;
};
// store content of names file into vector
void nameContent(string namesFile, vector<NameRecord> &name)
{
    ifstream inFS;
    inFS.open(namesFile);
    vector<string> names;
    NameRecord nameInfo;
    string n = "";
    getline(inFS, n);
    while (getline(inFS, n))
    {
        splitString(n, '\t', names);
        if (names.size() == 6)
        {
            nameInfo.nameId = names[0];
            nameInfo.primaryName = names[1];
            nameInfo.birthYear = names[2];
            nameInfo.deathYear = names[3];
            nameInfo.primaryProfession = names[4];
            name.push_back(nameInfo);
        }
        names.clear();
    }
    inFS.close();
}

// struct for information on which actor/actresses played which role in which movie
struct PrincipalRecord
{
    string idTitle;
    string idName;
    string characters;
};

// store content of movie information into vector
void movieContent(string principalsFile, vector<PrincipalRecord> &principals)
{
    ifstream inFS;
    inFS.open(principalsFile);
    vector<string> movies;
    PrincipalRecord record;
    string m = "";
    getline(inFS, m);
    while (getline(inFS, m))
    {
        splitString(m, '\t', movies);
        if (movies.size() == 6)
        {
            record.idTitle = movies[0];
            record.idName = movies[3];
            record.characters = movies[5];
            principals.push_back(record);
        }
        movies.clear();
    }
    inFS.close();
}



// search for movies
void findMovie(string userInput, const vector<TitleRecord> &title, const vector<NameRecord> &name, const vector<PrincipalRecord> &principal)
{
    TitleRecord titleSearch;
    vector<string> splitUserInput;
    // Replace the '+' characters with a space and lowercase
    for (int i = 0; i < userInput.size(); i++)
    {
        if (userInput[i] != '+')
        {
            userInput[i] = tolower(userInput[i]);
        }
    }

    splitString(userInput, '+', splitUserInput);

    // compare the userInput with primaryTitles, if movie found then pushback into nameFound vector
    long counter = 0;
    vector<TitleRecord> matches;

    for (int i = 0; i < title.size(); i++)
    {
        int matchedKeywords = 0;
        for (int j = 0; j < splitUserInput.size(); j++)
        {
            if (title.at(i).titleComparison.find(splitUserInput.at(j).c_str()) != string::npos)
            {
                matchedKeywords++;
            }
        }

        if (matchedKeywords == splitUserInput.size())
        {
            cout << counter << ":" << endl;
            cout << "Title: " << title.at(i).primaryTitle << endl;
            cout << "Year: " << title.at(i).startYear << endl;
            cout << "Genre: " << title.at(i).genres << endl;
            cout << "---------------" << endl;
            matches.push_back(title.at(i));
            counter++;
        }
    }

    int menuInput;
    cout << "Select the movie to see it's actors/actresses (-1 to go back to the previous menu): ";
    cin >> menuInput;

    while (menuInput != -1)
    {

        // select movie and return cast members for that movie
        vector<string> matchedNameIds;

        for (int i = 0; i < principal.size(); i++)
        {
            if (principal.at(i).idTitle == matches.at(menuInput).titleId)
            {

                cout << principal.at(i).characters << endl;
            }
        }

        cout << "Select the movie to see it's actors/actresses (-1 to go back to the previous menu): ";
        cin >> menuInput;
    }
}

// displays the menu option
int displayMenu()
{
    int choice;
    cout << "   1. Search for movies" << endl;
    cout << "   2. Search for actors/actresses" << endl;
    cout << "   3. Exit" << endl;
    cout << "Select a menu option: ";
    cin >> choice;
    return choice;
}
/**********************************************************************************************************/

int run(string titlesFile, string namesFile, string principalsFile)
{

    // Initialize variables to store title, name, principals, and movies matched
    vector<TitleRecord> title;
    vector<NameRecord> name;
    vector<PrincipalRecord> principals;

    titleContent(titlesFile, title);
    nameContent(namesFile, name);
    movieContent(principalsFile, principals);

    string menuInput;
    int menuOption = displayMenu();

    while (menuOption != 3)
    {
        if (menuOption == 1)
        {
            cout << "Enter search phrase: ";
            cin >> menuInput;
            findMovie(menuInput, title, name, principals);
        }
        else if (menuOption == 2)
        {
        }

        menuOption = displayMenu();
    }

    return 0;
}

int main()
{
    // Change these addresses according to your local machine
    string titlesFile = "C:\\Users\\alani\\OneDrive\\Desktop\\movie.titles.tsv";
    string namesFile = "C:\\Users\\alani\\OneDrive\\Desktop\\movie.names.tsv";
    string principalsFile = "C:\\Users\\alani\\OneDrive\\Desktop\\movie.principals.tsv";

    run(titlesFile, namesFile, principalsFile);
    return 0;
}
