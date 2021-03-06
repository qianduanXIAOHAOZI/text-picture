#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include <string.h>
#include <algorithm>
#include <map>
#include "../include/picture.h"
using std::string;
using std::vector;
using std::max;

void SplitString(const string& s, vector<string>& v, const string& c)
{
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}

picture::picture(string* list, int l, align a = LEFT)
        : pic("")
{
    int m = 0;
    for (int i = 0 ; i < l ; i++) {
        m = max((unsigned long)m, list[i].length());
    }
    string tmp("");
    for (int i = 0 ; i < l ; i++) {
        tmp = "";
        for (int j = 0 ;j < m - list[i].length() ; j++) {
            tmp += " ";
        }
        if (a == LEFT) {
//            std::cout << "l" << std::endl;
            list[i] += tmp;
        } else if (a == RIGHT) {
//            std::cout << m - list[i].length() << std::endl;
            list[i] = tmp + list[i];
        } else {
            tmp = "";
            int c = (m - list[i].length()) / 2;
            bool n = false;
            if ((m - list[i].length()) % 2 == 1) {
                n = true;
            }
            for (int j = 0 ;j < c ; j++) {
                tmp += " ";
            }
            list[i] = tmp + list[i];
            if (n) {
                tmp += " ";
            }
            list[i] += tmp;
        }
    }
    for (int i = 0 ; i < l ; i++) {
        pic += list[i] + "\n";
    }
}
picture::picture(const picture & p) {
    pic = p.pic;
}

string picture::frame(string s = "+", string l = "-", string p = "|") {
    vector<string> v;
    SplitString(pic, v, "\n");
    for(vector<string>::size_type i = 0 ; i != v.size() ; ++i)
        v[i] = p + " " + v[i] + " " + p + "\n";
    int len = v[0].length() - 3;
    pic = "";
    for(vector<string>::size_type i = 0 ; i != v.size() ; ++i) {
        pic += v[i];
    }
    string tmp("");
    for (int i = 0 ; i < len ; i++) {
        tmp += l;
    }
    pic = s + tmp + s + "\n" + pic + s + tmp + s;
    return pic;
}
picture& picture::connectionL(picture& p) {
    string picTmp = pic;
    vector<string> v1;
    SplitString(pic, v1, "\n");
    vector<string> v2;
    SplitString(p.pic, v2, "\n");
    int len = v1[0].length();
    string space = "";
    for (int i = 0 ; i < len ; i++) {
        space += " ";
    }
    pic = "";
    if (v2.size() < v1.size()) {
        len = v2[0].length();
        space = "";
        for (int i = 0 ; i < len ; i++) {
            space += " ";
        }
        for (int i = 0 ; i < v2.size() ; i++) {
            pic += v1[i] + v2[i] + "\n";
        }
        for (int i = v2.size() ; i < v1.size() - 1 ; i++) {
            pic += v1[i] + space + "\n";
        }
        pic += v1[v1.size() - 1] + space;
    } else {
        for (int i = 0 ; i < v1.size() ; i++) {
            pic += v1[i] + v2[i] + "\n";
        }
        for (int i = v1.size() ; i < v2.size() - 1 ; i++) {
            pic += space + v2[i] + "\n";
        }
        pic += space + v2[v2.size() - 1];
    }
    return *this;
}
picture& picture::connectionP(picture& p) {
    vector<string> v1;
    SplitString(pic, v1, "\n");
    vector<string> v2;
    SplitString(p.pic, v2, "\n");
    int len = v1[0].length() - v2[0].length();
    if (v1[0].length() > v2[0].length()) {
        for (int i = 0 ; i < v2.size() ; i++) {
            for (int j = 0 ; j < len ; j++) {
                v2[i] += " ";
            }
        }
    } else {
        len = -len;
        for (int i = 0 ; i < v1.size() ; i++) {
            for (int j = 0 ; j < len ; j++) {
                v1[i] += " ";
            }
        }
    }
    pic = "";
    for (int i = 0 ; i < v1.size() ; i++) {
        pic += v1[i] + "\n";
    }
    for (int i = 0 ; i < v2.size() - 1 ; i++) {
        pic += v2[i] + "\n";
    }
    pic += v2[v2.size() - 1];
    return *this;
}
picture& picture::load(character* chs, int len, size s) {
    char p[s.y][s.x];
    for (int i = 0 ; i < s.y ; i++) {
        for (int j = 0 ; j < s.x ; j++) {
            p[i][j] = ' ';
        }
    }
    for (int i = 0 ; i < len ; i++) {
        character tmp = *(chs + i);
        int l = tmp.len;
        for (int j = 0 ; j < l ; j++) {
            point pos = *(tmp.points + j);
            if (
                    pos.x < 0 || pos.x >= s.x
                    || pos.y < 0 || pos.y >= s.y
                    )
            {
                return *this;
            }
            p[pos.y][pos.x] = tmp.style;
        }
    }
    pic = "";
    for (int i = 0 ; i < s.y - 1 ; i++) {
        for (int j = 0 ; j < s.x ; j++) {
            pic += p[i][j];
        }
        pic += "\n";
    }
    for (int j = 0 ; j < s.x ; j++) {
        pic += p[s.y - 1][j];
    }
    return *this;
}
picture& picture::operator=(picture & p) {
    pic = p.pic;
    return *this;
}
picture& picture::operator-=(picture& p) {
    return connectionL(p);
}
picture& picture::operator|=(picture& p) {
    return connectionP(p);
}
picture& picture::operator+=(string& s) {
    picture test(*this);
    test = test + s;
    pic = test.pic;
    return *this;
}

picture& operator-(picture& a, picture& b) {
    picture test(a);
    return test.connectionL(b);
}
picture& operator|(picture& a, picture& b) {
    picture test(a);
    return test.connectionP(b);
}