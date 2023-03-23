#include "movie.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

void alphabet(vector<Movie>& x, Movie newobj) { //function from the lecture
    int count = x.size();
    x.push_back(newobj);
    int loc = count;
    while (loc > 0 && x[loc - 1].getName() > newobj.getName()) {
        x[loc] = x[loc - 1];
        loc--;
    }

    x[loc] = newobj;
}

void insert(vector<int>& a, int newnum) {  		// NOT const vector
    // pre: a[0] <= … <= a[a.size()-1], a is sorted
    // post: newnum inserted into a, a still sorted
    int count = a.size();  // size before insertion
    a.push_back(newnum);   // increase size –newnum is inserted at the
    // end but inserted value is not important
    int loc = count;      // start searching insertion loc from end

    while (loc > 0 && a[loc - 1] < newnum) {
        a[loc] = a[loc - 1];
        loc--;        // shift right until the proper insertion cell
    }
    a[loc] = newnum;  //actual insertion
}

int owncount_film(vector<Movie> y, Movie obj) {
    if (y.size() == 0) {
        return 0;
    }
    else if (y.size() > 0) {
        int x = 0;
        for (int i = 0; i < y.size(); i++) {
            if (y[i].getName() == obj.getName()) {
                x++;
            }
        }
        return x;
    }
    return -1;
}

int owncount(vector<int> y, int obj) {
    if (y.size() == 0) {
        return 0;
    }
    else if (y.size() > 0) {
        int x = 0;
        for (int i = 0; i < y.size(); i++) {
            if (y[i] == obj) {
                x++;
            }
        }
        return x;
    }
    return -1;
}


