#include <iostream>      //Including standard libraries
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <vector>
#include<bits/stdc++.h>

using namespace std;



//Defining macros
#define rep(i,n) for(int (i)=0;(i)<(n);(i)++)
#define pb push_back
#define all(v) v.begin(),v.end()
#define ll long long int
#define vll vector<int>




//Defining Data-Structures for trees
typedef struct node{
    vector<vector<int> > list;          //vector of vectors to store all list of literals
    vector<int> tta;                    //stores the truth value of literals
    int check;                          //used to check stisfiability of tree
    node *left;                         //pointer to left child tree
    node *right;                        //pointer to right child tree
    node *parent;                       //pointer to parent tree
} node;




//Global Variables
bool sat;                               //Global sat which is true if we fnd a satisfiable solution otherwise false
vector<int> fin;                        //This vector stores the possible final solution
int prop;                               //Total number of propositions to be solved
int conflicts;
int decisions;




// Defining the functions used
node* unit_propagation(node *tree,int p);//Simplifies the node assuming p is true
int best_poss(node *tree);              //This function return the best possible integer which can be removed from the tree-clauses
void make_branch(node *tree , int p);   //Makes the left and right branch of tree
void sat_solver(node *tree);            //Main function which solves sudoku
void pre_process(node *tree);           //For pre-processing the input before sending to sat-solver






