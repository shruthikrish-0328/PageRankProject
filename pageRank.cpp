#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <iomanip>
using namespace std;

struct linkInfo {
    int index;
    int numOutLink;
    double outDegree;
    double tempRank;
};

void pageRank(int p);
map<string, linkInfo> linkMap;
map<string, linkInfo>::iterator mapIterator;

struct Node {
    string linkName;
    int index;
    double outDegree;
    Node* next;
};

class AdjacencyList {
private:
    string linkName;
    double rank;
    Node* startNode;
    int index;
public:
    void PageRank(int n);
    void setPageName(string);
    string getPageName();
    void setNode(Node*);
    Node* getNode();
    void setRank(double);
    void setIndex(int);
    double getRank();
    int getIndex();
};

void AdjacencyList::PageRank(int n){

}
void AdjacencyList::setPageName(string name){
    linkName = name;
}
string AdjacencyList::getPageName(){
    return linkName;
}
void AdjacencyList::setNode(Node* head){
    startNode = head;
}
Node* AdjacencyList::getNode(){
    return startNode;
}
void AdjacencyList::setRank(double inputRank) {
    rank = inputRank;
}
double AdjacencyList::getRank() {
    return rank;
}
int AdjacencyList::getIndex() {
    return index;
}
void AdjacencyList::setIndex(int inputIndex) {
    index = inputIndex;
}

vector <AdjacencyList*> objVec;
void pageRank(int p){

    std::cout << std::setprecision(2) << std::fixed;
    if (p == 1) { //if the power iteration is 1 I return r(0) which is the initial rank
        for (mapIterator = linkMap.begin(); mapIterator != linkMap.end(); mapIterator++) { //O(n)
            int index = mapIterator->second.index;
            cout << mapIterator->first << " " << objVec.at(index)->getRank() << endl;
        }

    }
    else { //if the power iteration is greater than 1 then I need to start calculating and updating ranks
        //O(p)
        for (int i = 1; i < p; i++) { //if p = 2 I need to do the multiplication once so i start the loop at i = 1

            //O(n)
            for (mapIterator = linkMap.begin(); mapIterator != linkMap.end(); mapIterator++) { //iterates through map
                double sum = 0; //sum variable to add the matrix multiplications for a row

                int elementIndex = mapIterator->second.index; //the index that link is at in the object vector
                Node* temp = objVec.at(elementIndex)->getNode(); //link's first node in linked list of inlinks

                //O(n)
                while (temp != NULL) { //goes through linked list
                    double rank = objVec.at(temp->index)->getRank(); //rank of first node in list
                    double outdeg = temp->outDegree; //outdegree of first node in list
                    double numToAdd = (rank * outdeg); //the new rank is the sum of all inlinks (rank * outdegree)

                    sum = sum + numToAdd; //updating sum
                    temp = temp->next; //going to next node in linked list
                }

                mapIterator->second.tempRank = sum; //storing the rank in the map
            }

            //O(n)
            for (mapIterator = linkMap.begin(); mapIterator != linkMap.end(); mapIterator++) {
                int index = mapIterator->second.index;
                objVec.at(index)->setRank(mapIterator->second.tempRank); //sets rank in vector
            }

        }

        //O(n)
        std::cout << std::setprecision(2) << std::fixed;
        for (mapIterator = linkMap.begin(); mapIterator != linkMap.end(); mapIterator++) {//prints link name & rank
            int index = mapIterator->second.index;
            cout << mapIterator->first << " " << objVec.at(index)->getRank() << endl;
        }
    }
}


