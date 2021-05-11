#ifndef TASK6_MENU_HPP
#define TASK6_MENU_HPP

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <cmath>

using std::map;
using std::string;
using std::cout;
using std::endl;
using std::size_t;
using std::ceil;
using std::max;

typedef void (*pfunc)();

struct menuItem {
    string description;
    string cmdSyntax;
    pfunc func;
};

class Menu {
public:
    map<string, menuItem> menuItems;
    size_t width = 80;
    double ratio = 0.5;
    size_t descrWidth = 0;
    size_t cmdStxWidth = 0;
    string title = "Menu";


    void addItem(const string &name, const menuItem &item);

    void setTitle(const string &name);

    void removeItem(const string &name);

    void setWidth(const size_t &w);

    size_t getWidth() const;

    void setRatio(const double &r);

    double getRatio() const;

    void printHeader(const string &str);

    void printItem(const menuItem &item);

    void printFooter();

    void computeWiths();

    void print();


};

void Menu::addItem(const string &name, const menuItem &item) {
    this->menuItems[name] = item;
}

void Menu::removeItem(const string &name) {
    this->menuItems.erase(name);
}

void Menu::setWidth(const size_t &w) {
    this->width = w;
}

size_t Menu::getWidth() const {
    return this->width;
}

void Menu::print() {
    this->computeWiths();
    this->printHeader(this->title);
    for (auto const &[key, value] : this->menuItems) {
        this->printItem(value);
    }
    this->printFooter();
}

void Menu::printHeader(const string &str) {
    cout << (char) 201; // ╔
    for (int i = 0; i < this->width - 2; ++i) {
        cout << (char) 205; // ═
    }
    cout << (char) 187; // ╗
    cout << endl;

    cout << (char) 186; // ║
    for (int i = 0; i < (width - str.size() - 2) / 2; ++i) {
        cout << ' ';
    }

    cout << str;

    for (int i = 0; i < (width - str.size() - 1) / 2; ++i) {
        cout << ' ';
    }
    cout << (char) 186; // ║
    cout << endl;

    cout << (char) 204; // ╠
    for (int i = 0; i < this->cmdStxWidth; ++i) {
        cout << (char) 205; // ═
    }
    cout << (char) 203; // ╦
    for (int i = 0; i < this->descrWidth; ++i) {
        cout << (char) 205; // ═
    }

    cout << (char) 185; // ╣
    cout << endl;


}

void Menu::printItem(const menuItem &item) {

    if (item.cmdSyntax.size() > this->cmdStxWidth) {

    }
    size_t stxLinesCount = item.cmdSyntax.size() / this->cmdStxWidth
                           + !!(item.cmdSyntax.size() % this->cmdStxWidth);

    size_t descrLinesCount = item.description.size() / this->descrWidth
                             + !!(item.description.size() % this->descrWidth);

    size_t n = max(stxLinesCount, descrLinesCount);

    for (int k = 0; k < n; ++k) {

        string
                stx = k < stxLinesCount ?
                      item.cmdSyntax.substr(k * this->cmdStxWidth, this->cmdStxWidth) : "",
                descr = k < descrLinesCount ?
                        item.description.substr(k * this->descrWidth, this->descrWidth) : "";


        cout << (char) 186; // ║

        size_t stxSpaceBefore = k ? 0 : (this->cmdStxWidth - stx.size()) / 2;

        for (int i = 0; i < stxSpaceBefore; ++i) {
            cout << ' ';
        }
        cout << stx;
        for (int i = 0; i < this->cmdStxWidth - stxSpaceBefore - stx.size(); ++i) {
            cout << ' ';
        }

        cout << (char) 186; // ║


        size_t descrSpaceBefore = k ? 0 : (this->descrWidth - descr.size()) / 2;
        for (int i = 0; i < descrSpaceBefore; ++i) {
            cout << ' ';
        }
        cout << descr;
        for (int i = 0; i < this->descrWidth - descrSpaceBefore - descr.size(); ++i) {
            cout << ' ';
        }

        cout << (char) 186; // ║

        cout << endl;


    }

    cout << (char) 204; // ╠

    for (int i = 0; i < this->cmdStxWidth; ++i) {
        cout << (char) 205; // ═
    }

    cout << (char) 203; //╦

    for (int i = 0; i < this->descrWidth; ++i) {
        cout << (char) 205; // ═
    }

    cout << (char) 185; // ╣

    cout << endl;
}

void Menu::setRatio(const double &r) {
    this->ratio = r;
}

double Menu::getRatio() const {
    return this->ratio;
}

void Menu::computeWiths() {
    if (!this->cmdStxWidth) {
        this->cmdStxWidth = ceil(this->width * this->ratio / (1 + this->ratio));
        this->descrWidth = this->width - cmdStxWidth - 3;
    }
}

void Menu::printFooter() {
    cout << (char) 200; //╚

    for (int i = 0; i < this->cmdStxWidth; ++i) {
        cout << (char) 205; // ═
    }

    cout << (char) 202; //╩

    for (int i = 0; i < this->descrWidth; ++i) {
        cout << (char) 205; // ═
    }

    cout << (char) 188; //╝
}

void Menu::setTitle(const string &name) {
    this->title = name;
}


#endif //TASK6_MENU_HPP
