#include <bits/stdc++.h>
#include <iostream>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <fstream>
#include <dos.h>
#include <conio.h>
#include <limits.h>
#include <stdlib.h>
#define delay Sleep

#define LINE 219
#define WALL '#'
#define DOOR 254

#define MIN_SIZE 5

using namespace std;

class Player
{
public:
    int x;
    int y;
    int lifes;
    int ammunition;
    int resultpawnTime;
    char movingDirection;   /// As in 'w' = UP; 's' = DOWN; 'a' = LEFT; 'd' = RIGHT
    bool lostLife;
    int checkPoint;
};

struct coor{
	int x;
	int y;
}player2;

struct portal{
	int roomIndex;
	int x,y;
};

int playerx,playery;

struct room{
	int x;
	int y;
	
	int splitH;
	int portalAvailable;
	
	int roomx;
	int roomy;
	
	int portalX,portalY; //lokasi portal;
	portal portal_dst;	//tujuan portal
	
	int isThereKey;
	
	int roomHeight;
	int roomWidth;
	
	int height;
	int width;
	
	struct room *parent,*left, *right;


};


void gotoxy(int x, int y)
{
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

struct dungeon{
	int x;
	int y;
	
	int width,height;
	
	char tile[100][100];

	struct room *root;
	vector<struct room*> rooms;
	
	void init(){
//		printf("Initialized!\n");
		x = 0;
		y = 0;
		width = 100;
		height = 40;
		
		for(int i = 0;i < 40;i++){
			for(int j = 0;j < 100;j++){
//				if(i == 0 || j == 0 || i == 39 || j == 99){
//					tile[i][j] = LINE;continue;
//				}
				tile[i][j] = ' ';
			}
		}
	}
	
	void print(){
		for(int i = 0;i <  height;i++){
			for(int j = 0;j < width;j++){
				printf("%c",tile[i][j]);
			}
			puts("");
		}
	}
	
	void clean(){
		for(int i = 0;i < height;i++){
			for(int j = 0;j < width;j++){
				if(tile[i][j] == LINE) tile[i][j] = ' '; 
			}
		}
	}
	
	void makePortal(int idx1,int idx2){
		do{
			int portalx1 = rooms[idx1]->roomx + (rand()%(rooms[idx1]->roomWidth));
			
			int portaly1 = rooms[idx1]->roomy + (rand()%(rooms[idx1]->roomHeight));
		
			
			int portalx2 = rooms[idx2]->roomx + (rand()%(rooms[idx2]->roomWidth));
			
			int portaly2 = rooms[idx2]->roomy + (rand()%(rooms[idx2]->roomHeight));
			
			if(portalx1 > rooms[idx1]->roomx + rooms[idx1]->roomWidth){
				portalx1 = rooms[idx1]->roomx + rooms[idx1]->roomWidth - 2;
			}
			if(portaly1 > rooms[idx1]->roomy + rooms[idx1]->roomHeight){
				portaly1 = rooms[idx1]->roomy + rooms[idx1]->roomHeight -2;
			}
			if(portalx2 > rooms[idx2]->roomx + rooms[idx2]->roomWidth){
				portalx2 = rooms[idx2]->roomx + rooms[idx2]->roomWidth-2;
			}
			if(portaly2 > rooms[idx2]->roomy + rooms[idx2]->roomHeight){
				portaly2 = rooms[idx2]->roomy + rooms[idx2]->roomHeight-2;
			}
			
			printf("value initialized\n");
			
			rooms[idx2]->portalX = portalx2;
			rooms[idx2]->portalY = portaly2;
			
			rooms[idx2]->portalAvailable = 1;
			
			//passing arah portal dst & src
			
			portal temp_portal;
			temp_portal.x = portalx1;
			temp_portal.y = portaly1;
			
			rooms[idx2]->portal_dst = temp_portal;
			
			temp_portal.x = portalx2;
			temp_portal.y = portaly2;
			
			rooms[idx1]->portal_dst = temp_portal;
			
		}while(tile[rooms[idx1]->portalY][rooms[idx1]->portalX] != ' ' || tile[rooms[idx2]->portalY][rooms[idx2]->portalX] != ' ');
			tile[rooms[idx1]->portalY][rooms[idx1]->portalX] = '@';
			tile[rooms[idx2]->portalY][rooms[idx2]->portalX] = '@';
	}
	
	void makePuzzle(){
		
		//spanning tree puzzles
		
		int connecting = 1;
		
		int parent[8];
		
		for(int i = 0;i < 8;i++){
			parent[i] = i;
		}
		
		printf("initialized2 !\n");
		
		if(connecting == 1);
		
		int root = rand()%8;
		int now;
		
		do{
			now = rand()%8;
		}while(parent[now] == root);
		
		parent[now] = root;
		
		printf("Root Selected!\n");
		
		makePortal(root,now);
		
		print();
		
		int idx;
		
		for(int i = 0;i < 6;i++){
			printf("infinity\n");
			do{
				idx= rand()%8;
			}while(parent[idx] == root);
			parent[idx] = root;
			makePortal(root,now);
		}
		printf("portal making done!\n");
	}
	
	void connectRoom(){
//		print();
//		printf("Connecting Level 1...\n");
//		getchar();fflush(stdin);
		for(int i = 0;i < rooms.size();i+=2){
			if(rooms[i]->splitH){
//				printf("case 1");
				int startx;
				int starty;
				
				int idxRange1 = rooms[i]->roomx > rooms[i+1]->roomx ? i : i+1;
				int idxRange2 = rooms[i]->roomx + rooms[i]->roomWidth < rooms[i+1]->roomx + rooms[i+1]->roomWidth ? i : i+1;
				
				startx = ((rand() % (rooms[idxRange2]->roomWidth - rooms[idxRange1]->roomx)) + rooms[idxRange1]->roomx) + 3;
				starty = rooms[i]->roomy + rooms[i]->roomHeight;
				
				if(startx <= rooms[idxRange1]->roomx){
					startx = rooms[idxRange1]->roomx + 3;
				}
				
				if(startx >= rooms[idxRange2]->roomx + rooms[idxRange2]->roomWidth){
					startx = rooms[idxRange2]->roomx + rooms[idxRange2]->roomWidth - 3;
				}
				
				int len = rooms[i+1]->roomy - starty + 2;
				
				for(int j = 0;j < len;j++){
					tile[starty+j][startx] = ' ';
					tile[starty+j][startx+1] = WALL;
					tile[starty+j][startx-1] = WALL; 
					if(j == 0 || j == len-1){
						tile[starty+j][startx] = 254;	
					}
				}
			}else if(!rooms[i]->splitH){
				int startx;
				int starty;
				
				int idxRange1 = rooms[i]->roomy < rooms[i+1]->roomy ? i+1 : i;
				int idxRange2 = rooms[i]->roomy + rooms[i]->roomHeight < rooms[i+1]->roomy + rooms[i+1]->roomHeight ? i : i+1;
				
				startx = rooms[i]->roomx + rooms[i]->roomWidth - 1;
				starty = ((rand() % (rooms[idxRange2]->roomHeight - rooms[idxRange1]->roomy)) + rooms[idxRange1]->roomy) + 3;
				
				if(starty <= rooms[idxRange1]->roomy){
					starty = rooms[idxRange1]->roomy + 1;
				}
				
				if(starty >= rooms[idxRange2]->roomy + rooms[idxRange2]->roomHeight){
					starty = rooms[idxRange2]->roomy + rooms[idxRange2]->roomHeight - 1;
				}
				
				
				int len = rooms[i+1]->roomx - startx+1;
				
				for(int j = 0;j < len;j++){
					tile[starty][startx+j+1] = ' ';
					tile[starty-1][startx+j+1] = WALL;
					tile[starty+1][startx+j+1] = WALL;
					if(j == 0 || j == len-1){
						tile[starty][startx+j+1] = 219;	
					}
				}
			}
		}
//		printf("Level 1 Connected!\n");
//		print();
//		getchar();
		
//		printf("Connecting Level 2...\n");
		
//		print();
		
		for(int i = 0;i < rooms.size()/2-1;i++){
			if(rooms[i*2]->parent->splitH){
//				printf("infinity\n");
				
				int idx1 = i*2;
				int cur = rooms[idx1]->roomy + rooms[idx1]->roomHeight;
				for(int j = i*2;j < i*2+2;j++){
					if(rooms[j]->roomy + rooms[j]->roomHeight > cur){
						cur = rooms[j]->roomy + rooms[j]->roomHeight;
						idx1 = j;
					}
				}
				
				int idx2 = (i+1)*2;
				cur = rooms[idx2]->roomy;
				for(int j = (i+1)*2;j < (i+1)*2+2;j++){
					if(rooms[j]->roomy < cur){
						cur = rooms[j]->roomy;
						idx2 = j;
					}
				}
				
//				printf("lewat...\n");
				
				if(rooms[idx1]->roomx + rooms[idx1]->roomWidth < rooms[idx2]->roomx || 
				rooms[idx2]->roomx + rooms[idx2]->roomWidth < rooms[idx1]->roomx){
					idx2 = idx2 == (i+1)*2 ? (i+1)*2+1 : (i+1)*2;
				}
				
//				printf("lewat!");
				
				int min = rooms[idx1]->roomx > rooms[idx2]->roomx ? rooms[idx1]->roomx + 2: rooms[idx2]->roomx + 2;
				int max = rooms[idx1]->roomx + rooms[idx1]->roomWidth < rooms[idx2]->roomx + rooms[idx2]->roomWidth ?
							rooms[idx1]->roomx + rooms[idx1]->roomWidth - 3 : rooms[idx2]->roomx + rooms[idx2]->roomWidth - 3;
				
				
				int starty = rooms[idx1]->roomy + rooms[idx1]->roomHeight;
				int hall = (rand()%(max - min) + min);
				
				int len = rooms[idx2]->roomy - (rooms[idx1]->roomy + rooms[idx1]->roomHeight);
				
				if(hall < min){
					hall = min;
				}
				
				
				if(hall > max){
					hall = max-2;
				}
				
				
				
//				tile[rooms[idx1]->roomy][rooms[idx1]->roomx] = '1' + i;
				
				for(int j = 0;j < len+2;j++){
//					system("cls");
//					print();
//					getchar();
//					tile[starty+j][hall] = ' ';
					if(j == 0 || j == len+1){
						tile[starty+j][hall] = 254;
					}
					tile[starty+j][hall-1] = WALL;
					tile[starty+j][hall+1] = WALL;
				}
			}else if(!rooms[i*2]->parent->splitH){
				
				int idx1 = i*2;
				int cur = rooms[idx1]->roomx + rooms[idx1]->roomWidth;
				for(int j = i*2;j < i*2+2;j++){
					if(rooms[j]->roomx + rooms[j]->roomWidth > cur){
						cur = rooms[j]->roomx + rooms[j]->roomWidth;
						idx1 = j;
					}
				}
				
				int idx2 = (i+1)*2;
				cur = rooms[idx2]->roomx;
				for(int j = (i+1)*2;j < (i+1)*2+2;j++){
					if(rooms[j]->roomx < cur){
						cur = rooms[j]->roomx;
						idx2 = j;
					}
				}
				
				if(rooms[idx1]->roomy + rooms[idx1]->roomHeight  < rooms[idx2]->roomy || 
					rooms[idx2]->roomy + rooms[idx2]->roomHeight < rooms[idx1]->roomy){
					idx2 = idx2 == (i+1)*2 ? (i+1)*2+1 : (i+1)*2;
				}
				
				int min = rooms[idx1]->roomy > rooms[idx2]->roomy ? rooms[idx1]->roomy + 2 : rooms[idx2]->roomy + 2;
				int max = rooms[idx1]->roomy + rooms[idx1]->roomHeight < rooms[idx2]->roomy + rooms[idx2]->roomHeight ?
							rooms[idx1]->roomy + rooms[idx1]->roomHeight - 3 : rooms[idx2]->roomy + rooms[idx2]->roomHeight - 3;
				
				
				int startx = rooms[idx1]->roomx + rooms[idx1]->roomWidth;
				int hall = (rand()%(max - min) + min);
				
				int len = rooms[idx2]->roomx - (rooms[idx1]->roomx + rooms[idx1]->roomWidth);
				
				if(hall < min){
					hall = min;
				}
				
				if(hall > max){
					hall =max-2;
				}
				
				
//				tile[rooms[idx1]->roomy][rooms[idx1]->roomx] = '1' + i;
				
				for(int j = 0;j < len+2;j++){
					tile[hall][startx+j] = ' ';
//					system("cls");
//					print();
//					getchar();
					if(j==0 || j == len+1){
						tile[hall][startx+j] = 219;
//						printf("buat pintu!");
//						print();
//						getchar();
					}
					tile[hall-1][startx+j] = WALL;
					tile[hall+1][startx+j] = WALL;
				}
			}
		}
		
//		printf("Level 2 Connected!?\n");
		
		
//		printf("Connecting level 3...\n");
		
		
		//random nomor room dari dua child
		int idx1 = rand()%4;
		int idx2 = rand()%4+4;
		//
		
		int portalx1 = rooms[idx1]->roomx + (rand()%rooms[idx1]->roomWidth) + 2;
		int portaly1 = rooms[idx1]->roomy + (rand()%rooms[idx1]->roomHeight) + 2;
		
		rooms[idx1]->portalX = portalx1;
		rooms[idx1]->portalY = portaly1;
		
		rooms[idx1]->portalAvailable = 1;
		
		int portalx2 = rooms[idx2]->roomx + (rand()%rooms[idx2]->roomWidth) + 2;
		int portaly2 = rooms[idx2]->roomy + (rand()%rooms[idx2]->roomHeight) + 2;
		
		if(portalx1 > rooms[idx1]->roomx + rooms[idx1]->roomWidth){
			portalx1 = rooms[idx1]->roomx + rooms[idx1]->roomWidth - 2;
		}
		if(portaly1 > rooms[idx1]->roomy + rooms[idx1]->roomHeight){
			portaly1 = rooms[idx1]->roomy + rooms[idx1]->roomHeight -2;
		}
		if(portalx2 > rooms[idx2]->roomx + rooms[idx2]->roomWidth){
			portalx2 = rooms[idx2]->roomx + rooms[idx2]->roomWidth-2;
		}
		if(portaly2 > rooms[idx2]->roomy + rooms[idx2]->roomHeight){
			portaly2 = rooms[idx1]->roomy + rooms[idx1]->roomHeight-2;
		}
		
		rooms[idx2]->portalX = portalx2;
		rooms[idx2]->portalY = portaly2;
		
		rooms[idx2]->portalAvailable = 1;
		
		//passing arah portal dst & src
		
		portal temp_portal;
		temp_portal.x = portalx1;
		temp_portal.y = portaly1;
		
		rooms[idx2]->portal_dst = temp_portal;
		
		temp_portal.x = portalx2;
		temp_portal.y = portaly2;
		
		rooms[idx1]->portal_dst = temp_portal;
		
		tile[rooms[idx1]->portalY][rooms[idx1]->portalX] = '@';
		
		tile[rooms[idx2]->portalY][rooms[idx2]->portalX] = '@';
		
		
//		int portalx1 = rand()%(rooms[idx1]->roomWidth-4) + 4;
//		int portaly1 = rand()%(rooms[idx1]->roomHeight-4) + 4;
//		
//		int portalx2 = rand()%(rooms[idx2]->roomWidth-4) + 4;
//		int portaly2 = rand()%(rooms[idx2]->roomHeight-4) + 4;
//		
//		rooms[idx1]->portalX = rooms[idx1]->roomx + portalx1;
//		rooms[idx1]->portalY = rooms[idx1]->roomy + portaly1;
//		
//		rooms[idx1]->portalAvailable = 1;
//		
//		portal portal;
//		
//		tile[rooms[idx1]->roomy + portaly1][rooms[idx1]->roomx + portalx1] = '@';
//		
//		rooms[idx2]->portalX = rooms[idx2]->roomx + portalx2;
//		rooms[idx2]->portalY = rooms[idx2]->roomy + portaly2;
//		
//		rooms[idx2]->portalAvailable = 1;
//		
//		portal.dst.x = rooms[idx1]->portalX;
//		portal.dst.y = rooms[idx1]->portalY;
//		portal.y = rooms[idx2]->roomy + portaly2;
//		portal.x = rooms[idx2]->roomx + portalx2;
//		rooms[idx2]->portal_dst = portal;
//		
//		
//		portal.dst.x = rooms[idx2]->portalX;
//		portal.dst.y = rooms[idx2]->portalY;
//		portal.x = rooms[idx1]->roomx + portalx1;
//		portal.y = rooms[idx1]->roomy + portaly1;
//		rooms[idx1]->portal_dst = portal;
//		
		
		
//		tile[rooms[idx2]->roomy + portaly2][rooms[idx2]->roomx + portalx2] = '@';
	}
	
	void generateDungeon(int stage){
		init();
		
//		printf("generating!\n");
		
		struct room *curr = (struct room*) malloc (sizeof(struct room));
		
		curr->x = x;
		curr->y = y;
		curr->height = height;
		curr->width = width;
		
		root = curr;
		
//		printf("generating!\n");
		bsp(0,root); //DONE
		clean();
		if(stage == 1){
			connectRoom();
		}else if(stage == 2){
			makePuzzle();
		}
//		print();
//		printf("Vector Size = %d\n",rooms.size());
	}
	
	void fill(int horizontal, int point,struct room *node){
		if(horizontal){
			for(int i = 0;i < node->width;i++){
				tile[point][node->x+i] = LINE;
			}
		}else{
			for(int i = 0;i < node->height;i++){
				tile[node->y+i][point] = LINE;
			}
		}
//		print();
	}
	
	void buildRoom(struct room *node){
		for(int i = 0;i < node->roomHeight;i++){
			for(int j = 0;j < node->roomWidth;j++){
				if(i == 0 || j == 0 || i == node->roomHeight - 1|| j == node->roomWidth -1){
					tile[node->roomy+i+1][node->roomx+1+j] = WALL; continue;
				}
				tile[node->roomy+i+1][node->roomx+1+j] = ' ';
			}
		}
	}
	
	void makeRoom(struct room *node){
//		print();
		int minHeight = 0.8*node->height;
		int minWidth = 0.7*node->width;
		
		int maxHeight = 0.9*node->height;
		int maxWidth = 0.9*node->width;
		
//		printf("\n=====================================\nMasuk make room\nMinimal Height = %d\nMinimal Width = %d\n",minHeight,minWidth);
		
		int sizeY = (rand()%(maxHeight - minHeight) + minHeight);
		int sizeX = (rand()%(maxWidth - minWidth) + minWidth);
		
		if(sizeY < 5) sizeY = 5;
		if(sizeX < 5) sizeX = 5;
		
//		printf("Size room x = %d\nSize room y = %d\n",sizeX,sizeY);
		
		int posY = node->y + (rand()%(node->height - sizeY - 1));
		int posX = node->x + (rand()%(node->width - sizeX - 1));
		
//		printf("Start Position :\npos X = %d\npos Y = %d\n================================\n",posX,posY);
		
//		node->roomAvailable = 1;
		node->roomHeight = sizeY;
		node->roomWidth = sizeX;
		node->roomx = posX;
		node->roomy = posY;
		
//		node->printStatus();
		
		buildRoom(node);
//		print(); // print per Room
		
		rooms.push_back(node);
	}
	
	void bsp(int level, struct room *node){
		
		if(level == 3){
			return;
		}
		
//		printf("Current Level : %d\n",level);
		
//		print();
		
//		printf("determining split direction...\n");
		
		int splitH = rand()%2;
		
//		printf("getting width and height ratio...\n");
		
		int h = node->height;
		int w = node->width;
		
//		printf("height extracted!\nHeight = %d\n",h);
//		printf("width extracted!\nWidth = %d\n",w);
		
		float rHeight = (float)h/height;
		float rWidth = (float)w/width;
		
//		printf("check current ratio \nheight = %.2f \nwidth = %.2f\n",rHeight,rWidth);
		
		if(rHeight > rWidth && rHeight/rWidth > 1.10){
			splitH = 1;
		}else if(rWidth > rHeight && rWidth/rHeight > 1.10){
			splitH = 0;
		}
		
		if(splitH){
			int minSplit = 0.45*node->height;
			int maxSplit = 0.55*node->height;
			
			int splitP = node->y+(rand() %(maxSplit - minSplit)) + minSplit;
			
//			printf("min : %d\nmax : %d\nsplit horizontal %d !\n",minSplit,maxSplit,splitP);
			
//			fill(splitH,splitP,node);
			
			struct room *left = (struct room*) malloc (sizeof(struct room));
			struct room *right = (struct room*) malloc (sizeof(struct room));
			//TODO : cek rumus
			left->height = splitP - node->y;
			left->width = node->width;
			left->x = node->x;
			left->y = node->y;
			left->splitH = splitH;
			left->parent = node;
			node->left = left;
			
			
			right->height = node->height - (splitP - node->y);
			right->width = node->width;
			right->x = node->x;
			right->y = splitP;
			right->splitH = splitH;
			right->parent = node;
			node->right = right;
			
//			printf("\nLEFT  : \nx = %d\ny = %d\nheight = %d\nwidth = %d\n\n",left->x,left->y,left->height,left->width);
//			printf("RIGHT : \nx = %d\ny = %d\nheight = %d\nwidth = %d\n\n",right->x,right->y,right->height,right->width);
//			if(level+1 == 3){
//				makeRoom(node->left);
//				makeRoom(node->right);
//				return;
//			}
			
			
			
			if(level == 2){
				makeRoom(node->left);
				makeRoom(node->right);
			}
			
			bsp(level+1,node->left);
			bsp(level+1,node->right);
			
			
			
		}else{
			int minSplit = 0.45*node->width;
			int maxSplit = 0.55*node->width;
			
			int splitP = node->x+(rand()%(maxSplit - minSplit)) + minSplit;
			
//			printf("min : %d\nmax : %d\nsplit vertical %d !\n",minSplit,maxSplit,splitP);
//			fill(splitH,splitP,node);
			
			struct room *left = (struct room*) malloc (sizeof(struct room));
			struct room *right = (struct room*) malloc (sizeof(struct room));
			
			//TODO : cek rumus
			left->height = node->height;
			left->width = splitP- node->x;
			left->x = node->x;
			left->y = node->y;
			left->splitH = splitH;
			left->parent = node;
			node->left = left;
			
			right->height = node->height;
			right->width = node->width - (splitP - node->x);
			right->x = splitP;
			right->y = node->y;
			right->splitH = splitH;
			left->parent = node;
			node->right = right;
			
//			printf("LEFT  : \nx = %d\ny = %d\nheight = %d\nwidth = %d\n\n",left->x,left->y,left->height,left->width);
//			printf("RIGHT : \nx = %d\ny = %d\nheight = %d\nwidth = %d\n\n",right->x,right->y,right->height,right->width);
//			if(level+1 == 3){
//				makeRoom(node->left);
//				makeRoom(node->right);
//				return;
//			}
			
			if(level == 2){
				makeRoom(node->left);
				makeRoom(node->right);
			}
		
//			print();
		
			bsp(level+1, node->left);
			bsp(level+1, node->right);
			
			
		}
	}
};


int play(){
	
	dungeon dungeon[2];
	
	dungeon[0].generateDungeon(1);
	
	int roomNum = rand()%7;
	player2.x = rand()%(dungeon[0].rooms[roomNum]->roomWidth) + dungeon[0].rooms[roomNum]->roomx;
	player2.y = rand()%(dungeon[0].rooms[roomNum]->roomHeight) + dungeon[0].rooms[roomNum]->roomy;
	
	if(player2.x < dungeon[0].rooms[roomNum]->roomx){
		player2.x = player2.x + 2;
	}else if(player2.x > dungeon[0].rooms[roomNum]->roomx + dungeon[0].rooms[roomNum]->roomWidth){
		player2.x = dungeon[0].rooms[roomNum]->roomWidth + dungeon[0].rooms[roomNum]->roomx - 3;
	}
	
	if(player2.y < dungeon[0].rooms[roomNum]->roomy){
		player2.y = player2.y + 2;
	}else if(player2.y > dungeon[0].rooms[roomNum]->roomy + dungeon[0].rooms[roomNum]->roomHeight){
		player2.y = dungeon[0].rooms[roomNum]->roomHeight + dungeon[0].rooms[roomNum]->roomy - 3;
	}
	
	
	int keycount = 2;
	
	int key1X,key1Y,key2X,key2Y;
	
//	dungeon[0].print();;
	
	int roomNum1 = rand()%4;
	int roomNum2 = rand()%4+4;
	
	
	do{
		key1X = rand()%(dungeon[0].rooms[roomNum1]->roomWidth) + dungeon[0].rooms[roomNum1]->roomx+2;
		key1Y = rand()%(dungeon[0].rooms[roomNum1]->roomHeight) + dungeon[0].rooms[roomNum1]->roomy+2;
	
	}while(dungeon[0].tile[key1Y][key1X] != ' ');
	
	
	dungeon[0].tile[key1Y][key1X] = 237;
	
	do{
	
		key2X = rand()%(dungeon[0].rooms[roomNum2]->roomWidth) + dungeon[0].rooms[roomNum2]->roomx+2;
		key2Y = rand()%(dungeon[0].rooms[roomNum2]->roomHeight) + dungeon[0].rooms[roomNum2]->roomy+2;
	
	}while(dungeon[0].tile[key2Y][key2X] != ' ');
	
	dungeon[0].tile[key2Y][key2X] = 237;
	
	gotoxy(0,0);
	dungeon[0].print();
	
	portal portal[2];
	int cnt = 0;
	for(int i = 0;i < 7;i++){
		if(dungeon[0].rooms[i]->portalAvailable){
			portal[cnt].roomIndex = i;
			portal[cnt].x = dungeon[0].rooms[i]->portalX;
			portal[cnt++].y = dungeon[0].rooms[i]->portalY;
			
		}
	}
	
	gotoxy(player2.x,player2.y);
	printf("%c",161);
	
	while(1){
		
		//TODO : print Legend
		
		//
		
		
//		gotoxy(0,42);
//		printf("key left = %d\n",keycount);
		fflush(stdin);
//		printf("player2 x = %d\nplayer y = %d\nportal1 x = %d\nportal1 y = %d\nportal2 x = %d\nportal2 y = %d",player2.x,player2.y,portal[0].x,portal[0].y,portal[1].x,portal[1].y);
		char inp = getch();
		switch(inp){
			case 'w':
			case 'W':{
				
				int tempx = player2.x;
				int tempy = player2.y;
				if(dungeon[0].tile[player2.y-1][player2.x] != '#'){
					player2.y--;
					gotoxy(player2.x,player2.y);
					printf("%c",161);
					for(int i = 0;i < 2;i++){
						if(player2.x == portal[i].x && player2.y == portal[i].y){
							gotoxy(player2.x,player2.y);
							printf("@");
							player2.x = dungeon[0].rooms[portal[i].roomIndex]->portal_dst.x;
							player2.y = dungeon[0].rooms[portal[i].roomIndex]->portal_dst.y;
							gotoxy(player2.x,player2.y);
							break;
						}
					}
					gotoxy(tempx,tempy);
//					printf(".");
					printf("%c",dungeon[0].tile[tempy][tempx]);
				}if((tempx == key1X && tempy == key1Y) || (tempx == key2X && tempy == key2Y)){
						keycount--;
//						gotoxy(43,0);a
						
						if(tempx == key1X){
							key1X = -1;
							key1Y = -1;
						}else{
							key2X = -1;
							key2Y = -1;
						}
						
						
						gotoxy(tempx,tempy);
						printf(" ");
						dungeon[0].tile[tempx][tempy] = ' ';
					}
				break;
			}
			case 's':
			case 'S':{
				int tempx = player2.x;
				int tempy = player2.y;
				if(dungeon[0].tile[player2.y+1][player2.x] != '#' && dungeon[0].tile[player2.y+1][player2.x] != 235){
					player2.y++;
					gotoxy(player2.x,player2.y);
					printf("%c",161);
					for(int i = 0;i < 2;i++){
						if(player2.x == portal[i].x && player2.y == portal[i].y){
							gotoxy(player2.x,player2.y);
							printf("@");
							player2.x = dungeon[0].rooms[portal[i].roomIndex]->portal_dst.x;
							player2.y = dungeon[0].rooms[portal[i].roomIndex]->portal_dst.y;
							gotoxy(player2.x,player2.y);
							break;
						}
					}
					gotoxy(tempx,tempy);
//					printf(".");
					printf("%c",dungeon[0].tile[tempy][tempx]);
				}if((tempx == key1X && tempy == key1Y) || (tempx == key2X && tempy == key2Y)){
						keycount--;
//						gotoxy(43,0);
//						printf("Masuk!");
						
						if(tempx == key1X){
							key1X = -1;
							key1Y = -1;
						}else{
							key2X = -1;
							key2Y = -1;
						}
						
						
						gotoxy(tempx,tempy);
						printf(" ");
						dungeon[0].tile[tempx][tempy] = ' ';
					}
				break;
			}
				
				
			case 'a':
			case 'A':{
				int tempx = player2.x;
				int tempy = player2.y;
				if(dungeon[0].tile[player2.y][player2.x-1] != '#' && dungeon[0].tile[player2.y][player2.x-1] != 235){
					player2.x--;
					gotoxy(player2.x,player2.y);
					printf("%c",161);
					for(int i = 0;i < 2;i++){
						if(player2.x == portal[i].x && player2.y == portal[i].y){
							gotoxy(player2.x,player2.y);
							printf("@");
							player2.x = dungeon[0].rooms[portal[i].roomIndex]->portal_dst.x;
							player2.y = dungeon[0].rooms[portal[i].roomIndex]->portal_dst.y;
							gotoxy(player2.x,player2.y);
							break;
						}
					}
					
					gotoxy(tempx,tempy);
//					printf(".");
					printf("%c",dungeon[0].tile[tempy][tempx]);
				}if((tempx == key1X && tempy == key1Y) || (tempx == key2X && tempy == key2Y)){
						keycount--;
//						gotoxy(43,0);
//						printf("Masuk!");
						
						if(tempx == key1X){
							key1X = -1;
							key1Y = -1;
						}else{
							key2X = -1;
							key2Y = -1;
						}
						
						
						gotoxy(tempx,tempy);
						printf(" ");
						dungeon[0].tile[tempx][tempy] = ' ';
					}
				break;
			}
				
				
			case 'd':
			case 'D':{
				int tempx = player2.x;
				int tempy = player2.y;
				if(dungeon[0].tile[player2.y][player2.x+1] != '#' && dungeon[0].tile[player2.y][player2.x+1] != 235){
					player2.x++;
					gotoxy(player2.x,player2.y);
					printf("%c",161);
					for(int i = 0;i < 2;i++){
						if(player2.x == portal[i].x && player2.y == portal[i].y){
							gotoxy(player2.x,player2.y);
							printf("@");
							player2.x = dungeon[0].rooms[portal[i].roomIndex]->portal_dst.x;
							player2.y = dungeon[0].rooms[portal[i].roomIndex]->portal_dst.y;
							gotoxy(player2.x,player2.y);
							break;
						}
					}
					gotoxy(tempx,tempy);
//					printf(".");
					printf("%c",dungeon[0].tile[tempy][tempx]);
				}
				if((tempx == key1X && tempy == key1Y) || (tempx == key2X && tempy == key2Y)){
						keycount--;
//						gotoxy(43,0);
//						printf("Masuk!");
						
						if(tempx == key1X){
							key1X = -1;
							key1Y = -1;
						}else{
							key2X = -1;
							key2Y = -1;
						}
						
						
						gotoxy(tempx,tempy);
						printf(" ");
						dungeon[0].tile[tempx][tempy] = ' ';
					}
				break;
			}	
		}
		if(!keycount){
			break;
		}
	}
	           
	//transisi
	system("cls");
	//transisi
	
//	dungeon[1].generateDungeon(1);
//	
//	gotoxy(0,0);
//	
//	dungeon[1].print();
//	
//	roomNum = rand()%8;
//	player2.x = rand()%(dungeon[1].rooms[roomNum]->roomWidth) + dungeon[1].rooms[roomNum]->roomx;
//	player2.y = rand()%(dungeon[1].rooms[roomNum]->roomHeight) + dungeon[1].rooms[roomNum]->roomy;
//	
//	if(player2.x < dungeon[0].rooms[roomNum]->roomx){
//		player2.x = player2.x + 2;
//	}else if(player2.x > dungeon[1].rooms[roomNum]->roomx + dungeon[1].rooms[roomNum]->roomWidth){
//		player2.x = dungeon[1].rooms[roomNum]->roomWidth + dungeon[1].rooms[roomNum]->roomx - 3;
//	}
//	
//	if(player2.y < dungeon[1].rooms[roomNum]->roomy){
//		player2.y = player2.y + 2;
//	}else if(player2.y > dungeon[1].rooms[roomNum]->roomy + dungeon[1].rooms[roomNum]->roomHeight){
//		player2.y = dungeon[1].rooms[roomNum]->roomHeight + dungeon[1].rooms[roomNum]->roomy - 3;
//	}
//	
//	coor key[8];
//	
//	for(int i = 0;i < 8;i++){
//		do{
//			key[i].x = (rand()%(dungeon[1].rooms[i]->roomWidth)) + dungeon[1].rooms[i]->roomx+2;
//			key[i].y = (rand()%(dungeon[1].rooms[i]->roomHeight)) + dungeon[1].rooms[i]->roomy+2;
//	
//		}while(dungeon[1].tile[key[i].y][key[i].x] != ' ');
//	}
//	
//	gotoxy(0,0);
//	dungeon[1].print();
//	
	return 1;
}

class PickUp				/// pick item
{
public:
    int x;
    int y;
    bool exists;
};

class RunnerHorizontal				/// obstacle kanan kiri
{
public:
    int x;
    int y;
    int lifes;
    char movingDirection;
};


class CannonHorizontal        /// PISTOL HORIZONTAL
{
public:
    int x;
    int y;
    char shootingDirection;   /// A S W D
    int speed;
    bool exists;
};
class CannonVertical          /// PISTOL VERTICAL
{
public:
    int x;
    int y;
    char shootingDirection;   /// A S W D
    int speed;
    bool exists;
};

class Randomer                 /// MONSTER GERAK RANDOM, -> char(208)
{
public:
    int x;
    int y;
    int xMax;
    int xMin;
    int yMax;
    int yMin;
    int lifes;
    bool successfulMove;
    int speed;
};
class Chaser                   /// SI TUKANG NGEJAR(AI) -> char(190)
{
public:
    int x;
    int y;
    bool exists;
    int speed();
};

class Boss{
	public:
	int x;
    int y;
    bool exists;
    int speed();	
};

class Bullet					/// PELURU
{
public:
    int x;
    int y;
    char movingDirection;       /// A S W D
    bool exists;
    bool firstOccurrence;
};

	bool gameStatus=false;
	Player player;
	int framesDelay;
	int bossLife; 
	bool bossPlaced=false;     
	bool chasersPlaced=false;       
	char c;    
	bool exitConsole=false;
	bool inGame;
	Chaser chaser[30];
	PickUp pickUp[100];
	Randomer randomer[50];
	CannonHorizontal cannonHorizontal[50];
	CannonVertical cannonVertical[50];
	RunnerHorizontal runnerHorizontal[100];
	Boss boss[100];
	Bullet bullet[400];
	int nrBullet;       
	int frameCount;



char getCursorChar()    
{
    char c = '\0';
    CONSOLE_SCREEN_BUFFER_INFO con;
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hcon != INVALID_HANDLE_VALUE &&
        GetConsoleScreenBufferInfo(hcon,&con))
    {
        DWORD read = 0;
        if (!ReadConsoleOutputCharacterA(hcon,&c,1,
            con.dwCursorPosition,&read) || read != 1
            )
            c = '\0';
    }
    return c;
}

