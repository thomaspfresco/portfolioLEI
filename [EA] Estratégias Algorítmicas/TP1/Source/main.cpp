#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class Piece{
    bool used;
    
public:
    vector<int> cores;
    Piece(){
        this->cores.resize(4);
        used=false;
    }
    void setUsed(bool isUsed) {
        used=isUsed;
    }
    bool getUsed() const{
        return this->used;
    }

};
class Board{
    int lSize, cSize,l,c;
    map<int,int> contagem;
    map<vector<int>,vector<Piece*>> pares;
    map<vector<int>,vector<Piece*>> trios;

public:
    vector<vector<Piece>> board;

    Board(int lSize,int cSize){
        this->lSize=lSize;
        this->cSize=cSize;
        this->l=0,this->c=0;
        this->board.resize(lSize);
        for (int i=0;i<lSize;i++)
            this->board[i].resize(cSize);
    }

    bool solve(){
        if(this->board[lSize-1][cSize-1].getUsed())
            return true;
        vector<int> needed=subSequenceNeededInt();
        if(!doesExistInt(needed))
            return false;

        vector<Piece*> &toTest = getDictInt(needed);
        for (auto &piece : toTest) {
            if(piece->getUsed())
                continue;
            piece->setUsed(true);
            rotatePieceInt(piece->cores, needed);
            this->board[l][c] = *piece;
            updateCoords();
            if(solve())
                return true;
            decrementCoords();
            piece->setUsed(false);
        }
        return false;
    }

    vector<Piece*> &getDictInt(const vector<int>& needed){
        if(needed.size()==2) return this->pares.find(needed)->second;
        else return this->trios.find(needed)->second;
    }

    void addAnchorPiece(Piece piece){
        this->board[0][0]=std::move(piece);
        this->updateCoords();
    }

    bool doesExistInt(const vector<int>& needed){
        if(needed.size()==2) {
            if (pares.count(needed))
                return true;
        }
        else
            if(trios.count(needed)) return true;
        return false;
    }
    
    void addCount(int toAdd){
        if(contagem.count(toAdd)) contagem[toAdd]++;
        else contagem.insert({toAdd,1});
    }

    [[nodiscard]] int getCase() const {
        if(l==0) return 1;
        if(c==0)return 0;
        return 2;
    }

    void rotatePieceInt(vector<int> &piece, vector<int> find){
        
        rotate(piece.begin(),piece.begin()+1,piece.end());
        
        switch (this->getCase()) {
            case 0:
                while (piece[0]!=find[0] || piece[1]!=find[1])
                    rotate(piece.begin(),piece.begin()+1,piece.end());
                break;
            case 1:
                while (piece[0]!=find[1] || piece[3]!=find[0])
                    rotate(piece.begin(),piece.begin()+1,piece.end());
                break;
            case 2:
                while (piece[3] != find[0] || piece[0]!=find[1] || piece[1]!=find[2])
                    rotate(piece.begin(),piece.begin()+1,piece.end());
                break;
        }
    }

    vector<int> subSequenceNeededInt(){
        vector<int> find;
        switch (getCase()) {
            case 0://x=0
                find.resize(2);
                find[0]=this->board[l-1][c].cores[3];
                find[1]=this->board[l-1][c].cores[2];

                return find;
            case 1://y=0
                find.resize(2);
                find[0]=this->board[l][c-1].cores[2];
                find[1]=this->board[l][c-1].cores[1];
                return find;
            case 2:
                find.resize(3);
                find[0]=this->board[l][c-1].cores[2];
                find[1]=this->board[l][c-1].cores[1];
                find[2]=this->board[l-1][c].cores[2];
                return find;
        }
        return find;
    }

    void updateCoords(){
        this->c=this->c+1;
        if(this->c == cSize) {
            this->l += 1;
            this->c = 0;
        }
    }

    void decrementCoords(){
        if(this->c==0){
            this->l-=1;
            this->c= this->cSize-1;
            return;
        }
        this->c-=1;
    }

    void insertIntoDicts(Piece &piece){
        int j;
        vector<int> par;
        par.resize(2);

        for (int i = 0; i < 4; ++i) {
            j = i;
            par[0] = piece.cores[j];
            if(i>2 && i%3==0) j = -1;
            par[1] = piece.cores[j+1];

            if(pares.count(par)) pares[par].insert(pares[par].end(),&piece);
            else{
                vector<Piece*> aux;
                aux.insert(aux.begin(),&piece);
                pares.insert({par,aux});
            }
        }

        vector<int> trio;
        trio.resize(3);

        for (int i = 0; i < 4; ++i) {

            j=i;
            trio[0] = piece.cores[i];
            if (i>2 && i%3==0) j = -1;
            trio[1] = piece.cores[j + 1];
            if ((j+1)>2 && ((j+1)%3==0)) j = -2;
            if (i>2 && i%3==0) j = -1;
            trio[2] = piece.cores[j + 2];

            if(trios.count(trio)){
                trios[trio].insert(trios[trio].end(),&piece);
            } else{
                vector<Piece*> aux;
                aux.insert(aux.begin(),&piece);
                trios.insert({trio,aux});
            }
        }
    }
    
    bool checkValidity(){
        int a=0;
        for (auto color: this->contagem) {
            if(color.second % 2 != 0) {
                a++;
                if(a>4) return false;
            }
        }
        return true;
    }
};

static void printLineInt(vector<Piece> line){
    for (int i = 0; i < 2; i++) {
        for (long unsigned collumn = 0; collumn < line.size(); collumn++) {
            if(i==0) {
                std::cout << line[collumn].cores[0];
                std::cout << " ";
                std::cout << line[collumn].cores[1];
            }else{
                std::cout << line[collumn].cores[3];
                std::cout << " ";
                std::cout << line[collumn].cores[2];
            }
            if(collumn<line.size()-1) std::cout << "  ";
        }
        if(i!=1)std::cout << "\n";
    }
}

static void printBoardInt(Board board){
    for (long unsigned line = 0; line < board.board.size(); line++) {
        printLineInt(board.board[line]);
        if(line<board.board.size()-1) std::cout << "\n\n";
    }
}

int nextInt(){
    int next;
    std::cin >> next;
    return next;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Board board = *new Board(2,2);

    int nTestCases;
    std::cin >> nTestCases;
    int N,L,C,next;

    for(int i=0;i<nTestCases;i++){
        if(i!=0) std::cout << "\n";
        std::cin >> N;
        std::cin >> L;
        std::cin >> C;

        board=*new Board(L,C);

        for (int j = 0; j < N; ++j) {
            int cor[4];
            for (int k = 0; k < 4; k++) {
                next=nextInt();
                cor[k]=next;
                board.addCount(next);
            }
            Piece &toAdd=*new Piece();
            copy(begin(cor), end(cor), begin(toAdd.cores));

            if(j==0) board.addAnchorPiece(toAdd);
            else board.insertIntoDicts(toAdd);
        }
        if(!board.checkValidity() || !board.solve()) std::cout << "impossible puzzle!";
        else printBoardInt(board);
        
        if(i==nTestCases-1) cout << "\n";
    }
    return 0;
}