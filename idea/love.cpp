#include<iostream>
#include<string>
#include<pthread.h>

const int NUM_DIMS = /* should be infinite but on a computer there is limits */ 8;

class Energy {
public:
    Energy() = default;
    Energy(const std::string &t) : e(t) { }
    std::string &thought() { return e; }
    friend std::ostream &operator<<(std::ostream &out, Energy &e);
    friend std::istream &operator>>(std::istream &in, Energy &e);
    bool operator==(const Energy &w) { return (e == w.e); }
    bool operator!=(const Energy &w) { return (e != w.e); }
protected:
    std::string e;
};

std::ostream &operator<<(std::ostream &out, Energy &e) {
    out << e.thought() << "\n";
    return out;
}

std::istream &operator>>(std::istream &in, Energy &e) {
    in >> e.thought();
    return in;
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

template<typename E>
class Life {
public:
    explicit Life(E e) : end(e) {
        int s;
        s = pthread_mutex_lock(&mutex);
        if(s != 0) return;
        std::cout << "New life brought into this dimension\n";
        s = pthread_mutex_unlock(&mutex);
        if(s != 0) return;
    }
    ~Life() {
        int s;
        s = pthread_mutex_lock(&mutex);
        if(s != 0) return;
        std::cout << "Leaving this dimension.\n";
        s = pthread_mutex_unlock(&mutex);
        if(s != 0) return;
    }
    bool get(E energy) {
        if(energy.thought() == "1" || energy.thought() == "love") {
            std::cout << "Gave Love\n";
        }
        else if(energy != end) {
            std::cout << "Gave fear\n";
        }
        else {
            std::cout << "Return ...\n";
        }
        return (energy == end) ? false : true;
    }
    E give() {
        std::cout << "Enter thought: ";
        E e;
        std::getline(std::cin, e.thought());
        return e;
    }
private:
    E end;
};


void *life(void *) {
    int rvalue = 0;
    while(1) {
        Life<Energy> being(Energy("death"));
        bool karma = true;
        do {
            rvalue = pthread_mutex_lock(&mutex);
            if(rvalue != 0) {
                std::cerr << "Error locking..\n";
                return 0;
            }
            karma = being.get(being.give());
            std::cout << "Karma cycle\n";
            rvalue = pthread_mutex_unlock(&mutex);
            if(rvalue != 0) {
                std::cerr << "Error unlocking..\n";
                return 0;
            }
        } while(karma == true);
    }
}

int main(int argc, char **argv) {
    pthread_t numDimensions[/*INFINITE but on comptuer */ NUM_DIMS];
    for(unsigned int i = 0; i < NUM_DIMS; ++i) {
        pthread_create(&numDimensions[i], 0, life, 0);
    }
    for(unsigned int i = 0; i < NUM_DIMS; ++i) {
        pthread_join(numDimensions[i], 0);
    }
    return 0;
}