void loadLevelBase(string fileName)    // dari txt map ini
{
    string getContent;
    ifstream openfile(fileName.c_str());
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    if(openfile.is_open())
    {
        while(! openfile.eof())
        {
            getline(openfile, getContent);
            cout << getContent << endl;
        }
    }
}


char readChar(int x,int y)  // KOORDINAT PLAYER
{
    gotoxy(x,y);
    char ccccc = getCursorChar();
    return ccccc;
}

void createMovableBox(int x,int y) // BUAT BATU YG D PINDAH
{
    gotoxy(x,y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
    cout<<"©";
}
void createChaser(int id,int x,int y) //BUAT SI PENGEJAR 
{
    chaser[id].x=x;
    chaser[id].y=y;
    chaser[id].exists=true;
    gotoxy(x,y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9);
    cout<<char(190);
}

void createBoss(int id,int x,int y) //BUAT SI BOSS 
{
    boss[id].x=x;
    boss[id].y=y;
    boss[id].exists=true;
    gotoxy(x,y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),28);
    cout<<char(245);
}

//void createCannonHorizontal(int id, int x,int y, char shootingDirection,int speed) //BUAT PISTOL HORIZONTAL
//{
//    cannonHorizontal[id].x=x;
//    cannonHorizontal[id].y=y;
//    cannonHorizontal[id].shootingDirection=shootingDirection;
//    cannonHorizontal[id].speed=speed;
//    cannonHorizontal[id].exists=true;
//    gotoxy(x,y);
//    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
//    cout<<char(196);
//}
//void createCannonVertical(int id, int x,int y, char shootingDirection,int speed) //BUAT PISTOL VERTICAL
//{
//    cannonVertical[id].x=x;
//    cannonVertical[id].y=y;
//    cannonVertical[id].shootingDirection=shootingDirection;
//    cannonVertical[id].speed=speed;
//    cannonVertical[id].exists=true;
//    gotoxy(x,y);
//    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
//    cout<<"!";
//}

