#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

class Student {
	public:
		// Student(int id,  string name, int rating, int mark) {
		// 	this->id = id;
		// 	this->name = name;
		// 	this->rating = rating;
		// 	this->mark = mark;
		// }

		int getId() { return this->id; }
		string getName() { return this->name; }
		int getRating() { return this->rating; }
		int getMark() { return this->mark; }
		
		bool setFromString(string fieldsStr) {
			istringstream streamStr(fieldsStr);
			string token;
			int orderIndex = 0;
			while(getline(streamStr, token, ';')) {
				switch (orderIndex) {
					case 0:
					 this->id = stoi(token);
					 if (this->id == 0) {
					 	return false;
					 }
					 break;
					case 1:
					 this->name = token;
					  break;
					case 2:
					 this->rating = stoi(token);
					 if (this->rating == 0) {
					 	return false;
					 }
					 break;
					case 3:
					 this->mark = stoi(token);
					 if (this->mark == 0) {
					 	return false;
					 }
					 break;
				}
				
				orderIndex++;

			}
			return true;

		}

	private:
		int id;
		string name;
		int rating;
		int mark;
};

class ExamQueue {
	private:
		vector<Student*> students;
		void heapify(int i) {
			int leftChild;
			int rightChild;
			int largest;

			for ( ; ; ) {
				leftChild = i + 1;
				rightChild = i + 2;
				largest = i;

				if (leftChild < this->length() && this->needSwap(this->students[leftChild], this->students[largest])) {
					largest = leftChild;
				}

				if (rightChild < this->length() && this->needSwap(this->students[rightChild], this->students[largest])) {
					largest = rightChild;
				}				

				if (largest == i) { break; }

				Student* val = this->students[largest];
				this->students[largest] = this->students[i];
				this->students[i] = val;

				i = largest;
			}
		}
		bool needSwap(Student* st1, Student* st2) {
			return st1->getRating() > st2->getRating();
		}
	public:
		int length() { return this->students.size(); }
		bool insertStudent(Student* st) {
			this->students.push_back(st);
			int index = this->length() - 1;
			int parentIndex = (index - 1) / 2;

			while (index > 0 && this->needSwap(this->students[index], this->students[parentIndex])) {
				Student* temp = this->students[index];
				this->students[index] = this->students[parentIndex];
				this->students[parentIndex] = temp;

				index = parentIndex;
				parentIndex = (index - 1) / 2;
			}
			return true;
		}
		Student* getStudentWithMaxRating() {
			int size = this->length();
			if (size == 0) { return nullptr; }
			if (size == 1) { 
				Student* st = this->students[0];
				this->students.erase(this->students.begin());
				return st;
			}

			Student* maxVal = this->students[0];
			Student* minVal = this->students[size-1];

			this->students[0] = minVal;
			this->students.erase(this->students.cend());
			this->heapify(0);

			return maxVal;
		}
};

class FileStudent {
	public:
		FileStudent(string filenameFrom, string filenameRes) {
			this->filenameFrom = filenameFrom;
			this->filenameRes = filenameRes;
		}

		bool sortStudents() {
			ifstream in(this->filenameFrom);
			ExamQueue* q = new ExamQueue();
			string studentString;

			if (in.is_open()) {
				while(getline(in, studentString)) {
					Student* st = new Student();
					st->setFromString(studentString); 
					q->insertStudent(st);
				}
			} else { return false; }


			ofstream out(this->filenameRes);

			if (out.is_open()) {
				int size = q->length();
				for (int i = 0; i < size; i++) {
					Student* st = q->getStudentWithMaxRating();
					out << st->getName() << " " << st->getRating() << endl;
				}
			} else { return false; }

			return true;
		}

	private:
		string filenameFrom;
		string filenameRes;
};

int main() {
	ExamQueue* q = new ExamQueue();

	FileStudent* f = new FileStudent("from.txt", "res.txt");
	f->sortStudents();
}