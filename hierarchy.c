#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hierarchy.h"

#define NAME 20
#define TEAM 30
#define EMPLOYEES 50

TreeNode* createNode(char *employee_name) {
	TreeNode *node =(TreeNode *) malloc(sizeof(TreeNode));

    node->name =(char *) malloc(NAME * sizeof(char));
	strcpy(node->name, employee_name);
    node->team =(TreeNode **) malloc(TEAM * sizeof(TreeNode *));
	node->direct_employees_no = 0;
    node->manager = NULL;

	return node;
}


TreeNode* find(Tree tree, char *name){
    int idx;
    if(tree)
        if(strcmp(tree->name, name) == 0)
            return tree;
        else{
            for(idx = 0; idx < tree->direct_employees_no; idx++){
                TreeNode* node = find(tree->team[idx], name);
                if(node != NULL)
                    return node;
            }
    } 
    return NULL;

}

int cmp(char *name1, char *name2){
    int ctr = 0;

    while(name1[ctr] == name2[ctr]){
        if(name1[ctr] == '\0' || name2[ctr] == '\0')
            break;
        ctr++;
    }
    if(name1[ctr]=='\0' && name2[ctr]=='\0'){
        return 0;
    }
    else
        if(name1[ctr] < name2[ctr])
            return -1;
        else return 1;
}

void addEmployee(Tree manager, Tree employee){
    int i = 0;
    while( i < manager->direct_employees_no && \
                cmp(manager->team[i]->name, employee->name) < 0){
            i++;
    }

    manager->direct_employees_no++;
        
    for(int j = manager->direct_employees_no - 1; j >= i; j--){
            manager->team[j+1] = manager->team[j];
    }

    manager->team [i] = employee;
    employee->manager = manager;
    
}

/* Adauga un angajat nou in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 * manager_name: numele sefului noului angajat
 *
 * return: intoarce ierarhia modificata. Daca tree si manager_name sunt NULL, 
           atunci employee_name e primul om din ierarhie.
 */
Tree hire(Tree tree, char *employee_name, char *manager_name) {
    TreeNode *employee = createNode(employee_name);

    if(manager_name == NULL && tree == NULL)  
        return employee;
    else
    {
        TreeNode *manager = find(tree, manager_name);
        addEmployee(manager, employee);
        return tree;
    }
}

/* Sterge un angajat din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului concediat
 *
 * return: intoarce ierarhia modificata.
 */
void deleteEmployee(Tree manager, char *employee_name){
    int i = 0;
    while(cmp(employee_name, manager->team[i]->name) != 0)
        i++;
    for(int j = i+1; j < manager->direct_employees_no; j++){
        manager->team[j - 1] = manager->team[j]; 
    }
    manager->team[ --manager->direct_employees_no ] = NULL;
}

Tree fire(Tree tree, char *employee_name) {
    TreeNode *fired = find(tree, employee_name);

    if(fired == NULL) return tree;
    if(fired == tree)   return tree;

    TreeNode *manager = fired->manager;

    //eliminam angajatul din echipa manager-ului
    deleteEmployee(manager, employee_name);

    //adaugam oamenii din echipa celui concediat in echipa manager-ului
    
    for(int j = 0; j < fired->direct_employees_no; j++){
        addEmployee(manager, fired->team[j]);
        fired->team[j] = NULL;
    }

    //eliberam spatiul

    free(fired->team);
    free(fired->name);
    free(fired);

    return tree;
}

/* Promoveaza un angajat in ierarhie. Verifica faptul ca angajatul e cel putin 
 * pe nivelul 2 pentru a putea efectua operatia.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 *
 * return: intoarce ierarhia modificata.
 */
