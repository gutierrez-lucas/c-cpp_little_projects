#include <iostream>
#include <iomanip> 
#include <cstdint>
#include <assert.h>
#include <vector>

const int INF = 0xFFFFFF;

class graphClass{
    public:
        graphClass():vertices(0), edges(0){}
        graphClass(int v, int density){
            assert(v>=0);
            srand(time(0));
            vertices = v;
            edges=0;
            graph = new int*[ vertices];
            for(int i=0; i<= vertices; i++) graph[i] = new int[ vertices];
            for(int i=0; i< vertices; ++i){
                for(int j=i; j< vertices; ++j){
                    if(i==j) graph[i][j] = 0;
                    else
                        if(rand()%100 < density && density != 0){
                            graph[i][j] = graph[j][i] = rand()%10;
                            edges++;
                        }else{
                            graph[i][j] = graph[j][i] = false;
                        }
                }
            }
        }
        void add_edge(int x, int y, int weight){
            if(graph[x][y]>0) return;
            else graph[x][y]=weight; edges++;
        }
        void remove_edge(int x, int y){
            if(!graph[x][y]) return;
            else graph[x][y]=false; edges--;
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
            // std::cout<<x<<"has the following neighbors: ";
            // for(auto i = neighbors_list.begin(); i != neighbors_list.end(); ++i){
            //     std::cout << *i << " ";
            // }
            // std::cout << std::endl;
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

typedef struct spt{
    std::vector<bool>   vertex;
    std::vector<int>    distances;
    std::vector<int>    previous_vertex;
}spt;

void print_spt(spt s){
    for(int i = 0; i < s.distances.size(); i++){
        if(s.distances[i] == INF) std::cout << "[INF,"; 
        else std::cout << "[" << s.distances[i] << ","; 
        std::cout << std::boolalpha << s.vertex[i] << "," << s.previous_vertex[i] << "]";
    }
    std::cout<<std::endl;
}

int find_min(spt s){
    int min, target = 0;
    min = INF;
    for(int i=0; i<s.distances.size();i++){
        if(s.vertex.at(i) == false){          // if this vertex has not been evaluated
            if(s.distances.at(i) < min){
                min = s.distances.at(i); target = i;
            }
        }
    }

    return(target);
}

void print_shortest_path(spt s, int from, int to){
    int next = s.previous_vertex.at(to);
    
    std::cout << to << "-->";
    do{
        std::cout << next << "-->";
        next = s.previous_vertex.at(next);
    }while(next != from);
    std::cout << from << std::endl;
}

void shortest_path(graphClass& g, int from, int to){
    spt my_spt;
    int size = g.get_size();

    std::cout << std::endl << "spt" << std::endl;
    for(int i=0; i < size; i++){
    // init every element as un-evaluated and with infinite distance
        my_spt.vertex.push_back(false);
        my_spt.distances.push_back(INF);
        my_spt.previous_vertex.push_back(0);
    }
    my_spt.distances.at(from) = 0; // distance to starting point is zero

    print_spt(my_spt);
    
    for(int i=0; i < size; i++){
        int current_v = find_min(my_spt);
        my_spt.vertex.at(current_v) = true;

        for(int j=0; j < size; j++){
            if(my_spt.vertex.at(j) == false && my_spt.distances.at(current_v) != INF){
                if(g.adjacent(current_v, j) != false){
                    if(my_spt.distances.at(current_v) + g.get_weight(current_v, j) < my_spt.distances.at(j)){
                        my_spt.distances.at(j) = my_spt.distances.at(current_v) + g.get_weight(current_v, j);
                        my_spt.previous_vertex.at(j) = current_v;
                    }
                }
            }
        }
        print_spt(my_spt);
    }
    print_shortest_path(my_spt,from,to);
}

int main(){
    graphClass my_graph(7, 0);
    my_graph.add_edge(0,1,1);
    my_graph.add_edge(1,0,1);my_graph.add_edge(1,2,1);my_graph.add_edge(1,4,1);
    my_graph.add_edge(2,1,1);my_graph.add_edge(2,3,1);
    my_graph.add_edge(3,2,1);my_graph.add_edge(3,4,1);
    my_graph.add_edge(4,1,1);my_graph.add_edge(4,3,1);my_graph.add_edge(4,5,1);
    my_graph.add_edge(5,4,1);my_graph.add_edge(5,6,1);
    my_graph.add_edge(6,5,1);

    // my_graph.add_edge(0,1);
    std::cout << my_graph << std::endl;
    std::cout << "this graph has " << my_graph.get_size() << " vertices" << std::endl;
    std::cout << "this graph has " << my_graph.get_edges() << " edges" << std::endl;
    std::cout << "vertices 0 and 1 is " << my_graph.adjacent(0,2) << std::endl;

    my_graph.neighbors(0);

    shortest_path(my_graph, 0, 6);
    return 0; 
}