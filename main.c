#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 30 
// go[0] = 右邊, go[1] = 左邊, go[2] = 下邊, go[3] = 上邊 

typedef struct Node *map_ptr;				//地圖結構 
typedef struct Node
{
	int view;
	map_ptr go[4];
	int set;
	int number;
	int cost;
};

typedef struct Node2 *tree_ptr;				//HEAP結構 
typedef struct Node2
{
	int data;
	tree_ptr parent, leftChild, rightChild;
	int min;
};

int row, column;
int path[900];
map_ptr map[SIZE][SIZE];
tree_ptr tree[900];
int treeEnd = 1;

void pb1();
void pb2();
void initial();
void printMap();
void produceMaze();
void walkMaze();
void findRoad(map_ptr, int);

void initialTree();
int maintainTree(tree_ptr);
void swapTree(tree_ptr, tree_ptr);
void insertTree(int);
void printTree(tree_ptr);
void deleteMin();
void deleteMax();
void deleteMaintain(tree_ptr);

int main(int argc, char *argv[]) 
{
	pb1();
	pb2();
	
	return 0;
}

void pb1 ()
{
	printf("row = ");						//輸入大小 
	scanf("%d", &row);
	printf("column = ");
	scanf("%d", &column);
	
	initial();
	produceMaze();
	printf("\n");
	printMap();
	printf("\npath : ");
	walkMaze();
	int i;
	for (i = 0; i <= map[row-1][column-1]->cost; i++)
		printf("%d ", path[i]);
	printf("\n\n");

	return;
}

void pb2 ()
{
	initialTree();
	int i;
	for(i = 0; i < map[row-1][column-1]->cost + 1; i++)
		insertTree(path[i]);
	printf("heap\n");
	printTree(tree[1]);
	printf("\ndeleteMin\n");
	deleteMin();
	printf("deleteMAX\n");
	deleteMax();
	
	int input;
	int insert;
	printf("1:insert   2:deleteMin   3:deleteMax   4:exit\ninput:");
	while(scanf("%d", &input) != EOF)
	{
		switch (input)
		{
			case 1:
				printf("insert :");
				scanf("%d", &insert);
				insertTree(insert);
				printTree(tree[1]);
				printf("1:insert   2:deleteMin   3:deleteMax   4:exit\ninput:");
				break;
			
			case 2:
				deleteMin();
				printf("1:insert   2:deleteMin   3:deleteMax   4:exit\ninput:");
				break;
			case 3:
				deleteMax();
				printf("1:insert   2:deleteMin   3:deleteMax   4:exit\ninput:");
				break;
			case 4:
				return;
			
			default:
				printf("input wrond input again\n");
				printf("1:insert   2:deleteMin   3:deleteMax   4:exit\ninput:");
				break;
		}
	}
	
} 

void initial ()						//MAP的初始化 
{
	int i, j, k;
	for(i = 0; i < row; i++)
		for(j = 0; j < column; j++)
		{
			map[i][j] = malloc(sizeof(*map[i][j]));
			map[i][j]->view = 0;
			for(k = 0; k < 4; k ++)
				map[i][j]->go[k] = NULL;
			map[i][j]->set = i*100+j;
			map[i][j]->cost = 999;
		}
	map[0][0]->cost = 0;
}

void printMap () 				//列印出地圖 
{
	int i, j, k;
	int count = 1;
	
	printf(" ");
	for(j = 0; j < column; j++)
	{
		printf(" @ ");
		printf("---");
	}
	printf(" @");
	printf("\n");
		
	for(i = 0; i < row; i++)
	{		
		printf("  | ");
		for(j = 0; j < column; j++)
		{
			if(map[i][j]->view == 0)
				printf(" # ");
			else	
			{
				if (count < 10)
				{
					map[i][j]->number = count;				//count是編號 
					printf(" %d ", map[i][j]->number);
				}
				else if (count < 100)
				{
					printf(" %d", count);
					map[i][j]->number = count;
				}
				else
				{
					printf("%d", count);
					map[i][j]->number = count;
				}
				count++;
			}
			if(map[i][j]->go[0] == NULL)
				printf(" | ");
			else
				printf("   ");
		}
		
		printf("\n");
		
		printf(" ");
		for(j = 0; j < column; j++)
		{
			printf(" @ ");
			if (map[i][j]->go[2] == NULL)
				printf("---");
			else
				printf("   ");
		}
		printf(" @");
		printf("\n");

	}
}

