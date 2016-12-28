/* 

VCC CMPT 1020 - Assignment 1

   *********************************
   *                               *
   *  PLAGIARISM DETECTION SYSTEM  *
   *                               *
   *********************************

This program takes two files and split them into shingles of length k,
where the union and intersection of the shingle sets are made to calculate the
Jaccardi Index to compare the similarities between the two documents.

Authors: Junho Sohn, Tom Dorval, Eleanor Kim, Nick Lau

Date: 01/24/16   

*/

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <iomanip>

using namespace std;

void intro_msg();
int get_input(int,int);
void print_vector(vector<string>);
void print_char(vector<char>);
void read_file_check(string&, string&, int&);
void copytext(string, string&, string&); //keeps punctuation for letter-shingles
void copytext2(string, string&, vector<string>&); //deletes punctuation for word-shingles
string punctremoval(string&);
vector<string> lettershingle(string,int);
vector<string> wordshingle(vector<string>, int);
bool contains(const vector<string>&, int);
vector<string> vector_union (const vector<string>&, const vector<string>&);
vector<string> vector_intersection (const vector<string>&, const vector<string>&);
void jaccard2(vector<string>,vector<string>,vector<string>,vector<string>);

int main()
{
begin:
    int int_k;
    string file1,file2,f1,f2,f_1,f_2,temp_punct1,temp_punct2;
    vector<string> temp_shingle1,temp_shingle2, letter_shingle1,letter_shingle2,word_shingle1,word_shingle2;

        intro_msg();
        read_file_check(file1,file2,int_k);
        copytext(file1,f_1,temp_punct1);
        copytext2(file1,f1,temp_shingle1);
        letter_shingle1 = lettershingle(temp_punct1,int_k);
        word_shingle1 = wordshingle(temp_shingle1,int_k);
        copytext(file2,f_2,temp_punct2);
        copytext2(file2,f2,temp_shingle2);
        letter_shingle2 = lettershingle(temp_punct2,int_k);
        word_shingle2 = wordshingle(temp_shingle2,int_k);
        jaccard2(letter_shingle1,word_shingle1,letter_shingle2,word_shingle2);


    return 0;
}

//Intro message
//PRE: None
//POST: Shows message
void intro_msg()
{
    cout << "*********************************" << endl;
    cout << "*                               *" << endl;
    cout << "*  PLAGIARISM DETECTION SYSTEM  *" << endl;
    cout << "*                               *" << endl;
    cout << "*********************************" << endl << endl;
}

