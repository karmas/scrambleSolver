#include <iostream>
#include <fstream>
#include <cstdlib>
#include <set>
using namespace std;

const int dataRows = 4;

void echo(const string &msg)
{
  cout << msg << endl;
}

void echo(char msg)
{
  cout << msg << endl;
}

void error(const string &msg)
{
  echo(msg);
  exit(1);
}

void checkFile(int argc, char **argv)
{
  if (argc < 2) error("Give dictionary file as an argument");
  if (argc < 3) error("Give data file as an argument");
}

void readDictionary(char *fileName, set<string> &lexicon)
{
  ifstream inf(fileName);
  if (!inf) error("Error reading: " + string(fileName));

  echo("Creating lexicon from: " + string(fileName));
  string line;
  while (inf) {
    getline(inf, line);
    if (inf.eof() || inf.fail()) break;
    lexicon.insert(line);
  }

  inf.close();
}

char **readFile(char *fileName)
{
  ifstream inf(fileName);
  if (!inf) error("Error reading: " + string(fileName));

  char **dataMatrix = new char*[dataRows];
  for (int i = 0; i < dataRows; i++)
    dataMatrix[i] = new char;

  echo("Reading file: " + string(fileName));
  string line;
  int row = 0;
  int col = 0;
  while (inf) {
    getline(inf, line);
    if (inf.eof() || inf.fail()) break;

    col = 0;
    for (size_t i = 0; i < line.size(); i++)
      if (isalpha(line[i]))
	dataMatrix[row][col++] = line[i];

    row++;
  }

  inf.close();

  return dataMatrix;
}

void printDataMatrix(char **dataMatrix)
{
  for (int r = 0; r < dataRows; r++) {
    for (int c = 0; c < dataRows; c++)
      cout << dataMatrix[r][c] << " ";
    cout << endl;
  }
}

bool inDataMatrix(int row, int col)
{
  return (0 <= row && row < dataRows) &&
    	 (0 <= col && col < dataRows);
}

void emptyMatrix(char visitedMatrix[dataRows][dataRows])
{
  for (int r = 0; r < dataRows; r++)
    for (int c = 0; c < dataRows; c++)
      visitedMatrix[r][c] = 0;
}

void getWords(char **dataMatrix, set<string> &lexicon,
    	      char visitedMatrix[dataRows][dataRows],
   	      int row, int col, string word) 
{
  if (!inDataMatrix(row, col)) {
    return;
  }
  if (visitedMatrix[row][col] == 1) {
    // unmark only if it is not in path
    if (word.find(dataMatrix[row][col]) >= 0) return;
    visitedMatrix[row][col] = 0;
    cout << word << " unmarked " << dataMatrix[row][col] << endl;
    return;
  }

  // mark position as visited
  visitedMatrix[row][col] = 1;

  // add current character to word
  word += dataMatrix[row][col];
  if (lexicon.count(word)) echo(word);

  getWords(dataMatrix, lexicon, visitedMatrix, row, col + 1, word);
  getWords(dataMatrix, lexicon, visitedMatrix, row + 1, col, word);
  getWords(dataMatrix, lexicon, visitedMatrix, row, col - 1, word);
  getWords(dataMatrix, lexicon, visitedMatrix, row - 1, col, word);
  getWords(dataMatrix, lexicon, visitedMatrix, row - 1, col + 1, word);
  getWords(dataMatrix, lexicon, visitedMatrix, row + 1, col + 1, word);
  getWords(dataMatrix, lexicon, visitedMatrix, row - 1, col - 1, word);
  getWords(dataMatrix, lexicon, visitedMatrix, row + 1, col - 1, word);
  visitedMatrix[row][col] = 0;
}

int main(int argc, char *argv[])
{
  checkFile(argc, argv);

  set<string> lexicon;
  readDictionary(argv[1], lexicon);

  char **dataMatrix = readFile(argv[2]);
  printDataMatrix(dataMatrix);
  string word;

  char visitedMatrix[dataRows][dataRows];
  emptyMatrix(visitedMatrix);
  for (int r = 0; r < dataRows; r++)
    for (int c = 0; c < dataRows; c++)
      getWords(dataMatrix, lexicon, visitedMatrix, r, c, word);

  return 0;
}
