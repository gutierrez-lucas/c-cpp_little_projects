#include <iostream>
#include <iomanip> 
#include <cstdint>
#include <assert.h>
#include <vector>
#include <fstream>

// using namespace std;
#define DIMENSION 3
const int INF = 0xFFFFFF;
const int MAX_WEIGHT        = 10;

class graphClass{
    public:
        graphClass():vertices(0), edges(0){}
        graphClass(int v, int density){
            assert(v>=0);
            srand(clock()); 
            // every time an object is construct, a new seed is feed into srand, and so the next objetc will be random with respecto to the previous one
            vertices = v;
            edges=0;
            graph = new int*[ vertices];
            for(int i=0; i<= vertices; i++) graph[i] = new int[ vertices];
            for(int i=0; i< vertices; ++i){
                for(int j=i; j< vertices; ++j){
                    if(i==j) graph[i][j] = 0;
                    else
                        if(rand()%100 < density && density != 0){
                            graph[i][j] = graph[j][i] = (rand()%(MAX_WEIGHT-1))+1; //workaround geting 1 to 10 range for random
                            edges++;
                        }else{
                            graph[i][j] = graph[j][i] = 0;  //no self to self road
                        }
                }
            }
        }

        graphClass(const char* filename){
            
            std::vector<int> data;
            std::string line, aux_line;
            int8_t character_index;

            vertices = 0;
            edges=0;

            std::ifstream my_file(filename);
            assert(my_file && "File does not exist.");          

            bool first_line = true;
            while (std::getline(my_file, line)) {
                if(first_line == true){                  // first line is the amount of vertices 
                    first_line = false;
                    character_index=0;
                    while(line.size() != character_index){
                        aux_line.push_back(line.at(character_index));
                        character_index++;
                    }
                    vertices = stoi(aux_line);
                    std::cout << "vertices " << vertices << std::endl;
                    graph = new int*[vertices];
                    for(int g=0; g<= vertices; g++) graph[g] = new int[vertices]; // create graf matrix
                    for(int g=0; g< vertices; ++g){                               // init graf matrix to 0
                        for(int l=g; l< vertices; ++l) graph[g][l] = 0;
                    }
                    aux_line.clear();
                }else{
                    character_index=0;
                    data.clear();
                    while(character_index != line.size()){
                        if(isspace(line.at(character_index)) || (character_index == line.size()-1)){
                            if (character_index == line.size()-1){
                                    aux_line.push_back(line.at(character_index)); // capture last char
                            }
                            aux_line.push_back('\0');                               // force an end of string at the end
                            data.push_back(std::stoi(aux_line));                    
                            
                            aux_line.clear();
                        }else{
                            aux_line.push_back(line.at(character_index));
                        }
                        character_index++;
                    }
                    // std::cout << "from edge # " << data.at(0) << " to edge #" << data.at(1) << " costing " << data.at(2) << std::endl;
                    graph[data.at(0)][data.at(1)] = data.at(2);
                }
                // std::cout << line << std::endl;
            }
            my_file.close();
        }

        ~graphClass(){
            delete graph;
        }

        void add_edge(int x, int y, int weight){
            if(graph[x][y]>0){
                return;    
            }else{
                graph[x][y]=weight; edges++;
            }
        }
        void remove_edge(int x, int y){
            if(!graph[x][y]){
                return;
            }else{
                graph[x][y]=false; edges--;
            }
        }
        void set_weight(int x, int y, int weigth){
            if(!graph[x][y]) edges++;
            graph[x][y] = weigth;
        }
        int get_vertex(int x, int y){return(graph[x][y]);}
        int get_size(){return(vertices);}
        int get_edges(){return(edges);}
        int get_weight(int x, int y){
            assert(x>=0);assert(y>=0);assert(x<vertices);assert(y<vertices);
            return(graph[x][y]);
        }
        bool adjacent(int x, int y){
            assert(x>=0);assert(y>=0);assert(x<vertices);assert(y<vertices);
            return(graph[x][y]>0);
        }

        bool is_valid(int column, int row, int player){
            

        }

        bool set_position(int column, int row, int player){
            if(is_valid(column, row, player)){
                set_weight(column, row, player);
                return(true);
            }else{
                return(false);
            }

        }
        std::vector<int> neighbors(int x){
            std::vector<int> neighbors_list;
            for(int i=0; i<vertices; i++){
                if(graph[x][i]>0){
                    neighbors_list.push_back(i);
                }
            }
            if(neighbors_list.size() != 0){
                std::cout<<x<<"has the following neighbors: ";
                for(auto i = neighbors_list.begin(); i != neighbors_list.end(); ++i){
                    std::cout << *i << " ";
                }
                std::cout << std::endl;
            }else{
                std::cout<<x<<"does not has neighbors: ";
            }
            return(neighbors_list);
        }

        void print_hexagonal(){
            for(int column = 0; column < get_size(); column++){
                for(int row = 0; row < DIMENSION; row++){
                    if(row == 0){
                        int repeat = 0;
                        while(repeat++ != 2*(column)) std::cout << " ";
                    }
                    switch(get_weight(column, row)){
                        case(10):
                            std::cout << "x";
                            break;
                        case(20):
                            std::cout << "o";
                            break;
                        case(0):
                            std::cout << ". ";
                            break;
                        default: break;

                    }
                    if(row != get_size() - 1) std::cout << "- ";
                }
                std::cout << std::endl;
                if(column < get_size()-1){
                    for(int row = 0; row <= (get_size() - 1) * 2; row++){
                        if(row == 0){
                            int repeat = 0;
                            while(repeat++ != 2*column+1) std::cout << " ";
                        }
                        if(row%2 != 0){
                            std::cout << "/ ";
                        }else{
                            std::cout << "\\ ";
                        }
                    }
                }
                std::cout << std::endl;
            }
        }
    private:
        int** graph;
        int vertices;
        int edges;
};

std::ostream& operator<<(std::ostream& out, graphClass& gc){
    out << "   ";
    for(int i=0; i < gc.get_size(); i++) 
        out << std::setfill (' ') << std::setw (2) << i << " ";
    out << std::endl;
    out << "   ";
    for(int i=0; i < gc.get_size(); i++) 
        out << "---";
    
    for(int i=0; i < gc.get_size(); i++){
        out << std::endl;
        out << std::setfill (' ') << std::setw (2) << i << "|";
        for(int j=0; j < gc.get_size(); j++){
            out << std::setfill (' ') << std::setw (2) << gc.get_vertex(i,j) << " ";
        }
    } 
    out << std::endl;

    return(out);
}

const int PLAYER_A = 10;
const int PLAYER_B = 20;

int main(){
    graphClass my_graph(DIMENSION,0);
    int input_row, input_column;
    int player = PLAYER_A;

    std::cout<<"Player A: select row and column: " << std::endl;
    while(true){        
        std::cin>>input_column;
        std::cout << "\033[F";
        std::cin>>input_row;
        std::cout << "\033[F";
        my_graph.set_position(input_column, input_row, player);        
        my_graph.print_hexagonal();
        std::cout << my_graph << std::endl;
        my_graph.neighbors(input_column);
        if(player == PLAYER_A){
            std::cout<<"Player B1: select row and column: ";
            player = PLAYER_B;
        }else{
            std::cout<<"Player A: select row and column: ";
            player = PLAYER_A;
        }
        std::cout << std::endl;
    }


    return 0;
}