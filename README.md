# msl-clang-001

This a simple binary tree structure. Each node contains a reference to it's parent,  
references to two children nodes, a word, and an integer field corresponding to the  
number of occurences of that word in the input file. The input is a text file containing  
any chunk of text. The program tokenizes the input file into words, cleans the words of  
any trailing characters recognized by the C standard library ispunct() function as  
punctuation, and converts all the characters in each word to lower case. Once all words  
have been 'cleaned' they are added to a binary tree which organizes them by lexical  
comparison. The tree's contents is then printed to an output file by way of pre-order  
traversal, and the tree is promptly destructed. The file to be read is passed in as a 
command-line argument.
