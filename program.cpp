#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>
#include <openssl/evp.h>
#include <bits/stdc++.h>
#include "jthread.hpp"
#include <chrono>
#include <condition_variable>
#include <mutex>

using namespace std;
///////Pierwsza linia hasel odpada ;/
int globalIndex = 0;
class jthread;
std::mutex Mutex, Mutex2;
std::condition_variable cv;
stop_source st;

class Data
{
public:
    string hash;
    string mail;
    string user_name;
    string password;
    Data(string hash1, string mail1, string user_name1, string password1)
        : hash(hash1), mail(mail1), user_name(user_name1), password(password1){};
};

vector<string> words_array;
vector<Data> data_array;

void bytes2md5(const char *data, int len, char *md5buf)
{
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_md5();
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len, i;
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, data, len);
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_free(mdctx);
    for (i = 0; i < md_len; i++)
    {
        snprintf(&(md5buf[i * 2]), 16 * 2, "%02x", md_value[i]);
    }
};

void hashHunting(string Password)
{
    const char *k = Password.c_str();
    char md5[33]; // 32 characters + null terminator
    bytes2md5(k, strlen(k), md5);
    string hash_to_check = md5;

    for (int n = 0; n < data_array.size(); n++)
    {
        if (data_array[n].password.empty())
        {

            if (hash_to_check == data_array[n].hash)
            {
                Mutex.lock();
                data_array[n].password = Password;
                globalIndex = n;
                cv.notify_one();
                this_thread::sleep_for(std::chrono::seconds(1));

                // this_thread::sleep_for(std::chrono::seconds(1));
                Mutex.unlock();
            }
        }
    }
};

vector<string> basicVariantCreator(vector<string> words_array)
{
    vector<string> basicVariant_array;
    for (int p = 0; p < words_array.size(); p++)
    {
        basicVariant_array.push_back(words_array[p]);
        // hashHunting(basicVariant_array.back());
    }

    for (int i = 0; i < words_array.size(); i++)
    {
        string word = words_array[i];
        word[0] = toupper(word[0]);
        basicVariant_array.push_back(word);
        // hashHunting(basicVariant_array.back());
    }

    for (int j = 0; j < words_array.size(); j++)
    {
        string word = words_array[j];
        for (int n = 0; n < word.size(); n++)
        {
            word[n] = toupper(word[n]);
        }
        basicVariant_array.push_back(word);
        // hashHunting(basicVariant_array.back());
    }
    for (int k = 0; k < words_array.size(); k++)
    {
        string word = words_array[k];
        word.back() = toupper(word.back());
        basicVariant_array.push_back(word);
        // hashHunting(basicVariant_array.back());
    }
    return basicVariant_array;
};

void basicVariantchecker(vector<string> basicVariant_array)
{
    for (int z = 0; z < basicVariant_array.size(); z++)
    {

        hashHunting(basicVariant_array[z]);
    }
}
void frontNumbersgenerator(vector<string> basicVariant_array)
{
    vector<string> temp_FNG_array;
    for (int i = 0; i < 100; i++)
    {
        string number = to_string(i);
        for (int n = 0; n < basicVariant_array.size(); n++)
        {

            temp_FNG_array.push_back(number + basicVariant_array[n]);
            hashHunting(temp_FNG_array.back());
        }
    }
    for (int k = 0; k < temp_FNG_array.size(); k++)
    {
        // hashHunting(temp_FNG_array[k], data_array);
        //  cout << temp_FNG_array[k] << endl;
    }
};

void backNumbersgenerator(vector<string> basicVariant_array)
{
    vector<string> temp_BNG_array;
    for (int i = 0; i < 100; i++)
    {
        string number = to_string(i);
        for (int n = 0; n < basicVariant_array.size(); n++)
        {

            temp_BNG_array.push_back(basicVariant_array[n] + number);
            hashHunting(temp_BNG_array.back());
        }
    }
    for (int k = 0; k < temp_BNG_array.size(); k++)
    {
        // hashHunting(temp_BNG_array[k], data_array);
        //  cout << temp_BNG_array[k] << endl;
    }
};