int main(int argc, char **argv){
    clock_t startmain=clock();                 //Clock will give running time
    FILE *fp;
    FILE *in_file  = fopen(argv[1], "r");      //Open the file in read only mode
    if (in_file==NULL){                        //If file does not open show error
        printf("Error! Could not open file\n");
        exit(-1);
    }
    int con=0;                                 // con is number of literals
    prop =  0;                                 // prop is number of propositions
    int num,check,count;                       // Helper variables

    char initials[10000];                      //To take input
    fscanf(in_file,"%s",initials);
    if(initials[0]=='c')
        do{
            fgets(initials,10000,in_file);     //taking input of each line if the line is a comment
            fscanf(in_file,"%s",initials);     //taking first word of next line as input
        }while(initials[0]=='c');
    count=0;
    fscanf(in_file, "%s",initials);          //reading "cnf" as string
    fscanf(in_file, "%s",initials);          //reading number of propositions in string format
    int len=strlen(initials);                //length of initials string to calculate number of propositions
    while(count<len){
        prop=10*prop+initials[count]-'0';    //To convert string to int
        count++;
    }
    rep(i,prop+1) fin.pb(0);                //We declare a final vector fin which stores the final answer whose ith element is denoted as 1 if ith proposition is true and -1 if false

    count=0;
    fscanf(in_file, "%s",initials);         //reading number of conditions in string format
    // printf("%s\n",initials);
    len=strlen(initials);                   //length of initials string to calculate number of conditions
    while(count<len){
        con=10*con+initials[count]-'0';
        count++;
    }                                       //to convert string to int
    node *tree = new node;                  //We create a node in which we store the propositions and the possible truth values and nodes for left and right child and parent and check for unsatisfiability
    tree->check = 0;                        //initially assuming there is a solution we assign check of tree to true
    int sing=0;
    vector<int> inp;                        //Defining a vector to take input
    for(int i=0;(i<con);){
        count=0;num=0;check=0;
        fscanf(in_file,"%s",initials);      //Taking a number in string format
        len=strlen(initials);               //Determining the length of string to convert it to string
        if(initials[0]=='-'){
            count=1;                        //If first character is - then number starts from next index and we make check=1 which ensures we take care of sign
            check=1;
        }
        while(count<len){
            num=10*num+initials[count]-'0'; //Converting string to integer
            count++;
        }
        if(check==1)      num=-num;        //If first sign was - then taking negative of number obtained
        if(num==0){                      //If the number we get is 0 then we obtain full set of literals
            sort(inp.begin(),inp.end()); //Sorting given input to making processing easier
            vector<int> inp2,inp3;       //inp2 for removing duplicates and inp3 for removing duplicates
            if(inp.size()!=0) inp2.pb(inp[0]);              //Copying vector input to input2 if we find a unique element to get rid of duplicates
            rep(j,inp.size()) if(inp[j]!=inp2[inp2.size()-1]) inp2.pb(inp[j]);
            inp.clear();
            //If initial clause contains a proposition and its negation then we will not add the proposition to the vector list,
            // or if a literal contains a proposition multiple times then we need to include the proposition single time only
            rep(j,inp2.size()){
                if(inp2[j]>0) break;     //For all negation propositions we are searching for its positive proposition and if we find it we insert in inp3
                if (binary_search(all(inp2), inp2[j]*(-1))) inp3.pb(inp2[j]);
            }
            rep(j,inp2.size()){
                                        //if we find any element whose negation or it was in inp3 then we do not insert in input
                if(binary_search(all(inp3), inp2[j])||binary_search(all(inp3), inp2[j]*(-1))) continue;
                inp.pb(inp2[j]);
            }
            if(inp.size()==0){
                i++;                   //If the size of input remains 0 or we donot need any constraint then we need not push anything in final clauses list
                continue;
            }
            else{
                if(inp.size()==1) sing = 1;//If the clause contains only 1 literal then the prosition can only be true so we ha
                 tree->list.pb(inp);
            }
            inp.clear();                    //clearing input to taking input for next literal
            i++;
        }
        else{
            inp.pb(num);                    //If number is not 0 then keep adding it to the inp vector
        }
    }
    tree->parent = NULL;                    //Since this vector is parent node so its parent mustbe null
    clock_t startsim=clock();               //Clock will give the total simplification time
    pre_process(tree);
    int eliminatedclauses = con - tree->list.size();
    double simpletim = (double)(clock()-startsim)/CLOCKS_PER_SEC ;
    conflicts=0;                            //Initialize the total number of conflicts
    decisions=0;
    sat = false;
    clock_t startsolve = clock();           //Clock will give total time for solving the tree
     sat_solver(tree);
    double solvetim = (double)(clock()-startsolve)/CLOCKS_PER_SEC ;
    FILE *ansfile = fopen("answer.txt","w");               //File for printing final output
    if (ansfile==NULL){                                    //If file does not open show error
        printf("Error! Could not open file\n");            //and return
        exit(-1);
    }
    free(tree);
    if(sat == true){                                       //If no contradiction is found we print SAT
        fprintf(ansfile, "SAT\n" );
        int p;
        rep(i,prop+1){                                     //and one of the possible solution
            if(i==0) continue;
            p = i;
            if(fin[i]==-1) p *= -1;
            fprintf(ansfile, "%d ",p);
        }
    }
    cout<<"\n";
    if(sat== false){                                    //If no possible solution exists then we print UNSAT
        fprintf(ansfile,"UNSAT\n");
    }
    fclose(ansfile);                                    //Closing the answer file
        double tottim = (double)(clock()-startmain)/CLOCKS_PER_SEC ;
        printf("============================[ Problem Statistics ]=============================\n");
        printf("|                                                                             |\n");
        printf("|  Number of variables:       %8d                                        |\n",prop);
        printf("|  Number of clauses:         %8d                                        |\n",con);
        printf("|  Pre-Processing time:       %8lf sec                                    |\n",simpletim);
        printf("|  Eliminated clauses:        %8d                                        |\n",eliminatedclauses);
        printf("|  SAT Solving time:          %8lf sec                                    |\n",solvetim);
        printf("|                                                                             |\n");
        printf("===============================================================================\n");
        printf("===============================================================================\n");
        printf("Conflicts             : %d              (%d /sec)\n",conflicts,(int)((conflicts*(1.0))/solvetim));
        printf("Decisions             : %d              (%d /sec)\n",decisions,(int)((decisions*(1.0))/solvetim));
        printf("Total Time            : %lf sec\n",tottim);
        if(sat==true) cout<<"\nSATISFIABLE\n";
        else cout<<"\nUNSATISFIALE\n";
    return 0;
}



