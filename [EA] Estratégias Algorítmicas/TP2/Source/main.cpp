#include <iostream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

vector<int> split(const string &s, char delimiter) {
    vector<int> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) tokens.push_back(stoi(token));
    return tokens;
}

class Member {
public:
    int recruiter, value;
    int countVertex = 0;
    int sumValue = 0;
    vector<int> recruited;

    Member() {}

    Member(int value) {
        this->recruiter = -1;
        this->value = value;
    }
};

class Pyramid {
    vector<int> temp;
    map<int, Member> tree;

public:
    int first, total = 0;
    Pyramid() {}

    int findMember(int key) {
        auto aux = this->tree.find(key);
        if (aux != this->tree.end()) return aux->first;
        return -1;
    }

    void addPyramid(vector<int> line) {
        Member member = *new Member(line[line.size() - 1]);
        
        if (tree.empty()) this->first = line[0];
        
        int check = findMember(line[0]);
        
        if (check == -1) this->tree.insert({line[0], member});
        else this->tree[check].value = line[line.size() - 1];

        for (int i = 1; i < int(line.size() - 1); i++) {

            check = findMember(line[i]);

            if (check == -1) {
                member = *new Member();
                member.recruiter = line[0];
                this->tree.insert({line[i], member});
                this->tree[line[0]].recruited.push_back(line[i]);
            }

            else this->tree[line[0]].recruited.push_back(line[i]);
        }
    }

    vector<int> pyramidProblem(int member) {

        if (this->tree[member].recruited.empty()) return {0,0};

        if (this->tree[member].countVertex != 0) return {this->tree[member].countVertex,this->tree[member].sumValue};

        // quando o membro faz parte do conjunto de membros

        int vertexIn = 1;
        int auxValue1 = this->tree[member].value;

        for (auto recruited : this->tree[member].recruited) {
            temp = pyramidProblem(recruited);
            vertexIn += temp[0];
            auxValue1 += temp[1];
        }

        // quando o membro nao faz parte do menor conjunto de membros

        int vertexOut = 0;
        int valueOut = 0;

        for (auto recruited : this->tree[member].recruited) {
            vertexOut += 1;
            valueOut += this->tree[recruited].value;
            
            for (auto r : this->tree[recruited].recruited) {
                temp = pyramidProblem(r);
                vertexOut += temp[0];
                valueOut += temp[1];
            }
        }

        //pretende-se sempre escolher o conjunto com menor numero de membros (vertices)
        //em caso de empate, prefere-se o conjunto com maior valor de soma

        if (vertexIn == vertexOut) {
            this->tree[member].countVertex = vertexOut;
            this->tree[member].sumValue = max(auxValue1, valueOut);
        }
        else if (vertexIn < vertexOut) {
            this->tree[member].countVertex = vertexIn;
            this->tree[member].sumValue = auxValue1;
        }
        else {
            this->tree[member].countVertex = vertexOut;
            this->tree[member].sumValue = valueOut;
        }
        return {this->tree[member].countVertex,this->tree[member].sumValue};
    }
};

int main() {
    
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Pyramid piramide = *new Pyramid();

    try {
        string line;

        while (std::getline(std::cin, line)) {

            if (stoi(line) == -1) {
                vector<int> aux = piramide.pyramidProblem(piramide.first);
                printf("%d %d\n", aux[0], aux[1]);
                piramide = *new Pyramid();
            }
            else piramide.addPyramid(split(line, ' '));
        }
    }
    catch (std::invalid_argument const &){}
    return 0;
}