void produceMaze ()					//敲牆壁 
{
	srand(time(NULL));
	
	int i, j, k;
	int ri, rj, rk;
	int temp = 0;
	int con = 0;
	
	map[0][0]->view = 1;
	map[row - 1][column - 1]->view = 1;
	
	while(map[0][0]->set != map[row-1][column-1]->set)
	{
		ri = rand()%row;
		rj = rand()%column;
		rk = rand()%4;
		int min;
		
		if(rk == 0)							//只需要做兩個方向就好了 
		{
			if(rj == column-1)
				continue;
			
			int temp;
			map[ri][rj]->go[0] = map[ri][rj + 1];
			map[ri][rj + 1]->go[1] = map[ri][rj];
			map[ri][rj]->view = 1;
			map[ri][rj + 1]->view = 1;
			
			temp = map[ri][rj + 1]->set;					//更改逼號 
			
			for(i = 0; i < row; i++)
			{
				for(j = 0; j < column; j++)
				{
					if (map[i][j]->set == temp)
						map[i][j]->set = map[ri][rj]->set;
				}
			}
		}
		
		else if(rk == 2)
		{
			if(ri == row-1)
				continue;
			
			int temp;
			map[ri][rj]->go[2] = map[ri + 1][rj];
			map[ri + 1][rj]->go[3] = map[ri][rj];
			map[ri][rj]->view = 1;
			map[ri + 1][rj]->view = 1;
			
			temp = map[ri + 1][rj]->set;
			
			for(i = 0; i < row; i++)
				for(j = 0; j < column; j++)
					if (map[i][j]->set == temp)
						map[i][j]->set = map[ri][rj]->set;
		}
	}
	return;
}
		
void walkMaze ()							//走迷宮 利用HW1 
{
	map_ptr queue[100000];
	int front = 0, rear = 1;
	int k;
	queue[front] = map[0][0];
	
	while (queue[front])
	{
		for(k = 0; k < 4; k++)
			if(queue[front]->go[k])
				if( queue[front]->go[k]->cost > queue[front]->cost + 1)
				{
					queue[front]->go[k]->cost = queue[front]->cost + 1;
					queue[rear++] = queue[front]->go[k];
				}
		front++;
	}

	findRoad(map[row-1][column-1], map[row-1][column-1]->cost);

	return;
}

void findRoad (map_ptr ptr, int place)				//找最短路  一樣HW1 
{
	if(ptr)
	{
		if(ptr == map[0][0])
		{
			path[0] = ptr->number;
			return;
		}
		
		int k;
		for(k = 0; k < 4; k++)
		{
			if(ptr->go[k])
				if(ptr->cost -1 == ptr->go[k]->cost)
				{
					path[place--] = ptr->number;
					return findRoad (ptr->go[k], place);
				}
		}
	}
}

void initialTree ()					//HEAP初始化 
{
	int i;
	for(i = 0; i < 900; i++)
	{
		tree[i] = malloc(sizeof(*tree[i]));
		tree[i]->data = NULL;
	}
		
	tree[1]->min = 1;
	
	for(i = 1; i < 900; i++)
	{
		tree[i]->parent = tree[i/2];
		if(i*2 < 900)
			tree[i]->rightChild = tree[i*2 + 1];
		if(i*2 + 1 < 900)
			tree[i]->leftChild = tree[i*2];
	}
	tree[1]->parent = NULL;
			
	for(i = 2; i < 900; i++)
	{
		if(tree[i]->parent->min == 1)
			tree[i]->min = 0;
		else
			tree[i]->min = 1;
	}
	
	treeEnd = 1;
	
}

int maintainTree (tree_ptr ptr)				//維護HEAP 
{

	if (ptr)
	{
		if (ptr->min == 1)
		{
			if(ptr->parent)
			{
				if (ptr->data > ptr->parent->data)
				{
				//	printf("1\n");
					swapTree(ptr, ptr->parent);
					return maintainTree(ptr->parent);
				}
			
				if(ptr->parent->parent)
					if(ptr->data < ptr->parent->parent->data)
					{
					//	printf("2\n");
						swapTree(ptr, ptr->parent->parent);	
						return maintainTree(ptr->parent->parent);
					}
			}
		}
		
		else
		{
			if (ptr->data < ptr->parent->data)
			{
				//printf("3\n");
				swapTree(ptr, ptr->parent);
				return maintainTree(ptr->parent);
			}
			
			if(ptr->parent->parent)
			{
				if(ptr->data > ptr->parent->parent->data)
				{
					//printf("4\n");
					swapTree(ptr, ptr->parent->parent);
					return maintainTree(ptr->parent->parent);
				}
			}
		}
	}
}

void swapTree (tree_ptr ptr1, tree_ptr ptr2)		//交互樹禮的職值 
{
	int temp;
	temp = ptr1->data;
	ptr1->data = ptr2->data;
	ptr2->data = temp;

	return;
}

void insertTree (int num)						//新增節點 
{	
	tree[treeEnd]->data = num;	
	maintainTree(tree[treeEnd]);
	treeEnd++;
}

void printTree (tree_ptr ptr)			//列印HEAP 
{
	tree_ptr queue[600];
	int front = 0, rear = 1;
	queue[0] = ptr;
	int temp = 0;
	int i;
	
	
	printf("(%d)", queue[front]->data);
	
	while(queue[front]->data != NULL)
	{	
		if(queue[front]->min != temp)
		{
			
			printf("\n");
			temp = queue[front]->min;
			/*for(i = 0; i < space; i++)
			{
				printf(" ");
				space--;
			}*/ 
		}
		
	
		if(queue[front]->leftChild)
		{
			if(queue[front]->leftChild->data == 0)
			{
				printf("\n");
				return;
			}
			
			printf("(%d", queue[front]->leftChild->data);
			queue[rear++] = queue[front]->leftChild;
				
			if(queue[front]->rightChild)
			{
				if(queue[front]->rightChild->data == 0)
				{
					printf(")\n");
					return;
				}
				printf(",%d)", queue[front]->rightChild->data);
				queue[rear++] = queue[front]->rightChild;
			}
		}
		front++;
	}
}

