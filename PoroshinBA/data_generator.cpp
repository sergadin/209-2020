#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

std::vector<std::string> families = {
    "Ivanov",       "Smirnov",      "Kuznetsov",     "Popov",
    "Vasilev",      "Petrov",       "Sokolov",       "Mikhailov",
    "Novikov",      "Fedorov",      "Morozov",       "Volkov",
    "Alekseev",     "Lebedev",      "Semenov",       "Egorov",
    "Pavlov",       "Kozlov",       "Stepanov",      "Nikolaev",
    "Orlov",        "Andreev",      "Makarov",       "Nikitin",
    "Zakharov",     "Zaitsev",      "Solovev",       "Borisov",
    "IAkovlev",     "Grigorev",     "Romanov",       "Vorobev",
    "Sergeev",      "Kuzmin",       "Frolov",        "Aleksandrov",
    "Dmitriev",     "Korolev",      "Gusev",         "Kiselev",
    "Ilin",         "Maksimov",     "Poliakov",      "Sorokin",
    "Vinogradov",   "Kovalev",      "Belov",         "Medvedev",
    "Antonov",      "Tarasov",      "ZHukov",        "Baranov",
    "Filippov",     "Komarov",      "Davydov",       "Beliaev",
    "Gerasimov",    "Bogdanov",     "Osipov",        "Sidorov",
    "Matveev",      "Titov",        "Markov",        "Mironov",
    "Krylov",       "Kulikov",      "Karpov",        "Vlasov",
    "Melnikov",     "Denisov",      "Gavrilov",      "Tikhonov",
    "Kazakov",      "Afanasev",     "Danilov",       "Savelev",
    "Timofeev",     "Fomin",        "Chernov",       "Abramov",
    "Martynov",     "Efimov",       "Fedotov",       "Shcherbakov",
    "Nazarov",      "Kalinin",      "Isaev",         "Chernyshev",
    "Bykov",        "Maslov",       "Rodionov",      "Konovalov",
    "Lazarev",      "Voronin",      "Klimov",        "Filatov",
    "Ponomarev",    "Golubev",      "Kudriavtsev",   "Prokhorov",
    "Naumov",       "Potapov",      "ZHuravlev",     "Ovchinnikov",
    "Trofimov",     "Leonov",       "Sobolev",       "Ermakov",
    "Kolesnikov",   "Goncharov",    "Emelianov",     "Nikiforov",
    "Grachev",      "Kotov",        "Grishin",       "Efremov",
    "Arkhipov",     "Gromov",       "Kirillov",      "Malyshev",
    "Panov",        "Moiseev",      "Rumiantsev",    "Akimov",
    "Kondratev",    "Biriukov",     "Gorbunov",      "Anisimov",
    "Eremin",       "Tikhomirov",   "Galkin",        "Lukianov",
    "Mikheev",      "Skvortsov",    "Iudin",         "Belousov",
    "Nesterov",     "Simonov",      "Prokofev",      "KHaritonov",
    "Kniazev",      "TSvetkov",     "Levin",         "Mitrofanov",
    "Voronov",      "Aksenov",      "Sofronov",      "Maltsev",
    "Loginov",      "Gorshkov",     "Savin",         "Krasnov",
    "Maiorov",      "Demidov",      "Eliseev",       "Rybakov",
    "Safonov",      "Plotnikov",    "Demin",         "KHokhlov",
    "Fadeev",       "Molchanov",    "Ignatov",       "Litvinov",
    "Ershov",       "Ushakov",      "Dementev",      "Riabov",
    "Mukhin",       "Kalashnikov",  "Leontev",       "Lobanov",
    "Kuzin",        "Korneev",      "Evdokimov",     "Borodin",
    "Platonov",     "Nekrasov",     "Balashov",      "Bobrov",
    "ZHdanov",      "Blinov",       "Ignatev",       "Korotkov",
    "Muravev",      "Kriukov",      "Beliakov",      "Bogomolov",
    "Drozdov",      "Lavrov",       "Zuev",          "Petukhov",
    "Larin",        "Nikulin",      "Serov",         "Terentev",
    "Zotov",        "Ustinov",      "Fokin",         "Samoilov",
    "Konstantinov", "Sakharov",     "Shishkin",      "Samsonov",
    "Cherkasov",    "Chistiakov",   "Nosov",         "Spiridonov",
    "Karasev",      "Avdeev",       "Vorontsov",     "Zverev",
    "Vladimirov",   "Seleznev",     "Nechaev",       "Kudriashov",
    "Sedov",        "Firsov",       "Andrianov",     "Panin",
    "Golovin",      "Terekhov",     "Ulianov",       "Shestakov",
    "Ageev",        "Nikonov",      "Selivanov",     "Bazhenov",
    "Gordeev",      "Kozhevnikov",  "Pakhomov",      "Zimin",
    "Kostin",       "Shirokov",     "Filimonov",     "Larionov",
    "Ovsiannikov",  "Sazonov",      "Suvorov",       "Nefedov",
    "Kornilov",     "Liubimov",     "Lvov",          "Gorbachev",
    "Kopylov",      "Lukin",        "Tokarev",       "Kuleshov",
    "Shilov",       "Bolshakov",    "Pankratov",     "Rodin",
    "Shapovalov",   "Pokrovskii",   "Bocharov",      "Nikolskii",
    "Markin",       "Gorelov",      "Agafonov",      "Berezin",
    "Ermolaev",     "Zubkov",       "Kupriianov",    "Trifonov",
    "Maslennikov",  "Kruglov",      "Tretiakov",     "Kolosov",
    "Rozhkov",      "Artamonov",    "Shmelev",       "Laptev",
    "Lapshin",      "Fedoseev",     "Zinovev",       "Zorin",
    "Utkin",        "Stoliarov",    "Zubov",         "Tkachev",
    "Dorofeev",     "Antipov",      "Zavialov",      "Sviridov",
    "Zolotarev",    "Kulakov",      "Meshcheriakov", "Makeev",
    "Diakonov",     "Guliaev",      "Petrovskii",    "Bondarev",
    "Pozdniakov",   "Panfilov",     "Kochetkov",     "Sukhanov",
    "Ryzhov",       "Starostin",    "Kalmykov",      "Kolesov",
    "Zolotov",      "Kravtsov",     "Subbotin",      "Shubin",
    "Shchukin",     "Losev",        "Vinokurov",     "Lapin",
    "Parfenov",     "Isakov",       "Golovanov",     "Korovin",
    "Rozanov",      "Artemov",      "Kozyrev",       "Rusakov",
    "Aleshin",      "Kriuchkov",    "Bulgakov",      "Koshelev",
    "Sychev",       "Sinitsyn",     "Chernykh",      "Rogov",
    "Kononov",      "Lavrentev",    "Evseev",        "Pimenov",
    "Panteleev",    "Goriachev",    "Anikin",        "Lopatin",
    "Rudakov",      "Odintsov",     "Serebriakov",   "Pankov",
    "Degtiarev",    "Orekhov",      "TSarev",        "Shuvalov",
    "Kondrashov",   "Goriunov",     "Dubrovin",      "Golikov",
    "Kurochkin",    "Latyshev",     "Sevastianov",   "Vavilov",
    "Erofeev",      "Salnikov",     "Kliuev",        "Noskov",
    "Ozerov",       "Koltsov",      "Komissarov",    "Merkulov",
    "Kireev",       "KHomiakov",    "Bulatov",       "Ananev",
    "Burov",        "Shaposhnikov", "Druzhinin",     "Ostrovskii",
    "Shevelev",     "Dolgov",       "Suslov",        "Shevtsov",
    "Pastukhov",    "Rubtsov",      "Bychkov",       "Glebov",
    "Ilinskii",     "Uspenskii",    "Diakov",        "Kochetov",
    "Vishnevskii",  "Vysotskii",    "Glukhov",       "Dubov",
    "Bessonov",     "Sitnikov",     "Astafev",       "Meshkov",
    "Sharov",       "IAshin",       "Kozlovskii",    "Tumanov",
    "Basov",        "Korchagin",    "Boldyrev",      "Oleinikov",
    "Chumakov",     "Fomichev",     "Gubanov",       "Dubinin",
    "Shulgin",      "Kasatkin",     "Pirogov",       "Semin",
    "Troshin",      "Gorokhov",     "Starikov",      "Shcheglov",
    "Fetisov",      "Kolpakov",     "Chesnokov",     "Zykov",
    "Vereshchagin", "Minaev",       "Rudnev",        "Troitskii",
    "Okulov",       "Shiriaev",     "Malinin",       "Cherepanov",
    "Izmailov",     "Alekhin",      "Zelenin",       "Kasianov",
    "Pugachev",     "Pavlovskii",   "Chizhov",       "Kondratov",
    "Voronkov",     "Kapustin",     "Sotnikov",      "Demianov",
    "Kosarev",      "Belikov",      "Sukharev",      "Belkin",
    "Bespalov",     "Kulagin",      "Savitskii",     "ZHarov",
    "KHromov",      "Eremeev",      "Kartashov",     "Astakhov",
    "Rusanov",      "Sukhov",       "Veshniakov",    "Voloshin",
    "Kozin",        "KHudiakov",    "ZHilin",        "Malakhov",
    "Sizov",        "Ezhov",        "Tolkachev",     "Anokhin",
    "Vdovin",       "Babushkin",    "Usov",          "Lykov",
    "Gorlov",       "Korshunov",    "Markelov",      "Postnikov",
    "Chernyi",      "Dorokhov",     "Sveshnikov",    "Gushchin",
    "Kalugin",      "Blokhin",      "Surkov",        "Kochergin",
    "Grekov",       "Kazantsev",    "Shvetsov",      "Ermilov",
    "Paramonov",    "Agapov",       "Minin",         "Kornev",
    "Cherniaev",    "Gurov",        "Ermolov",       "Somov",
    "Dobrynin",     "Barsukov",     "Glushkov",      "Chebotarev",
    "Moskvin",      "Uvarov",       "Bezrukov",      "Muratov",
    "Rakov",        "Snegirev",     "Gladkov",       "Zlobin",
    "Morgunov",     "Polikarpov",   "Riabinin",      "Sudakov",
    "Kukushkin",    "Kalachev",     "Gribov",        "Elizarov",
    "Zviagintsev",  "Korolkov",     "Fedosov"};