void createRandomer(int id,int x,int y, int lifes,int range,int speed) //SI MONSTER GERAK RANDOM
{
    randomer[id].x=x;
    randomer[id].y=y;
    randomer[id].xMin=x-range;
    randomer[id].yMin=y-range;
    randomer[id].xMax=x+range;
    randomer[id].yMax=y+range;
    randomer[id].lifes=lifes;
    randomer[id].speed=speed;
    gotoxy(x,y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),13);
    cout<<char(208);
}

void createRunnerHorizontal(int id,int x,int y,char movingDirection,int lifes)
{
    runnerHorizontal[id].x=x;
    runnerHorizontal[id].y=y;
    runnerHorizontal[id].lifes=lifes;
    runnerHorizontal[id].movingDirection=movingDirection;
    gotoxy(x,y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
    cout<<char(207);
}

void createPickUpLife(int id,int x,int y)
{
    pickUp[id].exists=true;
    pickUp[id].x=x;
    pickUp[id].y=y;
    gotoxy(x,y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"+";
}

void createPickUpAmunicja(int id,int x,int y)
{
    pickUp[id].exists=true;
    pickUp[id].x=x;
    pickUp[id].y=y;
    gotoxy(x,y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
    cout<<char(159);
}

void createPickUpCoin(int id, int x,int y){
	pickUp[id].exists=true;
    pickUp[id].x=x;
    pickUp[id].y=y;
    gotoxy(x,y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
    cout<<"$";
}

void createPickUpCheckPoint(int id,int x,int y)
{
    pickUp[id].exists=true;
    pickUp[id].x=x;
    pickUp[id].y=y;
    gotoxy(x,y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),240);
    cout<<"C";
}


void loadLevelObjects(int levelNumber)    /// LETAK OBSTACLE & AI
{
    if (levelNumber==1)
    {
    	
    	// CREATE BATU
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
        createMovableBox(53,31);
        createMovableBox(56,33);
        createMovableBox(57,34);
        for (int i=0; i<=3; i++)
        {
            createMovableBox(52,15+i*2);
        }
        createMovableBox(53,15);
        for (int i=0; i<=2; i++)
        {
            createMovableBox(53,16+i*2);
        }
        for (int i=0; i<=3; i++)
        {
            createMovableBox(54,15+i*2);
        }
        for (int i=0; i<=1; i++)
        {
            createMovableBox(55,16+i*2);
        }
        for (int i=0; i<=2; i++)
        {
            createMovableBox(56,15+i*2);
        }
        createMovableBox(13,25);

        createMovableBox(1,13);
        createMovableBox(2,12);
        createMovableBox(2,11);
        createMovableBox(2,10);
        createMovableBox(3,11);
        createMovableBox(4,12);
        createMovableBox(4,11);
        createMovableBox(5,10);
        createMovableBox(1,9);
        createMovableBox(3,8);

        createMovableBox(4,9);
        createMovableBox(4,13);
        createMovableBox(6,12);
        createMovableBox(7,11);
        createMovableBox(8,10);
        createMovableBox(9,9);
        createMovableBox(10,8);
        createMovableBox(11,9);
        createMovableBox(11,12);
        createMovableBox(6,13);

        createMovableBox(35,12);
        createMovableBox(35,13);
        createMovableBox(36,12);
        createMovableBox(34,13);

        createMovableBox(34,7);
        createMovableBox(34,8);
        createMovableBox(34,6);

        createMovableBox(35,7);
        createMovableBox(35,8);
        createMovableBox(36,8);

        createMovableBox(35,10);

        for (int i=0; i<=4; i++)
        {
//            createMovableBox(36+i,9);
            createMovableBox(36+i,11);
        }
        createMovableBox(52,3);
        createMovableBox(42,13);

//        
        	// KANAN KIRI CANTIK OBSTACLE
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
        createRunnerHorizontal(1,7,33,'d',2);
        createRunnerHorizontal(2,13,31,'a',2);

        createRunnerHorizontal(3,38,33,'d',2);
        createRunnerHorizontal(4,40,32,'d',2);
        createRunnerHorizontal(5,42,31,'a',2);
        createRunnerHorizontal(6,44,30,'d',2);
        createRunnerHorizontal(7,46,29,'d',2);

        createRunnerHorizontal(8,48,28,'d',2);
        createRunnerHorizontal(9,54,30,'a',2);
        createRunnerHorizontal(10,54,34,'a',2);

        createRunnerHorizontal(11,7,17,'d',2);
        createRunnerHorizontal(12,22,18,'a',2);
        createRunnerHorizontal(13,7,19,'d',2);
        createRunnerHorizontal(14,22,20,'a',2);
        createRunnerHorizontal(15,7,21,'d',2);
        createRunnerHorizontal(16,22,22,'a',2);
        createRunnerHorizontal(17,7,23,'d',2);
        createRunnerHorizontal(18,22,24,'a',2);

        createRunnerHorizontal(19,1,2,'a',2);
        createRunnerHorizontal(20,2,3,'d',2);
        createRunnerHorizontal(23,8,2,'a',2);
        createRunnerHorizontal(24,7,3,'a',2);
        createRunnerHorizontal(27,9,4,'a',2);
        createRunnerHorizontal(28,9,3,'a',2);
        createRunnerHorizontal(29,15,4,'a',2);
        createRunnerHorizontal(30,15,5,'a',2);

        createRunnerHorizontal(31,36,10,'d',2);

        createRunnerHorizontal(32,37,7,'d',2);
        createRunnerHorizontal(33,37,8,'d',2);
        createRunnerHorizontal(34,37,12,'d',2);
//        createRunnerHorizontal(35,37,13,'d',2);
        createRunnerHorizontal(36,36,6,'d',2);

        createRunnerHorizontal(37,59,11,'d',2);
        createRunnerHorizontal(38,64,12,'d',2);
        createRunnerHorizontal(39,62,13,'d',2);
        createRunnerHorizontal(40,75,11,'d',2);
        createRunnerHorizontal(42,77,13,'d',2);

        createRunnerHorizontal(43,49,24,'a',2);
        createRunnerHorizontal(44,58,25,'d',2);
        createRunnerHorizontal(45,43,23,'d',2);
        createRunnerHorizontal(46,45,26,'d',2);

	// CREATE AI
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
        if (player.checkPoint>=4)
        {
            createChaser(1,42,9);
            createChaser(2,45,13);
            createChaser(3,42,4);
            createChaser(4,51,7);
            createChaser(5,55,7);
            createChaser(6,59,7);
            createChaser(7,61,7);
//            createChaser(8,60,36);
//            createChaser(9,5,30);
        }
        
// GAADA BOS
        if (player.checkPoint>=6)
        {
         
//        createBoss(8,60,36);
//          createChaser(1,60,36);
          createRandomer(16,65,20,3,6,7);
          createRandomer(17,80,25,3,6,7);
          createRandomer(18,70,20,3,6,7);
          createRandomer(19,85,30,3,6,7);
          createRandomer(20,65,35,3,6,7);
          createRandomer(21,80,33,3,6,7);
          createRandomer(22,70,32,3,6,7);
          createRandomer(23,85,30,3,6,7);
//          createPickUpAmunicja(4,83,15);
          createPickUpAmunicja(5,81,15);
          createPickUpLife(5,82,15);
          
//              createPickUpCheckPoint(11,85,15);
        }
        // CREATE MONSTER GERAK RANDOM
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
        createRandomer(1,18,33,2,3,3);
        createRandomer(2,30,21,2,6,3);
        
        
		
        createRandomer(3,41,18,2,3,2);
        createRandomer(4,30,21,2,6,2);
        createRandomer(5,30,21,2,6,3);

        
        createRandomer(7,3,20,2,1,4);
        createRandomer(8,2,16,3,1,5);
     
    
        
		createRandomer(10,24,11,3,1,2);
        createRandomer(11,26,9,3,1,7);

        
        createRandomer(13,32,32,3,4,2);
        createRandomer(14,31,31,3,3,7);
        createRandomer(15,36,30,3,4,2);
        
        	
//        createChaser(16,30,18);
//        createChaser(17,41,30);
//        createPickUpCheckPoint(8,33,11);

		//PICK ITEM
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
		if (pickUp[1].exists==true)
        {
            createPickUpAmunicja(1,15,36);
        }
        if (pickUp[2].exists==true)
        {
            createPickUpAmunicja(2,8,1);
        }
        if (pickUp[3].exists==true)
        {
            createPickUpLife(3,40,35);
        }
        if (pickUp[4].exists==true)
        {
            createPickUpLife(4,25,15);
        }
        if (pickUp[5].exists==true)
        {
            createPickUpLife(5,22,6);
        }
        if (pickUp[6].exists==true)
        {
            createPickUpCheckPoint(6,56,22);
        }
        if (pickUp[7].exists==true)
        {
            createPickUpCheckPoint(7,4,23);
        }
        if (pickUp[8].exists==true)
        {
            createPickUpCheckPoint(8,33,11);
        }
        if (pickUp[9].exists==true)
        {
            createPickUpCheckPoint(9,42,12);
        }
        if (pickUp[10].exists==true)
        {
            createPickUpLife(10,58,12);
        }
        if (pickUp[11].exists==true)
        {
            createPickUpCheckPoint(11,85,15);
        }
        if (pickUp[12].exists==true){
        	createPickUpAmunicja(12,5,25);
		}
		if (pickUp[13].exists==true)
        {
            createPickUpCheckPoint(13,37,35);
        }
        if(pickUp[14].exists==true){
        	createPickUpCheckPoint(13,60,36);
		}
			
 }
}


// GERAK PLAYER ft RANDOM MONSTER
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
int collisionRandomer(int id,char movingDirection)
{
	
    if(movingDirection=='w')
    {
        if ((readChar(randomer[id].x,randomer[id].y-1)==' ')||(readChar(randomer[id].x,randomer[id].y-1)==char(208)))
        {
            return 0;
        }else if ((readChar(randomer[id].x,randomer[id].y-1)=='#') || (readChar(randomer[id].x,randomer[id].y-1)=='©')|| 
		(readChar(randomer[id].x,randomer[id].y-1)==char(196))|| (readChar(randomer[id].x,randomer[id].y-1)=='!')|| 
		(readChar(randomer[id].x,randomer[id].y-1)==char(159))|| (readChar(randomer[id].x,randomer[id].y-1)=='+')||
		(readChar(randomer[id].x,randomer[id].y-1)=='$')|| 
		(readChar(randomer[id].x,randomer[id].y-1)==char(207))|| (readChar(randomer[id].x,randomer[id].y-1)==char(208))|| 
		(readChar(randomer[id].x,randomer[id].y-1)=='X'))
        {
            return 2;
        }else return 1;
    }
    
    if(movingDirection=='s')
    {
        if ((readChar(randomer[id].x,randomer[id].y+1)==' ')||(readChar(randomer[id].x,randomer[id].y+1)==char(208)))
        {
            return 0;
        }else if ((readChar(randomer[id].x,randomer[id].y+1)=='#') || (readChar(randomer[id].x,randomer[id].y+1)=='©')|| 
		(readChar(randomer[id].x,randomer[id].y+1)==char(196))|| (readChar(randomer[id].x,randomer[id].y+1)=='!')|| 
		(readChar(randomer[id].x,randomer[id].y+1)==char(159))|| (readChar(randomer[id].x,randomer[id].y+1)=='+')|| 
		(readChar(randomer[id].x,randomer[id].y-1)=='$')||
		(readChar(randomer[id].x,randomer[id].y+1)==char(207))|| (readChar(randomer[id].x,randomer[id].y+1)==char(208))|| 
		(readChar(randomer[id].x,randomer[id].y+1)=='X'))
        {
            return 2;
        }else return 1;
    }
    
    if(movingDirection=='a')
    {
        if ((readChar(randomer[id].x-1,randomer[id].y)==' ')||(readChar(randomer[id].x-1,randomer[id].y)==char(208)))
        {
            return 0;
        }else if ((readChar(randomer[id].x-1,randomer[id].y)=='#') || (readChar(randomer[id].x-1,randomer[id].y)=='©')|| 
		(readChar(randomer[id].x-1,randomer[id].y)==char(196))|| (readChar(randomer[id].x-1,randomer[id].y)=='!')|| 
		(readChar(randomer[id].x-1,randomer[id].y)==char(159))|| (readChar(randomer[id].x-1,randomer[id].y)=='+')|| 
		(readChar(randomer[id].x,randomer[id].y-1)=='$')||
		(readChar(randomer[id].x-1,randomer[id].y)==char(207))|| (readChar(randomer[id].x-1,randomer[id].y)==char(208))|| 
		(readChar(randomer[id].x-1,randomer[id].y)=='X'))
        {
            return 2;
        }else return 1;
    }
    
    if (movingDirection=='d')
    {
        if ((readChar(randomer[id].x+1,randomer[id].y)==' ')||(readChar(randomer[id].x+1,randomer[id].y)==char(208)))
        {
            return 0;
        }else if ((readChar(randomer[id].x+1,randomer[id].y)=='#') || (readChar(randomer[id].x+1,randomer[id].y)=='©')|| 
		(readChar(randomer[id].x+1,randomer[id].y)==char(196))|| (readChar(randomer[id].x+1,randomer[id].y)=='!')|| 
		(readChar(randomer[id].x+1,randomer[id].y)==char(159))|| (readChar(randomer[id].x+1,randomer[id].y)=='+')|| 
		(readChar(randomer[id].x,randomer[id].y-1)=='$')||
		(readChar(randomer[id].x+1,randomer[id].y)==char(207))|| (readChar(randomer[id].x+1,randomer[id].y)==char(208))|| 
		(readChar(randomer[id].x+1,randomer[id].y)=='X'))
        {
            return 2;
        }else return 1;
    }
}


// UPDATE POSISI PLAYER , AI, BATU
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
void update()
{
    if ((player.checkPoint==6) && (chasersPlaced==false))
    {
        createChaser(1,45,9);
        createChaser(2,45,13);
        createChaser(3,42,4);
        createChaser(4,51,7);
        createChaser(5,55,7);
        createChaser(6,59,7);
        createChaser(7,61,7);
        chasersPlaced=true;
    }
    
    if ((c=='w')|| (c=='s') || (c=='a') || (c=='d'))
    {
        if (c=='w')
        {
            player.movingDirection='w';
            gotoxy(player.x,player.y);
            cout<<" ";
            if ((readChar(player.x,player.y-1)!= '#') && (readChar(player.x,player.y-1)!= char(196)) 
				&& (readChar(player.x,player.y-1)!= '!')&& (readChar(player.x,player.y-1)!= 'X'))
            {
                if (readChar(player.x,player.y-1)=='©')
                {
                    if ((readChar(player.x,player.y-2)==' ') || (readChar(player.x,player.y-2)==char(111)))
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
                        gotoxy(player.x,player.y-2);
                        cout<<"©";
                        player.y-=1;
                    }
                }else
                {
                    player.y-=1;
                }
            }
        }

        if (c=='s')
        {
            player.movingDirection='s';
            gotoxy(player.x,player.y);
            cout<<" ";
            if ((readChar(player.x,player.y+1)!= '#') && (readChar(player.x,player.y+1)!= char(196)) 
				&& (readChar(player.x,player.y+1)!= '!')&& (readChar(player.x,player.y+1)!= 'X'))
            {
                if (readChar(player.x,player.y+1)=='©')
                {
                    if ((readChar(player.x,player.y+2)==' ') || (readChar(player.x,player.y+2)==char(111)))
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
                        gotoxy(player.x,player.y+2);
                        cout<<"©";
                        player.y+=1;
                    }
                }else
                {
                    player.y+=1;
                }
            }
        }

        if (c=='a')
        {
            player.movingDirection='a';
            gotoxy(player.x,player.y);
            cout<<" ";
            if ((readChar(player.x-1,player.y)!= '#') && (readChar(player.x-1,player.y)!= char(196)) 
				&& (readChar(player.x-1,player.y)!= '!')&& (readChar(player.x-1,player.y)!= 'X'))
            {
                if (readChar(player.x-1,player.y)=='©')
                {
                    if ((readChar(player.x-2,player.y)==' ') || (readChar(player.x-2,player.y)==char(111)))
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
                        gotoxy(player.x-2,player.y);
                        cout<<"©";
                        player.x-=1;
                    }
                }else
                {
                    player.x-=1;
                }
            }
        }

        if (c=='d')
        {
            player.movingDirection='d';
            gotoxy(player.x,player.y);
            cout<<" ";
            if ((readChar(player.x+1,player.y)!= '#') && (readChar(player.x+1,player.y)!= char(196)) 
				&& (readChar(player.x+1,player.y)!= '!')&& (readChar(player.x+1,player.y)!= 'X'))
            {
                if (readChar(player.x+1,player.y)=='©')
            {
                    if ((readChar(player.x+2,player.y)==' ') || (readChar(player.x+2,player.y)==char(111)))
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
                        gotoxy(player.x+2,player.y);
                        cout<<"©";
                        player.x+=1;
                    }
                }else
                {
                    player.x+=1;
                }
            }
        }
        
// KETIKA AMBIL ITEM
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
        if (readChar(player.x,player.y)== char(159))
        {
            player.ammunition+=5;
            for(int i=0; i<=20; i++)
            {
                if ((player.x==pickUp[i].x) && (player.y==pickUp[i].y))
                {
                    pickUp[i].exists=false;
                }
            }
           
        }
        if (readChar(player.x,player.y)== '+')
        {
            player.lifes+=1;
            for(int i=0; i<=20; i++)
            {
                if ((player.x==pickUp[i].x) && (player.y==pickUp[i].y))
                {
                    pickUp[i].exists=false;
                }
            }
           
        }
        if (readChar(player.x,player.y)== 'C')
        {
            player.checkPoint+=1;
               for(int i=0; i<=20; i++)
            {
                if ((player.x==pickUp[i].x) && (player.y==pickUp[i].y))
                {
                    pickUp[i].exists=false;
                }
            }
           
        }
        if (readChar(player.x,player.y)== char(207))
        {
            player.lifes-=1;
            player.lostLife=true;
        }
        if (readChar(player.x,player.y)== char(208))
        {
            player.lifes-=1;
            player.lostLife=true;
        }
        if (readChar(player.x,player.y)== char(111))
        {
            player.lifes-=1;
            player.lostLife=true;
        }
        if (readChar(player.x,player.y)== char(207))
        {
            player.lifes-=1;
            player.lostLife=true;
        }

// GERAK JA
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
        gotoxy(player.x,player.y);
        if (player.movingDirection=='w')
        {
            cout<<char(161);
        }
        if (player.movingDirection=='s')
        {
            cout<<char(161);
        }
        if (player.movingDirection=='a')
        {
            cout<<char(161);
        }
        if (player.movingDirection=='d')
        {
            cout<<char(161);
        }
    }
    
    
    int r;
    int emergencyLoopBreak;    
	
	// CREATE POSISI RANDOM MONSTER
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//--------------------------------------------------------------- 
    for (int i=0; i<=50; i++)
    {
        if (randomer[i].lifes>0)
        {
            if ((frameCount % randomer[i].speed)==0)
            {
                gotoxy(randomer[i].x,randomer[i].y);
                cout<<" ";
                randomer[i].successfulMove=false;
                while (randomer[i].successfulMove==false)
                {
                    emergencyLoopBreak+=1;
                    r=rand()%4+1;
                    if ((r==1) && (randomer[i].y>randomer[i].yMin) && (collisionRandomer(i,'w')==0))
                    {
                        randomer[i].y-=1;
                        randomer[i].successfulMove=true;
                    } else if ((r==1) && (randomer[i].y>randomer[i].yMin) && (collisionRandomer(i,'w')==1))
                    {
                        player.lifes-=1;
                        player.lostLife=true;
                    }
                    if ((r==2) && (randomer[i].y<randomer[i].yMax) && (collisionRandomer(i,'s')==0))
                    {
                        randomer[i].y+=1;
                        randomer[i].successfulMove=true;
                    } else if ((r==2) && (randomer[i].y<randomer[i].yMax) && (collisionRandomer(i,'s')==1))
                    {
                        player.lifes-=1;
                        player.lostLife=true;
                    }
                    if ((r==3) && (randomer[i].x>randomer[i].xMin) && (collisionRandomer(i,'a')==0))
                    {
                        randomer[i].x-=1;
                        randomer[i].successfulMove=true;
                    } else if ((r==3) && (randomer[i].x>randomer[i].xMin) && (collisionRandomer(i,'a')==1))
                    {
                        player.lifes-=1;
                        player.lostLife=true;
                    }
                    if ((r==4) && (randomer[i].x<randomer[i].xMax) && (collisionRandomer(i,'d')==0))
                    {
                        randomer[i].x+=1;
                        randomer[i].successfulMove=true;
                    } else if ((r==4) && (randomer[i].x<randomer[i].xMax) && (collisionRandomer(i,'d')==1))
                    {
                        player.lifes-=1;
                        player.lostLife=true;
                    }
                    if (emergencyLoopBreak>=50)
                    {
                        randomer[i].successfulMove=true;
                    }
                }
                
         // POSISI MONSTER
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
                gotoxy(randomer[i].x,randomer[i].y);
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),13);
                if (randomer[i].lifes<=1)
                {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),5);
                }
                cout<<char(208);
            }
        }
    }


