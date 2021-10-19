#include "func.h"

int length_list(node *head){
    node *curr;
    int i = 0;
    for (curr = head; curr; curr = curr -> next){
        ++i;
    }
    return i;
}

void delete_list(node *head){
    node* curr;
    node* next;
    for (curr = head; curr; curr = next){
        next = curr -> next;
        free(curr -> string);
        free(curr);
    }
}

void print_list(int p, node *head){
    node *curr = head;
    int i;
    for (i = 0; curr && i < p; ++i){
        printf("%s\n", curr -> string);
        curr = curr -> next;       
    }
}

int read_list(const char *name, node **head){
    FILE *fp;
    char buf[LEN];
    node* first;
    node* curr;
    node* next;
    int i; 

    if (!(fp = fopen(name, "r"))){
        return ERROR_OPEN;
    }
    if (!fgets(buf, LEN, fp)){
        fclose(fp); 
        return ERROR_READ;
    }
    first = (node*)malloc(sizeof(node));
    if (!first){
        fclose(fp); 
        return ERROR_MALLOC;
    }
    for (i = 0; buf[i]; ++i){
        if (buf[i] == '\n'){
            buf[i] = 0; 
            break;
        }
    }
    first -> string = (char*)malloc(i + 1);
    if (!(first -> string)){ 
        free(first); 
        fclose(fp); 
        return ERROR_MALLOC; 
    }
    strcpy(first -> string, buf);
    first -> next = 0;
    curr = first;
    while (fgets(buf, LEN, fp))
    {
        if (!(next = (node*)malloc(sizeof(node))))
        {
            delete_list(first);
            fclose(fp);
            return ERROR_MALLOC;
        }
        for (i = 0; buf[i]; i ++){
            if (buf[i] == '\n'){
                buf[i] = 0; 
                break;
            }
        }
        if (!(next -> string = (char*)malloc(i + 1))){
            delete_list(first);
            free(next);
            fclose(fp);
            return ERROR_MALLOC;
        }
        strcpy(next -> string, buf);
        next -> next = 0;
        curr -> next = next;
        curr = next;
    }
    if (!feof(fp)){
        delete_list(first);
        fclose(fp);
        return ERROR_READ;
    }
    *head = first;
    fclose(fp);
    return ERROR_SUCCESS;
}

int task01(node *head){
    int count = 1;
    int res = 0;
    node *curr = head;
    char *max_string = curr -> string;

    for (curr = curr -> next; curr; curr = curr->next){
        res = strcmp(curr -> string, max_string);
        if (res == 0){
            ++count;
        } else if (res > 0){
            count = 1;
            max_string = curr -> string;
        }
    }
    return count;
}

int task02(node *head){
    int count = 0;
    int res = 0;
    node *curr = head;
    char *prev_string = curr -> string, *tmp_string;
    for (curr = head -> next; curr; curr = curr -> next) {
        tmp_string = curr -> string;
        res = strcmp(tmp_string, prev_string);
        if (res > 0){
            ++count;
        }
        prev_string = tmp_string;
    }
    return count;
}

int task03(node *head) {
    int count = 0; 
    int res1 = 0, res3 = 0;
    node *curr = head;
    node *el1, *el2, *el3;
    el1 = curr;
    curr = head -> next;
    el2 = curr;
    if(!el2){
        return 0;
    }
    res1 = strcmp(el2 -> string, el1 -> string);
    for (curr = curr -> next; curr; curr = curr -> next){
        el3 = curr;
        res3 = strcmp(el2 -> string, el3 -> string);
        if (res1 >= 0 && res3 >= 0){
            ++count;
        }
        el1 = el2;
        el2 = el3;
        res1 = -res3;
    }
    return count;
}

