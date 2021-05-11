#ifndef TASK6_MENU_HPP
#define TASK6_MENU_HPP

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <cmath>

using std::map;
using std::string;
using std::cin;
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



    void addItem(const string &name, const menuItem &item);

    void setTitle(const string &name);

    void removeItem(const string &name);

    void setWidth(const size_t &w);

    size_t getWidth() const;

    void setRatio(const double &r);

    double getRatio() const;

    void print();

    void run();

private:
    map<string, menuItem> _menuItems;
    size_t _width = 80;
    double _ratio = 0.5;
    size_t _descrWidth = 0;
    size_t _cmdStxWidth = 0;
    string _title = "Menu";

    void _printHeader(const string &str);

    void _printItem(const menuItem &item);

    void _printFooter();

    void _computeWiths();

};

void Menu::addItem(const string &name, const menuItem &item) {
    this->_menuItems[name] = item;
}

void Menu::removeItem(const string &name) {
    this->_menuItems.erase(name);
}

void Menu::setWidth(const size_t &w) {
    this->_width = w;
}

size_t Menu::getWidth() const {
    return this->_width;
}

void Menu::print() {
    this->_computeWiths();
    this->_printHeader(this->_title);
    for (auto const &[key, value] : this->_menuItems) {
        this->_printItem(value);
    }
    this->_printFooter();
    cout<<endl;
}

void Menu::_printHeader(const string &str) {
    cout << (char) 201; // ╔
    for (int i = 0; i < this->_width - 2; ++i) {
        cout << (char) 205; // ═
    }
    cout << (char) 187; // ╗
    cout << endl;

    cout << (char) 186; // ║
    for (int i = 0; i < (_width - str.size() - 2) / 2; ++i) {
        cout << ' ';
    }

    cout << str;

    for (int i = 0; i < (_width - str.size() - 1) / 2; ++i) {
        cout << ' ';
    }
    cout << (char) 186; // ║
    cout << endl;

    cout << (char) 204; // ╠
    for (int i = 0; i < this->_cmdStxWidth; ++i) {
        cout << (char) 205; // ═
    }
    cout << (char) 203; // ╦
    for (int i = 0; i < this->_descrWidth; ++i) {
        cout << (char) 205; // ═
    }

    cout << (char) 185; // ╣
    cout << endl;


}

void Menu::_printItem(const menuItem &item) {

    if (item.cmdSyntax.size() > this->_cmdStxWidth) {

    }
    size_t stxLinesCount = item.cmdSyntax.size() / this->_cmdStxWidth
                           + !!(item.cmdSyntax.size() % this->_cmdStxWidth);

    size_t descrLinesCount = item.description.size() / this->_descrWidth
                             + !!(item.description.size() % this->_descrWidth);

    size_t n = max(stxLinesCount, descrLinesCount);

    for (int k = 0; k < n; ++k) {

        string
                stx = k < stxLinesCount ?
                      item.cmdSyntax.substr(k * this->_cmdStxWidth, this->_cmdStxWidth) : "",
                descr = k < descrLinesCount ?
                        item.description.substr(k * this->_descrWidth, this->_descrWidth) : "";


        cout << (char) 186; // ║

        size_t stxSpaceBefore = k ? 0 : (this->_cmdStxWidth - stx.size()) / 2;

        for (int i = 0; i < stxSpaceBefore; ++i) {
            cout << ' ';
        }
        cout << stx;
        for (int i = 0; i < this->_cmdStxWidth - stxSpaceBefore - stx.size(); ++i) {
            cout << ' ';
        }

        cout << (char) 186; // ║


        size_t descrSpaceBefore = k ? 0 : (this->_descrWidth - descr.size()) / 2;
        for (int i = 0; i < descrSpaceBefore; ++i) {
            cout << ' ';
        }
        cout << descr;
        for (int i = 0; i < this->_descrWidth - descrSpaceBefore - descr.size(); ++i) {
            cout << ' ';
        }

        cout << (char) 186; // ║

        cout << endl;


    }

    cout << (char) 204; // ╠

    for (int i = 0; i < this->_cmdStxWidth; ++i) {
        cout << (char) 205; // ═
    }

    cout << (char) 203; //╦

    for (int i = 0; i < this->_descrWidth; ++i) {
        cout << (char) 205; // ═
    }

    cout << (char) 185; // ╣

    cout << endl;
}

void Menu::setRatio(const double &r) {
    this->_ratio = r;
}

double Menu::getRatio() const {
    return this->_ratio;
}

void Menu::_computeWiths() {
    if (!this->_cmdStxWidth) {
        this->_cmdStxWidth = ceil(this->_width * this->_ratio / (1 + this->_ratio));
        this->_descrWidth = this->_width - _cmdStxWidth - 3;
    }
}

void Menu::_printFooter() {
    cout << (char) 200; //╚

    for (int i = 0; i < this->_cmdStxWidth; ++i) {
        cout << (char) 205; // ═
    }

    cout << (char) 202; //╩

    for (int i = 0; i < this->_descrWidth; ++i) {
        cout << (char) 205; // ═
    }

    cout << (char) 188; //╝
}

void Menu::setTitle(const string &name) {
    this->_title = name;
}

void Menu::run() {
    this->print();
    string cmd;
    while (cmd!="/exit"){
        cin>>cmd;
        for (auto const &[key, value] : this->_menuItems){
            if(key == cmd) value.func();
        }
    }
}


#endif //TASK6_MENU_HPP
