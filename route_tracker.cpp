#include<iostream>
#include<string>
#include <fstream>
#include<sstream>
#include<vector>
#include<climits>
#include<algorithm>
#define MAX_STATIONS 230
using namespace std;

class MetroStation{
    public:
        string name;
        int stationID;
};

class MetroGraph{
    public:
        MetroStation stations[MAX_STATIONS];
        float adjMatrixDistance[MAX_STATIONS][MAX_STATIONS];
        int numStations;
};


void initializeMetroGraph(MetroGraph *graph){
    graph->numStations = 0;
    for (int i = 0; i < MAX_STATIONS; i++) {
        for (int j = 0; j < MAX_STATIONS; j++) {
            graph->adjMatrixDistance[i][j] = 0; 
        }
    }
}

void addNodes(MetroGraph *graph) { 

    ifstream stationfile("stationnames1.txt");
    if (!stationfile.is_open()) {
        cerr << "Failed to open file for reading.\n";
        return;
    }
    string line;
    for(int i = 0 ; i< MAX_STATIONS; i++){
            getline(stationfile,line);
            // cout<<line<<endl;
            graph->stations[i].name=line; 

    }
    for(int i = 0 ; i< MAX_STATIONS; i++){
        graph->stations[i].stationID = i; 

    }
    stationfile.close();
    // for(int i = 0 ; i< MAX_STATIONS; i++){
    //     cout<<graph->stations[i].stationID<<" = "<<graph->stations[i].name<<endl; 

    // }

}

void addMetroConnections(MetroGraph *graph,string from,string to,float distance){   

    int fromId = -1;
    int toId = -1;
    for(int i = 0 ; i<  MAX_STATIONS; i++)
    {
        if(graph->stations[i].name==from)
        {
            fromId = i;
            // printf("%d",fromId);
        }
        
    }
    for(int i = 0 ; i< MAX_STATIONS; i++)
    {
        if(graph->stations[i].name==to)
        {
            toId = i;
            // printf("%d",toId);
        }
        
    }
    // cout<<fromId<<" "<<toId<<endl;
    graph->adjMatrixDistance[fromId][toId] = distance;
    graph->adjMatrixDistance[toId][fromId] = distance;
    // cout<<from<<"  "<<to<<"  "<<distance<<endl;
    //cout<<graph->adjMatrixDistance[fromId][toId]<<" "<<graph->adjMatrixDistance[toId][fromId]<<endl;
    //printf("Connection created between %s and %s and distance %d.\n", from, to, distance);

}


void establishConnections(MetroGraph *graph){
    ifstream database;
    string from,to;
    float dist;

    database.open("connection.txt");
    if (database.is_open()) {
        cout << "File has been opened" << endl;
    }
    else {
        cout << "NO FILE HAS BEEN OPENED" << endl;
    }
    vector<string> row;
    string line,word,temp;
    while(getline(database,line)){
        row.clear();
        stringstream ss(line);
        while(getline(ss,word,',')){
            row.push_back(word);
            // cout<<word<<endl;
        }
        dist=stof(row[2]);
        // cout<<row[0]<<"  "<<row[1]<<"  "<<dist<<endl;
        addMetroConnections(graph,row[0],row[1],dist);
    }
    database.close();

}
void printmatrix(MetroGraph * graph){
    for(int i=0;i<MAX_STATIONS;i++){
        for(int j=0;j<MAX_STATIONS;j++){
            cout<<graph->adjMatrixDistance[i][j]<<" ";
        }
        cout<<endl;
    }
}
void printPath(MetroGraph * graph,int parent[], int j,int & st) {
    if (parent[j] == -1){ 
        return;
    }
    st++;
    printPath(graph,parent, parent[j],st);
    cout<<graph->stations[j].name<<" -> ";
}
void printSolutionDist(MetroGraph * graph,float dist[], int parent[], int src, int dest) {
    cout<<"\nShortest Path from Station "<<graph->stations[src].name<<" to "<<graph->stations[dest].name<<" :\n\n";
    cout<<graph->stations[src].name<<" -> ";
    int st=1;
    printPath(graph,parent, dest,st);
    cout<<"end of jouney\n";
    cout<<"\nTotal Distance: "<<dist[dest]<<'\n';
    int time=(30*st)/60+((dist[dest]/45)*60);
    int d=dist[dest];
    cout<<"Time for journey : "<<time<<" minutes\n";
    cout<<"Fare : ";
    if(0<=d && d<=2) cout<<"Rs 10/-";
    else if(2<d && d<=5) cout<<"Rs 20/-";
    else if(5<d && d<=12) cout<<"Rs 30/-";
    else if(12<d && d<=21) cout<<"Rs 40/-";
    else if(21<d && d<=32) cout<<"Rs 50/-";
    else cout<<"Rs 60/-";
}

int minDistance(float dist[], bool sptSet[]) {
    float min = INT_MAX;
    int min_index;
    for (int v = 0; v < MAX_STATIONS; v++) {
        if (!sptSet[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

void dijkstraDistance(MetroGraph *graph, int src, int dest){
    float dist[MAX_STATIONS];
    bool sptSet[MAX_STATIONS];
    int parent[MAX_STATIONS];

    for (int i = 0; i < MAX_STATIONS; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = false;
        parent[i] = -1;
    }

    dist[src] = 0;

    for (int count = 0; count < MAX_STATIONS - 1; count++) {
        int u = minDistance(dist, sptSet);
        // cout<<u<<endl;
        sptSet[u] = true;

        for (int v = 0; v < MAX_STATIONS; v++) {
            if (!sptSet[v] && graph->adjMatrixDistance[u][v] &&  dist[u] != INT_MAX && dist[u] + graph->adjMatrixDistance[u][v] < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + graph->adjMatrixDistance[u][v];
            }
        }
    }
    // for(int i=0;i<MAX_STATIONS;i++){
    //     cout<<i<<" = "<<parent[i]<<endl;
    // }
    printSolutionDist(graph,dist, parent, src, dest);

}
void displaystations(MetroGraph * graph){
    for(int i = 0 ; i < MAX_STATIONS ; i++){
        cout<<"Station name : "<<graph->stations[i].name<<" -> Station Id : "<<graph->stations[i].stationID<<'\n';
    }
}
int main(){
    MetroGraph graph;
    initializeMetroGraph(&graph);
    addNodes(&graph);
    establishConnections(&graph);
    //printmatrix(&graph);
    cout<<"*********************    METRO APP        *****************************************\n\n";
    cout<<"These are the stations available along with thier ids\n\n";
    displaystations(&graph);
    int from,to;
    cout<<"Enter your source station ID : ";cin>>from;
    cout<<"Enter your destination station ID : ";cin>>to;
    dijkstraDistance(&graph,from,to);

    return 0;
}