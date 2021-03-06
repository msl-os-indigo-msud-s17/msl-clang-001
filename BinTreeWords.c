#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
  char *word;
  int occurences;
  struct Node *left_child;
  struct Node *right_child;
  struct Node *parent_node;
};

void AddWord(struct Node *new_node, struct Node *current_node);
char *CleanWord(char *word);
int CountPunctuationChars(char *string);
struct Node *DeleteNode(char *target_word, struct Node *current_node);
void DestructTree(struct Node *root);
void DestructNode(struct Node *);
int DetectTrailingPunctuation(char *word);
int FindWord(char *target, struct Node *current_node);
struct Node *InitializeNode(char *clean_word);
void ParseInputFileString();
void PreOrderTraversal(struct Node *current_node);
void PrintPreOrderTraversalToFile(struct Node *current_node);
void PostOrderTraversal(struct Node *current_node);
void ReadInputFile();
char *RemoveTrailingPunctuation(char *word);
struct Node *ReturnWord(char *target, struct Node *current_node);
char *StripInputFileIdentifier(char *in);
char *ToLowerCase(char *word);

struct Node *root = NULL;
char *input_file;
char *output_file;

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: BinTreeWords input_file.txt\n");
    exit(0);
  }

  input_file = argv[1];

  ParseInputFileString();

  ReadInputFile();

  PreOrderTraversal(root);

  PrintPreOrderTraversalToFile(root);

  printf("\n");

  DestructTree(root);

  free(output_file);
  free(input_file);

  return 0;
}

void AddWord(struct Node *new_node, struct Node *current_node) {
  if (new_node == NULL || new_node->word == NULL) {
    printf("Error: word to be added has value of NULL\n");
    return;
  }
  else if (root == NULL) {
    root = new_node;
  }
  else if (strcmp(new_node->word, current_node->word) == 0) {
    current_node->occurences++;
  }
  else if (strcmp(new_node->word, current_node->word) > 0) {
    if (current_node->right_child == NULL) {
      current_node->right_child = new_node;
      new_node->parent_node = current_node;
    }
    else if (strcmp(new_node->word, current_node->right_child->word) > 0)
      AddWord(new_node, current_node->right_child);
    else if (strcmp(new_node->word, current_node->right_child->word) < 0) {
      new_node->right_child = current_node->right_child;
      current_node->right_child = new_node;
      new_node->parent_node = current_node;
    }
    else
      current_node->right_child->occurences++;
  }
  else {
    if (current_node->left_child == NULL) {
      current_node->left_child = new_node;
      new_node->parent_node = current_node;
    }
    else if (strcmp(new_node->word, current_node->left_child->word) < 0)
      AddWord(new_node, current_node->left_child);
    else if (strcmp(new_node->word, current_node->left_child->word) > 0) {
      new_node->left_child = current_node->left_child;
      current_node->left_child = new_node;
      new_node->parent_node = current_node;
    }
    else
      current_node->left_child->occurences++;
  }
  return;
}

char *CleanWord(char *word) {
  char *trailing_punctuation_truncated_word,
       *lower_case_and_trailing_punctuation_truncated_word;

  if (DetectTrailingPunctuation(word)) {
    trailing_punctuation_truncated_word = RemoveTrailingPunctuation(word);
    lower_case_and_trailing_punctuation_truncated_word =
                         ToLowerCase(trailing_punctuation_truncated_word);
  }
  else
    lower_case_and_trailing_punctuation_truncated_word = ToLowerCase(word);

  free(trailing_punctuation_truncated_word);

  return lower_case_and_trailing_punctuation_truncated_word;
}

struct Node *CopyNode(struct Node *node_to_copy) {
  struct Node *new_node = (struct Node *) malloc(sizeof(struct Node));

  new_node->word = node_to_copy->word;
  new_node->occurences = node_to_copy->occurences;
  new_node->right_child = node_to_copy->right_child;
  new_node->left_child = node_to_copy->left_child;
  new_node->parent_node = node_to_copy->parent_node;

  return new_node;
}