int main(){

    int numLinks = 0;
    int powerIter = 0;
    string line;
    vector<string> values;
    vector<vector<string>> input;
    linkInfo linkNode;
    string link, outlink;

    cin >> numLinks >> powerIter;
    getline(cin,line);
    for(int i = 0; i < numLinks; i++){
        getline(cin,line);
        int delim = 0;
        int delim2 = 0;
        string link1;
        string link2;
        delim = line.find(' ');

        if(delim != -1 ){
            string newString = line.substr(delim+1, 100);
            delim2 = newString.find(' ');
            link1 = line.substr(0, delim);
            link2 = newString.substr(0,delim2);
        }

        values.push_back(link1);
        values.push_back(link2);
        input.push_back(values);
        values.clear();
    }
    sort(input.begin(), input.end()); //sorts vector in alphebetical order

    //O(n)
    for (int i = 0; i < input.size(); i++) { //creating a map where each value is a linkInfo object
        if (linkMap.find(input[i][0]) == linkMap.end()) {
            linkNode.index = -1; //sets all values to object to access later
            linkNode.tempRank = -1; //index and tempRank need to be updated later
            linkNode.numOutLink = 1;
            linkNode.outDegree = (1.0) / (linkNode.numOutLink);
            linkMap[input[i][0]] = linkNode;
        }
        else {//if the link already exists in the map we just update the outdegree since we know theres another outlink
            linkNode = (*linkMap.find(input[i][0])).second;
            linkNode.numOutLink++;
            linkNode.outDegree = (1.0) / (linkNode.numOutLink);
            linkMap[input[i][0]] = linkNode;
        }
    }

    //O(n)
    for (int i = 0; i < input.size(); i++) { //creating a map where each value is a linkInfo object

        if (linkMap.find(input[i][1]) == linkMap.end()) {
            linkNode.index = -1; //sets all values to object to access later
            linkNode.tempRank = -1; //index and tempRank need to be updated later
            linkNode.numOutLink = 0;
            linkNode.outDegree = 0;
            linkMap[input[i][1]] = linkNode;
        } //if the link already exists in the map we just update the outdegree since we know theres another outlink
    }

    int countIndex = 0;
    for (int i = 0; i < input.size(); i++) { //O(n)
        string vecLink = input[i][1];
        string vecInLink = input[i][0];

        //If the entry exists in map the index will be -1 else zero.
        if (linkMap.find(vecLink) != linkMap.end()) { // added this condition so we only perform if the entry exists in the map and it should
            if (linkMap[vecLink].index == -1) {
                AdjacencyList* link = new AdjacencyList; //create a new object
                Node* node = new Node; //creating newNode

                link->setPageName(input[i][1]); //set the name

                link->setIndex(countIndex);
                linkMap[vecLink].index = countIndex; //change the index to the index in the object vector
                objVec.push_back(link); //push the object to the vector

                countIndex++;

                node->linkName = vecInLink; //sets the name of the node to the inLink
                node->next = NULL;
                link->setNode(node);
            }
            else { //creates linked list with each inLink
                int index = linkMap[vecLink].index;
                Node* temp = objVec.at(index)->getNode();
                while (temp->next != NULL) { //O(n)
                    temp = temp->next;
                }
                Node* node = new Node;
                node->linkName = vecInLink;
                node->next = NULL;
                temp->next = node;
            }
        }
    }

    //O(n)
    for (int i = 0; i < input.size(); i++) {
        string vecLink = input[i][0];
        string vecInLink = "";
        //If the entry exists in map and object has not yet been created the index will be -1 else zero.
        // for this object, we are going to create its pagename and rank.
        // since no one called these sites, the nodes will NULL
        if (linkMap.find(vecLink) != linkMap.end()) { // making sure the entry exists in map and it should.
            if (linkMap[vecLink].index == -1) {
                AdjacencyList* link = new AdjacencyList; //create a new object
                //Node* node = new Node; //creating newNode

                link->setPageName(input[i][0]); //set the name

                link->setIndex(countIndex);
                linkMap[vecLink].index = countIndex; //change the index to the index in the object vector
                objVec.push_back(link); //push the object to the vector

                countIndex++;

                link->setNode(NULL);
            }
        }
    }

    for(int i = 0; i < objVec.size(); i++){ //O(n)

        Node* temp = objVec.at(i)->getNode();
        while(temp != NULL){ //O(n)
            temp->index = linkMap[temp->linkName].index;
            temp->outDegree = linkMap[temp->linkName].outDegree;
            temp = temp->next;
        }
    }

    for(mapIterator= linkMap.begin(); mapIterator != linkMap.end(); mapIterator++){ //O(n)
        int index = mapIterator->second.index;
        double mapSize = linkMap.size();
        objVec.at(index)->setRank((1.0)/(mapSize));
    }

    pageRank(powerIter);
}