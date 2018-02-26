/*
Owner: Hüseyin Tosun
Analysis of Algorithms Project
Merge Sort, Insertion Sort, Linear Seach algorithms are used.

Program finds nearest K warehouse locations among N warehouse locations.
Expected input file: warehouselocations.txt
Output file: output.txt
Expected command line format: ./program N K algorithmType(IS, MS or LS) x-axis y-axis
Sample: ./program 100000 10 MS 1234 5678

N : Total number of locations to be sorted
K : Number of closest locations to be found
algorithmType : Method to be used to solve the problem (Insertion Sort(IS), Merge Sort(MS) or Linear Search(LS))
x-axis: X-axis information of the point that closest K location will be found
y-axis: Y-axis information of the point that closest K location will be found
*/

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <cmath>
#include <limits>
#include <sstream>
#include <fstream>
#include <time.h>
using namespace std;
#define memoryLimit 32

//struct for informations of locations
struct locationsData{
	int id;
	int x;
	int y;
	int euclid;
};
//converts location strings to location integers
int convertToInt(char *);
//takes a line and returns a struct with full of these information
locationsData takeLocation(char *);
//swap function for the struct
void swap(locationsData &, locationsData &);
//split function of merge sort
void split(locationsData[], locationsData[], int, int);
//merge function of merge sort
void merge(locationsData[], locationsData[], int, int, int);

int main(int argc, char *argv[]){
	if (argc < 6){
		cout << "Not enough arguments" << endl;
		return 0;
	}
	clock_t t;
	t = clock();
	//max integer will be used for linear sort
	int max = std::numeric_limits<int>::max();
	//reading values from command line
	int N = convertToInt(argv[1]);
	int K = convertToInt(argv[2]);
	string type = argv[3];
	int x = convertToInt(argv[4]);
	int y = convertToInt(argv[5]);
	int euclid = sqrt((x*x) + (y*y));

	locationsData array[N];
	locationsData results[K];
	//reading data from text file
	FILE *fp;
	fp = fopen("warehouselocations.txt", "r+");
	if (fp != NULL)
	{
		char buffer[memoryLimit];
		int lineCounter = 0;
		while (lineCounter < N) {
			if (fgets(buffer, memoryLimit, fp) == NULL) break;
			char *rawLine = buffer;
			array[lineCounter] = takeLocation(buffer);
			lineCounter++;
		}
		fclose(fp);
	}
	//Merge Sort is handled here
	if (type == "MS"){
		locationsData temp[N];
		for (int i = 0; i < N; i++){
			temp[i] = array[i];
		}
		split(array, temp, 0, N - 1);
		for (int i = 0; i < K; i++){
			results[i] = temp[i];
		}
	}
	//Insertion Sort is handled here
	else if (type == "IS"){
		int j;
		for (int i = 1; i < N; i++){
			j = i;
			while (j > 0 && array[j - 1].euclid > array[j].euclid){
				swap(array[j], array[j - 1]);
				j--;
			}
		}
		for (int i = 0; i < K; i++){
			results[i] = array[i];
		}
	}
	//Linear Sort is handled here
	else if (type == "LS"){
		for (int i = 0; i < K; i++){
			results[i] = array[i];
		}
		for (int i = 0; i < K; i++){
			results[i].euclid = max;
		}
		for (int k = 0; k < N; k++){
			if (abs(array[k].euclid - euclid) < abs(results[0].euclid - euclid)){
				results[0].id = array[k].id;
				results[0].x = array[k].x;
				results[0].y = array[k].y;
				results[0].euclid = array[k].euclid;
				for (int j = 0; j < K - 1; j++){
					if (abs(results[j].euclid - euclid) < abs(results[j + 1].euclid - euclid)){
						swap(results[j], results[j + 1]);
					}
				}
			}
		}
	}
	else{
		cout << "Invalid Sort Type" << endl;
		return 0;
	}
	//Time calculation
	t = clock() - t;
	cout << "It took " << t / CLOCKS_PER_SEC << " seconds" << endl;
	//All output will be collected at one string file
	ostringstream adder;
	string allOutput = "";
	for (int i = 0; i < K; i++){
		adder << results[i].id << "\t" << results[i].x << "\t" << results[i].y << "\t" << results[i].euclid << "\n";
	}
	allOutput += adder.str();
	//The string will be written to output file
	ofstream out("output.txt");
	out << allOutput;
	out.close();

	return 0;
}

int convertToInt(char *data){
	int length = strlen(data);
	int result = 0;
	int i = 0;
	while (data[i]){
		result = result * 10 + (data[i] - '0');
		i++;
	}
	return result;
}

locationsData takeLocation(char *data){
	locationsData result;
	int i = 0, j = 0;
	int temp = 0;
	while (1){
		if (data[i] == '\t'){
			if (j == 0){ result.id = temp; }
			if (j == 1){ result.x = temp; }
			temp = 0;
			j++;
		}
		else{
			temp = temp * 10 + (data[i] - '0');
		}
		i++;
		if (data[i] == '\n'){
			i--;
			result.y = (temp - (data[i] - '0')) / 10;
			break;
		}
	}
	result.euclid = sqrt((result.x*result.x) + (result.y*result.y));
	return result;
}

void swap(locationsData &a, locationsData &b){
	locationsData temp;
	//temp = a;
	temp.id = a.id;
	temp.x = a.x;
	temp.y = a.y;
	temp.euclid = a.euclid;
	//a = b;
	a.id = b.id;
	a.x = b.x;
	a.y = b.y;
	a.euclid = b.euclid;
	//b = temp;
	b.id = temp.id;
	b.x = temp.x;
	b.y = temp.y;
	b.euclid = temp.euclid;
}

void split(locationsData B[], locationsData A[], int start, int end){
	int mid;
	if ((end - start) < 2){
		return;
	}
	mid = (end + start) / 2;
	split(A, B, start, mid);
	split(A, B, mid, end);
	merge(B, A, start, mid, end);
}

void merge(locationsData A[], locationsData B[], int start, int mid, int end){
	int a = start;
	int b = mid;
	for (int i = start; i < end; i++){
		if (a < mid && (b >= end || A[a].euclid <= A[b].euclid)){
			B[i].id = A[a].id;
			B[i].x = A[a].x;
			B[i].y = A[a].y;
			B[i].euclid = A[a].euclid;
			a++;
		}
		else{
			B[i].id = A[b].id;
			B[i].x = A[b].x;
			B[i].y = A[b].y;
			B[i].euclid = A[b].euclid;
			b++;
		}
	}
}
