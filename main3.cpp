#include  <iostream>
#include <fstream>
using namespace std;

class listNode {
	public:
		int data;
		listNode *next;//the next pointer
		
	listNode(int d){//assigns data to node upon initialization
		data = d;
	}	

	void printNode(listNode *node, ofstream &outFile){
	    if (this->next == NULL)
	        outFile << "(" << this->data <<", NULL)-->NULL";
	    if (this->next != NULL)
	        outFile << "(" << this->data <<" , "
	        << this->next->data << ")-->" ;
	}
};

class LLStack{
	public:
	int length = 0;
	
	listNode *top;//node pointer to the top
	
	LLStack(){
		top = new listNode(-9999);//creates the first value
		top->next = NULL;//points to the next empty value
	}
	
	void push(listNode *dNode) {
		dNode->next = top->next;
		top->next = dNode;
        length++;
    }
    listNode* pop(){
    	listNode* temp = top->next;
    	if(isEmpty()){
    		return NULL;
		}
		else{
			top->next = top->next->next;
			length--;
		}
		return temp;
	}
	bool isEmpty(){
		if (length == 0) return true;
		return false;
	} 
	void printStack(LLStack stack, ofstream &outFile2){
	    listNode *tmp;
	    tmp = stack.top;
	    outFile2<<"Top-->";
	    while (tmp->next != NULL){
		    outFile2 << "("<<tmp->data << "," 
	    	    << tmp->next->data <<")-->";
	        tmp = tmp->next;
	    }
		outFile2 << "("<<tmp->data << "," << "NULL)-->NULL"<<endl;
	}
};

class LLQueue{
	public:
	int length = 0;
	
	listNode *head,*tail;
	
	LLQueue(){
		listNode* dummy = new listNode(-9999);
	    dummy->next = NULL;
		head = dummy;
		tail = dummy;
	}
	void insertQ(listNode *dNode) {
        if (head->next == NULL){
        	head->next = dNode;
        	tail = dNode;
		}
        else{
            tail->next=dNode;
            tail=dNode;
        }
        length++;
    }
	listNode* deleteQ(){
    	listNode* tmp=head->next;
		if(isEmpty()){
			return NULL;
		}
		head->next = head->next->next;
        tmp->next = NULL;
        length--;
		return tmp;
	}
    bool isEmpty(){
		if (length == 0) return true;
		return false; 
	}
	void printQ(LLQueue* hashTable[2][10], int whichTable, int index,ofstream &outFile2){
	    listNode *tmp = hashTable[whichTable][index]->head;
        outFile2<<"Head-->";
        	while (tmp->next != NULL){
		    outFile2 <<"("<<tmp->data<< "," 
			<< tmp->next->data<<")-->";
	        tmp = tmp->next;
	    	}
		outFile2 << "("<<tmp->data << "," << "NULL)-->NULL";
		outFile2<<endl;
	}
};

class RadixSort{
	public:
		int tableSize = 10;
		LLQueue* hashTable[2][10];
		int data;
		int currentTable;
		int previousTable;
		int numDigits;
		int offSet;
		int currentPosition;
		int currentDigit;
		int hashIndex;
		
	RadixSort(){
		for (int i = 0; i<2; i++){
			for (int j=0; j<tableSize; j++){
				LLQueue* myQ = new LLQueue;
				hashTable[i][j] = myQ;
			}				
		}
	}	
	void firstReading(ifstream &inFile, ofstream &outFile2){
		outFile2<<"***Performing firstReading"<<endl;
		int negativeNum = 0;
		int positiveNum = 0;
		while (!inFile.eof()){
			inFile >> data;
			if (data < negativeNum) negativeNum = data;
			if (data > positiveNum) positiveNum = data;
		}
		if (negativeNum < 0) offSet = abs(negativeNum);
		else offSet = 0;
		positiveNum = positiveNum + offSet;
		numDigits = getLength(positiveNum);
		outFile2<<"Largest Value:"<<positiveNum
		<<"  Smallest Value:"<<negativeNum
		<<"  Offset:"<<offSet
		<<"  Maximum Digit Size:"<<numDigits<<endl<<endl;
	}
	LLStack loadStack(ifstream &inFile, ofstream &outFile2){
		outFile2<<"*Performing loadStack"<<endl;
		LLStack S;
		while (inFile >> data){
			data += offSet;
			listNode* newNode = new listNode(data);
			S.push(newNode);
		}
		return S;
	}
	void RSort(LLStack S, ofstream &outFile1, ofstream &outFile2){
		outFile2<<"***Performing RSort"<<endl;
		currentPosition = 0;
		currentTable = 0;
		moveStack(S, currentPosition, currentTable, outFile2);
		printTable(currentTable, outFile2);
		currentPosition++;
		currentTable = 1;
		previousTable = 0;
		int currentQueue = 0;
		while (currentPosition < numDigits){
		while (currentQueue < tableSize){
			while (hashTable[previousTable][currentQueue]->isEmpty() == false){
				listNode* newNode = hashTable[previousTable][currentQueue]->deleteQ();		
				hashIndex = getDigit (newNode->data, currentPosition);
				hashTable[currentTable][hashIndex]->insertQ(newNode);
			}
			currentQueue++;
		}
		printTable(currentTable ,outFile2);
		previousTable = currentTable;
		currentTable = (currentTable+1)%2;
		outFile2<<endl;
		currentQueue = 0;
		currentPosition++;
		}
		printSortedData(previousTable, outFile1);
	}
	void moveStack(LLStack S, int currentPosition, int currentTable, ofstream &outFile2){
		outFile2<<"*Performing moveStack"<<endl;
		while (!S.isEmpty()){
    		listNode* newNode = new listNode(S.pop()->data);
    		hashIndex = getDigit(newNode->data, currentPosition);
    		hashTable[currentTable][hashIndex]->insertQ(newNode);
		}
	}
	int getLength (int data){
		string num = to_string(data);
		return num.length();
	}
	int getDigit (int data, int position){
		string num = to_string(data);
		int length = to_string(data).length();
		int invDig =(length-1)-position;
		char c = num[invDig];
		int digPos = c-'0';	
		if (digPos<0){
			return 0;
		}
		else{
			return digPos;
		}
	}
	
	void printTable (int whichTable, ofstream &outFile2){
		for (int i = 0; i< 10; i++){
			outFile2<<"Table["<<whichTable<<"]["<<i<<"]: ";
			hashTable[whichTable][i]->printQ(hashTable, whichTable, i, outFile2);
		}		
		outFile2<<endl;
	}
	void printSortedData (int whichTable, ofstream &outFile1){
		outFile1<<"Smallest Value\n";
		for (int i=0; i<tableSize; i++){
			while (!hashTable[whichTable][i]->isEmpty()){
				outFile1<<hashTable[whichTable][i]->deleteQ()->data - offSet;
				outFile1<<endl;
			}
		}
		outFile1<<"Largest Value"<<endl;
	}
};

int main (int argc, char *argv[]){	
	ifstream input;
	input.open(argv[1]);
	ofstream output1, output2;
	output1.open(argv[2]);
	output2.open(argv[3]);

	RadixSort myRSort;
	myRSort.firstReading(input, output2);
	input.close();
	input.open(argv[1]);
	LLStack S;
	S = myRSort.loadStack(input, output2);
	S.printStack(S, output2);  

	
	output2<<endl;
	myRSort.RSort(S, output1, output2);
	
	input.close();
	output1.close();
	output2.close();

	cout<<"Done";
	return 0;
}