void deleteMaintain (tree_ptr ptr)							//刪除後要進行維護 
{
	tree_ptr temp1, temp2, temp3, temp4, min, max;
	if(ptr)
	{
		if(ptr->min == 1)
		{
			if(ptr->leftChild->data != 0)
			{
				if(ptr->leftChild->data == 0)
					return;
				if(ptr->rightChild->data != 0)
				{
					temp1 = ptr->leftChild;
					temp2 = ptr->rightChild;
					
					if(temp1->data<temp2->data)
						min = temp1;
					else
						min = temp2;
					
					if(min->data < ptr->data)
					{
						swapTree(min, ptr);
						return deleteMaintain(ptr);
					}
					else
					{
						temp1 = NULL;
						temp2 = NULL;
						temp3 = NULL;
						temp4 = NULL;
						if(ptr->leftChild->leftChild && ptr->leftChild->leftChild->data != 0)
							temp1 = ptr->leftChild->leftChild;
						else
							return;
						if(ptr->leftChild->rightChild && ptr->leftChild->rightChild->data != 0)
							temp2 = ptr->leftChild->rightChild;
						if(ptr->rightChild->leftChild && ptr->rightChild->leftChild->data != 0)
							temp3 = ptr->rightChild->leftChild;
						if(ptr->rightChild->rightChild && ptr->rightChild->rightChild->data != 0)
							temp4 = ptr->rightChild->rightChild;
						
						if(temp1)
						{
							min = temp1;
							if(temp2)
							{
								if(temp2->data < min->data)
									min = temp2;
									
								if (temp3)
								{
									if (temp3->data < min->data)
										min = temp3;
											
									if(temp4)		
										if(temp4->data < min->data)
											min = temp4;		
								}
							}
						}
						else
							return;
						
						if(ptr->data > min->data)
						{
							swapTree(ptr, min);
							return deleteMaintain(min);
						}
					}
				}
				else
				{
					if(ptr->leftChild->data < ptr->data)
					{
						swapTree(ptr, ptr->leftChild);
						return;
					}
				}
			}	
			else
				return;
		}
		else
		{
			if(ptr->leftChild->data != 0)
			{
				if(ptr->leftChild->data == 0)
					return;
				if(ptr->rightChild->data != 0)
				{
					temp1 = ptr->leftChild;
					temp2 = ptr->rightChild;
					
					if(temp1->data < temp2->data)
						min = temp1;
					else
						min = temp2;
					
					if(min->data > ptr->data)
					{
						swapTree(min, ptr);
						return deleteMaintain(ptr);
					}
					else
					{
						
						if(ptr->leftChild->leftChild)
							temp1 = ptr->leftChild->leftChild;
						if(ptr->leftChild->rightChild)
							temp2 =ptr->leftChild->rightChild;
						if(ptr->rightChild->leftChild)
							temp3 = ptr->rightChild->leftChild;
						if(ptr->rightChild->rightChild)
							temp4 = ptr->rightChild->rightChild;
							
						if(temp1->data < temp2->data)
							temp1 = temp2;
						if(temp3->data < temp4->data)
							temp3 = temp4;
						
						if(temp1->data > temp3->data)
							max = temp1;
						else
							max = temp3;
						
						if (max->data > ptr->data)
						{
							swapTree(max, ptr);
							return deleteMaintain(max);
						}
						
					}
				}
				else
					if(ptr->leftChild->data > ptr->data)
					{
						swapTree(ptr->leftChild, ptr);
						return;
					}
			}
			else
				return;
		}
	}
}

void deleteMin ()					//刪除最小 
{
	if(treeEnd == 2)
	{
		printf("can't not delete more\n");
		printTree(tree[1]);
		return;
	}
	tree[1]->data = tree[treeEnd-1]->data;
	tree[treeEnd-1]->data = NULL;
	treeEnd--;
	deleteMaintain(tree[1]);
	printTree(tree[1]);
	printf("\n");
}
	
void deleteMax ()				//刪除最大 
{
	if(treeEnd == 2)
	{
		printf("can't not delete more\n");
		printTree(tree[1]);
		return;
	}
	if(tree[2]->data > tree[3]->data)
	{
		tree[2]->data = tree[treeEnd-1]->data;
		tree[treeEnd-1]->data = NULL;
		treeEnd--;
		deleteMaintain(tree[2]);
	}
	else
	{
		tree[3]->data = tree[treeEnd-1]->data;
		tree[treeEnd-1]->data = NULL;
		treeEnd--;
		deleteMaintain(tree[3]);
	}
	printTree(tree[1]);
	printf("\n");
}