for (int i=0; i<=30; i++)
    {
        if (boss[i].exists==true)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9);
            if (abs(boss[i].x-player.x)>=(abs(boss[i].y-player.y)))
            {
                if(boss[i].x>=player.x)
                {
                    if(readChar(boss[i].x-1,boss[i].y)==' ')
                    {
                        gotoxy(boss[i].x,boss[i].y);
                        cout<<" ";
                        chaser[i].x-=1;
                        gotoxy(boss[i].x,boss[i].y);
                        cout<<char(245);
                    }else if ((readChar(boss[i].x-1,boss[i].y)!='#') && (readChar(boss[i].x-1,boss[i].y)!='©') 
						&&(readChar(boss[i].x-1,boss[i].y)!=char(159)) && (readChar(boss[i].x-1,boss[i].y)!='+') 
						&& (readChar(boss[i].x-1,boss[i].y)!=char(207)) && (readChar(boss[i].x-1,boss[i].y)!=char(207))
						&&(readChar(boss[i].x-1,boss[i].y)!=char(111)) && (readChar(boss[i].x-1,boss[i].y)!=char(208)))
                    {
                        player.lostLife=true;
                        player.lifes-=1;
                    }
                }
				
				else if(boss[i].x<player.x)
                {
                    if(readChar(boss[i].x+1,boss[i].y)==' ')
                    {
                        gotoxy(boss[i].x,boss[i].y);
                        cout<<" ";
                        chaser[i].x+=1;
                        gotoxy(boss[i].x,boss[i].y);
                        cout<<char(245);
                    }else if ((readChar(boss[i].x+1,boss[i].y)!='#') && (readChar(boss[i].x+1,boss[i].y)!='©') 
						&&(readChar(boss[i].x+1,boss[i].y)!=char(159)) && (readChar(boss[i].x+1,boss[i].y)!='+') 
						&& (readChar(boss[i].x+1,boss[i].y)!=char(207)) && (readChar(boss[i].x+1,boss[i].y)!=char(207))
						&&(readChar(boss[i].x+1,boss[i].y)!=char(111)) && (readChar(boss[i].x+1,boss[i].y)!=char(208)))
                    {
                        player.lostLife=true;
                        player.lifes-=1;
                    }
                }
            }
			
			else if (abs(boss[i].x-player.x)<(abs(boss[i].y-player.y)))
            {
                if(boss[i].y>=player.y)
                {
                    if(readChar(boss[i].x,boss[i].y-1)==' ')
                    {
                        gotoxy(boss[i].x,boss[i].y);
                        cout<<" ";
                        chaser[i].y-=1;
                        gotoxy(boss[i].x,boss[i].y);
                        cout<<char(245);
                        
                    }else if ((readChar(boss[i].x,boss[i].y-1)!='#') && (readChar(boss[i].x,boss[i].y-1)!='©') 
						&&(readChar(boss[i].x,boss[i].y-1)!=char(159)) && (readChar(boss[i].x,boss[i].y-1)!='+') 
						&& (readChar(boss[i].x,boss[i].y-1)!=char(207)) && (readChar(boss[i].x,boss[i].y-1)!=char(207))
						&&(readChar(boss[i].x,boss[i].y-1)!=char(111)) && (readChar(boss[i].x,boss[i].y-1)!=char(208)))
                    {
                        player.lostLife=true;
                        player.lifes-=1;
                    }
                }else if(boss[i].y<player.y)
                {
                    if(readChar(boss[i].x,boss[i].y+1)==' ')
                    {
                        gotoxy(boss[i].x,boss[i].y);
                        cout<<" ";
                        boss[i].y+=1;
                        gotoxy(boss[i].x,boss[i].y);
                        cout<<char(245);
                        
                    }else if ((readChar(boss[i].x,boss[i].y+1)!='#') && (readChar(boss[i].x,boss[i].y+1)!='©') 
						&&(readChar(boss[i].x,boss[i].y+1)!=char(159)) && (readChar(boss[i].x,boss[i].y+1)!='+') 
						&& (readChar(boss[i].x,boss[i].y+1)!=char(207)) && (readChar(boss[i].x,boss[i].y+1)!=char(207))
						&&(readChar(boss[i].x,boss[i].y+1)!=char(111)) && (readChar(boss[i].x,boss[i].y+1)!=char(208)))
                    {
                        player.lostLife=true;
                        player.lifes-=1;
                    }
                }
            }
        }
    }


