#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char *word;
    struct Node *right;
    struct Node *left;
} Node;
Node* newNode(char *word)
{
    Node *n=(Node*)malloc(sizeof(Node));
    n->word=malloc(strlen(word)+1);
    strcpy(n->word,word);
    n->left=NULL;
    n->right=NULL;
    return n;
}


Node* insertNode(Node* root,char *word)
{
    if(root== NULL)
        return newNode(word);
    if(strcasecmp(word,root->word)<0)
        root->left=insertNode(root->left,word);
    else if(strcasecmp(word,root->word)>0)
        root->right=insertNode(root->right,word);

    return root;

}
Node* readFile(Node *root,char *fileName)
{
    FILE *f= fopen(fileName,"r");
    if(!f)
    {
        printf("Dictionary cannot be found!!\n");
        return NULL;
    }
    char insertingWord[30];
    while(!feof(f))
    {
        fscanf(f,"%s",insertingWord);
        root=insertNode(root,insertingWord);
    }
    printf("Dictionary Loaded Successfully...!\n");
    fclose(f);
    return root;
}
Node* findMinInRight(Node *root)
{
    if (root == NULL)
    {
        return NULL;
    }
    while (root->left)
    {
        root = root->left;
    }
    return root;
}
Node* findMaxInLeft(Node *root)
{
    if (root == NULL)
    {
        return NULL;
    }
    while (root->right)
    {
        root = root->right;
    }
    return root;
}
Node* successor(Node *root, Node *checkedNode)
{
    if (checkedNode->right)
    {
        return findMinInRight(checkedNode->right);
    }
    else
    {
        Node* successor=NULL;
        while(strcasecmp(root->word,checkedNode->word)!=0)
        {
            if(strcasecmp(root->word,checkedNode->word)>0)
            {
                successor=root;
                root=root->left;
            }
            else
                root=root->right;
        }
        return successor;
    }
}
Node* predecessor(Node *root, Node *checkedNode)
{
    if (checkedNode->left)
    {
        return findMaxInLeft(checkedNode->left);
    }
    else
    {
        Node* predecessor=NULL;
        while(strcasecmp(root->word,checkedNode->word)!=0)
        {
            if(strcasecmp(root->word,checkedNode->word)<0)
            {
                predecessor=root;
                root=root->right;
            }
            else
                root=root->left;
        }
        return predecessor;
    }

}

void inorderPrint(Node *root)
{
    if (root)
    {
        inorderPrint(root->left);
        printf("%s\n",root->word);
        inorderPrint(root->right);
    }
}

int countNodes(Node* root)
{
    if(!root)
        return 0;
    return 1+countNodes(root->left)+countNodes(root->right);
}
int max(int x, int y)
{
    return x>y?x:y;
}
int getHeightEdges(Node *root)
{
    if ( root == NULL)
        return -1;
    else
        return 1 + max(getHeightEdges(root->left),getHeightEdges(root->right));
}
int getHeightNodes(Node* root)
{
    int left=0,right=0;
    if(!root)
        return 0;
    left=1+getHeightNodes(root->left);
    right=1+getHeightNodes(root->right);
    return left>right? left:right;
}
Node* searchWord(Node* root, char* word)
{
    while(root)
    {
        if(strcasecmp(root->word,word)==0)
            return NULL;
        if(strcasecmp(root->word,word)>0)
        {
            if(!(root->left))
                return root;
            root=root->left;
        }
        if(strcasecmp(root->word,word)<0)
        {
            if(!(root->right))
                return root;
            root=root->right;
        }
    }
}
void checkSpelling(Node* root, char* sentence)
{
    char* sent=malloc(sizeof(sentence));
    strcpy(sent,sentence);
    char* token=strtok(sent," ");
    Node* temp = NULL;
    while(token)
    {
        temp=searchWord(root,token);
        if(!temp)
            printf("%s - CORRECT",token);
        else
        {
            printf("%s - Incorrect, Suggestions : %s",token,temp->word);
            if(successor(root,temp))
                printf(" %s",successor(root,temp)->word);
            if(predecessor(root,temp))
                printf(" %s",predecessor(root,temp)->word);
        }
        printf("\n");
        token=strtok(NULL," ");
    }
}
int main()
{
    Node* p=NULL;
    p=readFile(p,"EN-US-Dictionary.txt");
    printf("Size: %d\n............................\n",countNodes(p));
    printf("Height(Number of Edges): %d\n............................\n",getHeightEdges(p));
    printf("Height(Number of Nodes): %d\n............................\n",getHeightNodes(p));
    printf("Enter a sentence: \n");
    char sentence[100];
    gets(sentence);
    checkSpelling(p,sentence);
    return 0;
}