int task04(node *head) {
    int count = 0, res_count = 0;
    int res1 = 0, res3 = 0;
    int check = 0;
    node *curr = head;
    node *el1, *el2, *el3;
    el1 = curr;
    curr = curr->next;
    el2 = curr;
    if(!el2){
        return 0;
    }
    res1 = strcmp(el2 -> string, el1 -> string);
    for (curr = curr -> next; curr; curr = curr -> next){
        el3 = curr;
        res3 = strcmp(el2 -> string, el3 -> string);
        if (res1 >= 0 && res3 >= 0){
            if (count > res_count){ 
                res_count = count;
            }
            count = 0;
            if(check == 0){
                check = 1;
            }
        } else if (check){
            ++count;
        }
        el1 = el2;
        el2 = el3;
        res1 = -res3;
    }
    return res_count;
}

int task05(node *head) {
    int count = 1;
    int res_count = 1;
    node *curr = head;
    node *el1, *el2;
    el1 = head;
    for (curr = curr -> next; curr; curr = curr -> next) {
        el2 = curr;
        if (strcmp(el2 -> string, el1 -> string) > 0){
            ++count;
        } else {
            if (count > res_count){
                res_count = count;
            }
            count = 1;
        }
        el1 = el2;
    }
    res_count = (res_count > count ? res_count : count);
    return res_count;
}

int task06(node *head) {
    int count = 0;
    int check = 0;
    node *curr = head;
    node *el1, *el2;
    el1 = head;
    for (curr = curr -> next; curr; curr = curr -> next) {
        el2 = curr;
        if (strcmp(el2 -> string, el1 -> string) == 0){
            if (!check) {
                check = 1;
                ++count;
            }
        }else {
            check = 0;
        }
        el1 = el2;
    }
    return count;
}

int task07(node *head){
    int count = 0, res_count = 0; 
    int check = 0;
    int res1 = 0;
    node *curr = head;
    node *el1, *el2;
    el1 = head;
    for (curr = curr->next; curr; curr = curr->next) {
        el2 = curr;
        res1 = strcmp(el1 -> string, el2 -> string);
        if (res1 == 0){
            if (count > res_count){ 
                res_count = count - 1;
            }
            if (check == 0){
                check = 1;
            }
            count = 0;
        } else if (check == 1){
            ++count;
        }
        el1 = el2;
    }
    return res_count;
}

node* task08(node *head){
    node *curr_el = head, *next_el = curr_el -> next;
    node* tmp = 0;
    node* tmp1 = 0;
    node* res = 0;
    int res1 = 0;
    for(curr_el = head; curr_el -> next; curr_el = next_el){
        tmp1 = curr_el -> next;
        res1 = strcmp(curr_el -> string, tmp1 -> string);
        if(res1 < 0){
            res = curr_el;
            tmp = curr_el;
            break;
        } else {
            next_el = curr_el -> next;
            free(curr_el -> string);
            free(curr_el);
        }
    }
    if(tmp == 0){
        return curr_el;
    }
    curr_el = curr_el -> next;
    for(next_el = curr_el -> next; curr_el -> next; curr_el = next_el){
        tmp1 = curr_el -> next;
        res1 = strcmp(curr_el -> string, tmp1 -> string);
        if(res1 < 0){
            next_el = curr_el -> next;
            tmp -> next = curr_el;
            tmp = tmp -> next;
        } else {
            next_el = curr_el -> next;
            free(curr_el -> string);
            free(curr_el);
        }
    }
    if(res != 0){
        tmp -> next = curr_el;
    }
    return res;
}

node* task09(node *head){
    node *curr;
    node* el1, *el3;
    node* tmp;
    int res1 = 0, res3 = 0;

    el1 = head;
    if((tmp = head -> next) == 0){
        return head;
    }
    res1 = strcmp(tmp -> string, el1 -> string);
    for (curr = head -> next; curr -> next; curr = el1 -> next){
        el3 = curr -> next;
        res3 = strcmp(curr -> string, el3 -> string);
        if (res1 >= 0 && res3 >= 0){
            el1 -> next = curr -> next;
            free(curr -> string);
            free(curr);
        } else {
            el1 = curr;
        }
        res1 = -res3;
    }
    return head;
}