int main()
{
    vector <Movie> films;
    ifstream listfile;
    cout << "Welcome to the movie recommender program!" << endl << endl
        << "Please enter the movie list filename: ";

    while (true) {
        string listfilename;
        cin >> listfilename;
        listfile.open((listfilename).c_str());
        if (listfile.fail()) {
            cout << "Please check filename! Enter a correct movie list filename: ";
        }
        else if (listfile.is_open()){
            break;
        }
    }

    string line;

    while (getline(listfile, line)) {
        //line.pop_back();
        for (int x = 0; x < line.size(); x++) {
            if (int(line[x]) < 58 && int(line[x]) > 47) {
                Movie film_add;
                film_add.setName(line.substr(0,(x-1)));
                film_add.setYear(stoi(line.substr(x, 4)));
                film_add.setGenre(line.substr(x+5));
                films.push_back(film_add);
                break;
            }
        }
    }
    listfile.close();
    cout << "Please select your action:" << endl << "1. Select the genre of the movie that you want to watch" << endl
        << "2. Select the year of the movie that you want to watch" << endl << "3. Exit program" << endl << endl;
    string choice;
    while (true){
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice == "1") {
            cout << "Please enter the genre of the movie you want to watch: ";
            string genre_search;
            vector <Movie> filmofgenre{};
            vector<vector <Movie> > filmofgenresorted{};
            vector <int> yearssorted{};
            cin >> genre_search;
            cout << endl;
            string genre_search_original = genre_search;
            for (int i = 0; i < genre_search.size(); i++) {
                genre_search[i] = toupper(genre_search[i]);
            }
            for (int i = 0; i < films.size(); i++) {
                string genre_string = films[i].getGenre();
                for (int u = 0; u < genre_string.size(); u++) {
                    genre_string[u] = toupper(genre_string[u]);

                    if (genre_string == genre_search && owncount_film(filmofgenre, films[i]) == 0) {
                        filmofgenre.push_back(films[i]);
                    }
                }
            }
            for (int u = 0; u < filmofgenre.size(); u++) {
                if (owncount(yearssorted, filmofgenre[u].getYear()) < 0) {
                    continue;
                }
                else if (owncount(yearssorted, filmofgenre[u].getYear()) == 0) {
                    vector<Movie> newvector{};
                    Movie x = filmofgenre[u];
                    insert(yearssorted, x.getYear());
                    filmofgenresorted.push_back(newvector);
                }
            }

            for (int u = 0; u < yearssorted.size(); u++) {
                for (int i = 0; i < films.size(); i++) {
                    if (films[i].getYear() == yearssorted[u] && owncount_film(filmofgenre, films[i]) == 1) {
                        alphabet(filmofgenresorted[u], films[i]);
                    }
                }
            }

            if (filmofgenresorted.size() == 0) {
                cout << "There are no " << genre_search_original << " movies!" << endl << endl;
            }
            else {
                cout << genre_search_original << " movies from newest to oldest:" << endl;
                for (int u = 0; u < yearssorted.size(); u++) {
                    for (int i = 0; i < filmofgenresorted[u].size(); i++) {
                        cout << "Movie name: " << filmofgenresorted[u][i].getName() << " Release Year: " << filmofgenresorted[u][i].getYear() << endl;
                    }
                }
                cout << endl;
            }
            for (int x = 0; x < films.size(); x++) {
                for (int y = 0; y < films[x].getGenre().size(); y++) {
                    if (y == 0) {
                        films[x].getGenre()[y] = toupper(films[x].getGenre()[y]);
                    }
                    else {
                        films[x].getGenre()[y] = tolower(films[x].getGenre()[y]);
                    }
                }
            }
        }
        else if (choice == "2") {
            cout << "Please enter the year of the movie you want to watch: ";
            string years;
            vector<int> yearssorted{};
            vector<Movie> filmofyears{};
            vector<vector<Movie> > filmofyearssorted{};
            cin >> years;
            if (years.size() == 9 && years[4] == '-') {
                int firstyear, secondyear;
                firstyear = stoi(years.substr(0, 4), 0, 10);
                secondyear = stoi(years.substr(5, 4), 0, 10);
                for (int i = 0; i < films.size(); i++) {
                    if (films[i].getYear() >= firstyear && films[i].getYear() <= secondyear) {
                        filmofyears.push_back(films[i]);
                    }
                }
            }
            else if (years.size() == 4) {
                int years_int = stoi(years);
                for (int i = 0; i < films.size(); i++) {
                    if (films[i].getYear() == years_int) {
                        filmofyears.push_back(films[i]);
                    }
                }
            }

            for (int i = 0; i < filmofyears.size(); i++) {
                if (owncount(yearssorted, filmofyears[i].getYear()) == 0) {
                    vector<Movie> newvector{};
                    insert(yearssorted, filmofyears[i].getYear());
                    filmofyearssorted.push_back(newvector);
                }
            }

            for (int n = 0; n < filmofyears.size(); n++) {
                for (int m = 0; m < yearssorted.size(); m++) {
                    if ((owncount_film(filmofyearssorted[m], filmofyears[n]) == 0) && (filmofyears[n].getYear() == yearssorted[m])) {
                        alphabet(filmofyearssorted[m], filmofyears[n]);
                    }

                }
            }

            if (filmofyearssorted.size() == 0) {
                if (years.size() == 9) {
                    cout << "There are no movies released between " << years << "!" << endl << endl;
                    continue;
                }
                else if (years.size() == 4) {
                    cout << "There are no movies released in " << years << "!" << endl << endl;
                    continue;
                }
            }
            else {
                if (years.size() == 9) {
                    cout << "Movies released between the years " << years << " from A to Z with decreasing year ordering:" << endl;
                    for (int i = 0; i < yearssorted.size(); i++) {
                        for (int u = 0; u < filmofyearssorted[i].size(); u++) {
                            filmofyearssorted[i][u].print();
                            cout << endl;
                        }
                    }
                }
                else if (years.size() == 4) {
                    cout << "Movies released in " << years << " from A to Z:" << endl;
                    for (int i = 0; i < yearssorted.size(); i++) {
                        for (int u = 0; u < filmofyearssorted[i].size(); u++) {
                            cout << "Movie name: " << filmofyearssorted[i][u].getName() << " Genre: " << filmofyearssorted[i][u].getGenre() << "\n";
                        }
                    }
                }
            }
            cout << endl;
        }
        else if (choice == "3") {
            cout << "Thank you..." << endl;
            break;
        }
        else {
            cout << "Invalid action!" << endl << endl;
        }
    }
}