int CountPunctuationChars(char *string) {
  int punctuation_count = 0, index = 0;
  char ch;

  ch = string[index++];

  while (ch != '\0') {
    if (ispunct(ch) || ch == '"' || ch == "”")
      punctuation_count++;
    ch = string[index++];
  }

  return punctuation_count;
}

struct Node *DeleteNode(char *target_word, struct Node *current_node) {
  if (current_node == NULL)
    return NULL;
  else if (strcmp(target_word, current_node->word) == 0) {
    if (current_node->right_child != NULL) {
      if (current_node->left_child == NULL) {
        if (current_node == current_node->parent_node->right_child) {
          current_node->parent_node->right_child = current_node->right_child;
        }
        else {
          current_node->parent_node->left_child = current_node->right_child;
        }
      }
      else {
        if (current_node->right_child->left_child == NULL) {
          current_node->right_child->left_child = current_node->left_child;
        }
        else {
          struct Node *trace_node = current_node->left_child;
          while (trace_node != NULL) {
            if (trace_node->left_child == NULL) {
              current_node->left_child = trace_node->left_child;
              trace_node->left_child->parent_node = trace_node;
            }
            trace_node = trace_node->left_child;
          }
        }
        current_node->left_child = NULL;
      }
    }
    current_node->right_child->parent_node = current_node->parent_node;
    current_node->right_child = NULL;
    current_node->parent_node = NULL;
    return current_node;
  }
  else if (strcmp(target_word, current_node->word) > 0)
    return ReturnWord(target_word, current_node->right_child);
  else if (strcmp(target_word, current_node->word) < 0)
    return ReturnWord(target_word, current_node->left_child);
  else
    return NULL;
}

void DestructNode(struct Node *node) {
  free(node->word);
  free(node);

  return;
}

void DestructTree(struct Node *current_node) {
  if (current_node == NULL)
    return;

  DestructTree(current_node->left_child);
  DestructTree(current_node->right_child);

  printf("Deleted: %s\n", current_node->word);

  DestructNode(current_node);

  return;
}

int DetectTrailingPunctuation(char *word) {
  // Return 1 if the word contains trailing punctuation.
  // Return 0 if the word does not contain trailing punctuation.
  char ch;
  int index = 0;

  ch = word[index++];

  while (ch != '\0') {
    if ((ispunct(ch)) && (word[index] == '\0'))
      return 1;
    ch = word[index++];
  }

  return 0;
}

int FindWord(char *target, struct Node *current_node) {
  // Return 1 if the target was found and its count was incremented.
  // Return 0 if the target word was not found.
  if (current_node == NULL)
    return 0;
  else if (strcmp(target, current_node->word) == 0)
    return 1;
  else if (strcmp(target, current_node->word) > 0)
    return FindWord(target, current_node->right_child);
  else
    return FindWord(target, current_node->left_child);
}

struct Node *InitializeNode(char *clean_word) {
  struct Node *new_node = (struct Node *) malloc(sizeof(struct Node));

  new_node->word = clean_word;
  new_node->occurences = 1;
  new_node->left_child = NULL;
  new_node->right_child = NULL;
  new_node->parent_node = NULL;

  return new_node;
}

void ParseInputFileString() {
  char *out = "output";
  char *txt = ".txt";
  char *in_file;
  int input_file_length;
  int index_length = 0;
  int input_file_index_index;
  int input_file_index;
  int txt_index = 0;
  int output_file_length;

  in_file = StripInputFileIdentifier(input_file);
  input_file_length = strlen(in_file);
  input_file_index = input_file_length;

  if (!isdigit(in_file[input_file_length-1])) {
    printf("Error: input file must be indexed, e.g. 'input01'\n");
    exit(0);
  }

  while (isdigit(in_file[--input_file_index]))
    index_length++;

  input_file_index_index = input_file_length - index_length;

  output_file_length = 6 + index_length + 4;

  output_file = (char *) malloc(output_file_length * sizeof(char) + 1);

  for (int i = 0; i < 6; i++)
    output_file[i] = out[i];

  for (int i = 6; i < 6 + index_length; i++)
    output_file[i] = in_file[input_file_index_index++];

  for (int i = 6 + index_length; i < 6 + index_length + 4; i++)
    output_file[i] = txt[txt_index++];

  output_file[6+index_length+4] = '\0';

  free(in_file);

  return;
}

