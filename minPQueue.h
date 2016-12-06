//
//  minPQueue.h
//  TestProject_2
//
//  Created by Xiaolong Chen on 2016/12/6.
//  Copyright © 2016年 Xiaolong Chen. All rights reserved.
//

#ifndef minPQueue_h

#define minPQueue_h
#include <stdio.h>
#include <stdlib.h>
#include "SqList.h"
#define INF 9999

/*
 -- 最小优先队列库说明: --
 这个库中优先队列的下标是从0开始的;
 -- 使用和测试范例: --
 int testsize = 6;
 Item node[testsize];
 node[0].key=10; node[0].label = 'A';
 node[1].key=20; node[1].label = 'B';
 node[2].key=100; node[2].label = 'C';
 node[3].key=40; node[3].label = 'D';
 node[4].key=50; node[4].label = 'E';
 node[5].key=60; node[5].label = 'F';
 PQueue Q;
 InitPQueueWithArray(&Q, node, testsize);  //用数组来初始化
 OutputPQueue(&Q);
 BuildMinHeap(&Q);    //建堆, 之后即可正常使用最小优先队列了;
 OutputPQueue(&Q);
 DecreaseKey(&Q, 3, 5);
 printf("%d, ", ExtractMin(&Q).key);
 printf("%d, ", ExtractMin(&Q).key);
 printf("%d, ", ExtractMin(&Q).key);
 printf("%d, ", ExtractMin(&Q).key);
 printf("%d, ", ExtractMin(&Q).key);
 printf("%d, ", ExtractMin(&Q).key);
 */

/*-- 数据结构 --*/
typedef struct {
    int key;    //建堆的依据
    char label;
}Item;
typedef struct {
    Item *elem;
    int heapsize;  //作为堆的属性
    int length;
    int capacity;
}PQueue;

/*-- 各种方法 --*/
/*InitPQueue*/
//初始化的时候,先检查是否成功(否则exit),然后要顾及到每个成员属性的初始化
Status InitPQueue(PQueue *Q) {
    Q->elem = (Item *)malloc(SIZE*sizeof(Item));
    if (!Q->elem) {printf("Init OVERFLOW\n");exit(OVERFLOW);}
    Q->heapsize = 0;
    Q->length = 0;
    Q->capacity = SIZE;
    return 0;
}

/*InsertPQueue*/
//插入的时候,先检查一下是否满了, 否则先移动插入位置到结尾的所有元素往后一位, 然后再插入;
//要顾及成员属性的变化;
Status InsertPQueue(PQueue *Q, int index, Item item) {
    if (Q->length>=Q->capacity) {
        //        printf("Reallocate memory\n");
        Q->elem = (Item *)realloc(Q->elem, (SIZE+INCREMENT)*sizeof(Item));
        if (!Q->elem) {printf("Insert OVERFLOW\n");exit(OVERFLOW);}
        Q->capacity += INCREMENT;
    }
    int i = 0;
    for (i=Q->length; i>index; i--) {  //从后往前,每个元素保存前一位的值,遍历到插入位置index+1的地方就可以了;
        //注意, 因为是插入, 所以要从arr[length] = arr[length-1]开始; 比如现在五个数arr[0]~arr[4],得先arr[5]=arr[4];
        Q->elem[i] = Q->elem[i-1];
    }
    Q->elem[i] = item;
    Q->length++;
    return 0;
}

/*InitPQueueWithArray*/
Status InitPQueueWithArray(PQueue *Q, Item arr[], int length) {
    InitPQueue(Q);
    int i;
    for (i=0; i<length; i++) {
        InsertPQueue(Q, Q->length, arr[i]);
    }
    return OK;
}

/*OutputPQueue*/
//遍历一下
Status OutputPQueue(PQueue *Q) {
    int i=0;
    for (i=0; i<Q->length; i++) {
        printf("%c-%d, ", Q->elem[i].label, Q->elem[i].key);
    }
    printf("\n");
    return OK;
}

/*Left*/
int Left(int i) {
    return i*2+1;
}
/*Right*/
int Right(int i) {
    return i*2+2;
}
/*Parent*/
int Parent(int i) {
    return (i-1)/2;
}
/*Exchange*/
int Exchange(PQueue *Q, int a, int b) {
    Item temp;
    temp.key = Q->elem[a].key; temp.label = Q->elem[a].label;
    Q->elem[a].key = Q->elem[b].key; Q->elem[a].label = Q->elem[b].label;
    Q->elem[b].key = temp.key; Q->elem[b].label = temp.key;
    return OK;
}

/*MinHeapify*/
Status MinHeapify(PQueue *Q, int i) {
    int l = Left(i);
    int r = Right(i);
    int tiniest = i;
    if (l<Q->heapsize && Q->elem[l].key<Q->elem[tiniest].key) {
        tiniest = l;
    }
    if (r<Q->heapsize && Q->elem[r].key<Q->elem[tiniest].key) {
        tiniest = r;
    }
    
    if (tiniest!=i) {
        Exchange(Q, i, tiniest);
        //recursive
        MinHeapify(Q, tiniest);
    }
    return OK;
}

/*BuildHeap*/
Status BuildMinHeap(PQueue *Q) {
    Q->heapsize = Q->length;
    int i;
    for (i=Parent(Q->heapsize-1);i>=0;i--) {
        MinHeapify(Q, i);
    }
    return OK;
}

/*ExtractMin*/
Item ExtractMin(PQueue *Q) {
    Item minItem = Q->elem[0];
    Q->elem[0] = Q->elem[Q->heapsize-1];
    Q->heapsize--;
    MinHeapify(Q, 0);
    return minItem;
}
/*HeapMinimun*/
Item HeapMinimum(PQueue *Q) {
    return Q->elem[0];
}
/*DecreaseKey*/
Status DecreaseKey(PQueue *Q, int i,int newKey) {
    if (newKey >= Q->elem[i].key) {
        printf("The newKey is not less than the oldKey!");
        ERROR;
    } else {
        Q->elem[i].key = newKey;
        while (i>0 && (Q->elem[i].key < Q->elem[Parent(i)].key)) {
            Exchange(Q, i, Parent(i));
            i = Parent(i);
        }
    }
    return OK;
}
/*MinHeapInsert*/
Status MinHeapInsert(PQueue *Q, Item item) {
    int newKey = item.key;
    item.key = INF;
    //A.heapsize++, A[heapsize] = INF;
    Q->heapsize++;
    InsertPQueue(Q, (Q->heapsize)-1, item);
    //DecraseKey(A, heapsize, keyvalue)
    DecreaseKey(Q, (Q->heapsize)-1, newKey);
    return OK;
}

/*-- END --*/

#endif /* minPQueue_h */