Tree promote(Tree tree, char *employee_name) {
    TreeNode *employee = find(tree, employee_name);

    if(employee == NULL) return tree;
    if(employee == tree) return tree;
    if(employee->manager == tree) return tree;

    TreeNode *manager = employee->manager;

    // eliminam angajatul din echipa manager-ului
    deleteEmployee(manager, employee_name);

    // adaugam oamenii din echipa celui promovat in echipa manager ului
    for(int j = 0; j < employee->direct_employees_no; j++){
        addEmployee(manager, employee->team[j]);
        employee->team[j] = NULL;
    }

    // promovam angajatul cu un nivel
    employee->direct_employees_no = 0;
    addEmployee(manager->manager, employee);

    return tree;
}

/* Muta un angajat in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_employee(Tree tree, char *employee_name, char *new_manager_name) {
    TreeNode *employee = find(tree, employee_name);
    TreeNode *new_manager = find(tree, new_manager_name);

    if(employee == NULL) return tree;
    if(employee == tree) return tree;
    if(new_manager == NULL) return tree;

    TreeNode *old_manager = employee->manager;
    
    if(new_manager == old_manager) return tree;

    // eliminam angajatul din echipa manager-ului vechi
    deleteEmployee(old_manager, employee_name);

    // adaugam oamenii in echipa manager ului vechi
    
    for(int j = 0; j < employee->direct_employees_no; j++){
        addEmployee(old_manager, employee->team[j]);
        employee->team[j] = NULL;
    }

    employee->direct_employees_no = 0;
    addEmployee(new_manager, employee);

    return tree;
}

/* Muta o echipa in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei mutate
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_team(Tree tree, char *employee_name, char *new_manager_name) {
    TreeNode *employee = find(tree, employee_name);
    TreeNode *new_manager = find(tree, new_manager_name);

    if(employee == NULL) return tree;
    if(employee == tree) return tree;
    if(new_manager == NULL) return tree;

    TreeNode *old_manager = employee->manager;
    
    if(new_manager == old_manager) return tree;

    // eliminam angajatul din echipa manager-ului vechi
    deleteEmployee(old_manager, employee_name);

    addEmployee(new_manager, employee);

    return tree;
    
}

/* Concediaza o echipa din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei concediate
 *
 * return: intoarce ierarhia modificata.
 */

Tree fire_team(Tree tree, char *employee_name) {
    TreeNode *fired_team = find(tree, employee_name);
    TreeNode *manager = fired_team->manager;

    if(fired_team == NULL) return tree;
    if(fired_team == tree) return tree;

    deleteEmployee(manager, employee_name);

    destroy_tree(fired_team);
    return tree;
}

void employees_by_manager(Tree tree, char **employees, int *no_of_employees){
    if(tree){
        employees[++*no_of_employees] = tree->name;
        for(int i = 0; i < tree->direct_employees_no; i++){
            employees_by_manager(tree->team[i], employees, no_of_employees);
        }
    }
}

int cmpa(const void *a, const void *b){
    return strcmp(*(const char **)a, *(const char **)b);
}

/* Afiseaza toti angajatii sub conducerea unui angajat.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei
 */

void get_employees_by_manager(FILE *f, Tree tree, char *employee_name) {
    TreeNode *manager = find(tree, employee_name);
    int no_of_employees = 0;

    char **employees = (char **)malloc(EMPLOYEES * sizeof(char *));

    employees_by_manager(manager, employees, &no_of_employees);
    qsort(employees + 1, no_of_employees, sizeof(*employees), cmpa);

    for(int i = 1; i <= no_of_employees; i++){
        fprintf(f, "%s ", employees[i]);
    }

    fprintf(f, "\n");

    free(employees);
    return;
}



/* Afiseaza toti angajatii de pe un nivel din ierarhie.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * level: nivelul din ierarhie
 */
int get_level(TreeNode *employee){
    if(employee->manager == NULL){
        return 0;
    }
    int level = 0;
    TreeNode *manager = employee->manager;

    while(manager != NULL){
        level ++;
        manager = manager -> manager;
    }

    return level;
}