void PostOrderTraversal(struct Node *current_node) {
  if (current_node == NULL)
    return;

  PostOrderTraversal(current_node->right_child);

  printf("%s, %d\n", current_node->word, current_node->occurences);

  PostOrderTraversal(current_node->left_child);

  return;
}

void PreOrderTraversal(struct Node *current_node) {
  if (current_node == NULL)
    return;

  PreOrderTraversal(current_node->left_child);

  printf("%s: %d\n", current_node->word, current_node->occurences);

  PreOrderTraversal(current_node->right_child);

  return;
}

void PrintPreOrderTraversalToFile(struct Node *current_node) {
  FILE *output;

  if (current_node == root)
    output = fopen(output_file, "w+");
  else
    output = fopen(output_file, "a");

  if (current_node == NULL)
    return;

  PrintPreOrderTraversalToFile(current_node->left_child);

  fprintf(output, "%s: %d\n", current_node->word, current_node->occurences);
  fclose(output);

  PrintPreOrderTraversalToFile(current_node->right_child);

  return;
}

void ReadInputFile() {
  // Read a .txt input file word by word. Delete any trailing punctuation
  // characters appended to each word, convert each word to lowercase, and
  // store each word in a binary tree.
  char word[256];
  FILE *input;

  input = fopen(input_file, "r");

  if (input != NULL) {
    while (fscanf(input, " %255s", word) == 1) {
      char *cleaned_word = CleanWord(word);
      struct Node *new_node = InitializeNode(cleaned_word);

      AddWord(new_node, root);
    }
  }

  return;
}

char *RemoveTrailingPunctuation(char *word) {
  unsigned long word_count;
  int punctuation_count, truncated_word_character_count;
  char *trailing_punctuation_truncated_word;

  word_count = strlen(word);

  punctuation_count = CountPunctuationChars(word);

  truncated_word_character_count = (word_count - punctuation_count);

  trailing_punctuation_truncated_word = (char *)
                  malloc(truncated_word_character_count * sizeof(char) + 1);

  for (int i = 0; i <= truncated_word_character_count; i++) {
    if (i == truncated_word_character_count)
      trailing_punctuation_truncated_word[i] = '\0';
    else
      trailing_punctuation_truncated_word[i] = word[i];
  }

  return trailing_punctuation_truncated_word;
}

struct Node *ReturnWord(char *target, struct Node *current_node) {
  struct Node *target_node;

  if (current_node == NULL)
    return NULL;
  else if (strcmp(target, current_node->word) == 0) {
    target_node = CopyNode(current_node);
    return target_node;
  }
  else if (strcmp(target, current_node->word) > 0)
    return ReturnWord(target, current_node->right_child);
  else if (strcmp(target, current_node->word) < 0)
    return ReturnWord(target, current_node->left_child);
  else
    return NULL;
}

char *StripInputFileIdentifier(char *in_string) {
  char *out_string;
  int out_string_length;
  int file_identifier_length;
  int file_identifier_index = 0;

  while (in_string[file_identifier_index] != '.')
    file_identifier_index++;

  file_identifier_length = strlen(in_string) - file_identifier_index;

  out_string_length = strlen(in_string) - file_identifier_length;

  out_string = (char *) malloc(out_string_length * sizeof(char) + 1);

  for (int i = 0; i < out_string_length; i++)
    out_string[i] = in_string[i];

  out_string[out_string_length] = '\0';

  return out_string;
}

char *ToLowerCase(char *word) {
  int character_count;
  char *lower_case_word;

  character_count = strlen(word);

  lower_case_word = (char *) malloc(character_count * sizeof(char)+1);

  for (int i = 0; i <= character_count; i++) {
    if (i == character_count)
      lower_case_word[i] = '\0';
    else if (isupper(word[i]))
      lower_case_word[i] = tolower(word[i]);
    else
      lower_case_word[i] = word[i];
  }

  return lower_case_word;
}