void pre_process(node *tree){
    //To remove single sized propositions
    int sing = 1;                           //Variable to check if size of any literal is 1
    vll elim;
    while(tree->list.size()>0&&sing==1){    //Running loop while tree size is not zero and there exist a literal containing only 1 proposition
        vector<vector<int> > inp;
        vector<int> rem;                    //To store all propositions which have size 1.
        sing = 0;                           //This indicates we have have processed all single propositions and currently there areno more
        rep(i,tree->list.size()){           //For each literal we check if the element is present then we remove the literal
            if(tree->list[i].size()==1){    //or if negation of proposition is present in any literal then we append the literal except the negateion proposition.
                int req = tree->list[i][0];
                rem.pb(req);
                if(req<0){
                    req *= (-1);
                    if(fin[req]==1){       //If the truth value of the proposition was true and we obtain false then no solution is possible and we return
                        conflicts++;
                        tree->check = 2;
                        return;
                    }
                    fin[req] = -1;    //We assign the final value of ith proposition to false
                }
                else{
                    if(fin[req]==-1){     //If the truth value of the proposition was false and we obtain true then no solution is possible and we return
                        conflicts++;
                         tree->check = 2;
                         return;
                    }
                    fin[req] = 1;         //We assign the final value of ith proposition to true
                }
            }
        }
        sort(rem.begin(),rem.end());     //Sorting list of single clause propositions to make searches easier
        rep(i,tree->list.size()){        //Searchinf for an element of rem in ith list of literals of tree
            vector<int> try1;
            rep(j,tree->list[i].size()){
                if(binary_search(all(rem), tree->list[i][j])){
                    try1.clear();        //If we find the proposition in any list then the clause becomes true
                    break;               //and we need not process it any furthur and we continue on to next literal
                }
                if(binary_search(all(rem), (-1)*(tree->list[i][j])))    continue;//If we find the negation of the proposition then we add it to try1.
                 try1.pb(tree->list[i][j]);   //If we donot find the negation of  proposition or proposition then we add it to try1
            }
            if(try1.size()==0) continue;      //If we get the literal satisfiable then continue
            if(try1.size()==1) sing = 1;       //If we are left with single proposition then we do this processing again so we assign sing to 1
            inp.pb(try1);                      //We input the formatted try1 to input
        }
        tree->list = inp;                      //We assign the improved list input to tree
    }
    vector<vector<int> > inp;
    //To remove duplicates
    sort(tree->list.begin(),tree->list.end());//To remove duplicate elements we sort accoring to first index
    rep(i,tree->list.size()){
        if(i==0){
            inp.pb(tree->list[i]);           //We insert first element
            continue;
        }
        if(tree->list[i]==tree->list[i-1]) continue;//If both vectors are same then we don't insert
        //As elements are sorted according to 1st element so if 2 vectors will be equal they will have common first element
        inp.pb(tree->list[i]);                      //else we insert
    }
    tree->list = inp;                               //We assign tree->list to input
    tree->tta = fin;
    return;
}




node* unit_propagation(node *tree,int p){       //Assuming t to bee true then simplifying the clauses
    vector<vector<int> > lisfin;
    if(p<0){
        if(tree->tta[-p]==1){
            conflicts++;
            tree->check=2;
            return tree;
        }
        tree->tta[-p] = -1;
    }
    else{
        if(tree->tta[p]==-1){
            conflicts++;
            tree->check=2;
            return tree;
        }
        tree->tta[p] = 1;
    }
    vll chan1;
    rep(i,tree->list.size()){//For prop with size 2 other has to be true
        if((tree->list[i].size()==2)&&(binary_search(all(tree->list[i]), -p ))){
                int tr12 = tree->list[i][0];
                if(tr12==-p) tr12 = tree->list[i][1];
                chan1.pb(tr12);
        }
    }
    rep(i,chan1.size()){
        if(chan1[i]<0){
            if(tree->tta[(-1)*chan1[i]]==1){
                    conflicts++;
                    tree->check = 2;
                    return tree;
            }
            tree->tta[chan1[i]*(-1)]= -1;
        }
        else{
            if(tree->tta[chan1[i]]==-1){
                    conflicts++;
                    tree->check = 2;
                    return tree;
            }
            tree->tta[chan1[i]]=1;
        }
    }
    rep(i,tree->list.size()){
        if(binary_search(all(tree->list[i]),-p))
            if(tree->list[i].size()==2) continue;
        vll inp;//For putting in the input removing p
        rep(j,tree->list[i].size()) if(tree->list[i][j]!=-p) inp.pb(tree->list[i][j]);
        //If p is false so -p is true so we remove those cases
        if (binary_search(all(inp), p )) continue;
        lisfin.pb(inp);
    }
    tree->list = lisfin;
    rep(i,chan1.size()){
        tree = unit_propagation(tree,chan1[i]);
    }
    return tree;

}