void employees_by_level(Tree tree, char **employees, int *no, int level){
    if(tree){
        if(level == get_level(tree)) {
            employees[++*no] = tree->name; 
            return;// nu se duce mai departe de nivelul "level"
        }
        for(int i = 0; i < tree->direct_employees_no; i++){
            employees_by_level(tree->team[i], employees, no, level);
        }
    }
}

void get_employees_by_level(FILE *f, Tree tree, int level) {
    int no_of_employees = 0, crt_level = -1;

    char **employees = (char **)malloc(EMPLOYEES * sizeof(char *));

    employees_by_level(tree, employees, &no_of_employees, level);
    qsort(employees + 1, no_of_employees, sizeof(*employees), cmpa);

    for(int i = 1; i <= no_of_employees; i++){
        fprintf(f, "%s ", employees[i]);
    }

    fprintf(f, "\n");

    free(employees);

    return;
}

/* Afiseaza angajatul cu cei mai multi oameni din echipa.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */

void max_employees(Tree tree,int *max){
    if(tree){
        if(*max < tree->direct_employees_no)
            *max = tree->direct_employees_no;
        for(int i = 0; i < tree->direct_employees_no; i++){
            max_employees(tree->team[i], max);
        }
    }
}

void best_managers(Tree tree, char **manager, int *no_of_managers, int max){
    if(tree){
        if(max == tree->direct_employees_no)
            manager[ ++*no_of_managers ] = tree->name;
        for(int i = 0; i < tree->direct_employees_no; i++){
            best_managers(tree->team[i], manager, no_of_managers, max);
        }
    }
}

void get_best_manager(FILE *f, Tree tree) {
    int max_no_of_employees = -1, crt_level = -1;

    char **managers = (char **)malloc(EMPLOYEES * sizeof(char *));
    int no_of_managers = 0;

    max_employees(tree, &max_no_of_employees);
    best_managers(tree, managers, &no_of_managers, max_no_of_employees);
    qsort(managers + 1, no_of_managers, sizeof(*managers), cmpa);

    for(int i = 1; i <= no_of_managers; i++){
        fprintf(f, "%s ", managers[i]);
    }

    fprintf(f, "\n");

    free(managers);

    return;
}

/* Reorganizarea ierarhiei cu un alt angajat in varful ierarhiei.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului care trece in varful ierarhiei
 */
void change(Tree employee, Tree manager){
    if(manager == NULL){
        return;
    }

    // eliminam employee din echipa manager-ului
    deleteEmployee(manager, employee->name);
    
    Tree next = manager -> manager;
    manager->manager = NULL;
    addEmployee(employee, manager);

    change(manager, next);
}


Tree reorganize(Tree tree, char *employee_name) {
    TreeNode *head = find(tree, employee_name);

    if( head == NULL) return tree;

    TreeNode *manager = head->manager;

    head->manager =  NULL;

    change(head, manager);

    return head;
}

void traversal(FILE *f, Tree tree){
    if(tree){
        if(tree->manager == NULL){
            fprintf(f, "%s ", tree->name);
        }else
        {
            fprintf(f, "%s-%s ", tree->name, tree->manager->name);
        }
        for(int i = 0; i < tree->direct_employees_no; i++){
            traversal(f, tree->team[i]);
        }
    }
}

/* Parcurge ierarhia conform parcurgerii preordine.
 *
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */
void preorder_traversal(FILE *f, Tree tree) {
    traversal(f, tree);
    fprintf(f,"\n");
}

/* Elibereaza memoria alocata nodurilor din arbore
 *
 * tree: ierarhia existenta
 */
void destroy_tree(Tree tree) {
    if(tree == NULL)    return;

    for(int i = 0; i < tree->direct_employees_no; i++){
        destroy_tree(tree->team[i]);
    }

    free(tree->name);
    free(tree->team);
    free(tree);

    tree = NULL;
}