// 			GERAK AI
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------

    for (int i=0; i<=30; i++)
    {
        if (chaser[i].exists==true)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9);
            if (abs(chaser[i].x-player.x)>=(abs(chaser[i].y-player.y)))
            {
                if(chaser[i].x>=player.x)
                {
                    if(readChar(chaser[i].x-1,chaser[i].y)==' ')
                    {
                        gotoxy(chaser[i].x,chaser[i].y);
                        cout<<" ";
                        chaser[i].x-=1;
                        gotoxy(chaser[i].x,chaser[i].y);
                        cout<<char(190);
                    }else if ((readChar(chaser[i].x-1,chaser[i].y)!='#') && (readChar(chaser[i].x-1,chaser[i].y)!='©') 
						&&(readChar(chaser[i].x-1,chaser[i].y)!=char(159)) && (readChar(chaser[i].x-1,chaser[i].y)!='+') 
						&& (readChar(chaser[i].x-1,chaser[i].y)!=char(207)) && (readChar(chaser[i].x-1,chaser[i].y)!=char(207))
						&&(readChar(chaser[i].x-1,chaser[i].y)!=char(111)) && (readChar(chaser[i].x-1,chaser[i].y)!=char(208)))
                    {
                        player.lostLife=true;
                        player.lifes-=1;
                    }
                }
				
				else if(chaser[i].x<player.x)
                {
                    if(readChar(chaser[i].x+1,chaser[i].y)==' ')
                    {
                        gotoxy(chaser[i].x,chaser[i].y);
                        cout<<" ";
                        chaser[i].x+=1;
                        gotoxy(chaser[i].x,chaser[i].y);
                        cout<<char(190);
                    }else if ((readChar(chaser[i].x+1,chaser[i].y)!='#') && (readChar(chaser[i].x+1,chaser[i].y)!='©') 
						&&(readChar(chaser[i].x+1,chaser[i].y)!=char(159)) && (readChar(chaser[i].x+1,chaser[i].y)!='+') 
						&& (readChar(chaser[i].x+1,chaser[i].y)!=char(207)) && (readChar(chaser[i].x+1,chaser[i].y)!=char(207))
						&&(readChar(chaser[i].x+1,chaser[i].y)!=char(111)) && (readChar(chaser[i].x+1,chaser[i].y)!=char(208)))
                    {
                        player.lostLife=true;
                        player.lifes-=1;
                    }
                }
            }
			
			else if (abs(chaser[i].x-player.x)<(abs(chaser[i].y-player.y)))
            {
                if(chaser[i].y>=player.y)
                {
                    if(readChar(chaser[i].x,chaser[i].y-1)==' ')
                    {
                        gotoxy(chaser[i].x,chaser[i].y);
                        cout<<" ";
                        chaser[i].y-=1;
                        gotoxy(chaser[i].x,chaser[i].y);
                        cout<<char(190);
                        
                    }else if ((readChar(chaser[i].x,chaser[i].y-1)!='#') && (readChar(chaser[i].x,chaser[i].y-1)!='©') 
						&&(readChar(chaser[i].x,chaser[i].y-1)!=char(159)) && (readChar(chaser[i].x,chaser[i].y-1)!='+') 
						&& (readChar(chaser[i].x,chaser[i].y-1)!=char(207)) && (readChar(chaser[i].x,chaser[i].y-1)!=char(207))
						&&(readChar(chaser[i].x,chaser[i].y-1)!=char(111)) && (readChar(chaser[i].x,chaser[i].y-1)!=char(208)))
                    {
                        player.lostLife=true;
                        player.lifes-=1;
                    }
                }else if(chaser[i].y<player.y)
                {
                    if(readChar(chaser[i].x,chaser[i].y+1)==' ')
                    {
                        gotoxy(chaser[i].x,chaser[i].y);
                        cout<<" ";
                        chaser[i].y+=1;
                        gotoxy(chaser[i].x,chaser[i].y);
                        cout<<char(190);
                        
                    }else if ((readChar(chaser[i].x,chaser[i].y+1)!='#') && (readChar(chaser[i].x,chaser[i].y+1)!='©') 
						&&(readChar(chaser[i].x,chaser[i].y+1)!=char(159)) && (readChar(chaser[i].x,chaser[i].y+1)!='+') 
						&& (readChar(chaser[i].x,chaser[i].y+1)!=char(207)) && (readChar(chaser[i].x,chaser[i].y+1)!=char(207))
						&&(readChar(chaser[i].x,chaser[i].y+1)!=char(111)) && (readChar(chaser[i].x,chaser[i].y+1)!=char(208)))
                    {
                        player.lostLife=true;
                        player.lifes-=1;
                    }
                }
            }
        }
    }
    
    	//    PELARI HORIZONTAL/ KANAN KIRI OBSTACLE
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
    for (int i=0;i<=100;i++)
    {
        if (runnerHorizontal[i].lifes>0)
        {
            gotoxy(runnerHorizontal[i].x,runnerHorizontal[i].y);
            cout<<" ";
            if (runnerHorizontal[i].movingDirection=='d')
            {
                if (readChar(runnerHorizontal[i].x+1,runnerHorizontal[i].y)==' ')
                {
                    runnerHorizontal[i].x+=1;
                }else if (readChar(runnerHorizontal[i].x+1,runnerHorizontal[i].y)=='#')
                {
                    runnerHorizontal[i].movingDirection='a';
                }else if (readChar(runnerHorizontal[i].x+1,runnerHorizontal[i].y)=='X')
                {
                    runnerHorizontal[i].movingDirection='a';
                }else if (readChar(runnerHorizontal[i].x+1,runnerHorizontal[i].y)=='©')
                {
                    runnerHorizontal[i].movingDirection='a';
                }else if (readChar(runnerHorizontal[i].x+1,runnerHorizontal[i].y)=='+')
                {
                    runnerHorizontal[i].movingDirection='a';
                }else if (readChar(runnerHorizontal[i].x+1,runnerHorizontal[i].y)==char(159))
                {
                    runnerHorizontal[i].movingDirection='a';
                }else if (readChar(runnerHorizontal[i].x+1,runnerHorizontal[i].y)==char(207))
                {
                    runnerHorizontal[i].movingDirection='a';
                }else if (readChar(runnerHorizontal[i].x+1,runnerHorizontal[i].y)==char(111))
                {
                    //NIC
                }else
                {
                    player.lifes-=1;
                    player.lostLife=true;
                }
            }else if (runnerHorizontal[i].movingDirection=='a')
            {
                if (readChar(runnerHorizontal[i].x-1,runnerHorizontal[i].y)==' ')
                {
                    runnerHorizontal[i].x-=1;
                }else if (readChar(runnerHorizontal[i].x-1,runnerHorizontal[i].y)=='#')
                {
                    runnerHorizontal[i].movingDirection='d';
                }else if (readChar(runnerHorizontal[i].x-1,runnerHorizontal[i].y)=='X')
                {
                    runnerHorizontal[i].movingDirection='d';
                }else if (readChar(runnerHorizontal[i].x-1,runnerHorizontal[i].y)=='©')
                {
                    runnerHorizontal[i].movingDirection='d';
                }else if (readChar(runnerHorizontal[i].x-1,runnerHorizontal[i].y)=='+')
                {
                    runnerHorizontal[i].movingDirection='d';
                }else if (readChar(runnerHorizontal[i].x-1,runnerHorizontal[i].y)==char(159))
                {
                    runnerHorizontal[i].movingDirection='d';
                }else if (readChar(runnerHorizontal[i].x-1,runnerHorizontal[i].y)==char(207))
                {
                    runnerHorizontal[i].movingDirection='d';
                }else if (readChar(runnerHorizontal[i].x-1,runnerHorizontal[i].y)==char(111))
                {
                    //NIC
                }else
                {
                    player.lifes-=1;
                    player.lostLife=true;
                }
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
            if (runnerHorizontal[i].lifes==1)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),2);
            }
            gotoxy(runnerHorizontal[i].x,runnerHorizontal[i].y);
            cout<<char(207);
        }
    }
    
    
	    
	    	//    KETIKA PENCET "M" -> TEMBAK
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
	if (c=='m')
	{
	    if (player.ammunition!=0)
	       {
	            nrBullet+=1;
	            bullet[nrBullet].exists=false;
	            bullet[nrBullet].firstOccurrence=true;
	            player.ammunition-=1;
	            if (player.movingDirection=='w')
	            {
	                bullet[nrBullet].x=player.x;
	                bullet[nrBullet].y=player.y;
	                bullet[nrBullet].movingDirection='w';
	                if ((readChar(player.x,player.y-1)==' ') || 
						(readChar(player.x,player.y-1)==char(207)) || 
						(readChar(player.x,player.y-1)==char(208)) || 
						(readChar(player.x,player.y-1)=='X'))
	                {
	                    bullet[nrBullet].exists=true;
	                }
	            }
	            if (player.movingDirection=='s')
	            {
	                bullet[nrBullet].x=player.x;
	                bullet[nrBullet].y=player.y;
	                bullet[nrBullet].movingDirection='s';
	                if ((readChar(player.x,player.y+1)==' ') || 
						(readChar(player.x,player.y+1)==char(207)) || 
						(readChar(player.x,player.y+1)==char(208)) || 
						(readChar(player.x,player.y+1)=='X'))
	                {
	                    bullet[nrBullet].exists=true;
	                }
	            }
	            if (player.movingDirection=='a')
	            {
	                bullet[nrBullet].x=player.x;
	                bullet[nrBullet].y=player.y;
	                bullet[nrBullet].movingDirection='a';
	                if ((readChar(player.x-1,player.y)==' ') || 
						(readChar(player.x-1,player.y)==char(207)) || 
						(readChar(player.x-1,player.y)==char(208)) || 
						(readChar(player.x-1,player.y)=='X'))
	                {
	                    bullet[nrBullet].exists=true;
	                }
	            }
	            if (player.movingDirection=='d')
	            {
	                bullet[nrBullet].x=player.x;
	                bullet[nrBullet].y=player.y;
	                bullet[nrBullet].movingDirection='d';
	                if ((readChar(player.x+1,player.y)==' ') || 
						(readChar(player.x+1,player.y)==char(207)) || 
						(readChar(player.x+1,player.y)==char(208)) || 
						(readChar(player.x+1,player.y)=='X'))
	                {
	                    bullet[nrBullet].exists=true;
	                }
	            }
	        }
	
	    }
	    
	    
	//    KONDISI KETIKA MENEMBAK
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
    for (int i=0; i<=400; i++)
	    {
	        if (bullet[i].exists==true)
	        {
	            bullet[i].exists=false;
	            if (readChar(bullet[i].x,bullet[i].y)!='©')
	            {
	                if ((bullet[i].firstOccurrence==false) && 
						(readChar(bullet[i].x,bullet[i].y)!='©'))
	                {
	                    cout<<" ";
	                }
	                if (bullet[i].movingDirection=='w')
	                {
	                    bullet[i].y-=1;
	                    if ((readChar(bullet[i].x,bullet[i].y)==' ') || 
							(readChar(bullet[i].x,bullet[i].y)==char(111)))
	                    {
	                        bullet[i].exists=true;
	                    }
	                }
	                if (bullet[i].movingDirection=='s')
	                {
	                    bullet[i].y+=1;
	                    if ((readChar(bullet[i].x,bullet[i].y)==' ') || 
							(readChar(bullet[i].x,bullet[i].y)==char(111)))
	                    {
	                        bullet[i].exists=true;
	                    }
	                }
	                if (bullet[i].movingDirection=='a')
	                {
	                    bullet[i].x-=1;
	                    if ((readChar(bullet[i].x,bullet[i].y)==' ') || 
							(readChar(bullet[i].x,bullet[i].y)==char(111)))
	                    {
	                        bullet[i].exists=true;
	                    }
	                }
	                if (bullet[i].movingDirection=='d')
	                {
	                    bullet[i].x+=1;
	                    if ((readChar(bullet[i].x,bullet[i].y)==' ') || 
							(readChar(bullet[i].x,bullet[i].y)==char(111)))
	                    {
	                        bullet[i].exists=true;
	                    }
	                }
	                
	                if (bullet[i].exists==false)
	                {
	                    if (readChar(bullet[i].x,bullet[i].y)==char(207))
                    {
                        for (int j=0; j<=100; j++)
                        {
                            if((bullet[i].x==runnerHorizontal[j].x) && (bullet[i].y==runnerHorizontal[j].y))
                            {
                                runnerHorizontal[j].lifes-=1;
                                gotoxy(runnerHorizontal[j].x,runnerHorizontal[j].y);
                                cout<<" ";
                            }
                        }
                    }else if (readChar(bullet[i].x,bullet[i].y)==char(208))
                    {
                        for (int j=0; j<=50; j++)
                        {
                            if((bullet[i].x==randomer[j].x) && (bullet[i].y==randomer[j].y))
                            {
                                randomer[j].lifes-=1;
                                gotoxy(randomer[j].x,randomer[j].y);
                                cout<<" ";
                            }
                        }
                    }else if ((readChar(bullet[i].x,bullet[i].y)!='©') && 
							(readChar(bullet[i].x,bullet[i].y)!='#') && 
							(readChar(bullet[i].x,bullet[i].y)!=char(111)) && 
							(readChar(bullet[i].x,bullet[i].y)!=char(196)) && 
							(readChar(bullet[i].x,bullet[i].y)!='!') && 
							(readChar(bullet[i].x,bullet[i].y)!=char(159)) && 
							 (readChar(bullet[i].x,bullet[i].y)!='+')&& 
							 (readChar(bullet[i].x,bullet[i].y)!='C'))
	                    {
	                        player.lifes-=1;
	                        player.lostLife=true;
	                    }
	                }
	                bullet[i].firstOccurrence=false;
	            }
	        }
	    }
	    
	    //    TAMPILAN PELURU
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
	for(int i=0; i<=400; i++)
    {
        if (bullet[i].exists==true)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
            gotoxy(bullet[i].x,bullet[i].y);
            cout<<char(111);
        }
    }
	//    TAMPILAN BAR SAMPING
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
    gotoxy(90,3);
    cout<<"                ";
    gotoxy(90,3);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
    cout<<"Lives : ";
	
	for(int i = 0; i < player.lifes; i++){
		cout<<char(254);
	}

    gotoxy(90,5);
    cout<<"                ";
    gotoxy(90,5);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
    cout<<"Ammo  : ";
    for(int i = 0; i < player.ammunition; i++){
		cout<<char(169);
	}
	fflush(stdin);
    gotoxy(90,7);
    cout<<"                           ";
    if (player.resultpawnTime>0)
    {
        gotoxy(90,7);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
        cout<<"resultpawn time: "<<player.resultpawnTime;
    }


    gotoxy(11,40);
    cout<<"                ";
    gotoxy(11,40);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"XY: ( "<<player.x<<", "<<player.y<<" )";

}


