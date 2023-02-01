#include <iostream>
#include <iomanip> 
#include <cstdint>
#include <assert.h>
#include <vector>
#include <fstream>

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
        std::vector<int> neighbors(int x){
            std::vector<int> neighbors_list;
            for(int i=0; i<vertices; i++){
                if(graph[x][i]>0){
                    neighbors_list.push_back(i);
                }
            }
            std::cout<<x<<"has the following neighbors: ";
            for(auto i = neighbors_list.begin(); i != neighbors_list.end(); ++i){
                std::cout << *i << " ";
            }
            std::cout << std::endl;
            return(neighbors_list);
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

class primMST{
    public: 
        primMST(graphClass* target){
            graphCopy = target;
            tree_size = graphCopy->get_size();
        }

        bool buildMST(){
            
            for(uint16_t i=0; i < tree_size; i++){
                vertex_key.push_back(INF); 
                proccessed_vertex.push_back(false);
                mst.push_back(INF); 
                // std::cout << std::endl << vertex_key.at(i) << " - " << proccessed_vertex.at(i) << std::endl;
            }
            
            vertex_key.at(0) = 0;
            mst.at(0) = -1;

            for(uint16_t j=0; j < tree_size - 1; j++){

                int min = get_min_index();
                proccessed_vertex.at(min) = true;
                // std::cout << "min" << min << " - " << proccessed_vertex.at(min) << std::endl;

                for(int16_t h=0; h < tree_size; h++){
                    if(graphCopy->get_weight(min, h) && proccessed_vertex.at(h) == false 
                    && graphCopy->get_weight(min, h) < vertex_key.at(h)){
                        mst.at(h) = min;
                        vertex_key.at(h) = graphCopy->get_weight(min, h);
                    }
                }
            }

            return(true);
        }

        bool get_mst(std::vector<int> *source){
            // copy private mst info into the asked direction
            for(int8_t i=0; i<=mst.size()-1; i++){
                source->push_back(mst.at(i));
                // std::cout << " " << source->at(i);
            }
            // std::cout << std::endl;0
            return(true);
        }

        void print(){
            std::cout << "MST for the given graph (by Prim)" << std::endl;
            std::cout << "Edge          Weight" << std::endl;
            std::cout << "from   to" << std::endl;
            for (int i = 1; i < tree_size; i++){
                std::cout << std::setw (2) << mst.at(i)<< "   - " << std::setw (2) << i << " -> " 
                << std::setw (2) << graphCopy->get_weight(i, mst.at(i)) << " \n";
            }
        }

    private:
        int tree_size;
        graphClass *graphCopy;
        std::vector<int> mst;
        std::vector<int> vertex_key;
        std::vector<bool> proccessed_vertex;

        uint16_t get_min_index(){
            uint16_t min_value = INF;
            uint16_t min_index;

            for(uint16_t i = 0; i < tree_size; i++){
                if( proccessed_vertex.at(i) == false && vertex_key.at(i) < min_value){
                    min_value = vertex_key.at(i);
                    min_index = i;
                }
            }
            return(min_index);
        }
};

int main(){
    graphClass incoming_graph("SampleData.txt");  // load data from file
    primMST my_mst(&incoming_graph); // create an insance of primMST class from the loaded data
    
    std::cout << "Input " << incoming_graph.get_size() << " vertex graph" << std::endl << incoming_graph << std::endl;

    my_mst.buildMST();  // process data
    my_mst.print();
    
    std::vector<int> mst_copy_for_use; // for data usage copy from class to external vector
    my_mst.get_mst(&mst_copy_for_use);

    return 0; 
}