#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <fstream> // Include the file stream library

#define MAX_TREE_HT 256

using namespace std;

// to map each character its huffman value
map<char, string> codes;

// To store the frequency of character of the input data
map<char, int> freq;

// A Huffman tree node
struct MinHeapNode
{
    char data;                 // One of the input characters
    int freq;                  // Frequency of the character
    MinHeapNode *left, *right; // Left and right child

    MinHeapNode(char data, int freq)
    {
        left = right = NULL;
        this->data = data;
        this->freq = freq;
    }
};

// utility function for the priority queue
struct compare
{
    bool operator()(MinHeapNode *l, MinHeapNode *r)
    {
        return (l->freq > r->freq);
    }
};

// utility function to print characters along with
// their huffman value
void printCodes(struct MinHeapNode *root, string str)
{
    if (!root)
        return;
    if (root->data != '$')
        cout << root->data << ": " << str << "\n";
    printCodes(root->left, str + "0");
    printCodes(root->right, str + "1");
}

// utility function to store characters along with
// their huffman value in a hash table, here we
// have C++ STL map
void storeCodes(struct MinHeapNode *root, string str)
{
    if (root == NULL)
        return;
    if (root->data != '$')
        codes[root->data] = str;
    storeCodes(root->left, str + "0");
    storeCodes(root->right, str + "1");
}

// STL priority queue to store heap tree, with respect
// to their heap root node value
priority_queue<MinHeapNode *, vector<MinHeapNode *>, compare>
    minHeap;

// function to build the Huffman tree and store it
// in minHeap
void HuffmanCodes(int size)
{
    struct MinHeapNode *left, *right, *top;
    for (map<char, int>::iterator v = freq.begin();
         v != freq.end(); v++)
        minHeap.push(new MinHeapNode(v->first, v->second));
    while (minHeap.size() != 1)
    {
        left = minHeap.top();
        minHeap.pop();
        right = minHeap.top();
        minHeap.pop();
        top = new MinHeapNode('$',
                              left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap.push(top);
    }
    storeCodes(minHeap.top(), "");
}

// utility function to store map each character with its
// frequency in input string
void calcFreq(string str, int n)
{
    for (int i = 0; i < str.size(); i++)
        freq[str[i]]++;
}

// function iterates through the encoded string s
// if s[i]=='1' then move to node->right
// if s[i]=='0' then move to node->left
// if leaf node append the node->data to our output string
string decode_file(struct MinHeapNode *root, string s)
{
    string ans = "";
    struct MinHeapNode *curr = root;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == '0')
            curr = curr->left;
        else
            curr = curr->right;

        // reached leaf node
        if (curr->left == NULL && curr->right == NULL)
        {
            ans += curr->data;
            curr = root;
        }
    }
    // cout<<ans<<endl;
    return ans + '\0';
}

void calcFreqFromFile(const string &filename)
{
    ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    char c;
    while (inputFile.get(c))
    {
        freq[c]++;
    }

    inputFile.close();
}

// Function to write output to a file
void writeOutputToFile(const string &filename, const string &data)
{
    ofstream outputFile(filename);
    if (!outputFile.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    outputFile << data;

    outputFile.close();
}
void writeCompressedToFile(const string &filename, const string &data)
{
    ofstream outputFile(filename);
    if (!outputFile.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    outputFile << data;

    outputFile.close();
}

int main()
{
    string inputFile = "input.txt";           // Name of the input file
    string outputFile = "output.txt";         // Name of the output file
    string compressedFile = "compressed.txt"; // Name of the compressed output file

    calcFreqFromFile(inputFile);
    HuffmanCodes(freq.size());

    cout << "Character With their Frequencies:\n";
    for (auto v = codes.begin(); v != codes.end(); v++)
        cout << v->first << ' ' << v->second << endl;

    ifstream infile(inputFile);
    string str((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());

    string encodedString, decodedString;
    for (auto i : str)
        encodedString += codes[i];

    cout << "\nEncoded Huffman data:\n"
         << encodedString << endl;

    // Writing the compressed output to a file
    writeCompressedToFile(compressedFile, encodedString);

    // Function call to decode the Huffman data
    decodedString = decode_file(minHeap.top(), encodedString);
    cout << "\nDecoded Huffman Data:\n"
         << decodedString << endl;

    // Writing the decoded output to a file
    writeOutputToFile(outputFile, decodedString);

    return 0;
}