int best_poss(node *tree){                                        //Returns the proposition to evalauate for best propagation
    decisions++;
    if(tree->list.size()==0){                                     //If no literals are left then we have satisfiability and we return
        fin = tree->tta;                                          //We assign tree-> truth table to final solution
        sat = true;
        tree->check = 2;
        return 0;
    }
    int freq[prop+1] = {0};                                       //To calculate frequency of all propositions
    int freqmax = 0;                                              //Frequency of maxfrequency element
    int doufreq[prop+1] = {0};                                    //To count the number of literals in which ith literal has 2 size
    int best  =  0;                                               //It stores the best proposed proposition for evalauation
    rep(i,tree->list.size()){
        rep(j,tree->list[i].size()){
            int a = tree->list[i][j];                              //We increase the frequency of the proposition
            if(a<0) a*=(-1);
            freq[a]++;
            if(tree->list[i].size()==2){                           //We increase doufreq by 2 of ith proposition if proposition occurs in literal of freq 2
                doufreq[a]+=2;
            }
        }
    }
    rep(i,prop+1){
        if(freq[freqmax]<freq[i]) freqmax = i;                     //If double freq +freq of any prop is max then we select it as best possible proposition for evaluation
        if((doufreq[best]+freq[best])<(doufreq[i]+freq[i])) best = i;
    }
    int flag  = 0;
    if(freq[freqmax]==1){                           //If maximum freq is 1
        rep(i,tree->list.size()){
            if(flag==1) break;
            flag = 1;
            rep(j,tree->list[i].size()){
                // if(flag==1) break;
                int t = tree->list[i][j];          //Frequency of t can be 1 only
                if(t<0){
                    t *= (-1);
                    if(tree->tta[t]==0||tree->tta[t]==-1){
                            flag = 0;              //First place where the literal can be made satisfiable ttta of proposition is assigned false and proposition is made false
                            tree->tta[t] = -1;
                            break;
                    }
                }
                else{
                    if(tree->tta[t]==0||tree->tta[t]==1){
                            flag = 0;              //First place where the literal can be made satisfiable ttta of proposition is assigned false and proposition is made true
                            tree->tta[t] = 1;
                            break;
                    }
                }
            }
        }
        if(flag==1){
            conflicts++;
            tree->check = 2;                    //If assigned all propositions any possible value cannot satisfy the literals then no solution exists
        }
        else{
            rep(i,fin.size()) fin[i] = tree->tta[i];//Otherwise we have obtained a solution
            sat = true;
        }
        return 0;
    }
    return best;                               //Return the best solvable sudoku according to our rankings
}



void make_branch(node *tree , int p){
    node *rtree = new node;                 //Assigning memory to left node
    node *ltree = new node;                 //Assigning memory to right node
    tree->left = ltree;                     //Tree left node is ltree
    tree->right = rtree;                    //Tree right node is rtree
    ltree->parent = tree;                   //Ltree's parent is tree
    rtree->parent = tree;                   //Rtree's parent is tree
    ltree->check = 0;                       //Assuming a solution exists for left node till now
    rtree->check  = 0;                      //Assuming a solution exists for right node till now

    //For creation of left branch where p is true
    ltree->tta = tree->tta;                 //The assigned values to propositions in parent node must be satisfied in left node
    ltree->tta[p] = 1;                      //Assuming p to be true for left tree
    ltree->list = tree->list;               //The list of literals for left tree is same as for parent tree

    //If p is true so -p is false

    unit_propagation(ltree,p);             //Now we propogate in set of literals and minimise number of literals
                                           //until no literal of size 2 is left or size of set of literals is 0


    //For creation of right branch where p is false
    rtree->tta = tree->tta;               //The assigned values to propositions in parent node must be satisfied in right node
    rtree->tta[p] = -1;                   //Assuming p to be false for right tree
    rtree->list = tree->list;             //The list of literals for left tree is same as for parent tree

    unit_propagation(rtree,-p);           //Now we propogate in set of literals and minimise number of literals
                                          //until no literal of size 2 is left or size of set of literals is 0
    return;
}

void sat_solver(node *tree){
    if((sat==true)||(tree->check == 2))    //If we find a solution or we find a contradiction we return
        return;
    int p = best_poss(tree);               //We findthe integer using the juristics which makes the solving easiest
    if((sat==true)||(tree->check ==2))     //If we find a solution or we find a contradiction we return
        return;
    make_branch(tree,p);                   //We try for solutin assuming p to true in left node and it to be false in right node

    sat_solver(tree->left);                //Solving sudoku assuming p to be true
    free(tree->left);
    if(sat==true)                          //If we donot a solution return
        return ;
    sat_solver(tree->right);               //If assuming p to be true doesnot give a solution i.e. left node we assume p to be false and solve right node
    free(tree->right);
}
