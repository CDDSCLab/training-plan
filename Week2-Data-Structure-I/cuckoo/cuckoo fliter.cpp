#include <iostream>
#include <fstream>
#include <map>
#include <random>
#include <ctime>
#include <string>

using namespace std;

class Filter {
private:
    static const int FINGER_PRINT_SIZE = 7;
    typedef uint8_t fp_t;
    typedef uint8_t Bucket[4];
    typedef uint64_t hash_t;
    uint64_t countBuckets;
    const int MAX_NUM_KICKS = 500;
public:
    Bucket *masBuckets;

    explicit Filter(int countBuckets) {
        this->countBuckets = roundToPowerOfTwo(ceil(countBuckets * 1.06));
        masBuckets = new Bucket[this->countBuckets];
        for (int i = 0; i < this->countBuckets; ++i) {
            for (int j = 0; j < 4; ++j) {
                masBuckets[i][j] = 0;
            }
        }
    }

    explicit Filter() {
        this->countBuckets = 0;
        masBuckets = new Bucket[this->countBuckets];
    }

    ~Filter() {
        delete[] masBuckets;
    }

    static fp_t hashFingerPrint(const char *str) {
        size_t hash = 5381;
        while (uint8_t c = *str++) {
            hash = ((hash << 5u) + hash) + c;
        }
        return hash % ((1u << unsigned(FINGER_PRINT_SIZE)) - 1) + 1;
    }


    hash_t hashBucket(const char *str) const {
        return std::hash<string>{}(str) % countBuckets;

    }


    void add(const char *str) const {
        uint8_t x = hashFingerPrint(str);
        uint64_t i = hashBucket(str);
        uint64_t j = (i ^ hashBucket(to_string(hashFingerPrint(str)).c_str())) % countBuckets;
        for (int count = 0; count < MAX_NUM_KICKS; ++count) {
            for (int k = 0; k < 4; ++k) {
                if (masBuckets[i][k] == 0) {
                    masBuckets[i][k] = x;
                    return;
                }
                if (masBuckets[j][k] == 0) {
                    masBuckets[j][k] = x;
                    return;
                }
                if (k == 3) {
                    std::random_device rand;
                    std::uniform_int_distribution<int> UID(0, 1);
                    std::uniform_int_distribution<int> UID2(0, 3);
                    int r = UID(rand);
                    uint64_t p;
                    if (r == 0) p = i;
                    else p = j;
                    int indexFingerPrint = UID2(rand) % 4;
                    uint8_t e = masBuckets[k][indexFingerPrint];
                    masBuckets[k][indexFingerPrint] = x;
                    p = p ^ hashBucket(to_string(e).c_str());
                    i = j = p;
                    x = e;
                }
            }
        }
    }

    string check(const char *str) const {
        uint8_t x = hashFingerPrint(str);
        uint64_t i = hashBucket(str);
        uint64_t j = (i ^ hashBucket(to_string(hashFingerPrint(str)).c_str())) % countBuckets;
        for (int k = 0; k < 4; ++k) {
            if (masBuckets[i][k] == x || masBuckets[j][k] == x) return "Probably";
        }
        return "No";
    }

    static uint64_t roundToPowerOfTwo(uint64_t size) {
        uint64_t k;
        uint64_t l;

        for (k = size; (l = k & (k - 1)) != 0; k = l);

        return (size + (k - 1)) & (~(k - 1));
    }


};

static map<string, Filter *> mapUsers;

static string fillInfoFromFile(const std::string &pathInput) {
    fstream file(pathInput);
    string way, user, video;
    uint64_t countVideo;
    string finalRes = "Ok";
    file >> video >> countVideo;
    while (!file.eof()) {
        file >> way >> user >> video;
        if (way == "check") {
            if (mapUsers.count(user) == 0) {
                finalRes += "\nNo";
            } else {
                string res = mapUsers[user]->check(video.c_str());
                finalRes += "\n" + res;
            }

        } else {
            if (mapUsers.count(user) > 0) {
                mapUsers[user]->add(video.c_str());
            } else {
                mapUsers.insert(make_pair(user, new Filter(countVideo)));
                mapUsers[user]->add(video.c_str());
            }
            finalRes += "\nOk";
        }


    }
    return finalRes;
}

static void printFile(const string &res, const string &pathPrint) {
    std::ofstream out; 
    out.open(pathPrint); 
    if (out.is_open()) {
        out << res;
    }
}

int main(int argc, char *argv[]) {
    if (argc == 3) {
        string path = argv[1];
        string res = fillInfoFromFile(path);
        for (const auto &elem:mapUsers) {
            delete elem.second;
        }
        printFile(res, argv[2]);
    }
    return 0;
}