//   	 CLEAR
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
void clearAllObjects()
{
    for (int i=0; i<=50; i++)
    {
        randomer[i].lifes=0;
    }
}

void poruszeniePlayer()
{
    c='0';
    if (_kbhit()==1)
    {
        c=getch();
    }
}


//    TAMPILAN BAR SAMPING (LEGEND)
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
void drawLegend()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(87,9);
    cout<<"--------------------------";
    gotoxy(91,10);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    cout<<"     LEGEND: ";

    gotoxy(89,12);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<char(161)<<" -> YOU";

    gotoxy(89,14);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    cout<<"# -> WALL";

    gotoxy(89,16);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),8);
    cout<<char(184);cout<<" -> STONE (YOU CAN MOVE THEM)";

    gotoxy(89,21);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
    cout<<char(207);cout<<" -> MAGIC AXE ";
    gotoxy(94,22);
    cout<<"enemy [2HP]";

    gotoxy(89,24);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),13);
    cout<<char(208);cout<<" -> CRAZY MONKEY";
    gotoxy(94,25);
    cout<<"HPNYA [2]";
  

    gotoxy(89,28);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
    cout<<char(190);cout<<" -> MUTANT MONSTER";

    gotoxy(89,31);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14);
    cout<<char(159);cout<<" -> Ammo (+2)";

    gotoxy(89,33);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"+ -> life (+1)";

    gotoxy(87,37);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    cout<<"--------------------------";

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    for (int i=0; i<=5; i++)
    {
        gotoxy(67,38+i);
        cout<<"!";
    }

    gotoxy(69,39);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    cout<<"ESC -> Quit to menu";

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    for (int i=0; i<=5; i++)
    {
        gotoxy(38,38+i);
        cout<<"!";
    }

    gotoxy(40,39);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"W/S/A/D -> Move character";
    gotoxy(40,40);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"       M -> Shoot        ";

}