void frontBackNumbersgenerator(vector<string> basicVariant_array)
{
    vector<string> temp_FBNG_array;
    for (int i = 0; i < 100; i++)
    {
        string number = to_string(i);
        for (int n = 0; n < basicVariant_array.size(); n++)
        {
            for (int k = 0; k < 100; k++)
            {
                string number2 = to_string(k);

                temp_FBNG_array.push_back(number + basicVariant_array[n] + number2);
                hashHunting(temp_FBNG_array.back());
            }
        }
    }
    for (int k = 0; k < temp_FBNG_array.size(); k++)
    {
        // hashHunting(temp_FNG_array[k], data_array);
        //  cout << temp_FNG_array[k] << endl;
    }
};
void twowordsgenerator(vector<string> basicVariant_array)
{
    vector<string> temp_2WG_array;
    for (int i = 0; i < basicVariant_array.size(); i++)
    {

        for (int n = 0; n < basicVariant_array.size(); n++)
        {

            temp_2WG_array.push_back(basicVariant_array[i] + basicVariant_array[n]);
            hashHunting(temp_2WG_array.back());
        }
    }
    for (int k = 0; k < temp_2WG_array.size(); k++)
    {
        // hashHunting(temp_FNG_array[k], data_array);
        //  cout << temp_FNG_array[k] << endl;
    }
};
void twoWordsWithCharGenerator(vector<string> basicVariant_array)
{
    vector<string> temp_2WG_array;
    for (int i = 0; i < basicVariant_array.size(); i++)
    {

        for (int n = 0; n < basicVariant_array.size(); n++)
        {
            for (int c = 32; c < 48; c++)
            {
                char character(c);
                temp_2WG_array.push_back(basicVariant_array[i] + character + basicVariant_array[n]);
                hashHunting(temp_2WG_array.back());
            }
        }
    }
    for (int k = 0; k < temp_2WG_array.size(); k++)
    {
        // hashHunting(temp_FNG_array[k], data_array);
        //  cout << temp_FNG_array[k] << endl;
    }
};

void passwordReader(stop_token st)
{
    while (1)
    {

        std::unique_lock<std::mutex> lock(Mutex2);
        cv.wait(lock);

        cout << "Haslo dla : " << data_array[globalIndex].mail << " To " << data_array[globalIndex].password << "\n";
        cout << "Jego nazwa użytkownika to : " << data_array[globalIndex].user_name << "\n";
        if (st.stop_requested())
        {
            break;
        }
    }
}
/*
////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// MAIN /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

*/

int main()
{

    fstream wordsfile;
    string filename, line;
    cout << "Wprowadź nazwę słownika:\n";
    cin >> filename;
    wordsfile.open(filename, ios::in);
    vector<string> words_array;
    if (wordsfile.is_open())
    {
        while (getline(wordsfile, line))
        {
            words_array.push_back(line);
        }
    }
    else
    {
        cout << "Błąd otwarcia pliku\n";
    }

    wordsfile.close();

    /*
    ///////////////////////////////////////////////////////////////////////////////////
    ///////////////////// WEKTOR BAZY DANYCH Z HASLAMI ///////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    */
    fstream datafile;
    string filename_d, line_d;
    cout << "Wprowadź nazwę bazy danych haseł:\n";
    cin >> filename_d;
    datafile.open(filename_d, ios::in);
    if (datafile.is_open())
    {
        while (getline(datafile, line_d))
        {
            string temp_username;
            string temp_id, temp_hash, temp_mail, temp_password;
            datafile >> temp_id >> temp_hash >> temp_mail >> temp_username;
            temp_password = "";
            data_array.emplace_back(temp_hash, temp_mail, temp_username, temp_password);
        }
    }

    else
    {
        cout << "Błąd otwarcia pliku z danymi\n";
    }

    datafile.close();

    vector<string> basicVariant_array = basicVariantCreator(words_array);

    std::jthread t0;

    std::jthread thread1([&basicVariant_array]
                         { basicVariantchecker(basicVariant_array); });
    std::jthread thread2([&basicVariant_array]
                         { frontNumbersgenerator(basicVariant_array); });
    std::jthread thread3([&basicVariant_array]
                         { backNumbersgenerator(basicVariant_array); });
    std::jthread thread4([&basicVariant_array]
                         { frontBackNumbersgenerator(basicVariant_array); });
    std::jthread thread5([&basicVariant_array]
                         { twowordsgenerator(basicVariant_array); });
    std::jthread thread6([&basicVariant_array]
                         { twoWordsWithCharGenerator(basicVariant_array); });
    std::jthread thread7([]
                         { passwordReader(st.get_token()); });
    // std::jthread thread5(passwordReader, stop.get_token());
    //  std::jthread thread1(frontNumbersgenerator, basicVariant_array, data_array)
    /*
    frontNumbersgenerator(basicVariant_array, data_array);
    backNumbersgenerator(basicVariant_array, data_array);
    frontBackNumbersgenerator(basicVariant_array, data_array);
    twowordsgenerator(basicVariant_array, data_array);
    */
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    thread6.join();

    cv.notify_all();
    st.request_stop();

    thread7.join();

    return 0;
}