//Gets valid input (0<=input<=100)
//PRE: Asks for menu selection
//POST: Returns input that is 0<=input<=100
int get_input(int min, int max)
{
    int num;
    cin.unsetf(ios::skipws);
    while ( !(cin >> num) || ( cin.get() != '\n') ||  (num > max) || (num < min) )
    {
        cout << "Invalid input! Re-enter an integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return num;
}

//Reads input name of file
//PRE: None
//POST: Returns input
void read_file_check(string& filename1, string& filename2, int& intk)
{
    cin.clear();
    cout << "Enter the name of your first document (leave out the .txt):  ";
    cin >> filename1;
    cout << "Enter the name of your second document (leave out the .txt):  ";
    cin >> filename2;
    cout << "Enter your 'k' value: ";
    cin.clear();
    cin.ignore();
    intk = get_input(0,100);

    filename1 += ".txt";
    filename2 += ".txt";
}

//Reads stream, stores words into vector + leaves punctuation
//PRE: User inputs name of files
//POST: Copies from stream into vector
void copytext(string filen, string& shingle, string& newshing)
{
    ifstream in_stream;
    in_stream.open(filen.c_str());

    if(in_stream.fail())
    {
        cout << "Couldn't open file 1!";
        exit(1);
    }

    while(!in_stream.eof())
    {
        getline(in_stream,shingle);
        newshing+=shingle;
    }
    in_stream.close();
}

//Reads stream, stores words into vector + deletes punctuation
//PRE: User inputs name of files
//POST: Copies from stream into vector with punctuation removed
void copytext2(string filen2, string& shingle2, vector<string>& newshing2)
{
    ifstream in_stream2;
    in_stream2.open(filen2.c_str());

    if(in_stream2.fail())
    {
        cout << "Couldn't open file 2!";
        exit(1);
    }

    while(in_stream2 >> shingle2)
    {
        punctremoval(shingle2);
        newshing2.insert(newshing2.end(), shingle2);
    }
    in_stream2.close();
}

//Reads from stream, recognizes punctation and deletes them
//PRE: Input stream
//POST: Deletes any characters other than a-z/A-Z/0-9, returns new string
string punctremoval (string& punct)
{
    for(int i=0; i<punct.size(); i++)
    {
        if(!(((punct.at(i)>='a') && (punct.at(i)<='z')) ||
                ((punct.at(i)>='A') && (punct.at(i)<='Z')) ||
                ((punct.at(i)>='0') && (punct.at(i)<='9'))   ))
            punct.erase(punct.begin()+i);
    }
    return punct;
}

//Creates a vector of 'k'-letter shingles
//PRE: sh1 (string and intk1 are defined
//POST: Returns new vector with 'k'-letter shingles
vector<string> lettershingle(string sh1,int intk1)
{
    vector<string> lshing;
    string temp1;

    for (int i=0; i<sh1.size()-intk1+1; i++)
        lshing.push_back(sh1.substr(i,intk1));

    return lshing;
}

//Creates shingles of 'k' to vector
//PRE: sh2 (vector) and intk2 are defined
//POST: Returns new vector with 'k' word shingles
vector<string> wordshingle(vector<string> sh2, int intk2)
{
    vector<string> wshing;
    string temp2;

    for(int i=0; i<sh2.size()-intk2+1; i++)
    {
        for(int j=0; j<intk2; j++)
            temp2+= sh2[i+j] + ' ';
        wshing.push_back(temp2);
        temp2 = "";
    }
    return wshing;
}

//Checks if the vector contains duplicates
//PRE: Reads vector, compares it to the 'word' at location i
//POST: Returns true if it matches (duplicate exists), false otherwise
bool contains(const vector<string>& vec, string value)
{
    for (int i = 0; i < vec.size(); i++)
        if(vec.at(i) == value)
            return true;
    return false;
}

//Creates the union vector.
//PRE: Reads the vectors.
//POST: Creates a new vector string from the two input strings of only the duplicate elements.
vector<string> vector_union(const vector<string>& vec1, const vector <string>& vec2)
{
    vector<string> union_vec(vec1);

    for (int i = 0; i < vec2.size(); i++)
        if (!contains(union_vec,vec2.at(i)))
            union_vec.push_back(vec2.at(i));
    return union_vec;
}

//Creates the intersection vector.
//PRE: Reads both vector strings.
//POST: Outputs a new string from the combination of both strings without any duplicates.
vector<string> vector_intersection(const vector<string>& vec1, const vector <string>& vec2)
{
    vector<string> intersection_vec;

    for (int i = 0; i < vec2.size(); i++)
        if (contains(vec1,vec2.at(i)))
            intersection_vec.push_back(vec2.at(i));
    return intersection_vec;
}

//Outputs Jacquard's similarity
//PRE: Reads letter shingle and word shingle strings from both files.
//POST: Combines the vector strings and Calculates the similarity of the two files.
void jaccard2(vector<string> temp1, vector<string> temp2, vector<string> temp3, vector<string> temp4)
{
    for(int i=0; i<temp2.size(); i++)
        temp1.push_back(temp2.at(i));
    for(int i=0; i<temp4.size(); i++)
        temp3.push_back(temp4.at(i));

    vector<string> letter_u = vector_union(temp1, temp3);
    vector<string> letter_in = vector_intersection(temp1, temp3);
    cout << endl;

    cout << "*********************************\n";
    cout << "*                               *\n";
    cout << "*  LETTER SIMILARITY: " << setprecision(2) << fixed << 100*(double)letter_in.size()/(double)letter_u.size() << "%\t*" << endl;

    vector<string> word_u = vector_union(temp2, temp4);
    vector<string> word_in = vector_intersection(temp2, temp4);

    cout << "*                               *\n";
    cout << "*    WORD SIMILARITY: " << setprecision(2) << fixed << 100*(double)word_in.size()/(double)word_u.size() << "%\t*" << endl;
    cout << "*                               *\n";
    cout << "*********************************\n";
}