void drawLegend2()
{
 
	gotoxy(95,26);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
    cout<<"-----------------------------------------";
    gotoxy(102,28);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
    cout<<"You Must Find All of the SECRET KEY !!!";
    gotoxy(95,30);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
    cout<<"-----------------------------------------";
  
}


//    FUNGSI resultTART
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
void levelresulttart()
{
    c='0';
    frameCount=0;
    system("CLS");
    if(player.checkPoint==0)
    {
    	
        player.x=2;
        player.y=35;
    }
    if(player.checkPoint==1){
    	
    	player.x=37;
    	player.y=35;
	}
    if (player.checkPoint==2)
    {
        player.x=56;
        player.y=22;
    }
    if (player.checkPoint==3)
    {
        player.x=4;
        player.y=23;
    }
    if (player.checkPoint==4)
    {
        player.x=33;
        player.y=11;
    }
    if (player.checkPoint==5)
    {
        player.x=42;
        player.y=12;
    }
    if (player.checkPoint==6)
    {
        player.x=85;
        player.y=15;
    }
     if (player.checkPoint==7)
    {
        bossLife=0;
        player.lifes=0;
    }
    clearAllObjects();
    loadLevelBase("level_1.txt");
  
    loadLevelObjects(1);
    drawLegend();
    gotoxy(player.x,player.y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<char(24);
}

//    FUNGSI GAME SPEED cuma untuk pistol
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
void speed()
{
    system("CLS");
    char c=' ';
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(72,16);
    cout<<"SPEED SEKARANG";
    gotoxy(64,21);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
    cout<<"<- A(-)";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    cout<<" GAME SPEEDNYA ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
    cout<<"(+)D ->";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(68,24);
    cout<<"   (ENTER TO tail)   ";
    while (c!=char(13))
    {
        gotoxy(77,18);
        cout<<"                    ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
        gotoxy(77,18);
        cout<<framesDelay;
        gotoxy(0,0);
        c=getch();
        if (c=='d')
        {
            framesDelay+=1;
        }
        if (c=='a')
        {
            if (framesDelay>=2)
            {
                framesDelay-=1;
            }
        }
    }
}

void trudnosc()
{

}


//    FUNGSI OPTION
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
void options()
{
    char c='0';
    int chosen=2;
    while (c!=' ')
    {
    system("CLS");
    
    gotoxy(53,5);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"      ¦¦¦¦¦¦+ ¦¦¦¦¦¦+ ¦¦¦¦¦¦¦¦+¦¦+ ¦¦¦¦¦¦+ ¦¦¦+   ¦¦+";
    gotoxy(53,6);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"     ¦¦+---¦¦+¦¦+--¦¦++--¦¦+--+¦¦¦¦¦+---¦¦+¦¦¦¦+  ¦¦¦";
    gotoxy(53,7);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"     ¦¦¦   ¦¦¦¦¦¦¦¦¦++   ¦¦¦   ¦¦¦¦¦¦   ¦¦¦¦¦+¦¦+ ¦¦¦";
    gotoxy(53,8); 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"     ¦¦¦   ¦¦¦¦¦+---+    ¦¦¦   ¦¦¦¦¦¦   ¦¦¦¦¦¦+¦¦+¦¦¦";
    gotoxy(53,9); 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"     +¦¦¦¦¦¦++¦¦¦        ¦¦¦   ¦¦¦+¦¦¦¦¦¦++¦¦¦ +¦¦¦¦¦";
    gotoxy(53,10); 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"      +-----+ +-+        +-+   +-+ +-----+ +-+  +---+"; 
    
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    gotoxy(80,14);
    cout<<char(176);
    gotoxy(80,15);
    cout<<"W";

    gotoxy(80,28);
    cout<<char(176);
    gotoxy(80,27);
    cout<<"S";

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(75,19);
    cout<<" DIFFICULTY ";
    gotoxy(75,21);
    cout<<" GAME SPEED ";
    gotoxy(75,23);
    cout<<"    tail    ";
    while (c!=char(13))
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
        gotoxy(73,19+chosen);
        cout<<"->";
        gotoxy(87,19+chosen);
        cout<<"<-";
        gotoxy(0,0);
        c=getch();
        gotoxy(73,19+chosen);
        cout<<"  ";
        gotoxy(87,19+chosen);
        cout<<"  ";
        if (c=='s')
        {
            if (chosen<4)
            {
                chosen+=2;
            }
        }
        if (c=='w')
        {
            if (chosen>0)
            {
                chosen-=2;
            }
        }
    }
    if (chosen==0)
    {
        trudnosc();
        c='0';
    }
    if (chosen==2)
    {
        speed();
        c='0';
    }
    if (chosen==4)
    {
        c=' ';
    }
    system("CLS");
    chosen=2;
    }

}