int lower_bound_s = 0;
int upper_bound_s = families.size() - 1;
std::uniform_int_distribution<int> unif_s(lower_bound_s, upper_bound_s);
std::default_random_engine re_s;
std::string random_string() { return families[unif_s(re_s)]; }

std::vector<char> letters = {'A', 'B', 'D', 'E', 'F', 'G', 'I', 'J', 'K',
                             'L', 'M', 'N', 'P', 'S', 'T', 'Y', 'Z'};
int lower_bound_c = 0;
int upper_bound_c = letters.size() - 1;
std::uniform_int_distribution<int> unif_c(lower_bound_c, upper_bound_c);
std::default_random_engine re_c;
char random_char() { return letters[unif_c(re_c)]; }

double lower_bound_d = 0;
double upper_bound_d = 10;
std::uniform_real_distribution<double> unif_d(lower_bound_d, upper_bound_d);
std::default_random_engine re_d;
double random_double() { return unif_d(re_d); }

int lower_bound_i = 100;
int upper_bound_i = 300;
std::uniform_int_distribution<int> unif_i(lower_bound_i, upper_bound_i);
std::default_random_engine re_i;
int random_int() { return unif_i(re_i); }

void create_file(int n, std::ostream &os) {
  char delim = ',';
  for (int i = 0; i < n; ++i) {
    os << random_string() << ' ' << random_char() << '.' << random_char() << '.'
       << delim << random_int() << delim << random_double() << delim << "good" << '\n';
  }
}

int main(void) {
    int n = 100000;
    std::ofstream ofs("__database.csv", std::ios::binary);
    create_file(n, ofs);
    return 0;
}