void startGame()
{
	//BOSS GAADA KOK
    srand(time(NULL));
    frameCount=0;
    player.x=2;
    player.y=35;
    player.ammunition=3;
    player.lifes=5;
    player.checkPoint=0;
    bossLife=50;
    bossPlaced=false;
    for (int i=0; i<=20;i++)
    {
        pickUp[i].exists=true;
    }
    clearAllObjects();
    loadLevelBase("level_1.txt");
    loadLevelObjects(1);
	drawLegend();
//	drawLegend2();
    gotoxy(player.x,player.y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<char(161);
}

void thx(){
	gotoxy(40,4);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"'########:'##::::'##::::'###::::'##::: ##:'##:::'##::::'##:::'##::'#######::'##::::'##:";
    gotoxy(40,5);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"... ##..:: ##:::: ##:::'## ##::: ###:: ##: ##::'##:::::. ##:'##::'##.... ##: ##:::: ##:";
    gotoxy(40,6);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"::: ##:::: ##:::: ##::'##:. ##:: ####: ##: ##:'##:::::::. ####::: ##:::: ##: ##:::: ##:";
    gotoxy(40,7);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"::: ##:::: #########:'##:::. ##: ## ## ##: #####:::::::::. ##:::: ##:::: ##: ##:::: ##:";
    gotoxy(40,8);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"::: ##:::: ##.... ##: #########: ##. ####: ##. ##::::::::: ##:::: ##:::: ##: ##:::: ##:";
    gotoxy(40,9);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"::: ##:::: ##:::: ##: ##.... ##: ##:. ###: ##:. ##:::::::: ##:::: ##:::: ##: ##:::: ##:";
    gotoxy(40,10);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"::: ##:::: ##:::: ##: ##:::: ##: ##::. ##: ##::. ##::::::: ##::::. #######::. #######::";
    gotoxy(40,11);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<":::..:::::..:::::..::..:::::..::..::::..::..::::..::::::::..::::::.......::::.......:::";
    gotoxy(65,29);
    cout<<". . S O U L  O F  M A G I C . .";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(53,30);
    cout<<"---------------------------------------------------------";
	
	delay(2000);                                                                          

}

void won(){
	
	gotoxy(50,5);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<" ¦¦+   ¦¦+ ¦¦¦¦¦¦+ ¦¦+   ¦¦+    ¦¦+    ¦¦+ ¦¦¦¦¦¦+ ¦¦¦+   ¦¦+¦¦+ ";
    gotoxy(50,5);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"+¦¦+ ¦¦++¦¦+---¦¦+¦¦¦   ¦¦¦    ¦¦¦    ¦¦¦¦¦+---¦¦+¦¦¦¦+  ¦¦¦¦¦¦ ";
    gotoxy(50,6);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<" +¦¦¦¦++ ¦¦¦   ¦¦¦¦¦¦   ¦¦¦    ¦¦¦ ¦+ ¦¦¦¦¦¦   ¦¦¦¦¦+¦¦+ ¦¦¦¦¦¦ ";
    gotoxy(50,7);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"  +¦¦++  ¦¦¦   ¦¦¦¦¦¦   ¦¦¦    ¦¦¦¦¦¦+¦¦¦¦¦¦   ¦¦¦¦¦¦+¦¦+¦¦¦+-+ ";
    gotoxy(50,8);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"  +¦¦++  ¦¦¦   ¦¦¦¦¦¦   ¦¦¦    ¦¦¦¦¦¦+¦¦¦¦¦¦   ¦¦¦¦¦¦+¦¦+¦¦¦+-+ ";
    gotoxy(50,9);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"   ¦¦¦   +¦¦¦¦¦¦+++¦¦¦¦¦¦++    +¦¦¦+¦¦¦+++¦¦¦¦¦¦++¦¦¦ +¦¦¦¦¦¦¦+ ";
    gotoxy(50,10);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"   +-+    +-----+  +-----+      +--++--+  +-----+ +-+  +---++-+ ";
 	gotoxy(70,28);
    cout<<". . T H A N K  Y O U . .";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(53,30);
    cout<<"---------------------------------------------------------";
	
	delay(2000);
	
}

void stage3(){
	
	gotoxy(48,5);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"¦¦¦¦¦¦¦+¦¦¦+   ¦¦+¦¦¦¦¦¦+      ¦¦¦¦¦¦+  ¦¦¦¦¦+ ¦¦¦+   ¦¦¦+¦¦¦¦¦¦¦+";
    
    gotoxy(48,6);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"¦¦+----+¦¦¦¦+  ¦¦¦¦¦+--¦¦+    ¦¦+----+ ¦¦+--¦¦+¦¦¦¦+ ¦¦¦¦¦¦¦+----+";
    
    gotoxy(48,7);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"¦¦¦¦¦+  ¦¦+¦¦+ ¦¦¦¦¦¦  ¦¦¦    ¦¦¦  ¦¦¦+¦¦¦¦¦¦¦¦¦¦+¦¦¦¦+¦¦¦¦¦¦¦¦+  ";
    
    gotoxy(48,8);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"¦¦+--+  ¦¦¦+¦¦+¦¦¦¦¦¦  ¦¦¦    ¦¦¦   ¦¦¦¦¦+--¦¦¦¦¦¦+¦¦++¦¦¦¦¦+--+  ";
    
    gotoxy(48,9);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"¦¦¦¦¦¦¦+¦¦¦ +¦¦¦¦¦¦¦¦¦¦¦++    +¦¦¦¦¦¦++¦¦¦  ¦¦¦¦¦¦ +-+ ¦¦¦¦¦¦¦¦¦¦+";
    
    gotoxy(48,10);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"+------++-+  +---++-----+      +-----+ +-+  +-++-+     +-++------+";	
	
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
	gotoxy(77,12);
    cout<<"Final Stage";
    
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(40,16);
    cout<<"- Ksatria kuku akhirnya menemukan ramuan terakhir di dalam tempat terlarang itu, -";
    gotoxy(57,17);
	cout<<"  banyak sekali rintangan yang harus ia hadapi ";
	gotoxy(55,18);
	cout<<"Apakah ia bisa menyelamatkan sang Ratu Da' Arline??";
	
	
		
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
	
    gotoxy(70,28);
    cout<<". . W E L C O M E . .";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(53,30);
    cout<<"---------------------------------------------------------";

	delay(2000);
	
}


void stage2(){
	gotoxy(20,5);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"¦¦¦¦¦¦¦+ ¦¦¦¦¦+ ¦¦¦¦¦¦+     ¦¦¦¦¦¦¦+¦¦¦¦¦¦+  ¦¦¦¦¦¦+ ¦¦¦+   ¦¦¦+     ¦¦¦¦¦¦+ ¦¦¦¦¦+ ¦¦¦¦¦¦¦+¦¦¦¦¦¦¦¦+¦¦+     ¦¦¦¦¦¦¦+";
    
    gotoxy(20,6);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"¦¦+----+¦¦+--¦¦+¦¦+--¦¦+    ¦¦+----+¦¦+--¦¦+¦¦+---¦¦+¦¦¦¦+ ¦¦¦¦¦    ¦¦+----+¦¦+--¦¦+¦¦+----++--¦¦+--+¦¦¦     ¦¦+----+";
    
    gotoxy(20,7);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"¦¦¦¦¦+  ¦¦¦¦¦¦¦¦¦¦¦¦¦¦++    ¦¦¦¦¦+  ¦¦¦¦¦¦++¦¦¦   ¦¦¦¦¦+¦¦¦¦+¦¦¦    ¦¦¦     ¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦+   ¦¦¦   ¦¦¦     ¦¦¦¦¦+  ";
    
    gotoxy(20,8);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"¦¦+--+  ¦¦+--¦¦¦¦¦+--¦¦+    ¦¦+--+  ¦¦+--¦¦+¦¦¦   ¦¦¦¦¦¦+¦¦++¦¦¦    ¦¦¦     ¦¦+--¦¦¦+----¦¦¦   ¦¦¦   ¦¦¦     ¦¦+--+  ";
    
    gotoxy(20,9);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"¦¦¦     ¦¦¦  ¦¦¦¦¦¦  ¦¦¦    ¦¦¦     ¦¦¦  ¦¦¦+¦¦¦¦¦¦++¦¦¦ +-+ ¦¦¦    +¦¦¦¦¦¦+¦¦¦  ¦¦¦¦¦¦¦¦¦¦¦   ¦¦¦   ¦¦¦¦¦¦¦+¦¦¦¦¦¦¦+";
    
    gotoxy(20,10);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"+-+     +-+  +-++-+  +-+    +-+     +-+  +-+ +-----+ +-+     +-+     +-----++-+  +-++------+   +-+   +------++------+";
	
	
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
	gotoxy(77,12);
    cout<<"Stage 2";
    
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(50,16);
    cout<<"- Ksatria kuku terus mencari obat tersebut dengan berkelana, -";
    gotoxy(37,17);
	cout<<"  menuju daerah terlarang, dia akan melakukan apapun demi sang ratu Da' Arline, ";
	gotoxy(60,18);
	cout<<"Akankah dia bisa kembali dengan selamat?";
	
	
		
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
	
    gotoxy(70,28);
    cout<<". . W E L C O M E . .";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(53,30);
    cout<<"---------------------------------------------------------";

	delay(2000);
	
}

void stage1(){
	gotoxy(15,5);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<" ¦¦¦¦¦¦+¦¦+      ¦¦¦¦¦¦+ ¦¦¦¦¦¦¦+¦¦¦¦¦¦¦+    ¦¦¦¦¦¦¦+¦¦¦¦¦¦+  ¦¦¦¦¦¦+ ¦¦¦+   ¦¦¦+     ¦¦¦¦¦¦+ ¦¦¦¦¦+ ¦¦¦¦¦¦¦+¦¦¦¦¦¦¦¦+¦¦+     ¦¦¦¦¦¦¦+";
    
	gotoxy(15,6);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"¦¦+----+¦¦¦     ¦¦+---¦¦+¦¦+----+¦¦+----+    ¦¦+----+¦¦+--¦¦+¦¦+---¦¦+¦¦¦¦+ ¦¦¦¦¦    ¦¦+----+¦¦+--¦¦+¦¦+----++--¦¦+--+¦¦¦     ¦¦+----+";
    
    gotoxy(15,7);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"¦¦¦     ¦¦¦     ¦¦¦   ¦¦¦¦¦¦¦¦¦¦+¦¦¦¦¦+      ¦¦¦¦¦+  ¦¦¦¦¦¦++¦¦¦   ¦¦¦¦¦+¦¦¦¦+¦¦¦    ¦¦¦     ¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦+   ¦¦¦   ¦¦¦     ¦¦¦¦¦+";
    
	gotoxy(15,8);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"¦¦¦     ¦¦¦     ¦¦¦   ¦¦¦+----¦¦¦¦¦+--+      ¦¦+--+  ¦¦+--¦¦+¦¦¦   ¦¦¦¦¦¦+¦¦++¦¦¦    ¦¦¦     ¦¦+--¦¦¦+----¦¦¦   ¦¦¦   ¦¦¦     ¦¦+--+";
    
	gotoxy(15,9);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"+¦¦¦¦¦¦+¦¦¦¦¦¦¦++¦¦¦¦¦¦++¦¦¦¦¦¦¦¦¦¦¦¦¦¦¦+    ¦¦¦     ¦¦¦  ¦¦¦+¦¦¦¦¦¦++¦¦¦ +-+ ¦¦¦    +¦¦¦¦¦¦+¦¦¦  ¦¦¦¦¦¦¦¦¦¦¦   ¦¦¦   ¦¦¦¦¦¦¦+¦¦¦¦¦¦¦+";
	
	gotoxy(15,10);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<" +-----++------+ +-----+ +------++------+    +-+     +-+  +-+ +-----+ +-+     +-+     +-----++-+  +-++------+   +-+   +------++------+";
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
	gotoxy(77,12);
    cout<<"Stage 1";
    
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(50,16);
    cout<<"- Penyihir jahat telah mengubah kehidupan Kerajaan Be Eli. -";
    gotoxy(37,17);
	cout<<"Penyihir jahat Gedo mengirimkan kutukan kepada sang Ratu Da' Arline, Ksatria Kuku ";
	gotoxy(47,18);
	cout<<"harus mencari obat herbal untuk kutukan itu , Apakah dia berhasil?";
	
	
		
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
	
    gotoxy(70,28);
    cout<<". . W E L C O M E . .";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(53,30);
    cout<<"---------------------------------------------------------";

	delay(2000);
}


void menu()
{
    char c='0';
    int chosen=0;
    exitConsole=false;
    system("CLS");
   
//	loadLevelBase("opening.txt");
	while ((inGame==false) && (exitConsole==false))
    {
    	
    	
    gotoxy(27,5);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"     ¦¦¦¦¦¦¦+ ¦¦¦¦¦¦+ ¦¦+   ¦¦+¦¦+          ¦¦¦¦¦¦+ ¦¦¦¦¦¦¦+    ¦¦¦+   ¦¦¦+ ¦¦¦¦¦+  ¦¦¦¦¦¦+ ¦¦+ ¦¦¦¦¦¦+ ";
    gotoxy(27,6);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"     ¦¦+----+¦¦+---¦¦+¦¦¦   ¦¦¦¦¦¦         ¦¦+---¦¦+¦¦+----+    ¦¦¦¦+ ¦¦¦¦¦¦¦+--¦¦+¦¦+----+ ¦¦¦¦¦+----+ ";
    gotoxy(27,7);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"     ¦¦¦¦¦¦¦+¦¦¦   ¦¦¦¦¦¦   ¦¦¦¦¦¦         ¦¦¦   ¦¦¦¦¦¦¦¦+      ¦¦+¦¦¦¦+¦¦¦¦¦¦¦¦¦¦¦¦¦¦  ¦¦¦+¦¦¦¦¦¦      ";
    gotoxy(27,8); 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"     +----¦¦¦¦¦¦   ¦¦¦¦¦¦   ¦¦¦¦¦¦         ¦¦¦   ¦¦¦¦¦+--+      ¦¦¦+¦¦++¦¦¦¦¦+--¦¦¦¦¦¦   ¦¦¦¦¦¦¦¦¦      ";
    gotoxy(27,9); 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"     ¦¦¦¦¦¦¦¦+¦¦¦¦¦¦+++¦¦¦¦¦¦++¦¦¦¦¦¦¦+    +¦¦¦¦¦¦++¦¦¦         ¦¦¦ +-+ ¦¦¦¦¦¦  ¦¦¦+¦¦¦¦¦¦++¦¦¦+¦¦¦¦¦¦+ ";
    gotoxy(27,10); 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
    cout<<"     +------+ +-----+  +-----+ +------+     +-----+ +-+         +-+     +-++-+  +-+ +-----+ +-+ +-----+ "; 
    
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);

    gotoxy(80,14);
    cout<<char(176);
    gotoxy(80,15);
    cout<<"W";

    gotoxy(80,28);
    cout<<char(176);
    gotoxy(80,27);
    cout<<"S";
	
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    gotoxy(77,19);
    cout<<" STORY ";
    gotoxy(77,21);
    cout<<" EXTRA ";
    gotoxy(77,23);
    cout<<"OPTIONS";
    gotoxy(77,25);
    cout<<" CLOSE ";
    while (c!=char(13))
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10);
        gotoxy(74,19+chosen);
        cout<<"->";
        gotoxy(85,19+chosen);
        cout<<"<-";
        gotoxy(0,0);
        c=getch();
        gotoxy(74,19+chosen);
        cout<<"  ";
        gotoxy(85,19+chosen);
        cout<<"  ";
        if (c=='s')
        {
            if (chosen<5)
            {
                chosen+=2;
            }
        }
        if (c=='w')
        {
            if (chosen>0)
            {
                chosen-=2;
            }
        }
    }
    if (chosen==0)
    {
    	system("cls");
    	stage1();
    	system("cls");
        drawLegend2();
        play();
        system("cls");
        stage2();
        system("cls");
        drawLegend2();
        play();
        system("cls");
        stage3();
        inGame = true;
        
    }
    if (chosen==2)
    {
        inGame=true;
    }
    if (chosen==4)
    {
		options();
    }
    if (chosen==6)
    {
    	system("CLS");
		thx();
        inGame=false;
        exitConsole=true;
    }
    chosen=0;
    c=' ';
    system("CLS");
    }
}

int main()
{
    framesDelay=100;
//    won();
    menu();
    while (exitConsole==false)
    {
        startGame();
        while(inGame==true)
        {
            player.lostLife=false;
            update();
            poruszeniePlayer();
            if (player.resultpawnTime>=1)
            {
                c='0';
                player.resultpawnTime-=1;
            }
            if (c==char(27))
            {
                inGame=false;
            }
            if ((player.lostLife==true) || (c=='r'))
            {
                for (int i=0; i<=4; i++)
                {
                    gotoxy(player.x,player.y);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),3);
                    cout<<char(24);
                    Sleep(50);
                    gotoxy(player.x,player.y);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
                    cout<<char(24);
                    Sleep(50);
                }
                if (c=='r')
                    {
                        player.lifes-=1;
                    }
                player.resultpawnTime=20;
                levelresulttart();
            }
            if (player.lifes<=0)
            {
                inGame=false;
            }
            gotoxy(0,0);
            Sleep(framesDelay);
            frameCount+=1;
            if (frameCount>2000000)
            {
                frameCount=0;
            }
            if (nrBullet>350)
            {
                nrBullet=0;
            }
            if(bossLife<=0)
            {
                inGame=false;
            }
        }
        if ((exitConsole==false)&&(bossLife>0))
        {
//        	won();
            system("CLS");
            gotoxy(52,20);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),6);
            cout<<"GAME OVER";
            gotoxy(44,23);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
            cout<<"Presults ESC to go tail to menu";
            while (c!=char(27))
            {
                c=getch();
            }
            menu();
        }if(bossLife<=1){
        	system("CLS");
        	won();
        	while (c!=char(27))
            {
                c=getch();
            }
        	menu();
			}
        
    }
    return 0;
}

